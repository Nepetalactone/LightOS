	 .global hal_mmu_activate_asm
	 .global hal_mmu_enable_write_buffer_asm
	 .global hal_mmu_set_ttbr_ctrl_bits_asm
	 .global hal_mmu_set_ttbr_0_asm
	 .global hal_mmu_set_ttbr_1_asm

hal_mmu_activate_asm:
	STMFD R13!, {R0-R1}

	MRC p15, #0, R0, c1, c0, #0
	ORR R0, R0, #0x1 ;Enable MMU
	MCR p15, #0, R0, c1, c0, #0

	LDMFD R13!, {R0-R1}
	MOV PC, LR

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

hal_mmu_set_ttbr_0_asm:
	MCR p15, #0, R0, c2, c0, #0
	MCR p15, #0, R1, c13, c0, #1
	MOV PC, LR

hal_mmu_set_ttbr_1_asm:
	MCR p15, #0, R0, c2, c0, #1
	MOV PC, LR

