#include "timer.h"

#define MAX_TIMER 12 //max number of GPTimer on the beagleboard
#define NULL (void*) 0 //maybe type.h?

typedef struct gptimer {
	int id;
	int irq;	//interrupt request line
	base_address base_address;	//base address of the GPTIMER
} gptimer_t;

static gptimer_t timers[MAX_TIMER];

/*
 * Initialize id and base address of GPTIMER module
 */
static gptimer_t* init_timer(int id, base_address address) {
	gptimer_t *timer;
	timer->id = id;
	timer->base_address = address;
	return timer;
}

void enable_timer_interrupt(gptimer_t *timer) {
	*(timer->base_address + TIER) |= 0x1;
}

void disable_timer_interrupt(gptimer_t *timer) {
	*(timer->base_address + TIER) &= 0x0;
}

void start_timer(gptimer_t *timer){
	*(timer->base_address + TCLR) |= 0x1;
}

void stop_timer(gptimer_t *timer) {
	disable_timer_interrupt(timer);
	*(timer->base_address + TCLR) &= 0x0;
}

void write_timer_counter(gptimer_t *timer, unsigned int count) {
	//timer->base_address +
}

void request_timer_irq(gptimer_t *timer) {

}

gptimer_t* get_timer_by_id(int id){
	if (id > MAX_TIMER){
		return NULL;
	}
	return (gptimer_t*) &timers[id];
}


/*
 * returns GPTIMER based on the id
 */
gptimer_t* request_timer_by_id(int id) {
	switch (id){
	case 1:
		return init_timer(1, GPTIMER1);
	case 2:
		return init_timer(2, GPTIMER2);
	case 3:
		return init_timer(3, GPTIMER3);
	case 4:
		return init_timer(4, GPTIMER4);
	case 5:
		return init_timer(5, GPTIMER5);
	case 6:
		return init_timer(6, GPTIMER6);
	case 7:
		return init_timer(7, GPTIMER7);
	case 8:
		return init_timer(8, GPTIMER8);
	case 9:
		return init_timer(9, GPTIMER9);
	case 10:
		return init_timer(10, GPTIMER10);
	case 11:
		return init_timer(11, GPTIMER11);
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
			return request_timer_by_id(index + 1);
		}
		++index;
	}
	return NULL;
}
