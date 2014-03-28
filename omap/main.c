/*
 * main.c
 */

#include "./kernel/interrupts/interrupt.h"
#include "kernel/timer.h"

int main(void) {
	_disable_interrupts();
	init_isr_handler();
	gptimer_t * timer = request_new_timer_by_id(2);
	clear_match(timer);
	set_match(timer, 0x10000000);
	set_trigger_output_mode(timer);


	//TODO clear CPSR

	//set interrupt unmasked
	unmask_mir0();
	enable_timer_interrupt(timer);

	_enable_interrupts();

	int asdfasdf = 100;
	while(asdfasdf > 0){
		asdfasdf--;
		if(asdfasdf == 1){
			volatile int qwe =0;
		}
	}


	start_timer(timer);
	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
	return 0;
}
