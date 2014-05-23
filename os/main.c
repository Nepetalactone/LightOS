/*
 * main.c
 */

#include "kernel/timer/gptimer.h"
#include "kernel/arch/command.h"

void handler_func(void);
void handler_func(){
	timer_reset_counter(GPTIMER4);
}

int main(void) {


	_disable_interrupts();
	init_interrupt_controller();
	reset_interrupt_module();


	timer_quick_init(GPTIMER4,0x02000000,handler_func,trigger_OverflowMatch);

	uint32_t i = 0;
	while(i < 95){
		BIT_CLEAR(MPU_INTC,ILR(i),0);
		i++;
	}

	timer_start(GPTIMER4);



	_enable_interrupts();
	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
}



