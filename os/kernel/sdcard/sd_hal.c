/*
 * sd_hal.c
 *
 *  Created on: 11.06.2014
 *      Author: phil
 *      (c) Jonathan Thaler
 */


#include "sd_hal.h"

#include "common.h"

#include <string.h>
// module-local data //////////////////////////////////////////////
// module-local functions /////////////////////////////////////////
// INITIALZATION & IDENTIFICATION
static void enableIfaceAndFunctionalClock( void );
static uint32_t softwareReset( void );
static void selectSupportedVoltage( uint32_t voltage );
static uint32_t resetLines( uint32_t lines );
static void systemConfig( uint32_t flags );
static void setBusWidth( uint32_t width );
static void setBusVoltage( uint32_t voltage );
static uint32_t setBusPower( bool enable );
static uint32_t setBusFrequency( uint32_t freq_in, uint32_t freq_out, bool bypass );
static uint32_t setInternalClock( bool enable );
static void sendInitStream( void );
static uint32_t identifyCard( void );
static uint32_t configBusWidth( void );
static uint32_t configTransferSpeed( void );

// ACCESS TO CARD
static uint32_t readTransferBuffer( uint32_t nBytes, uint8_t* buffer );

// HELPERS
static uint32_t isCardBusy( void );
static uint32_t awaitDataLineAvailable( uint32_t retries );
static uint32_t isBufferReadReady( void );
static void clearBufferReadReady( void );
static uint32_t isTransferComplete( void );
static uint32_t resetMMCIDataLine( uint32_t retries );
static uint32_t resetMMCICmdLine( uint32_t retries );
static uint32_t awaitCommandResponse( void );
static uint32_t awaitInternalClockStable( uint32_t retries );
static uint32_t isCommandComplete( void );
static uint32_t isCommandTimeout( void );
static uint32_t isDataTimeout( void );
static void clearCommandComplete( void );
static void clearTransferComplete( void );
static void clearInterruptBits( uint32_t bits );
static void clearAllInterruptsStatus( void );
static void setDataTimeout( uint32_t timeout );

// COMMANDS USED DURING INITIALZATION & IDENTIFICATION
// NOTE: see page 60 of sd_card spec 2.0 for details on commands
static uint32_t sendCmd0( void );
static uint32_t sendCmd2( void );
static uint32_t sendCmd3( void );
static uint32_t sendCmd5( void );
static uint32_t sendCmd6( uint32_t arg );
static uint32_t sendCmd7( void );
static uint32_t sendCmd8( void );
static uint32_t sendCmd9( void );
static uint32_t sendCmd12( void );
static uint32_t sendCmd16( void );
static uint32_t sendCmd18( uint32_t addr, uint32_t nblk );
static uint32_t sendCmd17( uint32_t addr );
static uint32_t sendCmd55( void );
static uint32_t sendACmd6( uint8_t busWidth );
static uint32_t sendACmd41( uint8_t hcsFlag );
static uint32_t sendACmd51();
///////////////////////////////////////////////////////////////////

// NOTE: omap35x.pdf page 3189

// NOTE: this implementation will work for MMCHS1 only
#define MMCHS1_ADDR			0x4809C000
#define MMCHS2_ADDR			0x480B4000
#define MMCHS3_ADDR			0x480AD000

#define SELECTED_CHS		MMCHS1_ADDR

// REGISTERS
#define MMCHS_SYSCONFIG		READ_REGISTER_OFFSET( SELECTED_CHS, 0x010 ) 	// RW
#define MMCHS_SYSSTATUS		READ_REGISTER_OFFSET( SELECTED_CHS, 0x014 ) 	// R
#define MMCHS_CSRE			READ_REGISTER_OFFSET( SELECTED_CHS, 0x024 ) 	// RW
#define MMCHS_SYSTEST		READ_REGISTER_OFFSET( SELECTED_CHS, 0x028 ) 	// RW
#define MMCHS_CON			READ_REGISTER_OFFSET( SELECTED_CHS, 0x02C ) 	// RW
#define MMCHS_PWCNT			READ_REGISTER_OFFSET( SELECTED_CHS, 0x030 ) 	// RW
#define MMCHS_BLK			READ_REGISTER_OFFSET( SELECTED_CHS, 0x104 ) 	// RW
#define MMCHS_ARG			READ_REGISTER_OFFSET( SELECTED_CHS, 0x108 ) 	// RW
#define MMCHS_CMD			READ_REGISTER_OFFSET( SELECTED_CHS, 0x10C ) 	// RW
#define MMCHS_RSP10			READ_REGISTER_OFFSET( SELECTED_CHS, 0x110 ) 	// R
#define MMCHS_RSP32			READ_REGISTER_OFFSET( SELECTED_CHS, 0x114 ) 	// R
#define MMCHS_RSP54			READ_REGISTER_OFFSET( SELECTED_CHS, 0x118 ) 	// R
#define MMCHS_RSP76			READ_REGISTER_OFFSET( SELECTED_CHS, 0x11C ) 	// R
#define MMCHS_DATA			READ_REGISTER_OFFSET( SELECTED_CHS, 0x120 ) 	// RW
#define MMCHS_PSTATE		READ_REGISTER_OFFSET( SELECTED_CHS, 0x124 ) 	// R
#define MMCHS_HCTL			READ_REGISTER_OFFSET( SELECTED_CHS, 0x128 ) 	// RW
#define MMCHS_SYSCTL		READ_REGISTER_OFFSET( SELECTED_CHS, 0x12C ) 	// RW
#define MMCHS_STAT			READ_REGISTER_OFFSET( SELECTED_CHS, 0x130 ) 	// RW
#define MMCHS_IE			READ_REGISTER_OFFSET( SELECTED_CHS, 0x134 ) 	// RW
#define MMCHS_ISE			READ_REGISTER_OFFSET( SELECTED_CHS, 0x138 ) 	// RW
#define MMCHS_AC12			READ_REGISTER_OFFSET( SELECTED_CHS, 0x13C ) 	// R
#define MMCHS_CAPA			READ_REGISTER_OFFSET( SELECTED_CHS, 0x140 ) 	// RW
#define MMCHS_CUR_CAPA		READ_REGISTER_OFFSET( SELECTED_CHS, 0x148 ) 	// RW
#define MMCHS_REV			READ_REGISTER_OFFSET( SELECTED_CHS, 0x1FC ) 	// R

// BITS for accessing the registers
#define MMCHS_SYSCONFIG_AUTOIDLE_BIT				0x1
#define MMCHS_SYSCONFIG_SOFTRESET_BIT 				0x2
#define MMCHS_SYSCONFIG_ENAWAKEUP_BIT 				0x4
#define MMCHS_SYSCONFIG_SIDLE_BITS 					0x18
#define MMCHS_SYSCONFIG_CLCKACT_BITS				0x300

#define MMCHS_SYSSTATUS_RESETDONE_BIT				0x1

#define MMCHS_HCTL_DTW_BIT 							0x2
#define MMCHS_HCTL_SDBP_BIT							0x100
#define MMCHS_HCTL_SDVS_18V_BIT 					0xA00
#define MMCHS_HCTL_SDVS_30V_BIT 					0xC00
#define MMCHS_HCTL_SDVS_33V_BIT 					0xE00
#define MMCHS_HCTL_IWE_BIT							0x1000000
#define MMCHS_HCTL_INS_BIT							0x2000000

#define MMCHS_CIRQ_ENABLE_BIT						0x100

#define MMCHS_SYSCTL_ICE_BIT 						0x1
#define MMCHS_SYSCTL_ICS_BIT 						0x2
#define MMCHS_SYSCTL_CEN_BIT						0x4
#define MMCHS_SYSCTL_CLKD_BIT						0x8000
#define MMCHS_SYSCTL_DTO_MASK						0xF0000
#define MMCHS_SYSCTL_DTO_BIT_INDEX					16
#define MMCHS_SYSCTL_SRA_BIT 						0x1000000
#define MMCHS_SYSCTL_SRC_BIT 						0x2000000
#define MMCHS_SYSCTL_SRD_BIT 						0x4000000

#define MMCHS_CON_OD_BIT 							0x1
#define MMCHS_CON_INIT_BIT 							0x2
#define MMCHS_CON_DW8_BIT 							0x20
#define MMCHS_CON_CEATA_BIT 						0x1000

#define MMCHS_CAPA_VS33_BIT							0x1000000
#define MMCHS_CAPA_VS30_BIT							0x2000000
#define MMCHS_CAPA_VS18_BIT							0x4000000

#define MMCHS_STAT_CC_BIT							0x1
#define MMCHS_STAT_TC_BIT 							0x2
#define MMCHS_STAT_BWR_BIT							0x10
#define MMCHS_STAT_BRR_BIT							0x20
#define MMCHS_STAT_CTO_BIT 							0x10000
#define MMCHS_STAT_CCRC_BIT 						0x20000
#define MMCHS_STAT_CEB_BIT							0x40000
#define MMCHS_STAT_CIE_BIT							0x80000
#define MMCHS_STAT_DTO_BIT 							0x100000
#define MMCHS_STAT_DCRC_BIT 						0x200000
#define MMCHS_STAT_DEB_BIT 							0x400000
#define MMCHS_STAT_CERR_BIT							0x10000000

#define MMCHS_RSP10_BUSY_BIT						0x80000000

#define MMCHS_PSTATE_CMDI_BIT						0x1
#define MMCHS_PSTATE_DATI_BIT						0x24

#define MMCHS_BLK_NBLK_BITS 						0xFFFF0000
#define MMCHS_BLK_BLEN_BITS 						0x7FF

#define MMCHS_CMD_DMA_BIT 							0x1
#define MMCHS_CMD_BCE_BIT 							0x2
#define MMCHS_CMD_ACEN_BIT							0x4
#define MMCHS_CMD_DDIR_BIT 							0x10
#define MMCHS_CMD_MSBS_BIT 							0x20
#define MMCHS_CMD_RSP_TYPE_136_BIT 					0x10000
#define MMCHS_CMD_RSP_TYPE_48_BIT 					0x20000
#define MMCHS_CMD_RSP_TYPE_48BUSY_BIT 				0x30000
#define MMCHS_CMD_CCCE_BIT 							0x80000
#define MMCHS_CMD_CICE_BIT 							0x100000
#define MMCHS_CMD_DP_BIT 							0x200000
#define MMCHS_CMD_TYPE_BIT_START					22
#define MMCHS_CMD_INDX_BIT_START					24
#define MMCHS_CMD_INDX_BITS( cmd )					( cmd & 0x3F ) << MMCHS_CMD_INDX_BIT_START

#define MMCHS_IE_CC_BIT 							0x1
#define MMCHS_IE_TC_BIT 							0x2
#define MMCHS_IE_BWR_BIT 							0x10
#define MMCHS_IE_BRR_BIT 							0x20
#define MMCHS_IE_CTO_BIT 							0x10000
#define MMCHS_IE_CCRC_BIT 							0x20000
#define MMCHS_IE_CEB_BIT 							0x40000
#define MMCHS_IE_CIE_BIT 							0x80000
#define MMCHS_IE_DTO_BIT 							0x100000
#define MMCHS_IE_DCRC_BIT 							0x200000
#define MMCHS_IE_DEB_BIT 							0x400000
#define MMCHS_IE_AC12_BIT 							0x1000000
#define MMCHS_IE_CERR_BIT 							0x10000000
#define MMCHS_IE_BADA_BIT 							0x20000000

#define MMCHS_ISE_CIRQ_SIGEN_BIT					0x100

#define CMD8_CHECK_PATTERN  	0xAA
#define CMD8_VOLT_2P7_3P6    	0x000100u
#define SD_OCR_VDD_WILDCARD		(0x1FF << 15)

#define BUS_WIDTH_1BIT			0x1
#define	BUS_WIDTH_4BIT			0x4
#define	BUS_WIDTH_8BIT			0x8

#define SDMMC_CONTROLLER_CLOCK	96000000		// BEAGLEBONE SD/MMC controller has a clock of 48MHz. see BBSRM_lates page 24. BUT OMAP35x.pdf says 96MHz at page 3143 22.3.1.1.1 Module Clocks
#define SDMMC_BUS_CLOCK			400000			// BEAGLEBONE SD/MMC bus has a clock of 400KHz. see ?

////////////////////////////////////////////
// NOTE: see OMAP35x.pdf page 452
#define CORE_CM_ADDR 					0x48004A00
#define CONTROL_PADCONF_MMC1_ADDR		0x48002144

#define CM_ICLKEN1_CORE				READ_REGISTER_OFFSET( CORE_CM_ADDR, 0x010 ) 	// RW
#define CM_FCLKEN1_CORE				READ_REGISTER_OFFSET( CORE_CM_ADDR, 0x0 ) 		// RW

#define CM_EN_MMCHS1_BIT			0x1000000
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
#define SD_CARD_VERSION(sdcard)		((sdcard).raw_scr[0] & 0xF)
#define SD_CARD_BUSWIDTH(sdcard)	(((sdcard).raw_scr[0] & 0xF00) >> 8)

#define SD_SWITCH_MODE        0x80FFFFFF
#define SD_CMD6_GRP1_SEL      0xFFFFFFF0
#define SD_CMD6_GRP1_HS       0x1

#define SD_TRANSPEED_25MBPS		(0x32u)
#define SD_TRANSPEED_50MBPS		(0x5Au)

#define SD_CARD_CSD_VERSION(crd) (((crd).raw_csd[3] & 0xC0000000) >> 30)

#define SD_CSD0_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x000003FF) << 2) | ((csd1 & 0xC0000000) >> 30))
#define SD_CSD0_MULT(csd3, csd2, csd1, csd0) ((csd1 & 0x00038000) >> 15)
#define SD_CSD0_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD0_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD0_DEV_SIZE(crd) SD_CSD0_DEV_SIZE((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD0_MULT(crd) SD_CSD0_MULT((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD0_RDBLKLEN(crd) SD_CSD0_RDBLKLEN((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD0_TRANSPEED(crd) SD_CSD0_TRANSPEED((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD0_NUMBLK(crd) ((SD_CARD0_DEV_SIZE((crd)) + 1) * (1 << (SD_CARD0_MULT((crd)) + 2)))
#define SD_CARD0_SIZE(crd) ((SD_CARD0_NUMBLK((crd))) * (1 << (SD_CARD0_RDBLKLEN(crd))))

#define SD_CSD1_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x0000003F) << 16) | ((csd1 & 0xFFFF0000) >> 16))
#define SD_CSD1_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD1_DEV_SIZE(crd) SD_CSD1_DEV_SIZE((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd) SD_CSD1_RDBLKLEN((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD1_TRANSPEED(crd) SD_CSD1_TRANSPEED((crd).raw_csd[3], (crd).raw_csd[2], (crd).raw_csd[1], (crd).raw_csd[0])
#define SD_CARD1_SIZE(crd) ((SD_CARD1_DEV_SIZE((crd)) + 1) * (512 * 1024))
/////////////////////////////////////////////////////////////////////////////////////////////

// NOTE: see OMAP35x.pdf 3141  Relation Between Configuration and Name of Response Type

// NOTE: internal buffer consists of 2x512 blocks and allows ping pong reading/writing => two transfers
// at the same time if one requests only blocks LE 512 => hardcode block-size to 512
#define BLOCK_LEN 		512

// the information about the current card
static CARD_INFO cardInfo;

uint32_t sdHalInit()
{
	enableIfaceAndFunctionalClock();

	if ( softwareReset() )
	{
		return 1;
	}

	if ( resetLines( MMCHS_SYSCTL_SRA_BIT ) )
	{
		return 1;
	}

	selectSupportedVoltage( MMCHS_CAPA_VS18_BIT | MMCHS_CAPA_VS30_BIT );

	// want enable wakeup when card is inserted
	systemConfig( MMCHS_SYSCONFIG_AUTOIDLE_BIT /*| MMCHS_SYSCONFIG_ENAWAKEUP_BIT */ );

	setBusWidth( BUS_WIDTH_1BIT );

	setBusVoltage( MMCHS_HCTL_SDVS_30V_BIT );

	if ( setBusPower( TRUE ) )
	{
		return 1;
	}

	// need some initial clock divider otherwise would not work to set the bus frequency
	MMCHS_SYSCTL = 0x0000a007;

	if ( setBusFrequency( SDMMC_CONTROLLER_CLOCK, SDMMC_BUS_CLOCK, FALSE ) )
	{
		return 1;
	}

	/*
	// wake-up event enabled on card-insertion
	MMCHS_HCTL |= MMCHS_HCTL_INS_BIT;
	// clear all and set card-interrupt enable
	MMCHS_ISE = MMCHS_ISE_CIRQ_SIGEN_BIT;

	// NOTE: the next only when card is inserted!
	*/

	sendInitStream();

	if ( identifyCard() )
	{
		return 1;
	}

	if ( configBusWidth() )
	{
		return 1;
	}

	configTransferSpeed();

	return 0;
}

uint32_t sdHalReadBlocks( uint32_t block, uint32_t nblk, uint8_t* buffer )
{
	// NOTE: see OMAP35x.pdf page 3168

	uint32_t address = 0;

	// check if datalines are available and stop after some retries
	if ( awaitDataLineAvailable( 0xFF ) )
	{
		return 1;
	}

	if ( cardInfo.highCap )
	{
		// high-capacity cards read in blocks
		address = block;
	}
	else
	{
		// standard-capacity cards read in bytes
		address = block * BLOCK_LEN;
	}

	if ( 1 == nblk )
	{
		// NOTE: address differ for standard and high capacity: see page 52 of sd_spec 2.0 4.3.14 Command Functional Difference in High Capacity SD Memory Card
		if ( sendCmd17( address ) )
		{
			// an error occured during sending the command, return immediately
			return 1;
		}
	}
	else
	{
		// NOTE: address differ for standard and high capacity: see page 52 of sd_spec 2.0 4.3.14 Command Functional Difference in High Capacity SD Memory Card
		if ( sendCmd18( address, nblk ) )
		{
			// an error occured during sending the command, return immediately
			return 1;
		}
	}

	uint32_t nBytes = nblk * BLOCK_LEN;

	if ( readTransferBuffer( nBytes, buffer ) )
	{
		return 1;
	}

	if ( 1 < nblk )
	{
		if ( sendCmd12() )
		{
			return 1;
		}
	}

	// transfer complete, no error occured
	return 0;
}

void enableIfaceAndFunctionalClock( void )
{
	// NOTE: see OMAP35x.pdf at page 3178

	// 1. Enable the interface clock for the MMCHS1 controller
	BIT_SET( CM_ICLKEN1_CORE, CM_EN_MMCHS1_BIT );
	// 2. Enable the functional clock for the MMCHS1 controller
	BIT_SET( CM_FCLKEN1_CORE, CM_EN_MMCHS1_BIT );
}

uint32_t softwareReset()
{
	// NOTE: see OMAP35x.pdf page 3160f and at page 3178
	uint32_t i = 0;

	// 1. trigger module reset
	BIT_SET( MMCHS_SYSCONFIG, MMCHS_SYSCONFIG_SOFTRESET_BIT );

	// 2. await finishing of module reset
	for ( i = 0xFF; i > 0; --i )
	{
		if ( BIT_CHECK( MMCHS_SYSSTATUS, MMCHS_SYSSTATUS_RESETDONE_BIT ) )
		{
			return 0;
		}
	}

	return 1;

}

uint32_t resetLines( uint32_t lines )
{
	uint32_t i = 0;

	BIT_SET( MMCHS_SYSCTL, lines );

	for ( i = 0xFF; i > 0; --i )
	{
		if ( ! BIT_CHECK( MMCHS_SYSCTL, lines ) )
		{
			return 0;
		}
	}

	return 1;
}

void selectSupportedVoltage( uint32_t voltage )
{
	// NOTE: see OMAP35x.pdf page 3160f and at page 3178

	BIT_CLEAR( MMCHS_CAPA, 0x7000000 );
	BIT_SET( MMCHS_CAPA, voltage );
}

void systemConfig( uint32_t flags )
{
	BIT_CLEAR( MMCHS_SYSCONFIG, MMCHS_SYSCONFIG_AUTOIDLE_BIT |
			MMCHS_SYSCONFIG_SOFTRESET_BIT |
			MMCHS_SYSCONFIG_ENAWAKEUP_BIT |
			MMCHS_SYSCONFIG_SIDLE_BITS |
			MMCHS_SYSCONFIG_CLCKACT_BITS );

	BIT_SET( MMCHS_SYSCONFIG, flags );
}

void setBusWidth( uint32_t busWidth )
{
	// NOTE: MMCHS_CON is configured only here in this function, wont be set during sending a command

	if ( BUS_WIDTH_8BIT == busWidth )
	{
		BIT_SET( MMCHS_CON, MMCHS_CON_DW8_BIT );
	}
	else if ( BUS_WIDTH_4BIT == busWidth )
	{
		BIT_CLEAR( MMCHS_CON, MMCHS_CON_DW8_BIT );
		BIT_SET( MMCHS_HCTL, MMCHS_HCTL_DTW_BIT );
	}
	else if ( BUS_WIDTH_1BIT == busWidth )
	{
		BIT_CLEAR( MMCHS_CON, MMCHS_CON_DW8_BIT );
		BIT_CLEAR( MMCHS_HCTL, MMCHS_HCTL_DTW_BIT );
	}
}

void setBusVoltage( uint32_t voltage )
{
	BIT_CLEAR( MMCHS_HCTL, MMCHS_HCTL_SDVS_33V_BIT );
	BIT_SET( MMCHS_HCTL, voltage );
}

uint32_t setBusPower( bool enable )
{
	if ( enable )
	{
		uint32_t i = 0;

		BIT_SET( MMCHS_HCTL, MMCHS_HCTL_SDBP_BIT );

		for ( i = 0xFF; i > 0; --i )
		{
			if ( BIT_CHECK( MMCHS_HCTL, MMCHS_HCTL_SDBP_BIT ) )
			{
				return 0;
			}
		}

		return 1;
	}
	else
	{
		BIT_CLEAR( MMCHS_HCTL, MMCHS_HCTL_SDBP_BIT );
	}

	return 0;
}

uint32_t setBusFrequency( uint32_t freq_in, uint32_t freq_out, bool bypass )
{
    volatile unsigned int clkd = 0;
	volatile unsigned int regVal = 0;

    /* First enable the internal clocks */
    if ( setInternalClock( TRUE ) )
    {
    	return 1;
    }

    if ( FALSE == bypass )
    {
        /* Calculate and program the divisor */
        clkd = freq_in / freq_out;
        clkd = (clkd < 2) ? 2 : clkd;
        clkd = (clkd > 1023) ? 1023 : clkd;

		/* Do not cross the required freq */
		while((freq_in/clkd) > freq_out)
		{
			if ( 1023 == clkd )
			{
				/* Return we we cannot set the clock freq */
			   return 1;
			}

			clkd++;
		}

		// NOTE: OMAP35x.pdf on page 3179 has a clock-divider of 240 which is the same as here
        regVal = MMCHS_SYSCTL & ~0x0000FFC0u;
        MMCHS_SYSCTL = regVal | ( clkd << 6 );

        /* Wait for the interface clock stabilization */
        if ( awaitInternalClockStable( 0xFF ) )
        {
        	return 1;
        }

        /* Enable clock to the card */
        BIT_SET( MMCHS_SYSCTL, MMCHS_SYSCTL_CEN_BIT );
    }

    return 0;
}

uint32_t setInternalClock( bool enable )
{
	if ( enable )
	{
		// await Internal clock stable
		BIT_SET( MMCHS_SYSCTL, MMCHS_SYSCTL_ICE_BIT );
		if ( awaitInternalClockStable( 0xFF ) )
		{
			return 1;
		}
	}
	else
	{
		BIT_CLEAR( MMCHS_SYSCTL, MMCHS_SYSCTL_ICE_BIT );
	}

	return 0;
}

void sendInitStream( void )
{
	/* Enable the command completion status to be set */
	BIT_SET( MMCHS_IE, MMCHS_IE_CC_BIT );
	 /* Initiate the INIT command */
	BIT_SET( MMCHS_CON, MMCHS_CON_INIT_BIT );
	MMCHS_CMD = 0x0;

	uint64_t waiting = 0;
	// WAIT 1ms to allow card initializing internal state
	//uint64_t sysMillis = getSysMillis();
	//while ( 1 > getSysMillis() - sysMillis ) {}
	while(waiting++ < 100000){}

	// end initialization sequence
	BIT_CLEAR( MMCHS_CON, MMCHS_CON_INIT_BIT );
	// clear all status
	clearAllInterruptsStatus();
}

uint32_t identifyCard( void )
{
	// NOTE: see OMAP35x.pdf page 3164, starterware and sd_spec 2.0

	uint8_t hcsFlag = 0;

	memset( &cardInfo, 0, sizeof( CARD_INFO ) );

	// send GO_IDLE_STATE
	if ( sendCmd0() )
	{
		// an error occured sending CMD0
		return 1;
	}

	// cmd5 is reserved for I/O cards: SDIO. will return 0 if it is SDIO
	if ( 0 == sendCmd5() )
	{
		return 1;
	}

	// send GO_IDLE_STATE again
	if ( sendCmd0() )
	{
		// an error occured sending CMD0
		return 1;
	}

	// send SEND_IF_COND
	if ( 0 == sendCmd8() )
	{
		// NOTE: it is an SD card compliant with standard 2.0 or later

		uint32_t rsp0 = MMCHS_RSP10 & 0x0000FFFF;
		uint32_t supportedVoltage = rsp0 & 0xFFF;
		if ( supportedVoltage != 0x1AA )
		{
			// NOT supported:
			return 1;
		}

		hcsFlag = TRUE;
	}

	while ( 1 )
	{
		// send SD_SEND_OP_COND
		// NOTE: sd_spec 2.0 page 26: While repeating ACMD41, the host shall not issue another command except CMD0. => other sources say repeat CMD55 too
		if ( 0 == sendACmd41( hcsFlag ) )
		{
			// if card is busy, repeat again, otherwise card is identified
			if ( ! isCardBusy() )
			{
				// NOTE: it is a SD card compliant with standard 1.x

				// store ocr in card-info
				cardInfo.ocr = MMCHS_RSP10;
				// check if card is high-capacity or not (HCR-bit in OCR at position 30)
				cardInfo.highCap = ( cardInfo.ocr & ( 1 << 30 ) ) ? 1 : 0;

				goto cardIdentified;
			}
		}
		// no response => its no SD meory card
		else
		{
			break;
		}
	}

	// NOTE: at this point we are a MMC card - we don't support them
	return 1;

cardIdentified:
	// send ALL_SEND_CID
	if ( sendCmd2() )
	{
		return 1;
	}

	// store card-info just returned by CMD2 in RSP10-76
	cardInfo.raw_cid[ 0 ] = MMCHS_RSP10;
	cardInfo.raw_cid[ 1 ] = MMCHS_RSP32;
	cardInfo.raw_cid[ 2 ] = MMCHS_RSP54;
	cardInfo.raw_cid[ 3 ] = MMCHS_RSP76;

	// send SEND_RELATIVE_ADDR to ask card to publish new realtive address
	if ( sendCmd3() )
	{
		return 1;
	}

	// store RCA just returned by CMD3
	cardInfo.rca = ( MMCHS_RSP10 & 0xFFFF0000 ) >> 16;

	// send SEND_CSD: request card-specific data
	if ( sendCmd9() )
	{
		return 1;
	}

	// store card-specific data just returned by CMD9 in RSP10-76
	cardInfo.raw_csd[ 0 ] = MMCHS_RSP10;
	cardInfo.raw_csd[ 1 ] = MMCHS_RSP32;
	cardInfo.raw_csd[ 2 ] = MMCHS_RSP54;
	cardInfo.raw_csd[ 3 ] = MMCHS_RSP76;

	if ( SD_CARD_CSD_VERSION( cardInfo ) )
	{
		cardInfo.tranSpeed = SD_CARD1_TRANSPEED(cardInfo);
		cardInfo.blkLen = 1 << (SD_CARD1_RDBLKLEN(cardInfo));
		cardInfo.size = SD_CARD1_SIZE(cardInfo);
		cardInfo.nBlks = cardInfo.size / cardInfo.blkLen;
	}
	else
	{
		cardInfo.tranSpeed = SD_CARD0_TRANSPEED(cardInfo);
		cardInfo.blkLen = 1 << (SD_CARD0_RDBLKLEN(cardInfo));
		cardInfo.nBlks = SD_CARD0_NUMBLK(cardInfo);
		cardInfo.size = SD_CARD0_SIZE(cardInfo);
	}

	// send SELECT/DESELECT_CARD to select card - now in transfer mode
	if ( sendCmd7() )
	{
		return 1;
	}

	// NOTE: at this point the card is initialized, identified and ready to be used

	// send block length only in case of standard capacity-card, send it ONCE after selection
	if ( ! cardInfo.highCap )
	{
		// send block length
		if ( sendCmd16() )
		{
			// an error occured during sending the command, return immediately
			return 1;
		}
	}

	// request SCR - will be transmitted throug a data-read!
	if ( sendACmd51() )
	{
		return 1;
	}

	uint8_t scrBuffer[ 8 ];
	memset( scrBuffer, 0, 8 );

	// SCR is transmitted in 8 bytes through a data-read
	if ( readTransferBuffer( 8, scrBuffer ) )
	{
		return 1;
	}

	cardInfo.raw_scr[ 0 ] = ( scrBuffer[ 3 ] << 24 ) | ( scrBuffer[ 2 ] << 16 ) | ( scrBuffer[ 1 ] << 8 ) | ( scrBuffer[ 0 ] );
	cardInfo.raw_scr[ 1 ] = ( scrBuffer[ 7 ] << 24 ) | ( scrBuffer[ 6 ] << 16 ) | ( scrBuffer[ 5 ] << 8 ) | ( scrBuffer[ 4 ] );

	cardInfo.sd_ver = SD_CARD_VERSION( cardInfo );
	cardInfo.busWidth = SD_CARD_BUSWIDTH( cardInfo );

	return 0;
}

uint32_t configBusWidth()
{
	uint8_t selectedBusWidth = 0x0;

	if ( cardInfo.busWidth & BUS_WIDTH_4BIT )
	{
		if ( cardInfo.busWidth & BUS_WIDTH_4BIT )
		{
			selectedBusWidth = 0x2;
		}
	}

	if ( sendACmd6( selectedBusWidth ) )
	{
		return 1;
	}

	if ( 0 == selectedBusWidth )
	{
		setBusWidth( BUS_WIDTH_1BIT );
	}
	else
	{
		setBusWidth( BUS_WIDTH_4BIT );
	}

	return 0;
}

uint32_t configTransferSpeed()
{
	uint32_t arg = ( ( SD_SWITCH_MODE & SD_CMD6_GRP1_SEL ) | ( SD_CMD6_GRP1_HS ) );

	if ( sendCmd6( arg ) )
	{
		return 1;
	}

	uint8_t buffer[ 64 ];
	memset( buffer, 0, 64 );

	if ( readTransferBuffer( 64, buffer ) )
	{
		return 1;
	}

	uint32_t speed = cardInfo.tranSpeed;

	if ((buffer[16] & 0xF) == SD_CMD6_GRP1_HS)
	{
		cardInfo.tranSpeed = SD_TRANSPEED_50MBPS;
	}

	if ( speed == SD_TRANSPEED_50MBPS)
	{
		if ( setBusFrequency( SDMMC_CONTROLLER_CLOCK, 50000000, FALSE ) )
		{
			return 1;
		}
	}
	else
	{
		if ( setBusFrequency( SDMMC_CONTROLLER_CLOCK, 25000000, FALSE ) )
		{
			return 1;
		}
	}

	return 0;
}

uint32_t readTransferBuffer( uint32_t nBytes, uint8_t* buffer )
{
	uint32_t status = 0;

	while ( 1 )
	{
		// await buffer-ready interrupt-flag set: will be signaled when MMCi.MMCHS_PSTATE[11] BRE is set.
		// if MMCHS_DATA is accessed before, this will lead to a  bad access (MMCi.MMCHS_STAT[29] BADA)
		if ( isBufferReadReady() )
		{
			uint32_t i = 0;

			for ( i = 0; i < nBytes; i += 4 )
			{
				// NOTE: read 4bytes of data from data-address, will be moved by controller automatically
				volatile uint32_t data = MMCHS_DATA;

				buffer[ i + 0 ] = *( ( uint8_t* ) &data + 0 );
				buffer[ i + 1 ] = *( ( uint8_t* ) &data + 1 );
				buffer[ i + 2 ] = *( ( uint8_t* ) &data + 2 );
				buffer[ i + 3 ] = *( ( uint8_t* ) &data + 3 );
			}

			// clear flag AFTER all data was read because will be asserted after having read the whole buffer
			clearBufferReadReady();
		}

		// NOTE: for dependencies between error flags and transfer complete in MMCHS_STAT see page 3157
		if ( isTransferComplete() )
		{
			clearTransferComplete();

			// NOTE: TC is set upon DEB & DCRC => need to check anyway for those errors if transfer has completed
			if ( BIT_CHECK( MMCHS_STAT, MMCHS_STAT_DEB_BIT ) ||
					BIT_CHECK( MMCHS_STAT, MMCHS_STAT_DCRC_BIT ) )
			{
				clearInterruptBits( MMCHS_STAT_DEB_BIT | MMCHS_STAT_DCRC_BIT );

				// finished with an error
				resetMMCIDataLine( 0xFF );
				status = 1;
			}

			break;
		}
		else
		{
			// NOTE: DTO and TC are mutually exclusive, DCRC & DEB cannot occur with DTO
			if ( isDataTimeout() )
			{
				clearInterruptBits( MMCHS_STAT_DTO_BIT );

				// finished with an error
				resetMMCIDataLine( 0xFF );
				status = 1;
				break;
			}
			else
			{
				// buffer not ready to read / no error occured / transfer not yet completed -> check again
			}
		}
	}

	return status;
}

uint32_t awaitCommandResponse()
{
	// NOTE: for dependencies between error flags and command complete in MMCHS_STAT see page 3157f

	while ( 1 )
	{
		// NOTE: CTO occured, is mutually exclusive with CC and the errors CIE, CEB and CERR cannot occur with CTO.
		if ( isCommandTimeout() )
		{
			// NOTE: CTO can occur at the same time as CCRC: it indicates a command abort due to a contention on CMD line. In this case no CC appears.
			if ( BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CCRC_BIT ) )
			{
				// NOTE: fuck it, don't care for this type of error
			}

			clearInterruptBits( MMCHS_STAT_CTO_BIT | MMCHS_STAT_CCRC_BIT );

			// always need to reset CMD line after error
			resetMMCICmdLine( 0xFF );
			// reset DATA line after error occured during a command which transmits data
			resetMMCIDataLine( 0xFF );
			// return 1 to indicate failure
			return 1;
		}
		// command complete
		else if ( isCommandComplete() )
		{
			clearCommandComplete();

			// NOTE: CC is set upon CIE, CEB, CCRC (if no CTO) and CERR => need to check for those errors anyway although command has completed
			if ( BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CIE_BIT ) ||
					BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CEB_BIT ) ||
					BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CCRC_BIT ) ||
					BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CERR_BIT ) )
			{
				clearInterruptBits( MMCHS_STAT_CIE_BIT | MMCHS_STAT_CEB_BIT | MMCHS_STAT_CCRC_BIT | MMCHS_STAT_CERR_BIT );

				// Command index error / Command end bit error / Command CRC Error /  Card error ?
				// return 1 to indicate failure
				return 1;
			}

			return 0;
		}
		else
		{
			// not yet completed, check again
		}
	}
}

uint32_t awaitInternalClockStable( uint32_t retries )
{
	uint32_t i = 0;

	for ( i = retries; i > 0; --i )
	{
		if ( BIT_CHECK( MMCHS_SYSCTL, MMCHS_SYSCTL_ICS_BIT ) )
		{
			return 0;
		}
	}

	return 1;
}

uint32_t resetMMCIDataLine( uint32_t retries )
{
	uint32_t i = 0;

	BIT_SET( MMCHS_SYSCTL, MMCHS_SYSCTL_SRD_BIT );

	for ( i = retries; i > 0; --i )
	{
		if ( ! BIT_CHECK( MMCHS_SYSCTL, MMCHS_SYSCTL_SRD_BIT ) )
		{
			return 0;
		}
	}

	return 1;
}

uint32_t resetMMCICmdLine( uint32_t retries )
{
	uint32_t i = 0;

	BIT_SET( MMCHS_SYSCTL, MMCHS_SYSCTL_SRC_BIT );

	for ( i = retries; i > 0; --i )
	{
		if ( ! BIT_CHECK( MMCHS_SYSCTL, MMCHS_SYSCTL_SRC_BIT ) )
		{
			return 0;
		}
	}

	return 1;
}

uint32_t isCommandComplete( void )
{
	return BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CC_BIT );
}

uint32_t isCommandTimeout( void )
{
	return BIT_CHECK( MMCHS_STAT, MMCHS_STAT_CTO_BIT );
}

uint32_t isTransferComplete( void )
{
	return BIT_CHECK( MMCHS_STAT, MMCHS_STAT_TC_BIT );
}

uint32_t isDataTimeout( void )
{
	return BIT_CHECK( MMCHS_STAT, MMCHS_STAT_DTO_BIT );
}

uint32_t isBufferReadReady( void )
{
	return BIT_CHECK( MMCHS_STAT, MMCHS_STAT_BRR_BIT );
}

uint32_t awaitDataLineAvailable( uint32_t retries )
{
	uint32_t i = 0;

	for ( i = retries; i > 0; --i )
	{
		// NOTE: if bit 1 (DATI) of MMCHS_PSTATE is 1 data-lines are busy
		if ( ! BIT_CHECK( MMCHS_PSTATE, MMCHS_PSTATE_DATI_BIT ) )
		{
			return 0;
		}
	}

	return 1;
}

void clearAllInterruptsStatus( void )
{
	// clear all interrupts by writing 1 to every bit in STAT-register
	MMCHS_STAT = 0xFFFFFFFF;
}

void clearBufferReadReady( void )
{
	MMCHS_STAT = MMCHS_STAT_BRR_BIT;
}

void clearCommandComplete( void )
{
	MMCHS_STAT = MMCHS_STAT_CC_BIT;
}

void clearTransferComplete( void )
{
	MMCHS_STAT = MMCHS_STAT_TC_BIT;
}

void clearInterruptBits( uint32_t bits )
{
	MMCHS_STAT = bits;
}

// NOTE: timeout must be in range of 13 to 27
void setDataTimeout( uint32_t timeout )
{
	BIT_CLEAR( MMCHS_SYSCTL, MMCHS_SYSCTL_DTO_MASK );
	MMCHS_SYSCTL |= ((((timeout) - 13) & 0xF) << MMCHS_SYSCTL_DTO_BIT_INDEX);
}

uint32_t isCardBusy()
{
	uint32_t busyBit = BIT_CHECK( MMCHS_RSP10, MMCHS_RSP10_BUSY_BIT );
	uint32_t busyFlag = busyBit == 0;

	// NOTE: if bit 31 of RSP10 is 0 then card is busy
	return busyFlag;
}

/* NOTE: GO_IDLE_STATE
 * Resets all cards to idle state
 *
 * type:	bc
 * resp:	-
 */
uint32_t sendCmd0( void )
{
	// NOTE: this command resets the MMC card (see OMAP35x.pdf page 3180)

	MMCHS_IE = MMCHS_IE_CC_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 0 );

	return awaitCommandResponse();
}

/* NOTE: ALL_SEND_CID
 * Asks any card to send the CID num-bers on the CMD line (any card that is connected to the host will respond)
 *
 * type:	bcr
 * resp:	R2
 */
uint32_t sendCmd2( void )
{
	// NOTE: This command asks the MMC card to send its CID register's content (see OMAP35x.pdf page 3182)

	MMCHS_IE =  MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 2 ) | MMCHS_CMD_RSP_TYPE_136_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: SEND_RELATIVE_ADDR
 * Ask the card to publish a new relative address (RCA)
 *
 * type:	bcr
 * resp:	R6
 */
uint32_t sendCmd3( void )
{
	// NOTE: This command sets MMC card address (see Table 22-19). Useful when MMCHS controller switches to addressed mode (see OMAP35x.pdf page 3182)

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = 0x0;
	MMCHS_CMD =	MMCHS_CMD_INDX_BITS( 3 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

// NOTE: reserved for I/O cards (refer to the "SDIO Card Specification")
uint32_t sendCmd5( void )
{
	// NOTE: This command asks a SDIO card to send its operating conditions. This command will fail if there is no SDIO card (see OMAP35x.pdf page 3180)

	MMCHS_IE = MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 5 ) | MMCHS_CMD_RSP_TYPE_48_BIT;

	return awaitCommandResponse();

	// NOTE: In case of success the response will be in MMCHS1.MMCHS_RSP10 register
}

/* NOTE: SWITCH_FUNC
 * Checks switchable function (mode 0) and switch card function (mode 1). See Chapter 4.3.10.
 *
 * type:	adtc
 * resp:	R1
 * arg:		[31] Mode 0:Check function 1:Switch function
 * 			[30:24] reserved (All 0)
 * 			[23:20] reserved for function group 6 (0h or Fh)
 * 			[19:16] reserved for function group 5 (0h or Fh)
 * 			[15:12] reserved for function group 4 (0h or Fh)
 * 			[11:8] reserved for function group 3 (0h or Fh)
 * 			[7:4] function group 2 for command system
 * 			[3:0] function group 1 for access mode
 */
uint32_t sendCmd6( uint32_t arg )
{
	// NOTE: Setting Data Bus Width to arg
	// NOTE: see sd_spec 2.0 page 42: 4.3.10 Switch Function Command

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT | MMCHS_IE_TC_BIT | MMCHS_IE_BRR_BIT | MMCHS_IE_DTO_BIT | MMCHS_IE_DCRC_BIT | MMCHS_IE_DEB_BIT;
	MMCHS_ARG = arg;
	MMCHS_BLK = ( 1 << 16 ) | ( 64 ); // read one block. with length of 64 bytes, will contain
	setDataTimeout( 27 );
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 6 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_DP_BIT | MMCHS_CMD_DDIR_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: SELECT/DESELECT_CARD
 * command toggles a card between the stand-by and transfer states or between the programming and disconnect states.
 * In both cases, the card is selected by its own relative address and gets deselected by any other address; address 0 deselects all.
 * In the case that the RCA equals 0, then the host may do one of the following:
 * 		- Use other RCA number to perform card de-selection.
 * 		- Re-send CMD3 to change its RCA number to other than 0 and then use CMD7 with RCA=0 for card de- selection.
 *
 * type:	ac
 * resp:	R1b
 * arg:		[31:16] RCA
 */
uint32_t sendCmd7( void )
{
	// NOTE: see OMAP35x.pdf page 3184

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = cardInfo.rca << 16;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 7 ) | MMCHS_CMD_RSP_TYPE_48BUSY_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: SEND_IF_COND
 * Sends SD Memory Card interface condition, which includes host supply voltage information and asks the card whether card supports voltage.
 * Reserved bits shall be set to '0'.
 *
 * type:	bcr
 * resp:	R7
 * arg:		[31:12]reserved bits, [11:8]supply voltage(VHS) [7:0]check pattern
 */
uint32_t sendCmd8( void )
{
	// NOTE: This command asks a SD card version 2.X to send its operating conditions (see OMAP35x.pdf page 3180f)
	// NOTE: see sd_spec 2.0 page 51 4.3.13 Send Interface Condition Command (CMD8)

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = CMD8_CHECK_PATTERN | CMD8_VOLT_2P7_3P6;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 8 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: SEND_CSD
 * Addressed card sends its card-specific data (CSD) on the CMD line.
 *
 * type:	ac
 * resp:	R2
 * arg:		[31:16] RCA
 */
uint32_t sendCmd9( void )
{
	// NOTE: This command asks the card to send its csd register's content

	MMCHS_IE = MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = cardInfo.rca << 16;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 9 ) | MMCHS_CMD_RSP_TYPE_136_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/** NOTE: STOP_TRANSMISSION
 * Forces the card to stop transmission.
 *
 * type:	ac
 * resp:	R1b (busy!)
 * arg:		[31:0] stuff bits
 */
uint32_t sendCmd12( void )
{
	// NOTE: seems to work without it for now

	MMCHS_IE = MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = 0x0;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 12 ) | MMCHS_CMD_RSP_TYPE_48BUSY_BIT | MMCHS_CMD_CCCE_BIT | 0x3 << MMCHS_CMD_TYPE_BIT_START;

	return awaitCommandResponse();
}

/* NOTE: SET_BLOCKLEN
 * In the case of a Standard Capacity SD Memory Card, this command sets the block length (in bytes) for all following
 * block commands (read, write, lock). Default block length is fixed to 512 Bytes. Set length is valid for memory access
 * commands only if partial block read operation are allowed in CSD. In the case of a High Capacity SD  Memory Card, block length set by CMD16
 * command does not affect the memory read and write commands. Always 512 Bytes fixed block length is used. This command is effective for LOCK_UNLOCK
 * command. In both cases, if block length is set larger than 512Bytes, the card sets the BLOCK_LEN_ERROR bit.
 *
 * type:	ac
 * resp:	R1
 * arg:		[31:0] block length
 */
uint32_t sendCmd16()
{
	// NOTE: Issuing CMD16 allows to set the block length (see OMAP35x.pdf page 3186)

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = BLOCK_LEN;	// Block length is 512 = 0x200
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 16 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: READ_SINGLE_BLOCK
 * In the case of a Standard Capacity SD Memory Card, this command, this command reads a block of the size selected by the SET_BLOCKLEN command.
 * 1 In the case of a High Capacity Card, block length is fixed 512 Bytes regardless of the SET_BLOCKLEN command
 *
 * type:	adtc
 * resp:	R1
 * arg:		[31:0] data address: Data address is in byte units in a Standard Capacity SD Memory Card and in block (512 Byte) units in a High Capacity SD Memory Card.
 */
uint32_t sendCmd17( uint32_t addr )
{
	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT | MMCHS_IE_TC_BIT | MMCHS_IE_BRR_BIT | MMCHS_IE_DTO_BIT | MMCHS_IE_DCRC_BIT | MMCHS_IE_DEB_BIT;
	MMCHS_ARG = addr;	// address
	MMCHS_BLK = ( 1 << 16 ) | ( BLOCK_LEN ); // read one block. starts at bit 16, bit 0-15 contains block-size and is ALWAYS set to 512
	setDataTimeout( 27 );
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 17 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_DP_BIT | MMCHS_CMD_DDIR_BIT | MMCHS_CMD_MSBS_BIT | MMCHS_CMD_BCE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: READ_MULTIPLE_BLOCK
 * Continuously transfers data blocks from card to host until interrupted by a STOP_TRANSMISSION command.
 * Block length is specified the same as READ_SINGLE_BLOCK command.
 *
 * type:	adtc
 * resp:	R1
 * arg:		[31:0] data address: Data address is in byte units in a Standard Capacity SD Memory Card and in block (512 Byte) units in a High Capacity SD Memory Card.
 */
uint32_t sendCmd18( uint32_t addr, uint32_t nblk )
{
	// NOTE: Issuing CMD18 starts the finite, multiple block read transfer. (see OMAP35x.pdf page 3187)

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT | MMCHS_IE_TC_BIT | MMCHS_IE_BRR_BIT | MMCHS_IE_DTO_BIT | MMCHS_IE_DCRC_BIT | MMCHS_IE_DEB_BIT;
	MMCHS_ARG = addr;	// address
	MMCHS_BLK = ( nblk << 16 ) | ( BLOCK_LEN ); // write number of blocks/bytes to read. starts at bit 16, bit 0-15 contains block-size and is ALWAYS set to 512
	setDataTimeout( 27 );
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 18 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_DP_BIT |
			MMCHS_CMD_DDIR_BIT | MMCHS_CMD_MSBS_BIT | MMCHS_CMD_BCE_BIT | MMCHS_CMD_CCCE_BIT |
			/*MMCHS_CMD_ACEN_BIT */ 0x3 << MMCHS_CMD_TYPE_BIT_START;

	return awaitCommandResponse();
}

/* NOTE: SET_BUS_WIDTH
 * Defines the data bus width (00=1bit or 10=4 bits bus) to be used for data transfer. The allowed data bus widths are given in SCR register.
 *
 * type:	ac
 * resp:	R1
 * arg:		[31:2] stuff bits, [1:0] bus width
 */
uint32_t sendACmd6( uint8_t busWidth )
{
	// send APP_CMD to notify card that next command will be application specific
	if ( sendCmd55() )
	{
		// an error occured sending CMD55
		return 1;
	}

	MMCHS_IE = MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = busWidth;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 6 ) | MMCHS_CMD_RSP_TYPE_48_BIT;

	return awaitCommandResponse();
}

/* NOTE: SD_SEND_OP_COND
 * Sends host capacity support information (HCS) and asks the accessed card to send its operating condition register (OCR) content in the response on the CMD line.
 * HCS is effective when card receives SEND_IF_COND command.
 * Reserved bit shall be set to '0'. CCS bit is assigned to OCR[30].
 *
 * type:	bcr
 * resp:	R3
 * arg:		[31]reserved bit [30]HCS(OCR[30]) [29:24]reserved bits [23:0] VDD Voltage Window(OCR[23:0])
 */
uint32_t sendACmd41( uint8_t hcsFlag )
{
	// send APP_CMD to notify card that next command will be application specific
	if ( sendCmd55() )
	{
		// an error occured sending CMD55
		return 1;
	}

	MMCHS_IE = MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = hcsFlag << 30 | SD_OCR_VDD_WILDCARD; // NOTE: if sdcard 2.0 or later: If host supports high capacity, HCS is set to 1
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 41 ) | MMCHS_CMD_RSP_TYPE_48_BIT;

	return awaitCommandResponse();
}

/* NOTE: SEND_SCR
 * Reads the SD Configuration Register (SCR).
 *
 * type:	adtc
 * resp:	R1
 * arg:		[31:0] stuff bits
 */
uint32_t sendACmd51()
{
	// send APP_CMD to notify card that next command will be application specific
	if ( sendCmd55() )
	{
		// an error occured sending CMD55
		return 1;
	}

	MMCHS_IE = MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CEB_BIT | MMCHS_IE_TC_BIT | MMCHS_IE_BRR_BIT | MMCHS_IE_DTO_BIT | MMCHS_IE_DCRC_BIT | MMCHS_IE_DEB_BIT;
	MMCHS_ARG = cardInfo.rca << 16;
	MMCHS_BLK = ( 1 << 16 ) | ( 8 ); // read one block, but only 8 bytes - response will contain SCR which is 8 bytes wide
	setDataTimeout( 27 );
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 51 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_DP_BIT | MMCHS_CMD_DDIR_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}

/* NOTE: APP_CMD
 * Indicates to the card that the next command is an application specific command rather than a standard command
 *
 * type:	ac
 * resp:	R1
 * arg:		[31:16] RCA
 */
uint32_t sendCmd55( void )
{
	// This is a special command used to prevent the card that the following command is going to be an application one (see OMAP35x.pdf page 3181)

	// NOTE: see sd_card spec 2.0 page 40f: Application-Specific Command APP_CMD (CMD55)

	MMCHS_IE = MMCHS_IE_CC_BIT | MMCHS_IE_CTO_BIT | MMCHS_IE_CERR_BIT | MMCHS_IE_CIE_BIT | MMCHS_IE_CCRC_BIT | MMCHS_IE_CEB_BIT;
	MMCHS_ARG = cardInfo.rca << 16;
	MMCHS_CMD = MMCHS_CMD_INDX_BITS( 55 ) | MMCHS_CMD_RSP_TYPE_48_BIT | MMCHS_CMD_CICE_BIT | MMCHS_CMD_CCCE_BIT;

	return awaitCommandResponse();
}
