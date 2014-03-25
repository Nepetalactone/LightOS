/*
 * main.c
 */

#include "./kernel/interrupts/interrupt.h"
#include "kernel/timer.h"

int main(void) {
	_enable_interrupts();
	init_isr_handler();
	gptimer_t * timer = request_new_timer_by_id(2);
	clear_match(timer);
	set_match(timer, 0x10000000);
	set_trigger_output_mode(timer);


	//TODO clear CPSR

	//set interrupt unmasked
	unmask_mir0();
	enable_timer_interrupt(timer);


	start_timer(timer);
	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
		int y =x;
	}
	return 0;
}
