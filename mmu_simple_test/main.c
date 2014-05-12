/*
 * main.c
 */
int main(void) {
	asm("	STMFD R13!, {R0-R1}\r\n	MRC p15, #0, R0, c1, c0, #0\r\n	ORR R0, R0, #1\r\n	MCR  p15, #0, R0, c1, c0, #0\r\n	LDMFD R13!, {R0-R1}\r\n	MOV PC, R14");
	return 0;
}
