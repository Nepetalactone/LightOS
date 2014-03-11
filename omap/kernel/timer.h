/*
 * timer.h
 *
 *  Created on: 08.03.2014
 *      Author: Chris
 */

#ifndef TIMER_H_
#define TIMER_H_

/*
 *General Purpose Timer Register Map S. 2632
 *4K Bytes
 */
typedef volatile unsigned int* address;

#define GPTimer1 (address) 0x48318000
#define GPTimer2 (address) 0x49032000
#define GPTimer3 (address) 0x49034000
#define GPTimer4 (address) 0x49036000
#define GPTimer5 (address) 0x49038000
#define GPTimer6 (address) 0x4903A000
#define GPTimer7 (address) 0x4903C000
#define GPTimer8 (address) 0x4903E000
#define GPTimer9 (address) 0x49040000
#define GPTimer10 (address) 0x48086000
#define GPTimer11 (address) 0x48088000




#endif /* TIMER_H_ */
