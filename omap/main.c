/*
 * main.c
 */

#include "./kernel/interrupt.h"
#include "kernel/timer.h"

int main(void) {
	gptimer_t * timer = request_timer_by_id(2);
	enable_timer_interrupt(timer);
	start_timer(timer);
	while(1)
	{
	}
	return 0;
}
