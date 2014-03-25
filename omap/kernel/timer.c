#include "timer.h"
#include <stdlib.h>
#include "arch/command.h"

#define MAX_TIMER 12 //max number of GPTimer on the beagleboard

static gptimer_t timers[MAX_TIMER];

/*
 * Initialize id and base address of GPTIMER module
 */
static gptimer_t* init_timer(int id, base_address address, int line) {
	gptimer_t *timer = (gptimer_t*)malloc(sizeof(gptimer_t));
	timer->id = id;
	timer->base_address = address;
	timer->irq = line;
	stop_timer(timer);
	reset_timer_counter(timer);
	return timer;
}

void set_trigger_output_mode(gptimer_t * timer){
	exec_command(timer->base_address, TCLR, BIT_CLEAR, 10);
	exec_command(timer->base_address, TCLR, BIT_SET, 11);
}

void enable_timer_interrupt(gptimer_t *timer) {
	*((address)(timer->base_address + TIER)) |= 0x1;
}

void disable_timer_interrupt(gptimer_t *timer) {
	*((address)(timer->base_address + TIER)) &= 0x0;
}

void start_timer(gptimer_t *timer){
	*((address)(timer->base_address + TCLR)) |= 0x1;
}

void stop_timer(gptimer_t *timer) {
	disable_timer_interrupt(timer);
	*((address)(timer->base_address + TCLR)) &= 0x0;
}

void write_timer_counter(gptimer_t *timer, unsigned int count) {
	//timer->base_address +
}

void request_timer_irq(gptimer_t *timer) {
	//put_irq_handler(timer->irq);
}

gptimer_t* get_timer_by_id(int id){
	if (id > MAX_TIMER){
		return NULL;
	}
	return (gptimer_t*) &timers[id];
}

void set_match(gptimer_t *timer, uint32_t value){
	*((address)(timer->base_address + TMAR)) = value;
}

void clear_match(gptimer_t *timer){
	*((address)(timer->base_address + TMAR)) &= 0;
}

void reset_timer_counter(gptimer_t *timer){
	*((address)(timer->base_address + TCRR)) &= 0;
}

/*
 * returns reset GPTIMER based on the id
 */
gptimer_t* request_new_timer_by_id(int id) {
	switch (id){
	case 1:
		return init_timer(1, GPTIMER1, 37);
	case 2:
		return init_timer(2, GPTIMER2, 38);
	case 3:
		return init_timer(3, GPTIMER3, 39);
	case 4:
		return init_timer(4, GPTIMER4, 40);
	case 5:
		return init_timer(5, GPTIMER5, 41);
	case 6:
		return init_timer(6, GPTIMER6, 42);
	case 7:
		return init_timer(7, GPTIMER7, 43);
	case 8:
		return init_timer(8, GPTIMER8, 44);
	case 9:
		return init_timer(9, GPTIMER9, 45);
	case 10:
		return init_timer(10, GPTIMER10, 46);
	case 11:
		return init_timer(11, GPTIMER11, 47);
	default:
		return NULL;
	}
}

/*
 * return any free timer
 */
gptimer_t* request_timer(void) {
	int index = 0;

	while (index < MAX_TIMER) {
		if (timers[index].id == 0) {
			return request_new_timer_by_id(index + 1);
		}
		++index;
	}
	return NULL;
}

void register_timer_handler(){
}
