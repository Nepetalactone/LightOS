#include "hal_sdcard_omap3530.h"
#include "../power/hal_power.h"

void sdcard_soft_reset(base_address mmc_register) {
	//SOFTWARE RESET
	BIT_SET(mmc_register, SYSCONFIG, 1);
	//WAIT FOR RESET
	WFC_BIT(MMCHS1, SYSSTATUS, 0);
}

void sdcard_set_data_transfer_width(base_address mmc_register, transfer_width width) {
	switch(width){
	case SHORT:
		BIT_CLEAR(mmc_register, HCTL, 1);
		break;
	case LONG:
		BIT_SET(mmc_register, HCTL, 1);
		break;
	default:
		/* not yet supported */
		break;
	}
}

void sdcard_wake_up() {

}

void sdcard_bus_config() {

}

void sdcard_set_power_on(base_address mmc_register) {
	BIT_SET(mmc_register, HCTL, 8);
	WFW_BIT(mmc_register, HCTL, 8);
}

void sdcard_set_power_off() {
	BIT_CLEAR(mmc_register, HCTL, 8);
}


void sdcard_set_voltage(voltage_level level) {
	switch (level){
	case LOW:
		BIT_SET(MMCHS1, HCTL, 9);
		BIT_CLEAR(MMCHS1, HCTL, 10);
		BIT_SET(MMCHS1, HCTL, 11);
		break;
	case MED:
		BIT_CLEAR(MMCHS1, HCTL, 9);
		BIT_SET(MMCHS1, HCTL, 10);
		BIT_SET(MMCHS1, HCTL, 11);
		break;
	case HIGH:
		BIT_SET(MMCHS1, HCTL, 9);
		BIT_SET(MMCHS1, HCTL, 10);
		BIT_SET(MMCHS1, HCTL, 11);
		break;
	default:
		/* not yet supported */
		break;
	}
	//BIT_SET();
}

void sdcard_bus_conf(base_address mmc_register) {
	BIT_CLEAR(mmc_register, CON, 0);
	BIT_CLEAR(mmc_register, CON, 5);
}

void sdcard_enable_functional_clock() {
	ENABLE_SDCARD_FUNCTIONAL_CLOCK;
}

void sdcard_enable_interface_clock(){
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

