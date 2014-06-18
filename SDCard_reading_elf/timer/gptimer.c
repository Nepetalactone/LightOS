#include <stdlib.h>
#include "gptimer.h"
#include "../arch/command.h"
#include "../interrupts/interrupt_controller.h"


#define NR_TIMERS 12 //max number of GPTimer on the beagleboard is 11 (starting with 1)

static uint8_t timers[NR_TIMERS] = {2,0,0,0,0,0,0,0,0,0,0,0};

uint8_t get_timer_nr(base_address timer_addr);
void _timer_enable(base_address timer);
void _timer_disable(base_address timer);


void timer_quick_init(base_address timer, uint32_t millisec, irq_interrupt_handler handler, trigger_mode mode){
	if(is_timer_running(timer) || handler == NULL){
			return;
	}

	_timer_enable(timer);
	timer_reset_state(timer);
	timer_reset_counter(timer);
	timer_set_compare_value(timer,millisec);
	uint32_t interrupt_n = get_interrupt_nr(timer);
	set_interrupt_mode(interrupt_n,FIQ);
	unmask_interrupt(MPU_INTC, interrupt_n);
	set_interrupt_handler(interrupt_n,handler);
	timer_set_trigger_mode(timer, mode);
}

void timer_init(base_address timer, uint32_t millisec, trigger_mode mode){
	if(is_timer_running(timer)){
		return;
	}

	timer_reset_state(timer);
	timer_reset_counter(timer);
	timer_set_compare_value(timer,millisec);
	timer_set_trigger_mode(timer, mode);

}

void timer_release(base_address timer){
	uint16_t running = BIT_READ(timer,TCLR,0);
	if(running){
		return;
	}
	_timer_disable(timer);
}

void timer_start(base_address timer){
	uint16_t running = BIT_READ(timer,TCLR,0);
	if(running){
		return;
	}
	timer_enable_interrupt(timer);
	timer_enable_compare(timer);
	BIT_SET1(timer,TCLR,0);
	//WFW_BIT(timer,TCLR,0);
}

void timer_stop(base_address timer) {
	BIT_CLEAR1(timer,TCLR,0);
	//WFC_BIT(timer,TCLR,0);
	timer_disable_interrupt(timer);
}

void timer_reset_state(base_address timer){
	BIT_SET1(timer,TSICR,1);
	BIT_SET1(timer,TISR,0);
	BIT_SET1(timer,TISR,1);
	BIT_SET1(timer,TISR,2);
	BIT_CLEAR1(timer,TIER,0);
	BIT_CLEAR1(timer,TIER,1);
	BIT_CLEAR1(timer,TIER,2);
}

void timer_reset_counter(base_address timer){
	REG_CLEAR(timer,TTGR);
}

void timer_enable_compare(base_address timer){
	BIT_SET1(timer,TCLR,6);
}

void timer_set_compare_value(base_address timer, uint32_t millisec) {
	REG_SET(timer,TMAR, millisec);
}

void timer_set_trigger_mode(base_address timer, trigger_mode mode){
	BIT_CLEAR1(timer, TCLR, 10);
	BIT_CLEAR1(timer, TCLR, 11);
	BIT_SET1(timer, TCLR, mode);
}

uint8_t is_timer_running(base_address timer){
	uint8_t timer_nr = get_timer_nr(timer);
	return timers[timer_nr];
}

void _timer_enable(base_address timer){
	timers[get_timer_nr(timer)] = 1;
}

void _timer_disable(base_address timer){
	timers[get_timer_nr(timer)] = 0;
}

void timer_enable_interrupt(base_address timer) {
	//TODO: TIER Modes..
	BIT_SET1(GPTIMER4,TIER,0);
	BIT_SET1(GPTIMER4,TIER,1);
	BIT_SET1(GPTIMER4,TIER,2);
}

void timer_disable_interrupt(base_address timer) {
	BIT_SET1(GPTIMER4,TIER,0);
	BIT_SET1(GPTIMER4,TIER,1);
	BIT_SET1(GPTIMER4,TIER,2);
}

uint8_t get_timer_nr(base_address timer_addr){
	switch(timer_addr){
		case GPTIMER1:
			return 1;
		case GPTIMER2:
			return 2;
		case GPTIMER3:
			return 3;
		case GPTIMER4:
			return 4;
		case GPTIMER5:
			return 5;
		case GPTIMER6:
			return 6;
		case GPTIMER7:
			return 7;
		case GPTIMER8:
			return 8;
		case GPTIMER9:
			return 9;
		case GPTIMER10:
			return 10;
		case GPTIMER11:
			return 11;
		default:
			return 0;
	}
}
