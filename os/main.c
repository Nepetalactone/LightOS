#include "kernel/arch/command.h"
#include "kernel/timer/gptimer.h"
#include "kernel/interrupts/interrupt_controller.h"
#include "kernel/interrupts/interrupt.h"
#include "kernel/scheduler/process.h"
#include "kernel/scheduler/scheduler.h"
#include "kernel/mmu/mmu.h"
#include "kernel/elfloader/loader.h"


void asdf(){
	timer_reset_counter(GPTIMER4);
}

int main(){
	_disable_interrupts();
//TODO tobi clear warnings
	mmu_init();
	mmu_activate();

	reset_interrupt_module();
	init_interrupt_controller();

	//TODO move to interrupt controller
	uint32_t i = 0;
	while(i < 95){
		BIT_CLEAR(MPU_INTC,ILR(i),0);
		i++;
	}

	init_scheduler(GPTIMER4);
	mmu_init_process(process_create("procA", &proc_led_on));
	mmu_init_process(process_create("procB", &proc_led_off));
	//loadTaskFromFile("sys/blink.out");
	_enable_interrupts();
	start_scheduling();
}
