/*
 * hal_mmu.h
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#ifndef HAL_MMU_H_
#define HAL_MMU_H_

#include <inttypes.h>
#include "../../../kernel/scheduler/process.h"

#define MAX_L2_TABLES 1024

typedef struct {
	uint32_t ptAddress;
	uint32_t masterPtAddress;
} mmu_pagetable_t;



typedef struct {
	uint32_t vAddress;
	uint32_t pAddress;
	uint32_t AP;
	uint32_t CB;
	uint32_t type;
	uint32_t dom; // domain
	mmu_pagetable_t *PT;
} mmu_first_level_desc_t;


typedef struct {
	//uint32_t vAddress;
	//uint32_t pAddress;
	uint32_t AP0;
	uint32_t AP1;
	uint32_t AP2;
	uint32_t AP3;
	uint32_t CB;
	uint32_t type;
	mmu_pagetable_t *PT;
	mmu_first_level_desc_t* first_level_desc;
} mmu_second_level_desc_t;


void hal_mmu_activate(void);
void hal_mmu_init(void);

void hal_mmu_addProcess(process_t* proc);
void hal_mmu_removeProcess(uint16_t processId) ;


#endif /* HAL_MMU_H_ */
