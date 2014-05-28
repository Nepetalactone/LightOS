/*
 * hal_mmu.h
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#ifndef HAL_MMU_H_
#define HAL_MMU_H_

#include <inttypes.h>



typedef struct {
	unsigned int vAddress;
	unsigned int ptAddress;
	unsigned int masterPtAddress;
	unsigned int type;
	unsigned int dom; // domain
} mmu_pagetable_t;

typedef struct {
	unsigned int vAddress;
	unsigned int pageSize;
	unsigned int numPages;
	unsigned int AP;
	unsigned int CB;
	unsigned int pAddress;
	mmu_pagetable_t *PT;
} mmu_region_t;


#define FAULT 0
#define COARSE 1
#define MASTER 2
#define FINE 3

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

void hal_mmu_activate(void);
void hal_mmu_init(void);

void hal_mmu_addProcess(uint16_t processId);
void hal_mmu_removeProcess(uint16_t processId) ;





#endif /* HAL_MMU_H_ */
