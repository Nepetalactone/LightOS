
	.global context_switch_asm

context_switch_asm:
	MRS		R12, SPSR ;SPSR
	;MOV		R12, R0
	STR		R12, [R0], #8

	LDMFD	R13!, {R2, R3}

	STMIA	R0!, {R2, R3}

	LDMFD	R13!, {R2, R3, R12, R14}
	STR		R14, [R0, #-12]

	STMIA	R0, {R2-R14}^

	; Then load the new process User mode state and return to it
	LDMIA	R1!, {R12, R2}
	MSR		SPSR_fsxc, R2
	LDMIA	R1, {R0-R14}^
	NOP

	MOVS	PC, R14
