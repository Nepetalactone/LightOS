/*
 * main.c
 */
#include "try/fat32.h"
#include "task/loader.h"
#include "mmu/mmu.h"
#include "Scheduler/scheduler.h"
#include "interrupts/interrupt_controller.h"

int main(void) {
	//_disable_interrupts();
	//reset_interrupt_module();



	if(fat32Init()){
		return 1;
	}

	init_interrupt_controller();
	enable_all_interrupts();
	init_scheduler();
	mmu_init();
	mmu_activate();

	loadTaskFromFile("sys/blink.out");

	start_scheduling();
	return 0;
}
