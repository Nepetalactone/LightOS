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



void hal_mmu_activate(void);
void hal_mmu_init(void);

void hal_mmu_addProcess(uint16_t processId, uint8_t processSize);
void hal_mmu_removeProcess(uint16_t processId) ;





#endif /* HAL_MMU_H_ */
