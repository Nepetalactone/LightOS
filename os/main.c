#include "kernel/arch/command.h"
#include "kernel/timer/gptimer.h"
#include "kernel/interrupts/interrupt_controller.h"
#include "kernel/interrupts/interrupt.h"
#include "kernel/scheduler/process.h"
#include "kernel/scheduler/scheduler.h"

void asdf(){

}



int main(){


	_disable_interrupts();
	init_interrupt_controller();
	reset_interrupt_module();


	timer_quick_init(GPTIMER4,0x20000000,asdf,trigger_OverflowMatch);

	uint32_t i = 0;
	while(i < 95){
		BIT_CLEAR(MPU_INTC,ILR(i),0);
		i++;
	}
	init_scheduler(GPTIMER4);
	fork("procA", &proc_led_on);
	fork("procB", &proc_led_off);

	timer_start(GPTIMER4);
	start_scheduling();
}
