/*
 * main.c
 */
#include <stdlib.h>
#include "kernel/timer.h"
#include "kernel/interrupt_controller.h"
#include "kernel/interrupt.h"
#include <stdio.h>
#include "kernel/command.h"
#include "kernel/address.h"

void asdf(void);
void asdf(){
	exit(1);
}
#define MPU_INTC (base_address) 0x48200000
#define MIR0 (offset)        	0x084 /* Interrupt mask */
#define MIR_CLEAR0 (offset)  	0x088 /* Clear interrupt mask bits */
#define MIR_CLEAR1 (offset)  	0x0A8 /* Clear interrupt mask bits */
#define MIR_CLEAR2 (offset)  	0x0C8 /* Clear interrupt mask bits */
#define ILR0 (offset)        	0x100 /* Priority for interrupts */
#define ILR4 (offset)        	0x110 /* Priority for interrupts */

int main(void) {

	_disable_interrupts();


	init_interrupt_controller();
	set_interrupt_handler(0, asdf);


	//TODO p.2619
	//software reset, only use timer when TISTAT[0] bit is set(?)
	//enable compare irgendwo unterbringen BIT_SET(GPTIMER4,TCLR,6);
	reset_timer(GPTIMER4);
	init_timer(GPTIMER4,0x20000000,asdf,trigger_OverflowMatch);


	//interrupt mode fiq
	uint32_t interrupt_nr = get_interrupt_nr(GPTIMER4);
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



