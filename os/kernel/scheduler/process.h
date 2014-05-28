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
	void* pc;
} process_t;

void proc_led_off();
void proc_led_on();

#endif /* PROCESS_H_ */
