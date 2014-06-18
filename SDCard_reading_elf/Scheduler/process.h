/*
 * process.h
 *
 *  Created on: 14.06.2014
 *      Author: phil
 */
#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>
#include <stdio.h>
#include "../arch/address.h"

typedef uint32_t process_id;

typedef enum process_state {
	READY,
	RUNNING,
	BLOCKED,
	ZOMBIE
} process_state_t;

typedef struct  {
	process_id pID;
	process_state_t state;
	char* name;
	void* sp;
	void* pc;
	uint32_t* pageTable;
} process_t;


typedef struct {
	uint32_t cpsr;
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t r13; //SP
	uint32_t r14; //LR
	uint32_t r15; //PC
} process_stack_t;

void proc_led_off();
void proc_led_on();

#endif /* PROCESS_H_ */
