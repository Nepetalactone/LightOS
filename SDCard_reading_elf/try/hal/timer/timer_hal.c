/*
 * timer_hal.c
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#include "timer_hal.h"

// include bitset functions
#include "../../common.h"

// REGISTERS //////////////////////////////////////////////////////////////////////////////
// NOTE: this information can be found in chapter 16 in OMAP35x.pdf page 2609
#define TIDR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x000 ) // R
#define TIOCP_CFG_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x010 ) // RW
#define TISTAT_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x014 ) // R
#define TISR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x018 ) // RW
#define TIER_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x01C ) // RW
#define TWER_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x020 ) // RW
#define TCLR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x024 ) // RW
#define TCRR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x028 ) // RW
#define TLDR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x02C ) // RW
#define TTGR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x030 ) // RW
#define TWPS_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x034 ) // R
#define TMAR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x038 ) // RW
#define TCAR1_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x03C ) // R
#define TSICR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x040 ) // RW
#define TCAR2_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x044 ) // R
// the following registers are avilable in GPTIMER 1,2 & 10 only
#define TPIR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x048 ) // RW
#define TNIR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x04C ) // RW
#define TCVR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x050 ) // RW
#define TOCR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x054 ) // RW
#define TOWR_GPT( addr ) READ_REGISTER_OFFSET( addr, 0x058 ) // RW
//////////////////////////////////////////////////////////////////////////////////////////

// SPECIAL REGISTER BITS ////////////////////////////////////////////////////////////////
#define TCLR_ONOFF_BIT 0x01
#define TCLR_COMPARE_BIT 0x40
#define TCLR_OVERFLOW_BIT 0x400
#define TCLR_AUTORELOAD_BIT 0x02

#define TISR_ALLIT_BITS 0x07
//////////////////////////////////////////////////////////////////////////////////////////

uint32_t
halTimerStart( uint32_t addr, uint32_t retries )
{
uint32_t i = 0;

// NOTE: start the timer by writing 1 into the bit 0 of TCLR
BIT_SET( TCLR_GPT( addr ), TCLR_ONOFF_BIT );

for ( i = retries; i > 0; --i )
{
if ( BIT_CHECK( TCLR_GPT( addr ), TCLR_ONOFF_BIT ) )
{
return 0;
}
}

return 1;
}

uint32_t
halTimerStop( uint32_t addr, uint32_t retries )
{
uint32_t i = 0;

// NOTE: stop the timer by writing 0 into the bit 0 of TCLR
BIT_CLEAR( TCLR_GPT( addr ), TCLR_ONOFF_BIT );

for ( i = retries; i > 0; --i )
{
if ( ! BIT_CHECK( TCLR_GPT( addr ), TCLR_ONOFF_BIT ) )
{
return 0;
}
}

return 1;
}

void
halTimerReset( uint32_t addr )
{
// NOTE: reset timer by writing any value to TTGR
BIT_CLEAR_ALL( TTGR_GPT( addr ) );
}

void
halTimerClearAllInterrupts( uint32_t addr )
{
// NOTE: clear potentiall pending interrupts by writing 1 to bit 0-2 to TISR
// which is equal of setting the value of TISR to 7
BIT_SET( TISR_GPT( addr ), TISR_ALLIT_BITS );
}

void
halTimerClearInterrupt( uint32_t addr, uint32_t itBit )
{
// NOTE: writing the according bit clears a potentially pending interrupt
BIT_SET( TISR_GPT( addr ), itBit );
}

void
halTimerEnableInterrupt( uint32_t addr, uint32_t itBit )
{
// NOTE: enable specific interrupt by writing the according bit to TIER
BIT_SET( TIER_GPT( addr ), itBit );
}

void
halTimerSetCompareValue( uint32_t addr, uint32_t value )
{
// NOTE: set compare-value to be compared to counter-value in match-interrupt mode
TMAR_GPT( addr ) = value;
}

void
halTimerEnableCompare( uint32_t addr )
{
// NOTE: enable compare-mode by setting bit 6 of TCLR
BIT_SET( TCLR_GPT( addr ), TCLR_COMPARE_BIT );
}

void
halTimerEnableOverflow( uint32_t addr )
{
// NOTE: enable compare-mode by setting bit 11 of TCLR
BIT_SET( TCLR_GPT( addr ), TCLR_OVERFLOW_BIT );
}

void
halTimerSetLoadValue( uint32_t addr, uint32_t v )
{
// NOTE: set load-value by writing to TLDR register
// this value will be written to counter value when overflow
TLDR_GPT( addr ) = v;
}

void
halTimerEnableAutoReload( uint32_t addr )
{
// NOTE: enable auto-reload by setting bit 1 of TCLR
BIT_SET( TCLR_GPT( addr ), TCLR_AUTORELOAD_BIT );
}

void
halTimerSetPosInc( uint32_t addr, uint32_t v )
{
// NOTE: only available for GPTIMERS 1,2 & 10
TPIR_GPT( addr ) = v;
}

void
halTimerSetNegInc( uint32_t addr, int32_t v )
{
// NOTE: only available for GPTIMERS 1,2 & 10
TNIR_GPT( addr ) = v;
}

void
halTimerSetOvfWrapping( uint32_t addr, int32_t v )
{
TOWR_GPT( addr ) = v;
}

void
halTimerSetOvfMaskValue( uint32_t addr, int32_t v )
{
TOCR_GPT( addr ) = v;
}

uint32_t
halTimerCounterValue( uint32_t addr )
{
return TCRR_GPT( addr );
}

uint32_t
halTimerOvfMaskValue( uint32_t addr )
{
return TOCR_GPT( addr );
}
