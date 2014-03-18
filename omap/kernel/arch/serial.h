/*
 * serial.h
 *
 *  Created on: 12.03.2014
 *      Author: Christian
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/*
typedef enum
{
	serNO_PARITY,
	serODD_PARITY,
	serEVEN_PARITY,
	serMARK_PARITY,
	serSPACE_PARITY
} eParity;

typedef enum
{
	serSTOP_1,
	serSTOP_2
} eStopBits;

typedef enum
{
	serBITS_5,
	serBITS_6,
	serBITS_7,
	serBITS_8
} eDataBits;
*/

typedef enum
{
	ser50 = 50,
	ser75 = 75,
	ser110 = 110,
	ser134 = 134,
	ser150 = 150,
	ser200 = 200,
	ser300 = 300,
	ser600 = 600,
	ser1200 = 1200,
	ser1800 = 1800,
	ser2400 = 2400,
	ser4800 = 4800,
	ser9600 = 9600,
	ser19200 = 19200,
	ser38400 = 38400,
	ser57600 = 57600,
	ser115200 = 115200
} eBaud;


void ser_init( eBaud baud);
void ser_putc(signed char outChar);

void ser_puts( const signed char * outString);


#endif /* SERIAL_H_ */
