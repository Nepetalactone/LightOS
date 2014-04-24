#include "timer.h"
#include <stdlib.h>
#include "command.h"
#include "interrupt_controller.h"


#define NR_TIMERS 12 //max number of GPTimer on the beagleboard is 11

static uint8_t timers[NR_TIMERS] = {2,0,0,0,0,0,0,0,0,0,0,0};

uint8_t get_timer_nr(base_address timer_addr);
void _enable(base_address timer);
void _disable(base_address timer);

void reset_timer(base_address timer){
	BIT_SET(timer,TSICR,1);
	BIT_SET(timer,TISR,0);
	BIT_SET(timer,TISR,1);
	BIT_SET(timer,TISR,2);
	BIT_CLEAR(timer,TIER,0);
	BIT_CLEAR(timer,TIER,1);
	BIT_CLEAR(timer,TIER,2);
}

void init_timer(base_address timer, uint32_t millisec, interrupt_handler handler, trigger_mode mode){
	if(is_timer_running(timer) || handler == NULL){
		return;
	}
	reset_timer_counter(timer);
	REG_SET(timer,TMAR, millisec);
	//TODO get real interrupt ID
	set_interrupt_handler(0,handler);
	timer_set_trigger_mode(timer, mode);

}

void start_timer(base_address timer){
	if(is_timer_running(timer)){
		return;
	}
	_enable(timer);
	BIT_SET(timer,TCLR,0);
	enable_timer_interrupt(timer);
	timer_enable_compare(timer);
}

void stop_timer(base_address timer) {
	_disable(timer);
	BIT_CLEAR(timer,TCLR,0);
	disable_timer_interrupt(timer);
}

void reset_timer_counter(base_address timer){
	REG_CLEAR(timer,TCRR);
}

void timer_enable_compare(base_address timer){
	BIT_SET(timer,TCLR,6);
}

void set_compare_value(base_address timer, uint32_t millisec) {
	REG_SET(timer,TMAR, millisec);
}

void timer_set_trigger_mode(base_address timer, trigger_mode mode){
	BIT_CLEAR(timer, TCLR, 10);
	BIT_CLEAR(timer, TCLR, 11);
	BIT_SET(timer, TCLR, mode);
}

uint8_t is_timer_running(base_address timer){
	uint8_t timer_nr = get_timer_nr(timer);
	return timers[timer_nr];
}

void _enable(base_address timer){
	timers[get_timer_nr(timer)] = 1;
}

void _disable(base_address timer){
	timers[get_timer_nr(timer)] = 0;
}

void enable_timer_interrupt(base_address timer) {
	//TODO: TIER Modes..
	BIT_SET(GPTIMER4,TIER,0);
	BIT_SET(GPTIMER4,TIER,1);
	BIT_SET(GPTIMER4,TIER,2);
}

void disable_timer_interrupt(base_address timer) {
	BIT_CLEAR(timer, TIER, 0);
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
