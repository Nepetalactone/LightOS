#include "address.h"

address __CONCRETE_ADDRESS(base_address b, offset o){
	return (address) (b + o);
}
