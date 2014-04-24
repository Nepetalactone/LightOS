#ifndef LIGHT_TYPES_H_
#define LIGHT_TYPES_H_

#include "../arch/address.h"

typedef void (*interrupt_handler)(void);

#define IRQ 1
#define FIQ 0
#define NR_INTERRUPTS (126)
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
#define ITR(n) (offset)       	0x080 + (0x20 * n)/* Raw pre-masking interrupt status */
#define MIR(n) (offset)        	0x084 + (0x20 * n)/* Interrupt mask */
#define MIR_CLEAR(n) (offset)  	0x088 + (0x20 * n)/* Clear interrupt mask bits */
#define MIR_SET(n) (offset)    	0x08C + (0x20 * n)/* Set interrupt mask bits */
#define ISR_SET(n) (offset)    	0x090 + (0x20 * n)/* Set software int bits */
#define ISR_CLEAR(n) (offset)  	0x094 + (0x20 * n)/* Clear software int bits */
#define PENDING_IRQ(n) (offset) 0x098 + (0x20 * n)/* IRQ status post-masking */
#define PENDING_FIQ(n) (offset) 0x09C + (0x20 * n)/* FIQ status post-masking */
#define ILR(m) (offset)        	0x100 + (0x4 * m)/* Priority for interrupts */


#define IRQ_LINE (address)	MPU_INTC + SIR_IRQ
#define FIQ_LINE (address)	MPU_INTC + SIR_FIQ


void set_interrupt_mode(uint32_t interrupt_nr, uint8_t mode);
void unmask_interrupts (base_address INTC_REG, uint32_t interrupt_nr);
void mask_interrupts(base_address INTC_REG, offset REG, uint32_t interrupt_nr);
uint8_t get_interrupt_nr(base_address addr);



#endif
