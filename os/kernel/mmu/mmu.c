/*
 * mmu.c
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#include "mmu.h"
#include "../../hal/common/mmu/hal_mmu.h"

void mmu_activate(void) {
	hal_mmu_activate();
}

void mmu_init(void) {
	hal_mmu_init();
}
