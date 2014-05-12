/*
 * mmu.h
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#ifndef MMU_H_
#define MMU_H_

#include "../scheduler/process.h"

/* functions */
void mmu_init(void);
void mmu_start(void);

process_t mmu_init_process(void);
process_t mmu_start_process(process_t);
process_t mmu_kill_process(process_t);



#endif /* MMU_H_ */
