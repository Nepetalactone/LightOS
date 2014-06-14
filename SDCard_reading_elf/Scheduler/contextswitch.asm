
	.global context_switch_asm
	.global init_process_asm
	.global __ctx_switch_cleanup



;r0 = sp old process, r1 = lr old process, r2 = sp new process
context_switch_asm:
	STMEA 	r0!,{r1-r7} ;user registers							PUSH 7 values
	MRS		r10, SPSR
	STMEA 	r0!,{r1,r10} ;LR	& CPSR 		 						PUSH 2 values

	MOV 	r9,r0
	MOV		r0,r2
	LDMEA 	r0!, {r3,r4} 	;LR & CPSR							POP 2 values
	MOV		r8,r3			;LR to R8
	MSR		SPSR_cxsf, r3
	LDMEA	r0!,{r1-r7} 	;user registers						POP 7 values
	MOV r1,r0
	MOV r0,r9
	BL		__ctx_switch_cleanup
	MOV 	PC,r8



;r0 = process->sp, r1 = process->pc
init_process_asm:
	STMEA 	r0!,{r1-r7} ;user registers + LR					PUSH 7 values
	MRS		r2, SPSR
	STMEA 	r0!,{r1,r2} ;LR & CPSR								PUSH 2 values
	MOV 	PC,LR


