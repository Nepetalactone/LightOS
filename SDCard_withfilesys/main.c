/*
 * main.c
 */
#include "try/fat32.h"
int main(void) {
	if(fat32Init()){
		return 1;
	}
	return 0;
}
