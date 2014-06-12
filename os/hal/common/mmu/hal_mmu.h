/*
 * hal_mmu.h
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#ifndef HAL_MMU_H_
#define HAL_MMU_H_

#include <inttypes.h>

#define MAX_L2_TABLES 1024

typedef struct {
	uint32_t vAddress;
	uint32_t ptAddress;
	uint32_t masterPtAddress;
	uint32_t type;
	uint32_t dom; // domain

} mmu_l2_pagetable_t;

typedef struct {
	uint32_t vAddress;
	uint32_t ptAddress;
	uint32_t type;
	uint32_t dom; // domain
	mmu_l2_pagetable_t mmu_l2_tables[MAX_L2_TABLES];
} mmu_master_pagetable_t;

typedef struct {
	uint32_t vAddress;
	uint32_t pageSize;
	uint32_t numPages;
	uint32_t AP;
	uint32_t CB;
	uint32_t pAddress;
	mmu_master_pagetable_t *PT;
} mmu_region_t;


void hal_mmu_activate(void);
void hal_mmu_init(void);

void hal_mmu_addProcess(uint16_t processId, uint8_t processSize);
void hal_mmu_removeProcess(uint16_t processId) ;


#endif /* HAL_MMU_H_ */
