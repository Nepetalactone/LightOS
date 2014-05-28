/*
 * mmu.h
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#ifndef MMU_H_
#define MMU_H_

#include "../scheduler/process.h"

/* mmu startup */
void mmu_activate(void);
void mmu_init(void);

process_t mmu_init_process(void);
process_t mmu_start_process(process_t);
process_t mmu_kill_process(process_t);



#endif /* MMU_H_ */
