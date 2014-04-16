	.armfunc _c_int00
    .global _c_int00

;****************************************************************************
; Entry-Exit Point definition
;****************************************************************************
    .asg	_args_main,   ARGS_MAIN_RTN
    .asg	exit,         EXIT_RTN
    .asg    main_func_sp, MAIN_FUNC_SP

;***************************************************************
;* FUNCTION DEF: _c_int00
;***************************************************************
_c_int00: .asmfunc

        ;*------------------------------------------------------
        ;* Switch to IRQ Mode and store stack-pointer
        ;*------------------------------------------------------<
        CPS		0x12
        LDR     sp, c_sp_irq

        ;*------------------------------------------------------
        ;* Switch to Supervisor Mode and store stack-pointer
        ;*------------------------------------------------------
        CPS		0x13
        LDR     sp, c_sp_kernel

        ;*------------------------------------------------------
        ;* Switch to ABT Mode and store stack-pointer
        ;*------------------------------------------------------
        CPS		0x17
        LDR     sp, c_sp_abort

        ;*------------------------------------------------------
        ;* Switch to System Mode and Store stack-pointer
        ;*------------------------------------------------------
        CPS		0x1F
        LDR		sp, c_sp_system

		; Interrupt Vectors base register
    	LDR R0, _int_vecs
		MCR p15, #0, R0, c12, c0, #0


        ;*------------------------------------------------------
        ;* Initialize User-Mode Stack
        ;*------------------------------------------------------
        ; LDR     sp, c_stack
        ; LDR     r0, c_STACK_SIZE
        ; ADD     sp, sp, r0

        ;*------------------------------------------------------
        ;* Perform all the required initilizations:
        ;*   - Process BINIT Table
        ;*   - Perform C auto initialization
        ;*   - Call global constructors
        ;*------------------------------------------------------
        BL      __TI_auto_init

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
L1:     B       L1
		.endasmfunc

;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************
c_sp_irq		.long	irqStack
c_sp_kernel		.long	kernelStack
c_sp_abort		.long	abortStack
c_sp_system		.long	systemStack

_int_vecs		.long 	int_vecs

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global	__STACK_SIZE
	.global ARGS_MAIN_RTN
	.global MAIN_FUNC_SP


	.global int_vecs
	.global irqStack
	.global kernelStack
	.global abortStack
	.global systemStack

	.global	EXIT_RTN
	.global __TI_auto_init

.end
