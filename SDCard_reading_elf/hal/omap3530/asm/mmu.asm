	 .global hal_mmu_activate_asm
	 .global hal_mmu_set_domain
	 .global hal_mmu_enable_write_buffer_asm
	 .global hal_mmu_set_ttbr_ctrl_bits_asm
	 .global hal_mmu_set_ttbr_0_asm
	 .global hal_mmu_set_ttbr_1_asm
	 .global hal_mmu_set_ttbr_cr_n_asm

hal_mmu_activate_asm:
	STMFD R13!, {R0-R1, lr}

	MRC p15, #0, R1, c1, c0, #0
	ORR R1, R1, #0x1 ;Enable MMU
	MCR p15, #0, R1, c1, c0, #0

	LDMFD R13!, {R0-R1, pc}

	;MOV PC, LR ; set pc to origin pc value

hal_mmu_set_domain:
	STMFD R13!, {R0-R1, lr}

	MCR		p15, #0, R0, c3, c0, #0 ;	R1, R1, #0x
	;MOV PC, LR ; set pc to origin pc value

	LDMFD R13!, {R0-R1, pc}

hal_mmu_enable_write_buffer_asm:
	STMFD R13!, {R0-R1}

	MRC p15, #0, R0, c1, c0, #0
	ORR R0, R0, #0x8 ;Write buffer
	MCR p15, #0, R0, c1, c0, #0

	LDMFD R13!, {R0-R1}
	MOV PC, LR

hal_mmu_set_ttbr_ctrl_bits_asm:
	MCR p15, #0, R0, c2, c0, #2
	MOV PC, LR

; set the task ttbr register
hal_mmu_set_ttbr_0_asm:
	MCR p15, #0, R0, c2, c0, #0 ; set the translation table base of process
	MCR p15, #0, R1, c13, c0, #1 ; set the procid  and asid
	MOV PC, LR

; set the os ttbr register
hal_mmu_set_ttbr_1_asm:
	MCR p15, #0, R0, c2, c0, #1 ; set the translation table base of os
	MOV PC, LR

; set the N value of the ttbr control register to the param value (R0)
hal_mmu_set_ttbr_cr_n_asm:
	MCR p15, #0, R0, c2, c0, #2
	MOV PC, LR
