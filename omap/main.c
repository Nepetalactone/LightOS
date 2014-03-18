/*
 * main.c
 */

#include "./kernel/interrupts/interrupt.h"
#include "kernel/timer.h"

int main(void) {
	init_isr_handler();
	gptimer_t * timer = request_new_timer_by_id(2);
	enable_timer_interrupt(timer);
	clear_match(timer);
	set_match(timer, 0x10000000);

	//set interrupt unmasked
	unmask_mir0();

	//
	mir0_clear();


	start_timer(timer);
	while(1)
	{
	}
	return 0;
}
