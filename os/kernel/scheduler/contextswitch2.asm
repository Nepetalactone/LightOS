
	.global context_switch2_asm

context_switch2_asm:
	MRS		R12, SPSR 					;Get CPSR of interrupted process
	;MOV		R12, R0
	STR		R12, [R0], #8 				;Store CPSR to PCB, point R0 at PCB location for R0 value

	LDMFD	R13!, {R2, R3}				;Reload R0/R1 of interrupted process from stack

	STMIA	R0!, {R2, R3}				;Store R0/R1 values to PCB, point R0 at PCB location for R2 value

	LDMFD	R13!, {R2, R3, R12, R14}	;Reload remaining stacked values
	STR		R14, [R0, #-12]				;Store R14_irq, the interrupted process's restart address

	STMIA	R0, {R2-R14}^				;Store user R2-R14

	; Then load the new process User mode state and return to it
	LDMIA	R1!, {R12, R2}				;Put interrupted process's CPSR
	MSR		SPSR_fsxc, R2				; and restart address in SPSR_irq and R14_irq
	LDMIA	R1, {R8-R14}^				;Load user R0-R14
	NOP

	MOVS	PC, R14						;Return to address in R14_irq, with SPSR_irq -> CPSR transfer
