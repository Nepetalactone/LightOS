#ifndef PROCESS_H_
#define PROCESS_H_

#include "stdint.h"

typedef uint32_t process_id;

typedef enum process_state {
	RUNNING,
	READY,
	BLOCKED
} process_state_t;

typedef struct {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;
	uint32_t pc;
	uint32_t psr;
} hw_stack_frame_t;

typedef struct {
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
} sw_stack_frame_t;


typedef struct {
	void * sp; //stack pointer..
	uint32_t page_table; // virutal memory.. mmu
	int flags; // status flags includes activity status
} process_table_t;

typedef struct process {
	process_id procId;
	process_state_t state;
	hw_stack_frame_t hw_stack;
	sw_stack_frame_t sw_stack;
	process_table_t proc_table;
} process_t;

void proc_led_off();
void proc_led_on();

#endif /* PROCESS_H_ */
