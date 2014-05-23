
#include <stdint.h>
#include "interrupt.h"
#include "../timer/gptimer.h"
#include "../arch/command.h"
#include "interrupt_nrs.h"



uint32_t masking_bits[MAX_INTERRUPT_VECTORS] = {0};
uint32_t steering_bits[MAX_INTERRUPT_VECTORS] = {0};

void unmask_interrupt(base_address INTC_REG, uint32_t interrupt_nr) {
	//FIXME remove interrupt_nr range is only 0-2
	//right now unmaskes EVERYTHING
	//REG_SET(MPU_INTC,MIR_CLEAR(0),0xFFFFFFFF);
	//REG_SET(MPU_INTC,MIR_CLEAR(1),0xFFFFFFFF);
	//REG_SET(MPU_INTC,MIR_CLEAR(2),0xFFFFFFFF);
	int mir_nr = 0;
	masking_bits[interrupt_nr] = 1;
	while(interrupt_nr >= 32){
		interrupt_nr -= 32;
		mir_nr++;
	}

	BIT_SET(MPU_INTC,MIR_CLEAR(mir_nr),interrupt_nr);
}

void mask_interrupt(base_address INTC_REG, offset REG, uint32_t interrupt_nr){
	int mir_nr = 0;
	masking_bits[interrupt_nr] = 0;
	while(interrupt_nr >= 32){
		interrupt_nr -= 32;
		mir_nr++;
	}
	REG_SET(MPU_INTC,MIR_SET(mir_nr),interrupt_nr);
}

void reset_interrupt_module(){
	//interrupt module reset on startup
	BIT_SET(MPU_INTC,SYSCONFIG,1);

}

void re_init_interrupt_module(){
	int i = 0;
	while(i < MAX_INTERRUPT_VECTORS){
		if(masking_bits[i]){
			unmask_interrupt(MPU_INTC,i);
		}
		if(steering_bits[i]){
			set_interrupt_mode(i,FIQ);
		}
		i++;
	}
}


//TODO call after irq is handeled
void reset_irq(){
	BIT_SET(MPU_INTC,CONTROL,0);
}

void reset_fiq(){
	BIT_SET(MPU_INTC,CONTROL,1);
}

void set_interrupt_mode(uint32_t interrupt_nr, uint8_t mode){
	if(mode == FIQ){
		//FIXME FIQ not working, immediately triggers dabt
		BIT_SET(MPU_INTC,ILR(interrupt_nr),0);
		steering_bits[interrupt_nr] = 1;
	}else if(mode == IRQ){
		BIT_CLEAR(MPU_INTC,ILR(interrupt_nr),0);
		steering_bits[interrupt_nr] = 0;
	}
}

uint32_t get_interrupt_nr(base_address adr){
		switch(adr){
		//FIXME add all interrupt nrs
			case GPTIMER1:
				return IRQ_GPT1;
			case GPTIMER2:
				return IRQ_GPT2;
			case GPTIMER3:
				return IRQ_GPT3;
			case GPTIMER4:
				return IRQ_GPT4;
			case GPTIMER5:
				return IRQ_GPT5;
			case GPTIMER6:
				return IRQ_GPT6;
			case GPTIMER7:
				return IRQ_GPT7;
			case GPTIMER8:
				return IRQ_GPT8;
			case GPTIMER9:
				return IRQ_GPT9;
			case GPTIMER10:
				return IRQ_GPT10;
			case GPTIMER11:
				return IRQ_GPT11;
			default:
				return 0;
		}
}

