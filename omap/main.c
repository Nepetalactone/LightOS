/*
 * main.c
 */

#include "./kernel/interrupts/interrupt.h"
#include "kernel/timer.h"

int main(void) {

	_enable_interrupts();

	init_isr_handler();

	
	gptimer_t * timer = request_new_timer_by_id(2);
	enable_timer_interrupt(timer);
	clear_match(timer);
	set_match(timer, 0x00000001);

	//set interrupt unmasked
	unmask_mir0();

	//
	mir0_clear();
	start_timer(timer);


	// serial
	void serial_test(void); // prototype
	serial_test();

	while(1)
	{
	}
	return 0;
}
