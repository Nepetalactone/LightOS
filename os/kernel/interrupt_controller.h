#ifndef INT_CONTRL_
#define INT_CONTRL_


#include "interrupt.h"
#include <stdint.h>



void init_interrupt_controller(void);
void set_interrupt_handler(uint32_t int_nr, interrupt_handler handler);
void enable_irq();





#endif
