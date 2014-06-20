/*
 * hal_sdcard_omap3530.h
 *
 *  Created on: 20.06.2014
 *      Author: Chris
 */

#ifndef HAL_SDCARD_OMAP3530_H_
#define HAL_SDCARD_OMAP3530_H_

#include "../../../kernel/arch/address.h"
#include "../../../kernel/arch/command.h"

//Base addresses S. 3189
#define MMCHS1 (base_address)0x4809C000
#define MMCHS2 (base_address)0x480B4000
#define MMCHS3 (base_address)0x480AD000

#define SYSCONFIG 	(offset) 0x10	//This register allows controlling various parameters of the Interconnect interface
#define SYSSTATUS 	(offset) 0x14	//This register provides status information about the module excluding the interrupt status information
#define CSRE 		(offset) 0x24	//This register enables the host controller to detect card status errors of response type R1, R1b for all cards
									//and of R5, R5b and R6 response for cards types SD or SDIO.
#define SYSTEST 	(offset) 0x28	//This register is used to control the signals that connect to I/O pins when the module is configured in system
									//test (SYSTEST) mode for boundary connectivity verification.
#define CON 		(offset) 0x2C	//This register is used:
									//- to select the functional mode for any card.
									//- to send an initialization sequence to any card.
									//- to enable the detection on the mmci_dat[1] signal of a card interrupt for SDIO cards only.
									//And also to configure:
									//- specific data and command transfers for MMC cards only.
									//- the parameters related to the card detect and write protect input signals.
#define PWCNT 		(offset) 0x30	//This register is used to program a mmc counter to delay command transfers after activating the PAD power,
									//this value depends on PAD characteristics and voltage.
#define BLK 		(offset) 0x104	//Transfer Length Configuration register This register shall be used for any card.
#define ARG 		(offset) 0x108	//This register contains command argument specified as bit 39-8 of Command-Format
									//These registers must be initialized prior to sending the command itself to the card (write action into the
									//register MMCi.MMCHS_CMD register). Only exception is for a command index specifying stuff bits in
									//arguments, making a write unnecessary.
#define CMD 		(offset) 0x10C	//MMCi.MMCHS_CMD[31:16] = the command register
									//MMCi.MMCHS_CMD[15:0] = the transfer mode.
									//This register configures the data and command transfers. A write into the most significant byte send the
									//command. A write into MMCi.MMCHS_CMD[15:0] registers during data transfer has no effect.
									//This register shall be used for any card
#define RSP10 		(offset) 0x110	//This 32-bit register holds bits positions [31:0] of command response type R1/R1b/R2/R3/R4/R5/R5b/R6
#define RSP32 		(offset) 0x114	//This 32-bit register holds bits positions [63:32] of command response type R2
#define RSP54 		(offset) 0x118	//This 32-bit register holds bits positions [95:64] of command response type R2
#define RSP76 		(offset) 0x11C	//This 32-bit register holds bits positions [127:96] of command response type R2
#define DATA 		(offset) 0x120	//This register is the 32-bit entry point of the buffer for read or write data transfers.
									//The buffer size is 32bits x256(1024 bytes). Bytes within a word are stored and read in little endian format.
									//This buffer can be used as two 512 byte buffers to transfer data efficiently without reducing the throughput.
									//Sequential and contiguous access is necessary to increment the pointer correctly. Random or skipped
									//access is not allowed. In little endian, if the local host accesses this register byte-wise or 16bit-wise, the
									//least significant byte (bits [7:0]) must always be written/read first. The update of the buffer address is done
									//on the most significant byte write for full 32-bit DATA register or on the most significant byte of the last word
									//of block transfer
#define PSTATE 		(offset) 0x124 	//The Host can get status of the Host Controller from this 32-bit read only register.
#define HCTL 		(offset) 0x128	//This register defines the host controls to set power, wake up and transfer parameters.
									//MMCi.MMCHS_HCTL[31:24] = Wake-up control
									//MMCi.MMCHS_HCTL[23:16] = Block gap control
									//MMCi.MMCHS_HCTL[15:8] = Power control
									//MMCi.MMCHS_HCTL[7:0] = Host control
#define SYSCTL 		(offset) 0x12C	//This register defines the system controls to set software resets, clock frequency
									//management and data timeout.
									//MMCHS_SYSCTL[31:24] = Software resets
									//MMCHS_SYSCTL[23:16] = Timeout control
									//MMCHS_SYSCTL[15:0] = Clock control
#define STAT 		(offset) 0x130	//The interrupt status regroups all the status of the module internal events that can generate an interrupt.
									//MMCHS_STAT[31:16] = Error Interrupt Status
									//MMCHS_STAT[15:0] = Normal Interrupt Status
#define IE 			(offset) 0x134	//This register allows to enable/disable the module to set status bits, on an event-by-event basis.
									//MMCHS_IE[31:16] = Error Interrupt Status Enable
									//MMCHS_IE[15:0] = Normal Interrupt Status Enable
#define ISE 		(offset) 0x138	//This register allows to enable/disable the module internal sources of status, on an event-by-event basis.
									//MMCHS_ISE[31:16] = Error Interrupt Signal Enable
									//MMCHS_ISE[15:0] = Normal Interrupt Signal Enable
#define AC12 		(offset) 0x13C	//The host driver may determine which of the errors cases related to Auto
									//CMD12 has occurred by checking this MMCi.MMCHS_AC12 register when an Auto CMD12 Error interrupt
									//occurs.
									//This register is valid only when Auto CMD12 is enabled (MMCi.MMCHS_CMD[2] ACEN bit) and Auto
									//CMD12Error (MMCi.MMCHS_STAT[24] ACE bit) is set to 1.
									//Note: These bits are automatically reset when starting a new adtc command with data.
#define CAPA 		(offset) 0x140	//This register lists the capabilities of the MMC/SD/SDIO host controller
#define CUR_CAPA 	(offset) 0x148	//This register indicates the maximum current capability for each voltage. The value is meaningful if the
									//voltage support is set in the capabilities register (MMCi.MMCHS_CAPA).
									//Initialization of this register (via a write access to this register) depends on the system capabilities. The host
									//driver shall not modify this register after the initialization.
									//This register is only reinitialized by a hard reset (via MMCi_RESET signal)
#define REV			(offset) 0x1FC	//This register contains the hard coded RTL vendor revision number, the version number of
									//SD specification compliancy and a slot status bit.
									//MMCi.MMCHS_REV[31:16] = Host controller version
									//MMCi.MMCHS_REV[15:0] = Slot Interrupt Status





#endif /* HAL_SDCARD_OMAP3530_H_ */
