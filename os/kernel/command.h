#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "address.h"
#include "binary.h"


#define BIT_MASK(x) S_to_binary_(#x)



void BIT_SET(base_address a, offset o, uint32_t value);
void BIT_CLEAR(base_address a, offset o, uint32_t value);
uint32_t BIT_READ(base_address a, offset o, uint8_t start, uint8_t range);
void BIT_TOGGLE(base_address a, offset o, uint32_t value);
void REG_SET(base_address a, offset o, uint32_t value);
void REG_CLEAR(base_address a, offset o);
char UART_READ(base_address a, offset o, char character);
void UART_WRITE(base_address a, offset o, char character);


void WAIT_FOR_WRITE(base_address a, offset o, uint32_t position);

#endif
