;****************************************************************************
	.armfunc _c_int00	;c_int00: 1. switches to the appropriate mode, reserves space for the run-time stack and sets up the initial value of the stack pointer. in EABI mode, the stack is aligned on a 64- bit boundary.
	;Calls the function __TI_auto_init to perform the C/C++ autoinitialization
	.global _c_int00

;****************************************************************************
; Accomodate different lowerd names in different ABIs
;****************************************************************************
    .asg	_args_main,   	ARGS_MAIN_RTN
    .asg	exit,         	EXIT_RTN
    .asg    main_func_sp,	MAIN_FUNC_SP

;****************************************************************************
;*  32 BIT STATE BOOT ROUTINE                                               *
;****************************************************************************

	.global	_stack
	.global	_irqstack
	.global	_abortstack
	.global	_intvecsaddr
	.global __TI_auto_init

;***************************************************************
;* DEFINE THE USER MODE STACK (DEFAULT SIZE IS 512)
;***************************************************************
stack:.usect			"._stack", 0, 4
irqstack:.usect			"._irqstack", 512, 4
abortstack:.usect		"._abortstack", 512, 4
intvecsaddr:.usect		"._intvecs", 0, 4

IRQ_STACK_SIZE 		.long 	0x1000
ABORT_STACK_SIZE 	.long 	0x1000

_irq_stack		.long	irqStack
_intevecs_addr	.long	intvecsaddr
_abort_stack	.long	abortstack

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
	LDR		sp, _irq_stack
	LDR		r0, IRQ_STACK_SIZE
	ADD		sp, sp, r0

	;*------------------------------------------------------
	;* : SET TO ABORT MODE
    ;*------------------------------------------------------
	CPS		#0x17

	;*------------------------------------------------------
    ;* : INITIALIZE THE ABORT MODE STACK
    ;*------------------------------------------------------
	LDR		sp, _abort_stack
	LDR		r0, ABORT_STACK_SIZE
	ADD		sp, sp, r0

	LDR		r0, _intevecs_addr
	MCR		p15, #0, r0, c12, c0, #0


    ;*------------------------------------------------------
	;* SET TO SYSTEM MODE
    ;*------------------------------------------------------
	CPS		#0x1F

	BL	__TI_auto_init
.end
