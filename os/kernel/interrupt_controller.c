#include "interrupt_controller.h"
#include "command.h"
#include <stdio.h>


interrupt_handler handlers[NR_INTERRUPTS];

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

void enable_mir_(uint32_t mir_nr){
	//BIT_CLEAR(MPU_INTC,MIR_CLEAR(mir_nr));
}

void disable_mir_(uint32_t mir_nr){
	//TODO
}

void set_interrupt_handler(uint32_t int_nr, interrupt_handler handler){
	handlers[int_nr] = handler;

}

void remove_interrupt_handler(uint32_t int_nr){
	handlers[int_nr] = dummy_handler;
}

void handle_current_interrupt(uint32_t interrupt_nr){
	//get active interrupt nr
	//call handler
	handlers[interrupt_nr]();
	//reset interrupt pending bit

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

#pragma INTERRUPT(irq_handler, IRQ)
interrupt void irq_handler() {
	handle_current_interrupt(BIT_READ(MPU_INTC,SIR_IRQ, (32-7), 7));
	printf("irq_handler interrupt\n");
}

#pragma INTERRUPT(fiq_handler, FIQ)
interrupt void fiq_handler() {
	handle_current_interrupt(BIT_READ(MPU_INTC,SIR_FIQ, (32-7), 7));
	printf("fiq_handler interrupt\n");
}

