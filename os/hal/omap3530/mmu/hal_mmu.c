/*
 * mmu.c
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#include "../../omap3530/mmu/hal_mmu_omap3530.h"


/*
 * prototypes are static because it's only implementation specific
 *
 */
static void initTablesAndRegions();
static void writeSectionToMemory(mmu_region_t* region);
static void writeTableToMemory(mmu_region_t* region);

void hal_mmu_activate(void) {
	hal_mmu_activate_asm();
}


//size -> ram size in MB
static void set_ddr_memory(uint32_t size) {
	uint32_t sdr_val = *(uint32_t*) SDRC_MCFG;
	sdr_val &= 0xFFFC01FF; // reset size
	sdr_val |= (size << 8);
	*(uint32_t*) SDRC_MCFG = sdr_val;

	//BIT_SET(SDRC_MCFG, 0, 17);
}

void hal_mmu_init(void) {
	hal_mmu_enable_write_buffer_asm();

	set_ddr_memory(SDRAM_RAM_SIZE_256);

	hal_mmu_set_ttbr_ctrl_bits_asm(0x2); //First 3 bits != null OS/HW call else VM

	initTablesAndRegions();

	/* the master-pagetable base-address is referenced in the ttbr1 and has always a size of 16kB */
	uint32_t ttbr1 = OS_L1_PT_START & 0xFFFFC000;
	ttbr1 |= (WB & 0x3) << 3;
	ttbr1 |= 0x1; //caching activate

	hal_mmu_set_ttbr_1_asm(ttbr1);
	hal_mmu_set_domain(0x3);
}
/*
 * processSize in kB (4kB pages)
 *
 */
void hal_mmu_addProcess(uint16_t processId, uint8_t processSize) {

	/*mmu_l2_pagetable_t processTable;
	processTable.masterPtAddress =  TASK_PT_START + (uint32_t) processId * TASK_L1_PT_SIZE + TASK_L2_; //L1
	processTable.ptAddress = processTable.masterPtAddress + TASK_L1_PT_SIZE; //L2
	processTable.vAddress = VM_START;

	processTable.type = COARSE;
	processTable.dom = 0;

	mmu_l2_pagetable_t processRegion;
	processRegion.vAddress = VM_START;
	processRegion.pageSize = TASK_PAGE_SIZE;
	processRegion.numPages = processSize / TASK_PAGE_SIZE;
	processRegion.AP = RWRW;
	processRegion.CB = WB;
	processRegion.pAddress = TASKS_START + (processId * TASK_SIZE);
	processRegion.PT = &processTable;

	writeTableToMemory(&processRegion);*/
}

void hal_mmu_removeProcess(uint16_t processId) {
	//TODO: remove region and table
}

static void initTablesAndRegions() {
	mmu_master_pagetable_t masterTable;
	masterTable.ptAddress = OS_L1_PT_START;
	masterTable.vAddress = OS_L1_PT_START;
	//masterTable.masterPtAddress = MASTER_PT_START;
	masterTable.type = MASTER;
	masterTable.dom = 0;

	mmu_region_t hwRegion;
	hwRegion.vAddress = HW_START;
	hwRegion.pageSize = 0x100000; //HW_PAGE_SIZE;		//Page size 4KB
	hwRegion.numPages = HW_SIZE / 0x100000;// HW_PAGE_SIZE;
	hwRegion.AP = RWRW;
	hwRegion.CB = cb;
	hwRegion.pAddress = HW_START;
	hwRegion.PT = &masterTable;

	mmu_region_t taskRegion;
	taskRegion.vAddress = TASKS_START; //TASKS_START;
	taskRegion.pageSize = TASK_PAGE_SIZE;		//Page size 4KB
	taskRegion.numPages = TASK_REGION_SIZE / TASK_PAGE_SIZE;
	taskRegion.AP = RWRW;
	taskRegion.CB = cb;
	taskRegion.pAddress = TASKS_START;
	taskRegion.PT = &masterTable;


	mmu_region_t kernelRegion;
	kernelRegion.vAddress = KERNEL_START;
	kernelRegion.pageSize = 0x100000;// KERNEL_SECTION_SIZE;
	kernelRegion.numPages = KERNEL_SIZE / 0x100000;//KERNEL_SECTION_SIZE;
	kernelRegion.AP = RWNA;
	kernelRegion.CB = WB;
	kernelRegion.pAddress = KERNEL_START;
	kernelRegion.PT = &masterTable;

	mmu_region_t master_pt_region;
	master_pt_region.vAddress = TASK_L1_PT_START;
	master_pt_region.pageSize = TASK_L1_PT_SIZE;
	master_pt_region.numPages = PAGE_TABLE_REGION_SIZE / TASK_L1_PT_SIZE;
	master_pt_region.AP = RWNA;
	master_pt_region.CB = WB;
	master_pt_region.pAddress = TASK_L1_PT_START;
	master_pt_region.PT = &masterTable;


	mmu_region_t pt_l2_region;
	pt_l2_region.vAddress = TASK_L2_PT_START;
	pt_l2_region.pageSize = TASK_L2_PT_SIZE;
	pt_l2_region.numPages = PAGE_TABLE_REGION_SIZE / TASK_L1_PT_SIZE;
	pt_l2_region.AP = RWNA;
	pt_l2_region.CB = WB;
	pt_l2_region.pAddress = TASK_L2_PT_START;
	pt_l2_region.PT = &masterTable;

	writeSectionToMemory(&hwRegion);
	writeSectionToMemory(&kernelRegion);

	writeSectionToMemory(&master_pt_region);
	//writeSectionToMemory(&pt_l2_region);
	//writeTableToMemory(&taskRegion);



	//writeSectionToMemory(&master_pt_region);
}

static void writeSectionToMemory(mmu_region_t* region) {
	uint32_t* tablePos = (uint32_t*) region->PT->ptAddress;
	tablePos += region->vAddress >> 20;
	tablePos += region->numPages - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = region->pAddress & 0xFFFF0000;
	entry |= (region->AP & 0x3) << 10;
	entry |= region->PT->dom << 5;
	entry |= (region->CB & 0x3) << 2;
	entry |= 0x2; //section entry
	int i;
	for (i = region->numPages - 1; i >= 0; i--) {
		*tablePos-- = entry + (i << 20);
	}
}

static void writeTableToMemory(mmu_region_t* region) {
	uint32_t* tablePos = (uint32_t*) region->PT->ptAddress;
	tablePos += region->vAddress >> 20;
	tablePos += region->numPages - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = region->pAddress & 0xFFFFFC00;
	entry |= region->PT->dom >> 4;
	entry |= (region->CB & 0x3) << 2;
	entry &= 0xFFFFFFFD;
	entry |= 0x1 ; //0b01 -> coarse table descriptor

	int i;
	for (i = region->numPages - 1; i >= 0; i--) {
		*tablePos-- = entry + (i << 10);
	}
}
