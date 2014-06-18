/*
 * main.c
 */
#include <stdlib.h>
#include <stdio.h>
#include "kernel/timer.h"
#include "kernel/interrupts/interrupt_controller.h"
#include "kernel/interrupts/interrupt.h"
#include "kernel/arch/command.h"
#include "kernel/arch/address.h"
#include "kernel/scheduler/process.h"

#include "kernel/mmu/mmu.h"


int main(void) {


	init_interrupt_controller();

	mmu_init();
	mmu_activate();

	process_t* proc = malloc(sizeof(proc));
	proc->procId = 1;
	proc->size = 16;

	mmu_init_process(proc);

	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
	//exit(0);
}



