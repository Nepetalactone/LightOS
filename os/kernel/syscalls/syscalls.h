/*
 * Syscalls.h
 *
 *  Created on: 11.06.2014
 *      Author: Tobias
 *
 */

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include "../scheduler/process.h"
#include "../timer/gptimer.h"

//====================================Timercalls====================================

//Initializes a timer
base_address init_timer(uint32_t millisec, irq_interrupt_handler handler);

//Releases a timer
void release_timer(base_address timer);

//====================================Processcalls==================================

//Clones the current process
uint32_t fork();

//Creates a new process
uint32_t new_process();

//Terminates the specified process
uint32_t terminate_process(process_id id);

//====================================Memorycalls===================================

//Allocates a chunk of memory of the specified size
uint32_t allocate(int size);

//Frees a chunk of memory
uint32_t free(uint32_t address);

//====================================Filecalls===================================== TODO

#endif /* SYSCALLS_H_ */
