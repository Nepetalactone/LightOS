#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "address.h"
#include "../../utils/binary.h"


#define BIT_MASK(x) S_to_binary_(#x)



void BIT_SET(base_address b, offset o, uint32_t position);
void BIT_CLEAR(base_address b, offset o, uint32_t position);
uint32_t BIT_READ(base_address b, offset o, uint8_t start, uint8_t range);
void BIT_TOGGLE(base_address b, offset o, uint32_t value);
void REG_SET(base_address b, offset o, uint32_t value);
void REG_CLEAR(base_address b, offset o);
char UART_READ(base_address b, offset o, char character);
void UART_WRITE(base_address b, offset o, char character);

void WAIT_FOR_WRITE(base_address b, offset o, uint32_t position);

#endif
