	.global udef_handler
	.global swi_handler
	.global pabt_handler
	.global dabt_handler
	.global irq_handler_asm
	.global fiq_handler

	.sect ".intvecs"
		.word 0
		B udef_handler
		B swi_handler
		B pabt_handler
		B dabt_handler
		.word 0
		B irq_handler_asm
		B fiq_handler
