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
static void writeSectionsToMemory(mmu_first_level_desc_t* section, uint32_t numOfSections);
static void write_coarse_page_table_to_memory(mmu_first_level_desc_t* desc, uint32_t num_of_pages);
static void write_small_pages_to_memory(mmu_second_level_desc_t* desc, uint32_t nr_of_pages);

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
void hal_mmu_addProcess(process_t* proc) {
	//L1 Table
	mmu_pagetable_t task_l1_table;
	task_l1_table.ptAddress = TASK_L1_PT_START + proc->pID * TASK_L1_PT_SIZE;
	proc->pt_address = (uint32_t) &task_l1_table;

	// 1024 l2 pagetables are possible --> 1024 * 1mb
	uint32_t nr_of_pages = (proc->size *1024) / SMALL_PAGE_SIZE;

	if (nr_of_pages <= 256) {
		// L1 entry
		mmu_first_level_desc_t coarse_entry;
		//TODO fix adresses L1 L2 Tables
		//coarse_entry.vAddress = TASK_START + (COARSE_PAGE_SIZE * proc->procId); //TASKS_START;
		//coarse_entry.pAddress = TASK_START + (COARSE_PAGE_SIZE * proc->procId);
		coarse_entry.PT = &task_l1_table;
		coarse_entry.dom = DOMAIN;
		coarse_entry.type = COARSE;
		coarse_entry.vAddress = VM_TASK_START;

		write_coarse_page_table_to_memory(&coarse_entry, nr_of_pages);

		//L2 Table
		mmu_pagetable_t task_l2_table;
		task_l2_table.masterPtAddress = task_l1_table.ptAddress;
		task_l2_table.ptAddress = TASK_L2_PT_START + proc->pID * TASK_L2_PT_SIZE;

		//L2 entry
		mmu_second_level_desc_t small_page_entry;
		//small_page_entry.vAddress = TASK_START + proc->procId * SMALL_PAGE_SIZE; //TASKS_START;
		//small_page_entry.pAddress = TASK_START + proc->procId * SMALL_PAGE_SIZE;
		small_page_entry.AP0 = RWRW;
		small_page_entry.AP1 = RWRW;
		small_page_entry.AP2 = RWRW;
		small_page_entry.AP3 = RWRW;
		small_page_entry.CB = WB;
		small_page_entry.PT = &task_l2_table;
		small_page_entry.type = SMALL;
		small_page_entry.first_level_desc = &coarse_entry;
		write_small_pages_to_memory(&small_page_entry, nr_of_pages);
		//writeSmallPagesToMemory(&task_l2_table);
	} else {
		// split pages to several l2 pagetables
		// actually only one l2 pagetable --> 1mb max
	}

	//writeTableToMemory(&task_l1_table);
}

void hal_mmu_start_process(process_t* proc) {
	// set ttbr reg for process
	// flush tlb (via asid)
	hal_mmu_set_ttbr_0_asm(proc->pt_address, proc->pID);

}

void hal_mmu_removeProcess(uint16_t processId) {
	//TODO: remove region and table
}

static void initTablesAndRegions() {
	mmu_pagetable_t masterTable;
	masterTable.ptAddress = OS_L1_PT_START;

	mmu_first_level_desc_t hw_section;
	hw_section.type = SECTION;
	hw_section.dom = DOMAIN;
	hw_section.vAddress = HW_START;
	hw_section.AP = RWRW;
	hw_section.CB = cb;
	hw_section.pAddress = HW_START;
	hw_section.PT = &masterTable;
	writeSectionsToMemory(&hw_section, (HW_SIZE / SECTION_PAGE_SIZE));

	mmu_first_level_desc_t int_vec_section;
	int_vec_section.type = SECTION;
	int_vec_section.dom = DOMAIN;
	int_vec_section.vAddress = INT_VEC;
	int_vec_section.AP = RWRW;
	int_vec_section.CB = cb;
	int_vec_section.pAddress = INT_VEC;
	int_vec_section.PT = &masterTable;
	writeSectionsToMemory(&int_vec_section, 1);

	mmu_first_level_desc_t kernel_section;
	kernel_section.type = SECTION;
	kernel_section.dom = DOMAIN;
	kernel_section.vAddress = KERNEL_START;
	kernel_section.AP = RWNA;
	kernel_section.CB = WB;
	kernel_section.pAddress = KERNEL_START;
	kernel_section.PT = &masterTable;
	writeSectionsToMemory(&kernel_section, (KERNEL_SIZE / SECTION_PAGE_SIZE));

	mmu_first_level_desc_t master_pt_section;
	master_pt_section.type = SECTION;
	master_pt_section.dom = DOMAIN;
	master_pt_section.vAddress = TASK_L1_PT_START;
	master_pt_section.AP = RWNA;
	master_pt_section.CB = WB;
	master_pt_section.pAddress = TASK_L1_PT_START;
	master_pt_section.PT = &masterTable;
	writeSectionsToMemory(&master_pt_section, (PAGE_TABLE_REGION_SIZE / TASK_L1_PT_SIZE));


	mmu_first_level_desc_t pt_l2_section;
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
static void writeSectionsToMemory(mmu_first_level_desc_t* section, uint32_t numOfSections) {
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

static void write_coarse_page_table_to_memory(mmu_first_level_desc_t* desc, uint32_t nr_of_pages) {
	uint32_t* tablePos = (uint32_t*) desc->PT->ptAddress;
	tablePos += desc->vAddress >> 20;
	tablePos += nr_of_pages - 1;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = desc->pAddress & 0xFFFFFC00;
	entry |= desc->dom >> 4;
	//entry |= (desc->CB & 0x3) << 2;
	entry &= 0xFFFFFFFD;
	entry |= 0x1 ; //0b01 -> coarse table descriptor

	int i;
	for (i = nr_of_pages - 1; i >= 0; i--) {
		*tablePos-- = entry + (i << 10);
	}
}

static void write_small_pages_to_memory(mmu_second_level_desc_t* desc, uint32_t nr_of_pages) {
	uint32_t* tablePos = (uint32_t*) desc->PT->ptAddress;
	uint32_t vAddr = desc->first_level_desc->vAddress;
	vAddr &= 0x1FF000;
	vAddr = vAddr >> 10;

	tablePos += vAddr;
	tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

	uint32_t entry = desc->first_level_desc->pAddress & 0xFFFFF000;
	entry |= desc->AP3 << 10;
	entry |= desc->AP2 << 8;
	entry |= desc->AP1 << 6;
	entry |= desc->AP0 << 4;

	entry |= desc->CB << 2;

	//tablePos += desc->vAddress >> 20;
	//tablePos += nr_of_pages - 1;
	//tablePos = (uint32_t*)((uint32_t) tablePos & 0xFFFFFFFC);
	int i;
	for (i = nr_of_pages -1; i >= 0; i--) {
		*tablePos-- = entry + (i << 12);
	}
	//uint32_t entry = table->
}

