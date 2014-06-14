/*
 * timer.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef TIMER_H_
#define TIMER_H_


#include <inttypes.h>

void sysTimerInit( uint32_t ovfAfterMs );
void sysTimerResetInterrupt();
uint32_t sysTimerValue( void );

#endif /* TIMER_H_ */
