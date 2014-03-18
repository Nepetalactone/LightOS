/*
 * serial.c
 *
 *  Created on: 12.03.2014
 *      Author: Christian
 */


/* includes. */
#include <stdlib.h>
#include "serial.h"
#include "uart.h"
#include "../../utils/binary.h"



void ser_init(eBaud baud) {
	unsigned int efr, mcr, lcr;

	*((address)(UART03 + DLL)) = 0x0;
	*(UART03 + DLH) = 0x0;

	// Software Reset
	*(UART03 + SYSC) |= (1 << 1);

	while(getBitValue(UART03, SYSS, 0) != 0x0) {
		// wait till reset is done
	}

	// FIFOs and DMA Settings
	// Swich to mode B
	lcr=*(UART03 + LCR); // save lcr
	*(UART03 + LCR) = 0xBF;

	// Save EFR_REG register
	efr = *(UART03 + EFR);
	// Enable enhanced features
	*(UART03 + EFR) |= 0x10;

	// Switch to mode A
	*(UART03 + LCR) = 0x80;

	// Set loopback mode
	*(UART03 + MCR) |= 0x13;

	*(UART03 + WER) |= 0x1;

	// Save MCR Register
	mcr = *(UART03 + MCR);
	*(UART03 + MCR) |= 0x40;

	// Enable Fifo, set trigger levels p2696. Trigger levels are ignored for now.
	*(UART03 + FCR) = 0x7;
	//while((getBitValue(UART03, FCR, 1) != 0x0)  (getBitValue(UART03, FCR, 2) != 0x0)){}


	*(UART03 + FCR) = 0x0;

	// Switch to mode B
	*(UART03 + LCR) = 0xBF;

	// Setup trigger levels for interrupt generation to 0. p2710
	*(UART03 + TLR) = 0x0;
	*(UART03 + SCR) |= 0x3F;

	// Restore efr value
	*(UART03 + EFR) = efr;

	// Switch to mode A
	*(UART03 + LCR) = 0x80;

	// Restore MCR register
	*(UART03 + MCR) = mcr;

	// Restore LCR
	*(UART03 + LCR) = lcr;

	*(UART03 + RHR) = 0;
	*(UART03 + THR) = 0;
	//*(UART03 + LSR);
	*(UART03 + MSR);
	//*(UART03 + IIR);

	// Disable UART access
	*(UART03 + MDR1) |= 0x7;

	// Switch to mode B
	*(UART03 + LCR) = 0xBF;

	// Enable access to IER_REG
	efr = *(UART03 + EFR);
	*(UART03 + EFR) |= 0x10;

	// Switch to register operational mode
	*(UART03 + LCR) = 0x0;

	// Set IER register to 0x0000
	*(UART03 + IER) = 0x0;

	// Switch to mode B
	*(UART03 + LCR) = 0xBF;

	// Protocol, Baud Rate, and Interrupt Settings

	short dll, dlh;
	// Setup divisor and baudrate
	int exBaud = baud;
	switch(exBaud)
	{
		case 300: //300 bps
			dll = 0x27;
			dlh = 0x10;
			break;
		case 600:
			dll = 0x13;
			dlh = 0x88;
			break;
		case 1200:
			dll = 0x09;
			dlh = 0xC4;
			break;
		case 2400:
			dll = 0x04;
			dlh = 0xE2;
			break;
		case 4800:
			dll = 0x02;
			dlh = 0x71;
			break;
		case 9600:
			dll = 0x01;
			dlh = 0x38;
			break;
		case 14400:
			dll = 0x00;
			dlh = 0xD0;
			break;
		case 19200:
			dll = 0x00;
			dlh = 0x9C;
			break;
		case 28800:
			dll = 0x00;
			dlh = 0x68;
			break;
		case 38400:
			dll = 0x00;
			dlh = 0x4E;
			break;
		case 57600:
			dll = 0x00;
			dlh = 0x34;
			break;
		case 115200:
			dll = 0x00;
			dlh = 0x1A;
			break;
		case 230400:
			dll = 0x00;
			dlh = 0x0D;
			break;
		case 460800:
			dll = 0x00;
			dlh = 0x08;
			break;
		case 921600:
			dll = 0x00;
			dlh = 0x04;
			break;
		case 1843000:
			dll = 0x00;
			dlh = 0x02;
			break;
		case 3688400:
			dll = 0x00;
			dlh = 0x01;
			break;
		default:
			// falling back to 115200
			dll = 0x00;
			dlh = 0x1A;
			break;
	}
	// set registers for divider and baudrate
	*(UART03 + DLL) = dll;
	*(UART03 + DLH) = dlh;

	// Switch to operational mode
	*(UART03 + LCR) = 0x0;

	// Load Interrupt Configuration
	*(UART03 + IER) = 0x3;

	// Switch to mode B
	*(UART03 + LCR) = 0xBF;

	// Restore EFR
	*(UART03 + EFR) = efr;

	/* Load Protocol Format
	 * 8 bits length
	 * 1 stop bit
	 * no parity
	 */
	*(UART03 + LCR) = 0x3;

	// Load new mode
	*(UART03 + MDR1) = 0x0;

}


void ser_putc(signed char outChar) {
	*(UART03 + THR) = outChar;
}

void ser_puts( const signed char * outString)
{
	signed char *pxNext;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) outString;
	while( *pxNext )
	{
		ser_putc(*pxNext);
		pxNext++;
	}
}




