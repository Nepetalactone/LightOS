/*
 * main_test.c
 */
#include <string.h>
#include <stdlib.h>
#include "./kernel/interrupts/interrupt.h"
#include "kernel/arch/serial.h"

void serial_test(void) {

	// serial communication
	ser_init(ser115200); // init

	char* console = (char*) "\nlightOs> "; // console message
	char* buffer;
	int buffer_len = 64; // size of buffer
	char* recString;



	ser_puts((const signed char*) "\n welcome to lightOS\n");
	while(1)
	{
		buffer = (char*) malloc(sizeof(char)*buffer_len);
		ser_puts((const signed char*) console);
		recString = ser_gets(buffer, buffer_len);
		ser_puts((const signed char*) recString);
		free(buffer);
	}
}
