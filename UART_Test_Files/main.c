/*
 * main.c
 */
#include <stdio.h>

typedef volatile unsigned int* address;

#define MDR_REG (address) 0x49020020
#define DLL_REG (address) 0x49020000
#define DLH_REG (address) 0x49020004 // Clock msb Stores the 6-bit most-significant bit (MSB) divisor value
#define SYSC_REG (address) 0x49020054 // Smart idle: Idle request is acknowledged based on the internal module activity.
#define SYSS_REG (address) 0x49020058 // bit 0 RESETDONE
#define LCR_REG (address) 0x4902000C // Line control register
#define EFR_REG (address) 0x49020008 // enahnced feature register
#define MCR_REG (address) 0x49020010 // modem ctrl reg
#define WER_REG (address) 0x4902005C // wake up enable
#define THR_REG (address) 0x49020000 // transmission hold reg

/*
The DLL register, with DLH_REG, stores the 14-bit divisor for generation of the baud clock in the baud rate
generator. DLH_REG stores the most-significant part of the divisor. DLL_REG stores the least-significant part of
the divisor.
 */

void serialWrite(const signed char* const pcString);


int main(void) {
	float b = 15/2;
	printf("%d\n",b);
	// serial connection on uart 3
	serialWrite((const signed char* )"Hallo LightOS! \n I can write over serial port :-)");

	printf("asdf");

	return 0;
}

void serialWrite( const signed char * const pcString)
{
	signed char *pxNext;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		*THR_REG = *pxNext;
		pxNext++;
	}
}

