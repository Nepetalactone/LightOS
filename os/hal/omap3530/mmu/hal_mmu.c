/*
 * mmu.c
 *
 *  Created on: 08.05.2014
 *      Author: Christian
 */

#include "../../common/mmu/hal_mmu.h"



void hal_mmu_start(void) {
	//asm_mmu_start();
	asm("	STMFD R13!, {R0-R1}\r\n	LDR R0, tlb_base\r\n	MCR p15, #0, R0, c2, c0, #0\r\n	MRC p15, #0, R0, c1, c0, #0\r\n	ORR R0, R0, #1\r\n	MCR  p15, #0, R0, c1, c0, #0\r\n	LDMFD R13!, {R0-R1}\r\n	MOV PC, R14\r\ntlb_base:\r\n	.word 0x40200000");
}


