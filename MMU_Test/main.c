/*
 * main.c
 */
#include <stdlib.h>
#include <stdio.h>

int main(void) {

	asm("STMFD R13!, {R0-R1}\r\nMRC p15, #0, R0, c1, c0, #0\r\nORR R0, R0, #1\r\nMCR  p15, #0, R0, c1, c0, #0\r\nLDMFD R13!, {R0-R1}\r\nMOC PC, R14");

	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
	//exit(0);
}



