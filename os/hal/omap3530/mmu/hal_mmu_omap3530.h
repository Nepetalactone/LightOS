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
#define MASTER 0x02 /* 4KB of virtual addresses */
#define FINE 0x03   /* 1KB of virtual addresses */


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
//extern void hal_mmu_activate_asm();
//extern void hal_mmu_enable_write_buffer_asm();
//extern void hal_mmu_set_ttbr_ctrl_bits_asm();
//extern void hal_mmu_set_ttbr_0_asm();
//extern void hal_mmu_set_ttbr_1_asm();


/* master pagetable base-address */
//#define MASTER_PT_START 0xA0000000
#define MASTER_PT_START		0x89500000
#define MASTER_PT_SIZE 		0x4000 // 16kB

/* task pagetables base-address */
#define TASKS_PT_START		MASTER_PT_START + MASTER_PT_SIZE

/* pagetable definitions */
#define PT_START MASTER_PT_START
#define PT_SIZE 0x4000
#define PT_PAGE_SIZE 0x1000
#define VM_START 0x89500000

/* task definitions */
#define TASK_PT_SIZE 0x1000	// 4KB
#define TASK_PAGE_SIZE 0x400 // 1KB
#define TASK_SIZE 0x2000 //8KB
#define TASKS_START TASKS_PT_START + PT_SIZE /* base address of tasks */

/* hw definitions */
#define HW_START 0x82020000
#define HW_PAGE_SIZE 0x1000
#define HW_SIZE 0x18000
//#define HW_SIZE 0x0EAFFFFF

/* os definitions */
#define OS_START 0x8F800000
#define OS_PAGE_SIZE 0x100
#define OS_SIZE 0x800000		// 8 MB

/* sdram controller - ram-size (RAM address space size number of 2-MB chunks) */
#define SDRC_MCFG	0x6D000080
#define SDRAM_RAM_SIZE_256 0x100

#endif /* HAL_MMU_OMAP3530_H_ */
