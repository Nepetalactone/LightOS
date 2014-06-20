
#include <stdint.h>
#include "sdcard.h"
#include "../../hal/omap3530/power/hal_power.h"
#include "../../hal/omap3530/sdcard/hal_sdcard_omap3530.h"
#include "../arch/command.h"
#include "../../hal/omap3530/sdcard/hal_sdcard_omap3530.h"

uint32_t sdcard_module_init(base_address mmc_register){

	//Initialization page 3160 - configure interface and functional clocks
	sdcard_enable_interface_clock();
	sdcard_enable_functional_clock();

	//Software reset of the SD host controller
	sdcard_soft_reset(mmc_register);

	//wait 1 ms
	volatile int i = 0;

	while (i < 100000) {
		i++;
	}
	//TODO Write 0x0000 0000 in the mmchs_cmd register
	sdcard_write_command(mmc_register, 0x00000000);
	sdcard_init_end(mmc_register);

	init_bus_configuration(mmc_register);
	return 0;
}


void init_bus_configuration(base_address mmc_register) {
	sdcard_bus_config();
	sdcard_set_voltage(MED);
	sdcard_set_power_on();

	sdcard_set_data_transfer_width(mmc_register, SHORT);

}
