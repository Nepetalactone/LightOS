#ifndef INT_CONTRL_
#define INT_CONTRL_


#include "interrupt.h"
#include <stdint.h>



void init_interrupt_controller(void);
void set_interrupt_handler(uint32_t int_nr, irq_interrupt_handler handler);
void remove_interrupt_handler(uint32_t int_nr);
void enable_irq();
uint32_t get_active_interrupt(int type);




#endif
