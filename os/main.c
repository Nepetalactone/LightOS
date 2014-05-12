/*
 * main.c
 */
#include <stdlib.h>
#include "kernel/timer.h"
#include "kernel/interrupts/interrupt_controller.h"
#include "kernel/interrupts/interrupt.h"
#include <stdio.h>
#include "kernel/arch/command.h"
#include "kernel/arch/address.h"
#include "kernel/scheduler/process.h"

void timer_interrupt(void);
void timer_interrupt(){
	printf("test");
}

int main(void) {
	volatile uint8_t y =  BIT_READ(MPU_INTC, SIR_FIQ, (32-7), 7);
	_disable_interrupts();


	init_interrupt_controller();
	uint32_t interrupt_nr = get_interrupt_nr(GPTIMER4);
	set_interrupt_handler(interrupt_nr, timer_interrupt);


	//TODO p.2619
	//software reset, only use timer when TISTAT[0] bit is set(?)
	//enable compare irgendwo unterbringen BIT_SET(GPTIMER4,TCLR,6);
	reset_timer(GPTIMER4);
	init_timer(GPTIMER4,0x20000000,timer_interrupt,trigger_OverflowMatch);


	//interrupt mode fiq
	set_interrupt_mode(interrupt_nr,FIQ);

	//unmask_mir
	unmask_interrupts(MPU_INTC, interrupt_nr);

	//*((address)(MPU_INTC + MIR_CLEAR0)) &= 0x0;
	start_timer(GPTIMER4);
	_enable_interrupts();

	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
	//exit(0);
}



