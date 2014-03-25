
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "../arch/address.h"
#include "../type.h"

#define MAX_INTERRUPT_VECTORS 96

//RAM exception vectors p. 3438
#define ISR_IRQ_ADDRESS (address) 0x4020FFDC
#define FIQ_IRQ_ADDRESS (address) 0x4020FFE0

/* Page 1079 */
#define MPU_INTC (base_address) 0x48200000
#define MODEM_INTC (base_address) 0x480C7000

#define REVISION (offset) 		0x000 /* IP revision code */
#define SYSCONFIG (offset)   	0x010 /* Controls params */
#define SYSSTATUS (offset)   	0x014 /* Status */
#define SIR_IRQ (offset)     	0x040 /* Active IRQ number 1-95 */
#define SIR_FIQ (offset)     	0x044 /* Active FIQ number */
#define CONTROL (offset)     	0x048 /* New int agreement bits */
#define PROTECTION (offset)  	0x04C /* Protection for other regs */
#define IDLE (offset)        	0x050 /* Clock auto-idle/gating */
#define IRQ_PRIORITY (offset) 	0x060 /* Active IRQ priority level */
#define FIQ_PRIORITY (offset) 	0x064 /* Active FIQ priority level */
#define THRESHOLD (offset)   	0x068 /* Priority threshold */
#define ITR0 (offset)       	0x080 /* Raw pre-masking interrupt status */
#define MIR0 (offset)        	0x084 /* Interrupt mask */
#define MIR_CLEAR0 (offset)  	0x088 /* Clear interrupt mask bits */
#define MIR_SET0 (offset)    	0x08C /* Set interrupt mask bits */
#define ISR_SET0 (offset)    	0x090 /* Set software int bits */
#define ISR_CLEAR0 (offset)  	0x094 /* Clear software int bits */
#define PENDING_IRQ0 (offset) 	0x098 /* IRQ status post-masking */
#define PENDING_FIQ0 (offset) 	0x09C /* FIQ status post-masking */
#define ILR0 (offset)        	0x100 /* Priority for interrupts */

//TODO REMOVE ME
#define CPSR (address) 			0x00

#define IRQ_LINE (address)	MPU_INTC + SIR_IRQ
#define FIQ_LINE (address)	MPU_INTC + SIR_FIQ

void init_isr_handler(void);
void unmask_mir0(void);
void mask_mir0(void);
void mir0_clear(void);
void mir0_set(void);
void clear_CPSR_IRQ_FIQ_DISABLE(void);

#endif /* INTERRUPT_H_ */
