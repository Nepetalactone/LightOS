

#include "interrupt.h"
#include "../timer.h"
#include "../arch/command.h"

void unmask_interrupts (base_address INTC_REG, uint32_t interrupt_nr) {

	REG_SET(MPU_INTC,MIR_CLEAR(interrupt_nr),0xFFFFFFFF);

	//REG_SET(MPU_INTC,MIR_CLEAR0,0xFFFFFFFF);
	//REG_SET(MPU_INTC,MIR_CLEAR1,0xFFFFFFFF);
	//REG_SET(MPU_INTC,MIR_CLEAR2,0xFFFFFFFF);
}

void mask_interrupts(base_address INTC_REG, offset REG, uint32_t interrupt_nr){
	REG_SET(MPU_INTC,MIR_SET(interrupt_nr),0xFFFFFFFF);
}

void set_interrupt_mode(uint32_t interrupt_nr, uint8_t mode){
	BIT_SET(MPU_INTC,ILR(interrupt_nr),mode);
}

uint8_t get_interrupt_nr(base_address addr){
	//TODO add all interrupt numbers by address
	switch(addr){
		case GPTIMER1:
			return 37;
		case GPTIMER2:
			return 38;
		case GPTIMER3:
			return 39;
		case GPTIMER4:
			return 40;
		case GPTIMER5:
			return 41;
		case GPTIMER6:
			return 42;
		case GPTIMER7:
			return 43;
		case GPTIMER8:
			return 44;
		case GPTIMER9:
			return 45;
		case GPTIMER10:
			return 46;
		case GPTIMER11:
			return 47;
		default:
			return 0;
	}
}
