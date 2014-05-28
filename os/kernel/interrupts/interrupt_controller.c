#include <stdio.h>
#include "interrupt_controller.h"
#include "interrupt.h"
#include "../arch/command.h"
#include "../arch/address.h"

//includes of modules
//potential interrupt sources
#include "../timer/gptimer.h"

interrupt_handler handlers[MAX_INTERRUPT_VECTORS];

uint32_t active_interrupt = 0;


void dummy_handler(void);
void dummy_handler(void){
	//no handler set
}

void init_interrupt_controller(){
	uint32_t i;
	for(i = 0; i < NR_INTERRUPTS; i++){
		handlers[i] = dummy_handler;
	}
	//TODO initialize interrupt handling

}

void enable_all_interrupts(){
	_enable_interrupts();
}

void disable_all_interrupts(){
	_disable_interrupts();
}

void enable_irq(){
	_enable_IRQ();
}

void disable_irq(){
	_disable_IRQ();
}

void enable_fiq(){
	_enable_FIQ();
}

void disable_fiq(){
	_disable_FIQ();
}

void set_interrupt_handler(uint32_t int_nr, interrupt_handler handler){
	handlers[int_nr] = handler;
}

void remove_interrupt_handler(uint32_t int_nr){
	handlers[int_nr] = dummy_handler;
}

void _handle_current_interrupt(){
	handlers[active_interrupt]();
}


uint32_t get_active_interrupt(void){
	return BIT_READ_MASK(MPU_INTC,SIR_IRQ,BIT_MASK(1111111)); //TODO INTCPS_SIR_IRQ/FIQ (active interrupt) oder INTCPS_PENDING_IRQn (pending interrupt) lesen
}

void __identify_and_clear_source(){
	// bit 0-6 of MPU_INTC SIR_IRQ Register is the # of the current active IRQ interrupt
	active_interrupt = get_active_interrupt();
	//TODO implement clearing interrupts
	switch(active_interrupt){
		case 40: //GPTIMER4
			BIT_CLEAR(GPTIMER4,TISR,0);
			BIT_CLEAR(GPTIMER4,TISR,1);
			break;
		default:
			// no implementation
			break;
	}
	//reset_interrupt_module(); //workaround
	//re_init_interrupt_module(); //workaround
}


#pragma INTERRUPT(udef_handler, UDEF)
interrupt void udef_handler() {
	printf("udef_handler interrupt\n");
}

#pragma INTERRUPT(swi_handler, SWI)
interrupt void swi_handler() {
	printf("swi_handler interrupt\n");
}

#pragma INTERRUPT(pabt_handler, PABT)
interrupt void pabt_handler() {
	printf("pabt_handler interrupt\n");
}

#pragma INTERRUPT(dabt_handler, DABT)
interrupt void dabt_handler() {
	printf("dabt_handler interrupt\n");
}


//#pragma INTERRUPT(irq_handler, IRQ)
//interrupt
void irq_handler() {
	_disable_interrupts();
	_handle_current_interrupt();
	reset_irq();
	timer_reset_counter(GPTIMER4);
}


//#pragma INTERRUPT(fiq_handler, FIQ)
//interrupt
void fiq_handler() {
	//_handle_current_interrupt();
	//printf("fiq_handler interrupt\n");
}


