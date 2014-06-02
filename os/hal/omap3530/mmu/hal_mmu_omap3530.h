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
extern void hal_mmu_activate_asm();
extern void hal_mmu_enable_write_buffer_asm();
extern void hal_mmu_set_ttbr_ctrl_bits_asm();
extern void hal_mmu_set_ttbr_0_asm(uint32_t trans_table_base, uint32_t id);
extern void hal_mmu_set_ttbr_1_asm(uint32_t trans_table_base);
extern void hal_mmu_set_ttbr_cr_n_asm(uint32_t N);


#define N 0x2

/* master pagetable base-address */
//#define MASTER_PT_START 0xA0000000
#define MASTER_PT_START		0x80000000
#define MASTER_PT_SIZE 		0x1000 // 16kB

/* task pagetables base-address */
#define TASKS_PT_START MASTER_PT_START + MASTER_PT_SIZE

/* pagetable definitions */
#define PT_START MASTER_PT_START
#define PT_SIZE 0x1000
#define PT_PAGE_SIZE 0x100
unsigned int VM_START;

/* task definitions */
#define TASK_PT_SIZE 0x100
#define TASK_PAGE_SIZE 0x100
#define TASK_SIZE 0x100
#define TASKS_START TASKS_PT_START + PT_SIZE /* base address of tasks */

/* hw definitions */
#define HW_START 0x81000000
#define HW_PAGE_SIZE 0x100
#define HW_SIZE 0x1000

/* os definitions */
#define OS_START 0x82000000
#define OS_PAGE_SIZE 0x100
#define OS_SIZE 0x1000


#endif /* HAL_MMU_OMAP3530_H_ */
