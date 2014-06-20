#include "hal_sdcard_omap3530.h"
#include "../power/hal_power.h"

void sdcard_soft_reset(base_address mmc_register) {
	//SOFTWARE RESET
	BIT_SET(mmc_register, SYSCONFIG, 1);
	//WAIT FOR RESET
	WFC_BIT(MMCHS1, SYSSTATUS, 0);
}

void sdcard_wake_up() {

}

void sdcard_functional_clock() {
	ENABLE_SDCARD_FUNCTIONAL_CLOCK;
}

void sdcard_interface_clock(){
	ENABLE_SDCARD_INTERFACE_CLOCK;
}

void sdcard_write_command(base_address mmc_register, uint32_t value) {
	REG_SET(mmc_register, CON, value);
}

void sdcard_clear_command_state(base_address mmc_register) {
	BIT_SET(mmc_register, STAT, 0);
}

void sdcard_init_end(base_address mmc_register) {
	BIT_CLEAR(mmc_register, CON, 1);
}

void sdcard_clear_stat_register(base_address mmc_register) {
	REG_SET(mmc_register, STAT, 0xFFFFFFFF)
}
