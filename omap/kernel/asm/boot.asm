;****************************************************************************
	.armfunc _c_int00

;****************************************************************************
; Accomodate different lowerd names in different ABIs
;****************************************************************************
    .asg	_args_main,   	ARGS_MAIN_RTN
    .asg	exit,         	EXIT_RTN
    .asg    main_func_sp,	MAIN_FUNC_SP

;****************************************************************************
;*  32 BIT STATE BOOT ROUTINE                                               *
;****************************************************************************

	.global	__stack
	.global	__irqstack
	.global	__abortstack
	.global	__intvecsaddr
	.global	__svcstack

;***************************************************************
;* DEFINE THE USER MODE STACK (DEFAULT SIZE IS 512)
;***************************************************************
__stack:.usect			".stack", 0, 4
__irqstack:.usect		".irqstack", 512, 4
__abortstack:.usect		".abortstack", 512, 4
__intvecsaddr:.usect	".intvecs", 0, 4
__svcstack:.usect		".svcstack", 512, 4

	.global	_c_int00
;***************************************************************
;* FUNCTION DEF: _c_int00
;***************************************************************
_c_int00: .asmfunc
	;*------------------------------------------------------
	;* : SET TO IRQ MODE
    ;*------------------------------------------------------
	CPS		#0x12

	;*------------------------------------------------------
    ;* : INITIALIZE THE IRQ MODE STACK
    ;*------------------------------------------------------
	LDR		sp, irq_stack
	LDR		r0, IRQ_STACK_SIZE
	ADD		sp, sp, r0

	;*------------------------------------------------------
	;* : SET TO ABORT MODE
    ;*------------------------------------------------------
	CPS		#0x17

	;*------------------------------------------------------
    ;* : INITIALIZE THE ABORT MODE STACK
    ;*------------------------------------------------------
	LDR		sp, abort_stack
	LDR		r0, ABORT_STACK_SIZE
	ADD		sp, sp, r0

    ;*------------------------------------------------------
;* SET TO SYSTEM MODE
    ;*------------------------------------------------------
	CPS		#0x1F

    ;*------------------------------------------------------
    ;* INITIALIZE THE USER/SYSTEM MODE STACK
    ;*------------------------------------------------------
	LDR     sp, c_stack
    LDR     r0, c_STACK_SIZE
	ADD		sp, sp, r0

	;*-----------------------------------------------------
	;* ALIGN THE STACK TO 64-BITS IF EABI.
	;*-----------------------------------------------------
	BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.

	;*---------------------------------
	;* : set Vector base address to be 0x40200000
	; http://e2e.ti.com/support/dsp/omap_applications_processors/f/447/t/29274.aspx
	;*---------------------------------
    LDR		r0, intvecs_addr
    MCR 	p15, #0, r0, c12, c0, #0

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

    ;*------------------------------------------------------
	;* SET TO SUPERVISOR MODE - NOT BEVORE __TI_auto_init
    ;*------------------------------------------------------
	CPS		#0x13

    ;*------------------------------------------------------
    ;* INITIALIZE THE Supervisor MODE STACK
    ;*------------------------------------------------------
	LDR     sp, svc_stack
    LDR     r0, SVC_STACK_SIZE
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
c_stack			.long    __stack
c_STACK_SIZE  	.long    __STACK_SIZE
c_mf_sp	        .long    MAIN_FUNC_SP

intvecs_addr	.long	__intvecsaddr

abort_stack			.long	__abortstack
ABORT_STACK_SIZE 	.long    0x200		; TODO: move to some central constant-pool

irq_stack		.long    __irqstack
IRQ_STACK_SIZE 	.long    0x200		; TODO: move to some central constant-pool

svc_stack		.long    __svcstack
SVC_STACK_SIZE 	.long    0x200		; TODO: move to some central constant-pool

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
