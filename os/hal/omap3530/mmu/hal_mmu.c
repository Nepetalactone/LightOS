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
static void writeSectionsToMemory(mmu_first_level_desc_section_t* section, uint32_t numOfSections);
static void writeCoarsePageTablesToMemory(mmu_first_level_desc_section_t* region);
//static void writeSmallPagesToMemory(mmu_coarse_pagetable_t table);

void hal_mmu_activate(void) {
	hal_mmu_activate_asm();
}


//size -> ram size in MB
static void set_ddr_memory(uint32_t size) {
	uint32_t sdr_val = *(uint32_t*) SDRC_MCFG;
	sdr_val &= 0xFFFC01FF; // reset size
	sdr_val |= (size << 8);
	*(uint32_t*) SDRC_MCFG = sdr_val;
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
/*
	mmu_pagetable_t task_l1_table;
	task_l1_table.vAddress = VM_TASK_START;
	//task_l1_table.dom = DOMAIN;
	task_l1_table.type = COARSE;
	task_l1_table.ptAddress = TASK_L1_PT_START + processId * TASK_L1_PT_SIZE;


	// 1024 l2 pagetables are possible --> 1024 * 1mb
	uint32_t nr_of_pages = processSize / SMALL_PAGE_SIZE;

	if (nr_of_pages <= 256) {
		// only one l2 pt is used

		mmu_coarse_pagetable_t task_l2_table;
		task_l2_table.dom = DOMAIN;
		task_l2_table.type = COARSE;
		task_l2_table.vAddress = VM_TASK_START;
		task_l2_table.masterPtAddress = task_l1_table.ptAddress;
		task_l2_table.ptAddress = TASK_L2_PT_START + processId * TASK_L2_PT_SIZE;
		task_l2_table.numPages = nr_of_pages;

		task_l1_table.mmu_l2_tables[0] = task_l2_table;

		mmu_first_level_desc_section_t taskRegion;
		taskRegion.vAddress = TASKS_START; //TASKS_START;
		taskRegion.pageSize = SMALL_PAGE_SIZE;
		taskRegion.numPages = TASK_REGION_SIZE / TASK_PAGE_SIZE;
		taskRegion.AP = RWRW;
		taskRegion.CB = cb;
		taskRegion.pAddress = TASKS_START;
		taskRegion.PT = &task_l1_table;

		//writeCoarsePageTablesToMemory(&taskRegion);


		//writeSmallPagesToMemory(&task_l2_table);
	} else {
		// split pages to several l2 pagetables
		// actually only one l2 pagetable --> 1mb max
	}

	//writeTableToMemory(&task_l1_table);
*/
}

void hal_mmu_removeProcess(uint16_t processId) {
	//TODO: remove region and table
}

static void initTablesAndRegions() {
	mmu_pagetable_t masterTable;
	masterTable.ptAddress = OS_L1_PT_START;

	mmu_first_level_desc_section_t hw_section;
	hw_section.type = SECTION;
	hw_section.dom = DOMAIN;
	hw_section.vAddress = HW_START;
	hw_section.AP = RWRW;
	hw_section.CB = cb;
	hw_section.pAddress = HW_START;
	hw_section.PT = &masterTable;
	writeSectionsToMemory(&hw_section, (HW_SIZE / SECTION_PAGE_SIZE));


	mmu_first_level_desc_section_t kernel_section;
	kernel_section.type = SECTION;
	kernel_section.dom = DOMAIN;
	kernel_section.vAddress = KERNEL_START;
	kernel_section.AP = RWNA;
	kernel_section.CB = WB;
	kernel_section.pAddress = KERNEL_START;
	kernel_section.PT = &masterTable;
	writeSectionsToMemory(&kernel_section, (KERNEL_SIZE / SECTION_PAGE_SIZE));

	mmu_first_level_desc_section_t master_pt_section;
	master_pt_section.type = SECTION;
	master_pt_section.dom = DOMAIN;
	master_pt_section.vAddress = TASK_L1_PT_START;
	master_pt_section.AP = RWNA;
	master_pt_section.CB = WB;
	master_pt_section.pAddress = TASK_L1_PT_START;
	master_pt_section.PT = &masterTable;
	writeSectionsToMemory(&master_pt_section, (PAGE_TABLE_REGION_SIZE / TASK_L1_PT_SIZE));


	mmu_first_level_desc_section_t pt_l2_section;
	pt_l2_section.type = SECTION;
	pt_l2_section.dom = DOMAIN;
	pt_l2_section.vAddress = TASK_L2_PT_START;
	pt_l2_section.AP = RWNA;
	pt_l2_section.CB = WB;
	pt_l2_section.pAddress = TASK_L2_PT_START;
	pt_l2_section.PT = &masterTable;
	writeSectionsToMemory(&pt_l2_section, (PAGE_TABLE_REGION_SIZE / SECTION_PAGE_SIZE));
}

/*
 * write a pagetable containing only sections to memory
 */
static void writeSectionsToMemory(mmu_first_level_desc_section_t* section, uint32_t numOfSections) {
	uint32_t* tablePos = (uint32_t*) section->PT->ptAddress;
	tablePos += section->vAddress >> 20;
	tablePos += numOfSections - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = section->pAddress & 0xFFFF0000;
	entry |= (section->AP & 0x3) << 10;
	entry |= section->dom << 5;
	entry |= (section->CB & 0x3) << 2;
	entry |= 0x2; //section entry
	int i;
	for (i = numOfSections - 1; i >= 0; i--) {
		*tablePos-- = entry + (i << 20);
	}
}

/*

static void writeCoarsePageTablesToMemory(mmu_first_level_desc_section_t* region) {
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
*/
/*
static void writeSmallPagesToMemory(mmu_coarse_pagetable_t table) {
	uint32_t* tablePos = (uint32_t*) table->ptAddress;
	tablePos += table->vAddress << 20;
	tablePos += table->numPages - 1;
	tablePos = (uint32_t*)((uint32_t) tablePos & 0xFFFFFFFC);

	//uint32_t entry = table->
}
*/
