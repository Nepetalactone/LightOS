
#ifndef TIMER_H_
#define TIMER_H_

#include "../address.h"

#define GPTIMER1 (base_address) 0x48318000
#define GPTIMER2 (base_address) 0x49032000
#define GPTIMER3 (base_address) 0x49034000
#define GPTIMER4 (base_address) 0x49036000
#define GPTIMER5 (base_address) 0x49038000
#define GPTIMER6 (base_address) 0x4903A000
#define GPTIMER7 (base_address) 0x4903C000
#define GPTIMER8 (base_address) 0x4903E000
#define GPTIMER9 (base_address) 0x49040000
#define GPTIMER10 (base_address) 0x48086000
#define GPTIMER11 (base_address) 0x48088000


/* REGISTERS */
#define TIDR (offset)0x000 //This register contains the IP revision code p.2636
#define TIOCP_CFG (offset) 0x010 //This register controls the various parameters of the GP timer L4 interface p.2637
#define TISTAT (offset) 0x014 //This register provides status information about the module, excluding the interrupt status information p. 2639
#define TISR (offset) 0x018 //This register shows which interrupt events are pending inside the module p.2640
#define TIER (offset) 0x01C //This register controls (enable/disable) the interrupt events
#define TWER (offset) 0x020 //This register controls (enable/disable) the wake-up feature on specific interrupt events p.2642
#define TCLR (offset) 0x028 //This register controls optional features specific to the timer functionality. p.2643
#define TCRR (offset) 0x02C //Enable/disable IRQ generation p.3407
#define TLDR (offset) 0x030 //Controls the clock gating functionality p.3408
#define TTGR (offset) 0x034 //Used to enable the pins output capabilities. It's only function is to carry the pads configuration. p.3409
#define TWPS (offset) 0x038 //Register data that is read from the pin p.3409
#define TMAR (offset) 0x03C //Used to set output value of the pin p.3410
#define TCAR1 (offset) 0x040 //Enable/disable low-level detection for interrupt request generation p.3411
#define TSICR (offset) 0x044 //Enable/disable high-level detection for interrupt request generation p.3411
#define TCAR2 (offset) 0x048 //Enable/disable rising-edge (0=>1 transition) detection for interrupt request and wake-up generation p.3412
#define TPIR (offset) 0x04C //Enable/disable falling-edge (1=>0 transition) detection for interrupt request and wake-up generation p.3413
#define TNIR (offset) 0x050 //Enable/disable the debouncing feature for each input line p.3413
#define TCVR (offset) 0x054 //Controls debouncint time (value is global for all ports!) p.3414
#define TOCR (offset) 0x060 //Set the corresponding GPIO_IRQENABLE1 register to 0 p.3415
#define TOWR (offset) 0x064 //Set the corresponding GPIO_IRQENABLE1 register to 0 p.3415
/* REGISTERS END */

typedef struct gptimer {
	int id;
	int irq;	//interrupt request line
	base_address base_address;	//base address of the GPTIMER
} gptimer_t;

gptimer_t * request_timer(void);
gptimer_t* request_timer_by_id(int id);
gptimer_t* get_timer_by_id(int id);
void request_timer_irq(gptimer_t *timer);
void write_timer_counter(gptimer_t *timer, unsigned int count);
void stop_timer(gptimer_t *timer);
void start_timer(gptimer_t *timer);
void disable_timer_interrupt(gptimer_t *timer);
void enable_timer_interrupt(gptimer_t *timer);

#endif /* TIMER_H_ */
