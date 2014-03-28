
	.armfunc _c_int00

;****************************************************************************
; Accomodate different lowerd names in different ABIs
;****************************************************************************

		.asg	_args_main,   ARGS_MAIN_RTN
		.asg	exit,         EXIT_RTN
		.asg    main_func_sp, MAIN_FUNC_SP


;****************************************************************************
;*  32 BIT STATE BOOT ROUTINE                                               *
;****************************************************************************

	.global	_sys_stack
	.global _irq_stack
	.global _abort_stack
	.global _kernel_stack
	.global _intvecs_addr

;***************************************************************
;* DEFINE THE USER MODE STACK (DEFAULT SIZE IS 512)
;***************************************************************

_sys_stack:.usect	".sys_stack", 0, 4
_kernel_stack:.usect	".kernel_stack", 512, 4
_irq_stack:.usect		".irq_stack", 512, 4
_abort_stack:.usect		".abort_stack", 512, 4
_intvecs_addr:.usect		".intvecs_addr", 0, 4


	.global	_c_int00
;***************************************************************
;* FUNCTION DEF: _c_int00
;***************************************************************
_c_int00: .asmfunc

;==================================================================

	;*---------IRQ STACK-------------
	CPS		#0x12
	LDR		sp, irq_stack
	LDR		r0, c_STACK_SIZE
	ADD		sp, sp, r0

	;*---------ABORT STACK-------------
	CPS		#0x17
	LDR		sp, abort_stack
	LDR		r0, c_STACK_SIZE
	ADD		sp, sp, r0

    ;*------------------------------------------------------
    ;* INITIALIZE THE USER/SYSTEM MODE STACK
    ;*------------------------------------------------------
	CPS		#0x1F
	LDR     sp, sys_stack
	LDR     r0, c_STACK_SIZE
	ADD		sp, sp, r0


	;*-----------------------------------------------------
	;* ALIGN THE STACK TO 64-BITS IF EABI.
	;*-----------------------------------------------------
	BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.


	LDR		r0, intvecs_addr
	MCR		p15, #0, r0, c12, c0, #0

	;*-----------------------------------------------------
	;* SAVE CURRENT STACK POINTER FOR SDP ANALYSIS
	;*-----------------------------------------------------
	LDR	r0, c_mf_sp
	STR	sp, [r0]

        ;*------------------------------------------------------
        ;* Perform all the required initilizations:
        ;*   - Process BINIT Table
        ;*   - Perform C auto initialization
        ;*   - Call global constructors
        ;*------------------------------------------------------
        BL      __TI_auto_init


		;*---------KERNEL STACK-------------
		CPS		#0x13
		LDR		sp, kernel_stack
		LDR		r0, c_STACK_SIZE
		ADD		sp, sp, r0

        ;*------------------------------------------------------
	;* CALL APPLICATION
        ;*------------------------------------------------------
        BL      ARGS_MAIN_RTN

        ;*------------------------------------------------------
	;* IF APPLICATION DIDN'T CALL EXIT, CALL EXIT(1)
        ;*------------------------------------------------------
        MOV     R0, #1
        BL      EXIT_RTN

        ;*------------------------------------------------------
	;* DONE, LOOP FOREVER
        ;*------------------------------------------------------
L1:     B	L1
	.endasmfunc

;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************

abort_stack		.long abort_stack
irq_stack		.long irq_stack
sys_stack		.long sys_stack
kernel_stack	.long kernel_stack
intvecs_addr	.long intvecs_addr


c_stack		.long    _sys_stack
c_STACK_SIZE  	.long    0x200
c_mf_sp	        .long    MAIN_FUNC_SP

	.if __TI_EABI_ASSEMBLER
        .data
        .align 4
_stkchk_called:
        .field          0,32
        .else
        .sect   ".cinit"
        .align  4
        .field          4,32
        .field          _stkchk_called+0,32
        .field          0,32

        .bss    _stkchk_called,4,4
        .symdepend ".cinit", ".bss"
        .symdepend ".cinit", ".text"
        .symdepend ".bss", ".text"
	.endif

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global _stkchk_called
	.global	__STACK_SIZE
	.global ARGS_MAIN_RTN
	.global MAIN_FUNC_SP
	.global	EXIT_RTN
    .global __TI_auto_init


	.end
