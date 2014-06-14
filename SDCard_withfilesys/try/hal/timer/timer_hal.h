/*
 * timer_hal.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef TIMER_HAL_H_
#define TIMER_HAL_H_

#include <inttypes.h>

#define GPTIMER1_ADDR 0x48318000
#define GPTIMER2_ADDR 0x49032000
#define GPTIMER3_ADDR 0x49034000
#define GPTIMER4_ADDR 0x49036000
#define GPTIMER5_ADDR 0x49038000
#define GPTIMER6_ADDR 0x4903A000
#define GPTIMER7_ADDR 0x4903C000
#define GPTIMER8_ADDR 0x4903E000
#define GPTIMER9_ADDR 0x49040000
#define GPTIMER10_ADDR 0x48086000
#define GPTIMER11_ADDR 0x48088000

#define CAPTURE_IT_BIT 0x04 // capture interrupt
#define OVERFLOW_IT_BIT 0x02 // overflow interrupt
#define MATCH_IT_BIT 0x01 // match interrupt

#define HAL_TIMER_TICKS_PER_MS 1000 // system-ticks per ms

uint32_t halTimerStart( uint32_t addr, uint32_t retries );
uint32_t halTimerStop( uint32_t addr, uint32_t retries );
void halTimerReset( uint32_t addr );
void halTimerClearAllInterrupts( uint32_t addr );
void halTimerClearInterrupt( uint32_t addr, uint32_t itBit );
void halTimerEnableInterrupt( uint32_t addr, uint32_t itBit );
void halTimerSetCompareValue( uint32_t addr, uint32_t v );
void halTimerEnableCompare( uint32_t addr );
void halTimerEnableOverflow( uint32_t addr );
void halTimerSetLoadValue( uint32_t addr, uint32_t v );
void halTimerEnableAutoReload( uint32_t addr );
void halTimerSetPosInc( uint32_t addr, uint32_t v );
void halTimerSetNegInc( uint32_t addr, int32_t v );
void halTimerSetOvfWrapping( uint32_t addr, int32_t v );
void halTimerSetOvfMaskValue( uint32_t addr, int32_t v );
uint32_t halTimerCounterValue( uint32_t addr );
uint32_t halTimerOvfMaskValue( uint32_t addr );

#endif /* TIMER_HAL_H_ */
