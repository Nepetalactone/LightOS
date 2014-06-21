#include "syscalls.h"

//====================================Helperfunctions===============================
//switches to supervisor mode
void change_to_supervisor();
void change_to_supervisor(){
	//TODO implement
}

//switches to user mode
void change_to_user();
void change_to_user(){
	//TODO implement
}

//====================================Timercalls====================================

//Initializes a timer
base_address init_timer(uint32_t millisec, irq_interrupt_handler handler){
	change_to_supervisor();
	base_address timer = get_free_timer();
	if (timer != 0){
		timer_quick_init(timer, millisec, handler, trigger_OverflowMatch);
	}
	change_to_user();
	return timer;
}

//Releases a timer
void release_timer(base_address timer){
	change_to_supervisor();
	timer_release(timer);
	change_to_user();
}

//====================================Processcalls==================================

//Clones the current process
uint32_t fork();

//Creates a new process
uint32_t new_process(char* processName, void* entryPoint){
	change_to_supervisor();
	process_create(processName, entryPoint);
	change_to_user();
	return 0;
}

//Terminates the specified process
uint32_t terminate_process_by_id(process_id id){
	change_to_supervisor();
	process_kill_pid(id);
	change_to_user();
	return 0;
}

//Terminates the specified process
uint32_t terminate_process_by_name(char* name){
	change_to_supervisor();
	process_kill_name(name);
	change_to_user();
	return 0;
}

//====================================Memorycalls===================================

//Allocates a chunk of memory of the specified size
uint32_t allocate(int size);

//Frees a chunk of memory
uint32_t free(uint32_t address);

//====================================Filecalls=====================================


