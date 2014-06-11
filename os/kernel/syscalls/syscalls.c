#include "syscalls.h"

//====================================Timercalls====================================

//Initializes a timer
base_address init_timer(uint32_t millisec, irq_interrupt_handler handler){
	//TODO change_to_supervisor();
	base_address timer = get_free_timer();
	if (timer != 0){
		timer_quick_init(timer, millisec, handler, trigger_OverflowMatch);
	}
	//TODOchange_to_user();
	return timer;
}

//Releases a timer
void release_timer(base_address timer){
	//TODO change_to_supervisor();
	timer_release(timer);
	//TODO change_to_user();
}

//====================================Processcalls==================================

//Clones the current process
uint32_t fork();

//Creates a new process
uint32_t new_process();

//Terminates the specified process
uint32_t terminate_process(process_id id);

//====================================Memorycalls===================================

//Allocates a chunk of memory of the specified size
uint32_t allocate(int size);

//Frees a chunk of memory
uint32_t free(uint32_t address);

//====================================Filecalls=====================================

//====================================Helperfunctions===============================
