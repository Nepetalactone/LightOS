#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>
#include <stdio.h>

typedef uint32_t process_id;

typedef enum process_state {
	READY,
	RUNNING,
	BLOCKED
} process_state_t;

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
	uint32_t r13;
	uint32_t r14; //pc
} pcb_t;


typedef struct {
	void * sp; //stack pointer..
	uint32_t page_table; // virtual memory.. mmu
	int flags; // status flags includes activity status
} process_table_t;

typedef struct  {
	process_id pID;
	process_state_t state;
	uint16_t times_loaded;
	char* name;
	void* pc;
	unsigned int * sp;
	pcb_t pcb;
	process_table_t proc_table;
} process_t;

void proc_led_off();
void proc_led_on();

#endif /* PROCESS_H_ */
