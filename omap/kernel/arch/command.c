#include "command.h"


uint32_t exec_command(base_address a, offset o, command c,uint32_t value){
	switch(c){
		case BIT_SET:
			*((address)(a + o))  |= (1 << value);
			break;
		case BIT_CLEAR:
			*((address)(a + o))  &= ~(1 << value);
			break;
		case BIT_TOGGLE:
			*((address)(a + o)) ^= (1 << value);
			break;
		case REG_SET:
			*((address)(a + o)) = value;
			break;
		case REG_CLEAR:
			*((address)(a + o)) = 0x0;
			break;
		case UART_WRITE:
			*((address)(a + o)) = (char)value;
			break;
		case UART_READ:
			value = *((address)(a + o));
			break;
		default:
			break;
	}
	return value;
}

