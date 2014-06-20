
#include <stdint.h>
#include "sdcard.h"
#include "../power/powermanagement.h"
#include "../arch/command.h"


uint32_t sdcard_init(){

	// NOTE: see OMAP35x.pdf at page 3178

	// 1. Enable the interface clock for the MMCHS1 controller
	BIT_SET( CORE_CM, ICLKEN1_CORE, 6 );
	// 2. Enable the functional clock for the MMCHS1 controller
	//BIT_SET( CM_FCLKEN1_CORE, CM_EN_MMCHS1_BIT );



	// 1. trigger module reset
	BIT_SET(MMCHS1, SYSCONFIG, 1);
	WFC_BIT(MMCHS1, SYSSTATUS, 0);


	//TODO Write 0x0000 0000 in the mmchs_cmd register
	sdcard_write_command(MMCHS1, 0x00000000);
	volatile int i = 0;

	while (i < 100000) {
		i++;
	}


	sdcard_init_end(MMCHS1);

	return 0;
}



