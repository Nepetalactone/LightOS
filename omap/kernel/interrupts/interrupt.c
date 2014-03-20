
#include "../arch/address.h"
#include "interrupt.h"
//Ich geh�re weg
#include <stdio.h>

//static address irq_table[MAX_INTERRUPT_VECTORS];

static irq_hook_t* irq_lines[MAX_INTERRUPT_VECTORS];

/*
 * IRQ Interrupt Service Routine
 */
uint32_t irq_isr(void){
	//uint8_t activeIRQ = *get_bits(IRQ_LINE, 7);
	printf("test");
	// active irq number -> first 7 bits from ACTIVE_IRQ
	return 0;
}

/*
 * FIQ Interrupt Service Routine
 */
uint32_t fiq_isr(void) {
	//uint8_t activeFIQ = (uint8_t) (ACTIVE_FIQ);
	return 0;
}

void get_irq_line(uint8_t line){

}


void clear_irq_line(uint8_t line) {

}

void init_isr_handler(void){
	*(ISR_IRQ_ADDRESS) = (uint32_t) &irq_isr;
	*(FIQ_IRQ_ADDRESS) = (uint32_t) &fiq_isr;
	uint32_t asdf = *(ISR_IRQ_ADDRESS);
}

void unmask_mir0(void){
	*((address)(MPU_INTC + MIR0)) &= 0x0;
}

void mask_mir0(void){
	*((address)(MPU_INTC + MIR0)) |= 0x1;
}

void mir0_clear(void){
	*((address)(MPU_INTC + MIR_CLEAR0)) &= 0x0;
}

void mir0_set(void){
	*((address)(MPU_INTC + MIR_SET0)) |= 0x1;
}


void enable_irq_line(void) {

}

void isr_clear(void) {
	*((address)(MPU_INTC + ISR_CLEAR0)) &= 0x0;
}

/*
  void put_irq_handler(irq_hookt_t *line, int irq, uint_32 (*handler)(struct irq_hook)) {
	if (irq > MAX_INTERRUPT_VECTORS) {
		return;
	}
	line->handler = handler;
	irq_lines[irq] = line;
}
 */



//void inline __enable_interrupts__() {
//	_enable_interrupts();
//}


/* number of lists of IRQ hooks, one list per supported line. */
//static irq_hook_t* irq_handlers[NR_IRQ_VECTORS] = { 0 };
//
///*===========================================================================*
// *				put_irq_handler				     *
// *===========================================================================*/
///* Register an interrupt handler.  */
//void put_irq_handler(irq_hook_t* hook, int irq, const irq_handler_t handler) {
//	int id;
//	irq_hook_t **line;
//	unsigned long bitmap;
//
//	if (irq < 0 || irq >= NR_IRQ_VECTORS)
//		panic("invalid call to put_irq_handler: %d", irq);
//
//	line = &irq_handlers[irq];
//
//	bitmap = 0;
//	while (*line != NULL) {
//		if (hook == *line)
//			return; /* extra initialization */
//		bitmap |= (*line)->id; /* mark ids in use */
//		line = &(*line)->next;
//	}
//
//	/* find the lowest id not in use */
//	for (id = 1; id != 0; id <<= 1)
//		if (!(bitmap & id))
//			break;
//
//	if (id == 0)
//		panic("Too many handlers for irq: %d", irq);
//
//	hook->next = NULL;
//	hook->handler = handler;
//	hook->irq = irq;
//	hook->id = id;
//	*line = hook;
//
//	/* And as last enable the irq at the hardware.
//	 *
//	 * Internal this activates the line or source of the given interrupt.
//	 */
//	if ((irq_actids[hook->irq] &= ~hook->id) == 0) {
//		hw_intr_used(irq);
//		hw_intr_unmask(hook->irq);
//	}
//}
//
///*===========================================================================*
// *				rm_irq_handler				     *
// *===========================================================================*/
///* Unregister an interrupt handler.  */
//void rm_irq_handler(const irq_hook_t* hook) {
//	const int irq = hook->irq;
//	const int id = hook->id;
//	irq_hook_t **line;
//
//	if (irq < 0 || irq >= NR_IRQ_VECTORS)
//		panic("invalid call to rm_irq_handler: %d", irq);
//
//	/* remove the hook  */
//	line = &irq_handlers[irq];
//	while ((*line) != NULL) {
//		if ((*line)->id == id) {
//			(*line) = (*line)->next;
//			if (irq_actids[irq] & id)
//				irq_actids[irq] &= ~id;
//		} else {
//			line = &(*line)->next;
//		}
//	}
//
//	/* Disable the irq if there are no other handlers registered.
//	 * If the irq is shared, reenable it if there is no active handler.
//	 */
//	if (irq_handlers[irq] == NULL) {
//		hw_intr_mask(irq);
//		hw_intr_not_used(irq);
//	} else if (irq_actids[irq] == 0) {
//		hw_intr_unmask(irq);
//	}
//}
//
///*===========================================================================*
// *				irq_handle				     *
// *===========================================================================*/
///*
// * The function first disables interrupt is need be and restores the state at
// * the end. Before returning, it unmasks the IRQ if and only if all active ID
// * bits are cleared, and restart a process.
// */
//void irq_handle(int irq) {
//	irq_hook_t * hook;
//
//	/* here we need not to get this IRQ until all the handlers had a say */
//	assert(irq >= 0 && irq < NR_IRQ_VECTORS);
//	hw_intr_mask(irq);
//	hook = irq_handlers[irq];
//
//	/* Check for spurious interrupts. */
//	if (hook == NULL) {
//		static int nspurious[NR_IRQ_VECTORS], report_interval = 100;
//		nspurious[irq]++;
//		if (nspurious[irq] == 1 || !(nspurious[irq] % report_interval)) {
//			printf("irq_handle: spurious irq %d (count: %d); keeping masked\n",
//					irq, nspurious[irq]);
//			if (report_interval < INT_MAX / 2)
//				report_interval *= 2;
//		}
//		return;
//	}
//
//	/* Call list of handlers for an IRQ. */
//	while (hook != NULL) {
//		/* For each handler in the list, mark it active by setting its ID bit,
//		 * call the function, and unmark it if the function returns true.
//		 */
//		irq_actids[irq] |= hook->id;
//
//		/* Call the hooked function. */
//		if ((*hook->handler)(hook))
//			irq_actids[hook->irq] &= ~hook->id;
//
//		/* Next hooked function. */
//		hook = hook->next;
//	}
//
//	/* reenable the IRQ only if there is noctive handler */
//	if (irq_actids[irq] == 0)
//		hw_intr_unmask(irq);
//
//	hw_intr_ack(irq);
//}
//
///* Enable/Disable a interrupt line.  */
//void enable_irq(const irq_hook_t *hook) {
//	if ((irq_actids[hook->irq] &= ~hook->id) == 0) {
//		hw_intr_unmask(hook->irq);
//	}
//}
//
///* Return true if the interrupt was enabled before call.  */
//int disable_irq(const irq_hook_t *hook) {
//	if (irq_actids[hook->irq] & hook->id) /* already disabled */
//		return 0;
//	irq_actids[hook->irq] |= hook->id;
//	hw_intr_mask(hook->irq);
//	return TRUE;
//}