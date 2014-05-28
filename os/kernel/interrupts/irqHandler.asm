	.global irq_handler_asm
	.global irq_handler
	.global __identify_and_clear_source

irq_handler_asm:
	STMFD	SP, { R0 - R14 }^			; store user-registers on stack
	SUB		SP, SP, #60					; decrement stack-pointer: 15 * 4 bytes = 60bytes

	STMFD	SP!, { LR }					; store LR in stack to either restore it after BL to allow scheduler change it (and restore it after return from BL)

	MRS		R1, SPSR					; fetch user-cpsr (SPSR) value to R1
	STMFD	SP!, { R1 }					; store SPSR in stack to allow scheduler change it

	MOV 	R0, SP						; pointer to SP in R0, to point to UserContext-struct

	; irqHandler( UserContext* ctx )
	BL		__identify_and_clear_source
	BL		irq_handler					; branch AND link to irq parent handler

	; TODO: do a BEQ on R0:
	; if R0 == 1 then scheduling was performed

	LDMFD	SP!, { R1 }					; restore SPSR, if changed by scheduler
	MSR		SPSR_cxsf, R1				; set stored cpsr from user to the current CPSR - will be restored later during SUBS

	LDMFD	SP!, { LR }					; restore LR, if changed by scheduler

	LDMFD	SP, { R0 - R14 }^			; restore user-registers, if changed by scheduler
	ADD		SP, SP, #60					; increment stack-pointer: 15 * 4 bytes = 60bytes

	; NOTE: at this point SP should be at starting address again

	; TODO: when a process-switch was performed: MOVS	PC, LR should be enough, otherwise we must return to the instruction which was canceled by IRQ thus using SUBS

 	SUBS	PC, LR, #4					; return from IRQ



