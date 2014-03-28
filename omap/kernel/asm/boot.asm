	.armfunc _c_int00	;switches to the appropriate mode, reserves space for the run-time stack and sets up the initial value
						;of the stack pointer. In EABI mode, the stack is aligned on a 64-bit boundary.
						;Calls the function __TI_auto_init to perform the C/C++ autoinitialization
	.global _c_int00

;TI includes

	.asg __args_main, ARGS_MAIN_RTN
	.global ARGS_MAIN_RTN
	.global __TI_auto_init

;Included addresses from the Linker

	.global irqStack
	.global kernelStack
	.global abortStack
	.global systemStack

;Constants used by this module

c_r13_irq		.long	irqStack
c_r13_kernel	.long	kernelStack
c_r13_abt		.long	abortStack
c_r13_system	.long	systemStack

;Function def: _c_int00

_c_int00: .asmfunc

	;Set IRQ-Stack
	CPS 0x12
	LDR sp, c_r13_irq

	;Set SWI-Stack / Kernel-stack
	CPS 0x13
	LDR sp, c_r13_kernel

		;Set ABT-Stack
		CPS 0x17
		LDR sp, c_r13_abt

	;Set System-Stack
	CPS 0x1F
	LDR sp, c_r13_system

	;Disable Interrupts
	MRS R12, CPSR
	BIC R12, R12, #192
	MSR CPSR_cf, R12

	;Perform all the required initializations
	; - Process BINIT Table
	; - Perform C auto initialization
	; - Call global constructors
	BL __TI_auto_init

	;Call application
	BL ARGS_MAIN_RTN

.end
