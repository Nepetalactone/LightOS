/*
 * main.c
 */
#include <stdlib.h>
#include <stdio.h>

int main(void) {

	volatile int x=0;
	while(1)
	{
		volatile int i=0;
		int x=i;
	}
	//exit(0);
}



