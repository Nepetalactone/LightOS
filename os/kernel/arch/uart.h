#ifndef UART_H
#define UART_H

#include "address.h"

/*
OMAP35x technical reference manual pdf
Overview p.2740
*/

/*
General Information

UART1-3 size = 1k bytes
2 bytes per register

*/


/* UART */
#define UART01 (base_address) 0x4806A000
#define UART02 (base_address) 0x4806C000
#define UART03 (base_address) 0x49020000
/* UART END */

/* REGISTER */
#define DLL (offset) 0x000 // p.2742
#define RHR (offset) 0x000 // p.2742
#define THR (offset) 0x000 // p.2743
#define DLH (offset) 0x004 // p.2746
#define IER (offset) 0x004 // p.2744
#define IIR (offset) 0x008 // p.2748
#define FCR (offset) 0x008 // p.2747
#define EFR (offset) 0x008 // p.2750
#define LCR (offset) 0x00C // p.2752
#define MCR (offset) 0x010 // p.2753
#define XON1_ADDR1 (offset) 0x010 // p.2754
#define LSR (offset) 0x014 // p.2756
#define XON2_ADDR2 (offset) 0x014 // p.2757
#define MSR (offset) 0x018 // p.2759
#define TCR (offset) 0x018 // p.2758
#define XOFF1 (offset) 0x018 // p.2758
#define SPR (offset) 0x01C // p.2760
#define TLR (offset) 0x01C // p.2761
#define XOFF2 (offset) 0x01C // p.2760
#define MDR1 (offset) 0x020 // p.2762
#define MDR2 (offset) 0x024 // p.2763
#define SFLSR (offset) 0x028 // p.2765
#define TXFLL (offset) 0x028 // p.2764
#define RESUME (offset) 0x02C // p.2766
#define TXFLH (offset) 0x02C // p.2766
#define SFREGL (offset) 0x030 // p.2768
#define RXFLL (offset) 0x030 // p.2767
#define SFREGH (offset) 0x034 // p.2768
#define RXFLH (offset) 0x034 // p.2769
#define UASR (offset) 0x038 // p.2771
#define BLR (offset) 0x038 // p.2770
#define ACREG (offset) 0x03C // p.2772
#define SCR (offset) 0x040 // p.2773
#define SSR (offset) 0x044 // p.2774
#define EBLR (offset) 0x048 // p.2775
#define MVR (offset) 0x050 // p.2776
#define SYSC (offset) 0x054 // p.2776
#define SYSS (offset) 0x058 // p.2777
#define WER (offset) 0x05C // p.2778
#define CFPS (offset) 0x060 // p.2779
/* REGISTER  END*/


#endif
