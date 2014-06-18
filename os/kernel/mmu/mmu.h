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


/* mmu process handling */
void mmu_init_process(process_t* proc);
void mmu_start_process(uint16_t processId);
void mmu_kill_process(uint16_t processId);



#endif /* MMU_H_ */
