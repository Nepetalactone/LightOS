#include "hal_sdcard_omap3530.h"


void sdcard_soft_reset(address mmc_register) {
	BIT_SET(mmc_register, SYSCONFIG, 1);
}

void sdcard_wake_up() {

}
