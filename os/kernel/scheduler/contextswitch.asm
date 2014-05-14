
;context_switch_asm:
;	STM     sp,{R0-lr}^             ; Dump user registers above R13.
;    MRS     R0, SPSR                ; Pick up the user status
;    STMDB   sp, {R0, lr}            ; and dump with return address below.
;    LDR     sp, [R12], #4           ; Load next process info pointer.
;    CMP     sp, #0                  ; If it is zero, it is invalid
;    LDMDBNE sp, {R0, lr}            ; Pick up status and return address.
;    MSRNE   SPSR_cxsf, R0           ; Restore the status.
;    LDMNE   sp, {R0 - lr}^          ; Get the rest of the registers
;    NOP
;    SUBSNE pc, lr, #4              ; and return and restore CPSR.
                                    ; Insert "no next process code" here.
	.global store_context_asm
	.global load_context_asm
	.global get_stack_pointer_asm

store_context_asm:
	MRS	R12, SPSR
	STR R12, [R0], #8

	LDMFD	R13!, {R2, R3}

	STMIA	R0!, {R2, R3}

	LDMFD R13!, {R2, R3, R12, R14}
	STR	R14, [R0, #-12]

	STMIA	R0, {R2-R14}^


load_context_asm:
	LDMIA	R1!, {R12, R14}
	MSR	SPSR_fsxc, R12

	LDMIA	R1, {R0-R14}^
	NOP
	MOV		R0, R14
	MOVS	PC, R14



get_stack_pointer_asm:
	MOV		R0, R14
