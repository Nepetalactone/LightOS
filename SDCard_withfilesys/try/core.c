/*
 * core.c
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */
#include "core.h"
#include "timer.h"

static uint64_t systemMillis;

uint64_t
getSysMillis( void )
{
return systemMillis + sysTimerValue();
}


