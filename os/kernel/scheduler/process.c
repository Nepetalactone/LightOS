#include "process.h"

typedef volatile unsigned int* address;

#define GPIO05_DIR (address) 0x49056034
#define USR0 (1<<21)
#define USR1 (1<<22)
#define GPIO05_OUT (address) 0x4905603C


void proc_led_on() {
	*(GPIO05_DIR) &= ~(USR0 | USR1);
	*(GPIO05_OUT) &= ~USR1;
	volatile int i = 0;
	while (1) {
		for(i=0; i < 300000; i++) {

		}
		*(GPIO05_OUT) ^= (USR0 | USR1);
	}
	printf("Process A is running");
}

void proc_led_off() {
	*(GPIO05_DIR) &= ~(USR0 | USR1);
	*(GPIO05_OUT) &= ~USR1;

	volatile int i = 0;

	while (1) {
		for(i=0; i < 300000; i++) {

		}
		*(GPIO05_OUT) ^= (USR0 | USR1);
	}
	printf("Process B is running");
}
