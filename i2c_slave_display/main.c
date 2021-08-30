// slave with display
// P1.0-3 common anode
// P2 LED segments
// send two bytes over I2C, MSB first

#include "msp430g2553.h"
#include "main.h"
#include "timer.h"
#include "i2c.h"

// global vars
unsigned int I2C_DATA;			// received 16 bit word
unsigned int I2C_DATA_TMP;		// temporary storage of first byte
unsigned char DISPLAY_DIGIT;	// which digit are we displaying now?
unsigned char RCV_BYTE;			// which byte of a 16 bit word is going to be received next

// define this for common anode displays, otherwise comment out
//#define CA

int main(void){
//	unsigned int i;
//	unsigned int j;
//	unsigned int k;
//	unsigned int LastVal;

	WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
	BCSCTL1 = CALBC1_16MHZ;		// Set range
	DCOCTL = CALDCO_16MHZ;		// Set DCO step and modulation
	P2SEL &= ~(BIT6+BIT7);		// set these as outputs, not clk
	P1OUT = 0xf0;
	P1DIR = 0x0f;				// Set lower P1 to output direction
	P2OUT = 0xff;
	P2DIR = 0xff;				// Set all P2 to output direction

	i2c_init();
	timer_init();

	__bis_SR_register(GIE);		// Enable interrupts

	DISPLAY_DIGIT = 0;			// init display
	I2C_DATA = 0;				// clear I2C data buffer
	RCV_BYTE = 0;				// received byte counter
	
//	I2C_DATA=1234;	// testing
	while (1){
		//__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
		__bis_SR_register(LPM1_bits);			// LPM1 with interrupts enabled
//		__no_operation();                       // SET BREAKPOINT HERE
		//delay_ms(500);
	}
}

#define B(x) S_to_binary_(#x)

static inline char S_to_binary_(const char *s)
{
    char i = 0;
    while (*s) {
        i <<= 1;
        i += *s++ - '0';
    }
#ifdef CA
    return ~i;
#else
    return i;
#endif
}

char bcd27seg(char bcd){
	switch (bcd) {
		case 0: return B(1111110);       // 0
		case 1: return B(0110000);       // 1
		case 2: return B(1101101);       // 2
		case 3: return B(1111001);       // 3
		case 4: return B(0110011);       // 4
		case 5: return B(1011011);       // 5
		case 6: return B(1011111);       // 6
		case 7: return B(1110000);       // 7
		case 8: return B(1111111);       // 8
		case 9: return B(1111011);       // 9
		case 10: return B(1110111);       // A
		case 11: return B(0011111);       // b
		case 12: return B(0001101);       // c
		case 13: return B(0111101);       // d
		case 14: return B(1001111);       // E
		case 15: return B(1000111);       // F
	}
	return 0;
}
		
// about 10us delay here
void delay(volatile unsigned int i){
  volatile unsigned int j;
	do {
		for (j=14; j>0; j--);
		i--;
	} while (i != 0);
} // delay

// about 1ms delay here
void delay_ms(volatile unsigned int i){
	volatile unsigned int j;
	do {
		for (j=1600; j>0; j--);
		i--;
	} while (i != 0);
} // delay

//------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C master
// to the MSP430 memory.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
//  *PRxData++ = UCB0RXBUF;                   // Move RX data to address PRxData
//  RXByteCtr++;                              // Increment RX byte count
	if (!RCV_BYTE){
		I2C_DATA_TMP=UCB0RXBUF<<8;
		RCV_BYTE=1;
	} else {
		I2C_DATA=I2C_DATA_TMP+UCB0RXBUF;
		RCV_BYTE=0;
	}
}

//------------------------------------------------------------------------------
// The USCI_B0 state ISR is used to wake up the CPU from LPM0 in order to
// process the received data in the main program. LPM0 is only exit in case
// of a (re-)start or stop condition when actual data was received.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);       // Clear interrupt flags
//  if (RXByteCtr)                            // Check RX byte counter
//    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0 if data was
}                                           // received

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timera0_timer(void){

	P1OUT=(P1OUT & 0xf0);	// turn off display
	switch (DISPLAY_DIGIT) {
		case 0: P2OUT=(bcd27seg(I2C_DATA%10)); break;
		case 1: P2OUT=(bcd27seg((I2C_DATA / 10)%10)); break;
		case 2: P2OUT=(bcd27seg((I2C_DATA / 100)%10)); break;
		case 3: P2OUT=(bcd27seg((I2C_DATA / 1000)%10)); break;
	}
	P1OUT=(P1OUT & 0xf0) | (1<<DISPLAY_DIGIT);	// turn on digit
	DISPLAY_DIGIT++;
	if (DISPLAY_DIGIT > 3) DISPLAY_DIGIT=0;

// we never go to sleep. this was in sample code
//  __bic_SR_register_on_exit(LPM3_bits);     // Exit LPM3 on reti
}

/*
#pragma vector=NMI_VECTOR
__interrupt void nmi(void)
{
	IFG1 &= ~(OFIFG);
}
*/
