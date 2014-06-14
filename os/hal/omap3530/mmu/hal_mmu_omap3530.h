/*
 * hal_mmu_omap3530.h
 *
 *  Created on: 28.05.2014
 *      Author: Christian
 */

#ifndef HAL_MMU_OMAP3530_H_
#define HAL_MMU_OMAP3530_H_

#include "../../common/mmu/hal_mmu.h"

/* pagetable descriptor size */
#define FAULT 0x00  /* associated virtual addresses are unmapped */
#define COARSE 0x01 /* 64KB of virtual addresses */
#define SECTION 0x02 /* 4KB of virtual addresses */
#define FINE 0x03   /* 1KB of virtual addresses */
#define SMALL 0x02

#define SMALL_PAGE_SIZE 		0x1000
#define SECTION_PAGE_SIZE		0x100000
#define COARSE_PAGETABLE_SIZE   0x400

#define NANA 0x00
#define RWNA 0x01
#define RWRO 0x02
#define RWRW 0x03
/*	NA	=	no	access,	RO	=	read	only,	RW	=	read/write	*/

#define	cb	0x0
#define	cB	0x1
#define	WT	0x2
#define	WB	0x3
/*	cb	=	not	cached/not	buffered	*/
/*	cB	=	not	Cached/Buffered	*/
/*	Cb	=	Cached/not Buffered	*/
/*	WT	=	write	through	cache	*/
/*	WB	=	write	back	cache	*/

/* declare asm functions */
extern void hal_mmu_activate_asm();
extern void hal_mmu_enable_write_buffer_asm();
extern void hal_mmu_set_ttbr_ctrl_bits_asm(uint32_t ctrl_bits);
extern void hal_mmu_set_ttbr_0_asm(uint32_t ttbr_address, uint32_t context_id);
extern void hal_mmu_set_ttbr_1_asm(uint32_t ttbr_address);
extern void hal_mmu_set_domain(uint32_t domain_type);

#define MAX_L2_TABLES	1024
#define MAX_PROCESS_COUNT	100

#define DOMAIN 0

/* hw definitions */
#define HW_START 			0x48000000
#define HW_PAGE_SIZE 		0x1000 // 4KB
#define HW_SIZE 			0x1000000 // 16 MB

/* os definitions */
#define KERNEL_START		0x82000000
#define KERNEL_SIZE			0x800000		// 8 MB
#define KERNEL_SECTION_SIZE	0x100000

/* master pagetable base-address */
#define PAGE_TABLE_REGION_SIZE			0x500000	//5MB

//start page table region
#define OS_L1_PT_START		(KERNEL_START + KERNEL_SIZE)
#define OS_L1_PT_SIZE 		0x4000 // 16kB

/* task pagetables base-address */
#define TASK_L1_PT_START			(OS_L1_PT_START + OS_L1_PT_SIZE)
#define TASK_L1_PT_SIZE 			0x4000	// 16KB

/* 1 MASTER PT for each process*/
#define TASK_L2_PT_START			TASK_L1_PT_START + (TASK_L1_PT_SIZE * MAX_PROCESS_COUNT)
#define TASK_L2_PT_SIZE				0x400	// 1KB
#define TASK_L2_PT_END				TASK_L2_PT_START + (MAX_L2_TABLES * TASK_L2_SIZE)


/* task definitions */
#define TASKS_START 		TASK_L2_PT_START + (MAX_L2_TABLES * TASK_L2_PT_SIZE) /* base address of tasks */
#define TASK_PAGE_SIZE 		0x400 // 1KB L2 Page-Table
#define TASK_REGION_SIZE 	0x53FFFFF //82 MB
//#define TASK_SIZE 0x2000 //8KB

/* pagetable definitions */
//#define TASK_L2_START 		(TASK_L1_PT_START + OS_MASTER_PT_SIZE)
//#define PT_SIZE 			0x4000
//#define PT_PAGE_SIZE 		0x1000

//#define VM_START 0x00
#define VM_TASK_START 0x00004000


//#define OS_PAGE_SIZE 	0x100


/* sdram controller - ram-size (RAM address space size number of 2-MB chunks) */
#define SDRC_MCFG	0x6D000080
#define SDRAM_RAM_SIZE_256 0x100

#endif /* HAL_MMU_OMAP3530_H_ */
