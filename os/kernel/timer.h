
#ifndef TIMER_H_
#define TIMER_H_

#include "address.h"
#include "interrupt.h"


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
#define TCLR (offset) 0x024 //This register controls optional features specific to the timer functionality. p.2643
#define TCRR (offset) 0x028 //This register holds the value of the internal counter p.2645
#define TLDR (offset) 0x02C //This register holds the timer load values p.2646
#define TTGR (offset) 0x030 //This register triggers a counter reload of timer by writing any value in it p.2647
#define TWPS (offset) 0x034 //This register indicates if a Write-Posted is pending p.2648
#define TMAR (offset) 0x038 //This register holds the value to be compared with the counter value p.2650
#define TCAR1 (offset) 0x03C //This register holds the first captured value of the counter register p.2651
#define TSICR (offset) 0x040 //This register contains the bits that control the interface between the L4 interface and functional clock domains-posted mode and functional SW reset p.2652
#define TCAR2 (offset) 0x044 //This register holds the second captured value of the counter register p.2653
#define TPIR (offset) 0x048 //This register is used for 1 ms tick generation. The TPIR register holds the value of the positive increment. The value of this register is added with the value of the TCVR to define whether next value loaded in TCRR will be the sub-period value or the over-period value. p.2654
#define TNIR (offset) 0x04C //This register is used for 1 ms tick generation. The TNIR register holds the value of the negative increment. The value of this register is added with the value of the TCVR to define whether next value loaded in TCRR will be the sub-period value or the over-period value. p.2654
#define TCVR (offset) 0x050 //This register is used for 1 ms tick generation. The TCVR register defines whether next value loaded in TCRR will be the sub-period value or the over-period value p.2655
#define TOCR (offset) 0x054 //This register is used to mask the tick interrupt for a selected number of ticks p.2656
#define TOWR (offset) 0x058 //This register holds the number of masked overflow interrupts p.2656
/* REGISTERS END */


typedef enum {
	trigger_Overflow = 10,
	trigger_OverflowMatch = 11
} trigger_mode;

void init_timer(base_address timer, uint32_t millisec, interrupt_handler handler, trigger_mode mode);
void set_compare_value(base_address timer, uint32_t millisec);
void start_timer(base_address timer);
void stop_timer(base_address timer);
void reset_timer(base_address timer);
void reset_timer_counter(base_address timer);
void timer_set_trigger_mode(base_address timer, trigger_mode mode);
void enable_timer_interrupt(base_address timer);
void disable_timer_interrupt(base_address timer);
uint8_t is_timer_running(base_address timer);
void timer_enable_compare(base_address timer);

#endif /* TIMER_H_ */
