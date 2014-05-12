

typedef volatile unsigned int* address;

/*GPIO*/
#define GPIO01 0x48310000
#define GPIO02 0x49050000
#define GPIO03 0x49052000
#define GPIO04 0x49054000
#define GPIO05 0x49056000
#define GPIO06 0x49058000
/*GPIO END*/

/*REGISTER*/
#define REVISION 0x000 //Contains IP revision code p.3402
#define SYS_CONFIG 0x010 //Controls various parameters of L4 interconnect p.3402
#define SYS_STATUS 0x014 //Provides status information about the module, excluding interrupt status p.3403
#define IRQ_STATUS_1 0x018 //Provides IRQ 1 status information p.3404
#define IRQ_ENABLE_1 0x01C //Enable/disable IRQ generation p.3405
#define WAKE_UP_ENABLE 0x020 //Start/stop wake-up generation p.3406
#define IRQ_STATUS_2 0x028 //Provides IRQ2 status information p.3406
#define IRQ_ENABLE_2 0x02C //Enable/disable IRQ generation p.3407
#define CTRL 0x030 //Controls the clock gating functionality p.3408
#define OE 0x034 //Used to enable the pins output capabilities. It's only function is to carry the pads configuration. p.3409
#define DATA_IN 0x038 //Register data that is read from the pin p.3409
#define DATA_OUT 0x03C //Used to set output value of the pin p.3410
#define LEVEL_DETECT_0 0x040 //Enable/disable low-level detection for interrupt request generation p.3411
#define LEVEL_DETECT_1 0x044 //Enable/disable high-level detection for interrupt request generation p.3411
#define RISING_DETECT 0x048 //Enable/disable rising-edge (0=>1 transition) detection for interrupt request and wake-up generation p.3412
#define FALLING_DETECT 0x04C //Enable/disable falling-edge (1=>0 transition) detection for interrupt request and wake-up generation p.3413
#define DEBOUNCE_ENABLE 0x050 //Enable/disable thedebouncing feature for each input line p.3413
#define DEBOUNCING_TIME 0x054 //Controls debouncint time (value is global for all ports!) p.3414
#define CLEAR_IRQ_ENABLE_1 0x060 //Set the corresponding GPIO_IRQENABLE1 register to 0 p.3415
#define SET_IRQ_ENABLE_1 0x064 //Set the corresponding GPIO_IRQENABLE1 register to 0 p.3415
#define CLEAR_IRQ_ENABLE_2 0x070 //Set the corresponding GPIO_IRQENABLE2 register to 0 p.3416
#define SET_IRQ_ENABLE_2 0x074 //Set the corresponding GPIO_IRQENABLE2 register to 0 p.3416
#define CLEAR_WKU_ENA 0x080 //Set the corresponding GPIO_CLEARWKUENABLE register to 0 p.3417
#define SET_WKU_ENA 0x084 //Set the corresponding GPIO_CLEARWKUENABLE register to 0 p.3417
#define CLEAR_DATA_OUT 0x090 //Set the corresponding GPIO_CLEARDATAOUT register to 0 p.3418
#define SET_DATA_OUT 0x094 //Set the corresponding GPIO_SETDATAOUT register to 0 p.3419
/*REGISTER END*/


/*OPERATIONS*/
#define ENABLE 0x01;
#define DISABLE 0x00;
#define SET 0x01;
#define CLEAR 0x01;
/*OPERATIONS END*/


#define GPIO05_DIR (address) 0x49056034
#define USR0 (1<<21)
#define USR1 (1<<22)
#define GPIO05_OUT (address) 0x4905603C

int ledTest(void) {
	*(GPIO05_DIR) &= ~(USR0 | USR1);
	*(GPIO05_OUT) &= ~USR1;

	volatile int i;
	volatile int y = 300000;
	while(1){
		for (i = 0; i < y; i++) {

		}
		*(GPIO05_OUT) ^= (USR0 | USR1);

	}
}
