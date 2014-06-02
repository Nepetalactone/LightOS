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

void hal_mmu_init(void) {
	hal_mmu_enable_write_buffer_asm();

	hal_mmu_set_ttbr_ctrl_bits_asm(0x3); //First 3 bits != null OS/HW call else VM

	initTablesAndRegions();

	/* the master-pagetable base-address is referenced in the ttbr1 and has always a size of 16kB */
	uint32_t ttbr1 = MASTER_PT_START & 0xFFFFC000;
	ttbr1 |= (WB & 0x3) << 3;
	ttbr1 |= 0x1; //caching activate

	hal_mmu_set_ttbr_1_asm(ttbr1); // set the pt base of os

	uint32_t ttbr0_translation_table_base = 0x0; // TODO set correct pt
	uint32_t ttbr0_procid_asid = 0x0;
	// 31---------------------------8|7--------0|
	// |      PROCID                 |    ASID  |
	// |----------------------------------------|

	hal_mmu_set_ttbr_0_asm(ttbr0_translation_table_base, ttbr0_procid_asid); // set the pt base of the process

	hal_mmu_set_ttbr_cr_n_asm(N);
}

void hal_mmu_addProcess(uint16_t processId) {
	/* generate process pagetable */
	mmu_pagetable_t processTable;
	processTable.ptAddress = TASKS_PT_START + (uint32_t)processId * TASK_PT_SIZE;
	processTable.vAddress = VM_START;
	processTable.masterPtAddress = MASTER_PT_START;
	processTable.type = FINE;
	processTable.dom = 0;

	/* generate process-region and set pagetable */
	mmu_region_t processRegion;
	processRegion.vAddress = VM_START;
	processRegion.pageSize = TASK_PAGE_SIZE;
	processRegion.numPages = TASK_SIZE / TASK_PAGE_SIZE;
	processRegion.AP = RWRW;
	processRegion.CB = WB;
	processRegion.pAddress = TASKS_START + processId * TASK_SIZE;
	processRegion.PT = &processTable;

	writeTableToMemory(&processRegion);
}

void hal_mmu_removeProcess(uint16_t processId) {
	//TODO: remove region and table
}

static void initTablesAndRegions() {
	/* create master page table */
	mmu_pagetable_t masterTable;
	masterTable.ptAddress = MASTER_PT_START;
	masterTable.vAddress = HW_START;
	masterTable.masterPtAddress = MASTER_PT_START;
	masterTable.type = MASTER;
	masterTable.dom = 0;

	/* create hw region */
	mmu_region_t hwRegion;
	hwRegion.vAddress = HW_START;
	hwRegion.pageSize = HW_PAGE_SIZE;
	hwRegion.numPages = HW_SIZE / HW_PAGE_SIZE;
	hwRegion.AP = RWRW;
	hwRegion.CB = cb;
	hwRegion.pAddress = HW_START;
	hwRegion.PT = &masterTable;

	/* create kernel region */
	mmu_region_t kernelRegion;
	kernelRegion.vAddress = OS_START;
	kernelRegion.pageSize = OS_PAGE_SIZE;
	kernelRegion.numPages = OS_SIZE / OS_PAGE_SIZE;
	kernelRegion.AP = RWNA;
	kernelRegion.CB = WB;
	kernelRegion.pAddress = OS_START;
	kernelRegion.PT = &masterTable;

	/* pagetable region n*/
	mmu_region_t pagetableRegion;
	pagetableRegion.vAddress = PT_START;
	pagetableRegion.pageSize = PT_SIZE;
	pagetableRegion.numPages = PT_SIZE / PT_PAGE_SIZE;
	pagetableRegion.AP = RWNA;
	pagetableRegion.CB = WB;
	pagetableRegion.pAddress = PT_START;
	pagetableRegion.PT = &masterTable;

	writeSectionToMemory(&hwRegion);
	writeSectionToMemory(&kernelRegion);
	writeSectionToMemory(&pagetableRegion);
}

static void writeSectionToMemory(mmu_region_t* region) {
	uint32_t* tablePos = (uint32_t*) region->PT->ptAddress;
	tablePos += region->vAddress >> 18;
	tablePos += region->numPages - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = region->pAddress & 0xFFF00000;
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
	tablePos += region->vAddress >> 18;
	tablePos += region->numPages - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = region->pAddress & 0xFFFFFC00;
	entry |= region->PT->dom << 5;
	entry |= (region->CB & 0x3) << 2;
	entry |= 0x1; //first level descriptor

	int i;
	for (i = region->numPages - 1; i >= 0; i--) {
		*tablePos-- = entry + (i << 10);
	}
}
