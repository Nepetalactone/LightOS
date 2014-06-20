
#include <stdint.h>
#include "sdcard.h"



uint32_t sdcard_init(){

	// NOTE: see OMAP35x.pdf at page 3178

	// 1. Enable the interface clock for the MMCHS1 controller
	BIT_SET( CM_ICLKEN1_CORE, CM_EN_MMCHS1_BIT );
	// 2. Enable the functional clock for the MMCHS1 controller
	BIT_SET( CM_FCLKEN1_CORE, CM_EN_MMCHS1_BIT );



	// 1. trigger module reset
	BIT_SET(MMCHS1, SYSCONFIG, 1);
	WFC_BIT(MMCHS1, SYSSTATUS, 0);

}



