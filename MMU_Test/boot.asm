;******************************************************
;* MMU settings										  *
;******************************************************

entry4020		.word 0x40201008
val4020			.word 0x40200c02
tlb_l1_base		.word 0x40200000

	.global mmu_init

mmu_init:

	ldr r0, tlb_l1_base		  ; load xy to r0
	mcr p15, #0, r0, c2, c0, #0 ; arm to coprocessor (MMU)

	ldr r0, entry4020
	ldr r1, val4020
	str r1, [r0]



	; access permissions
	mov r0, #0x3
	mcr p15, #0, r0, c3, c0, #0

	; turn mmu on
	mrc p15, #0, r0, c1, c0, #0
	orr r0, r0, #0x1 ; logical OR
	mcr p15, #0, r0, c1, c0, #0

	.endasmfunc
	; end mmu init

		.armfunc _c_int00

    .asg	_args_main,   	ARGS_MAIN_RTN
    .asg	exit,         	EXIT_RTN
    .asg    main_func_sp,	MAIN_FUNC_SP


	.global	__stack
	.global	__irqstack
	.global	__abortstack
	.global	__intvecsaddr
	.global	__svcstack

__stack:.usect			".stack", 0, 4
__irqstack:.usect		".irqstack", 512, 4
__abortstack:.usect		".abortstack", 512, 4
__intvecsaddr:.usect	".intvecs", 0, 4
__svcstack:.usect		".svcstack", 512, 4

	.global	_c_int00

_c_int00: .asmfunc


	CPS		#0x12
	LDR		sp, irq_stack
	LDR		r0, IRQ_STACK_SIZE
	ADD		sp, sp, r0

	CPS		#0x17
	LDR		sp, abort_stack
	LDR		r0, ABORT_STACK_SIZE
	ADD		sp, sp, r0

	CPS		#0x1F
	LDR     sp, c_stack
    LDR     r0, c_STACK_SIZE
	ADD		sp, sp, r0

	BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.

    LDR		r0, intvecs_addr
    MCR 	p15, #0, r0, c12, c0, #0

	LDR	r0, c_mf_sp
	STR	sp, [r0]

    ;*------------------------------------------------------
    ;* Perform all the required initilizations:
    ;*   - Process BINIT Table
    ;*   - Perform C auto initialization
    ;*   - Call global constructors
    ;*------------------------------------------------------
    BL      __TI_auto_init

    BL		mmu_init

	CPS		#0x13
	LDR     sp, svc_stack
    LDR     r0, SVC_STACK_SIZE
	ADD		sp, sp, r0

    BL      ARGS_MAIN_RTN

    MOV     R0, #1
    BL      EXIT_RTN


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
