	.global _c_int00
	.global udef_handler
	.global swi_handler
	.global pabt_handler
	.global dabt_handler
	.global irq_handler
	.global fiq_handler

	.sect ".intvecs"
		;B _c_int00
		B udef_handler
		B swi_handler
		B pabt_handler
		B dabt_handler
		.word 0
		B irq_handler
		B fiq_handler
