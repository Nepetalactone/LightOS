/*
 * powermanagement.h
 *
 *  Created on: 20.06.2014
 *      Author: max power
 */

#ifndef POWERMANAGEMENT_H_
#define POWERMANAGEMENT_H_

//S. 452
/*================================CM Instance Summary========================*/
#define IVA2_CM 				(base_address) 0x48004000
#define OCP_System_Reg_CM 		(base_address) 0x48004800
#define MPU_CM 					(base_address) 0x48004900
#define CORE_CM 				(base_address) 0x48004A00
#define SGX_CM 					(base_address) 0x48004B00
#define WKUP_CM 				(base_address) 0x48004C00
#define Clock_Control_Reg_CM 	(base_address) 0x48004D00
#define DSS_CM 					(base_address) 0x48004E00
#define CAM_CM 					(base_address) 0x48004F00
#define PER_CM 					(base_address) 0x48005000
#define EMU_CM 					(base_address) 0x48005100
#define Global_Reg_CM 			(base_address) 0x48005200
#define NEON_CM 				(base_address) 0x48005300
#define USBHOST_CM 				(base_address) 0x48005400

/*===============================CORE_CM Register============================*/
#define FCLKEN1_CORE 	(offset) 0x0000
#define FCLKEN3_CORE 	(offset) 0x0008
#define ICLKEN1_CORE 	(offset) 0x0010
//#define Reserved for non-GP devices 0x0014
#define ICLKEN3_CORE 	(offset) 0x0018
#define IDLEST1_CORE 	(offset) 0x0020
//#define Reserved for non-GP devices. 0x0024
#define IDLEST3_CORE 	(offset) 0x0028
#define AUTOIDLE1_CORE 	(offset) 0x0030
//#define Reserved for non-GP devices. 0x0034
#define AUTOIDLE3_CORE 	(offset) 0x0038
#define CLKSEL_CORE 	(offset) 0x0040
#define CLKSTCTRL_CORE 	(offset) 0x0048
#define CLKSTST_CORE 	(offset) 0x004C

#endif /* POWERMANAGEMENT_H_ */
