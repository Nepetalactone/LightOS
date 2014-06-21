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

void sdcard_set_power_off(base_address mmc_register) {
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

/*
 * this command resets the mmc card page 3180
 */
void sdcard_send_cmd0() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x00040001);
	REG_SET(MMCHS1, ISE, 0x00040001);
	REG_SET(MMCHS1, CMD, 0x00000000);
}

/*
 * this command asks a sdio card to send its operating conditions.
 * this command will fail if there is no sdio card.
 * in case of success the response will be in mmchs1 _mmchs_rsp10 register.
 */
void sdcard_send_cmd1() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x00050001);
	REG_SET(MMCHS1, ISE, 0x00050001);
	REG_SET(MMCHS1, CMD, 0x01020000);
}

/*
 * This command asks the MMC card to send its CID register's content (see Table 22-18). The response is
 * 128 bits wide and is received in MMCHS1.MMCHS_RSP10, MMCHS1.MMCHS_RSP32,
 * MMCHS1.MMCHS_RSP54 and MMCHS1.MMCHS_RSP76 registers
 */
void sdcard_send_cmd2() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x00070001);
	REG_SET(MMCHS1, ISE, 0x00070001);
	REG_SET(MMCHS1, CMD, 0x02090000);
}

/*
 * This command sets MMC card address (see Table 22-19). Useful when MMCHS controller switches to
 * addressed mode.
 */
void sdcard_send_cmd3() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x100f0001);
	REG_SET(MMCHS1, ISE, 0x100f0001);
	REG_SET(MMCHS1, CMD, 0x031a0000);
	REG_SET(MMCHS1, ARG, 0x00010000);
}

/*
 * This command asks a SDIO card to send its operating conditions (see Table 22-14). This command will
 * fail if there is no SDIO card. In case of success the response will be in MMCHS1.MMCHS_RSP10
 * register.
 */
void sdcard_send_cmd5() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x00050001);
	REG_SET(MMCHS1, ISE, 0x00050001);
	REG_SET(MMCHS1, CMD, 0x05020000);
}

/*
 * This command asks a SD card version 2.X to send its operating conditions (see Table 22-15). This
 * command will fail if there is no SD card verison 2.X. In case of success the response will be in
 * MMCHS1.MMCHS_RSP10 register.
 */
void sdcard_send_cmd8() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x100f0001);
	REG_SET(MMCHS1, ISE, 0x100f0001);
	REG_SET(MMCHS1, CMD, 0x81a0000);
}

/*
 * This is a special command used to prevent the card that the following command is going to be an
 * application one (see Table 22-16). This is used to prepare the issuing of ACMD41 (opcode = 41) that
 * usually asks a SD card version 1.X to send its operating conditions. If no SD card version 1.X is
 * connected to the MMCHS controller this command will fail. In case of success, the response will be
 * received in MMCHS1.MMCHS_RSP10 register.
 */
void sdcard_send_cmd55() {
	REG_SET(MMCHS1, CON, 0x00000001);
	REG_SET(MMCHS1, IE, 0x100f0001);
	REG_SET(MMCHS1, ISE, 0x100f0001);
	REG_SET(MMCHS1, CMD, 0x371a0000);
}

