#include "command.h"


uint32_t command(base_address a, offset o, command c,uint32_t value){
	switch(c){
		case BIT_SET:
			*(a + o)  |= (1 << value);
			break;
		case BIT_CLEAR:
			*(a + o)  &= ~(1 << value);
			break;
		case BIT_TOGGLE:
			*(a + o) ^= (1 << value);
			break;
		case REG_SET:
			*(a + o) = value;
			break;
		case REG_CLEAR:
			*(a + o) = 0;
			break;
		case UART_WRITE:
			*(a + o) = (char)value;
			break;
		case UART_READ:
			value = *(a + o);
			break;
		default:
			break;
	}
	return value;
}

