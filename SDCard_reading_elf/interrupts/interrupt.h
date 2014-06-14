#ifndef LIGHT_TYPES_H_
#define LIGHT_TYPES_H_

#include "../arch/address.h"

/*
OMAP35x technical reference manual pdf
Overview p.1079
*/


/*
General Information


*/

typedef void (*irq_interrupt_handler)();

#define IRQ 1
#define FIQ 0
#define NR_INTERRUPTS (126)
#define MAX_INTERRUPT_VECTORS 96

//RAM exception vectors p. 3438
#define ISR_IRQ_ADDRESS (address) 0x4020FFDC
#define ISR_FIQ_ADDRESS (address) 0x4020FFE0

/* MPU_INTC BASE ADDRESSES*/
#define MPU_INTC (base_address) 0x48200000
#define MODEM_INTC (base_address) 0x480C7000
/* MPU_INTC BASE ADDRESSES END*/

/* REGISTER OFFSETS */
#define REVISION (offset) 		0x000 /* IP revision code */
#define SYSCONFIG (offset)   	0x010 /* Controls params */
#define SYSSTATUS (offset)   	0x014 /* Status */
#define SIR_IRQ (offset)     	0x040 /* Active IRQ number 1-95 */
#define SIR_FIQ (offset)     	0x044 /* Active FIQ number */
#define CONTROL (offset)     	0x048 /* New interrupt agreement bits */
#define PROTECTION (offset)  	0x04C /* Protection for other regs */
#define IDLE (offset)        	0x050 /* Clock auto-idle/gating */
#define IRQ_PRIORITY (offset) 	0x060 /* Active IRQ priority level */
#define FIQ_PRIORITY (offset) 	0x064 /* Active FIQ priority level */
#define THRESHOLD (offset)   	0x068 /* Priority threshold */
#define ITR(n) (offset)       	0x080 + (0x20 * n)/* Raw pre-masking interrupt status  n = 0-2 */
#define MIR(n) (offset)        	0x084 + (0x20 * n)/* Interrupt mask  n = 0-2 */
#define MIR_CLEAR(n) (offset)  	0x088 + (0x20 * n)/* Clear interrupt mask bits  n = 0-2 */
#define MIR_SET(n) (offset)    	0x08C + (0x20 * n)/* Set interrupt mask bits  n = 0-2 */
#define ISR_SET(n) (offset)    	0x090 + (0x20 * n)/* Set software interrupt bits  n = 0-2 */
#define ISR_CLEAR(n) (offset)  	0x094 + (0x20 * n)/* Clear software interrupt bits  n = 0-2 */
#define PENDING_IRQ(n) (offset) 0x098 + (0x20 * n)/* IRQ status post-masking  n = 0-2 */
#define PENDING_FIQ(n) (offset) 0x09C + (0x20 * n)/* FIQ status post-masking  n = 0-2 */
#define ILR(m) (offset)        	0x100 + (0x4 * m)/* Priority for interrupts  m = 0-95 */
/* REGISTER OFFSETS END */


#define IRQ_LINE (address)	MPU_INTC + SIR_IRQ
#define FIQ_LINE (address)	MPU_INTC + SIR_FIQ


void set_interrupt_mode(uint32_t interrupt_nr, uint8_t mode);
void unmask_interrupt (base_address INTC_REG, uint32_t interrupt_nr);
void init_interrupt_controller();
void reset_interrupt_module();
void re_init_interrupt_module(); //TODO remove when fixed
void mask_interrupt(base_address INTC_REG, offset REG, uint32_t interrupt_nr);
uint32_t get_interrupt_nr(base_address adr);
void reset_irq();
void reset_fiq();

#endif
