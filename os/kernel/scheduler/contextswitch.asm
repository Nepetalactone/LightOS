
context_switch_asm:
	STM     sp,{R0-lr}^             ; Dump user registers above R13.
    MRS     R0, SPSR                ; Pick up the user status
    STMDB   sp, {R0, lr}            ; and dump with return address below.
    LDR     sp, [R12], #4           ; Load next process info pointer.
    CMP     sp, #0                  ; If it is zero, it is invalid
    LDMDBNE sp, {R0, lr}            ; Pick up status and return address.
    MSRNE   SPSR_cxsf, R0           ; Restore the status.
    LDMNE   sp, {R0 - lr}^          ; Get the rest of the registers
    NOP
    SUBSNE pc, lr, #4              ; and return and restore CPSR.
                                    ; Insert "no next process code" here.
