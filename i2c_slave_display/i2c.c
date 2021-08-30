// slave with display

#include "msp430g2553.h"
#include "i2c.h"
#include "main.h"

#define CLK_PLS 10
//#define I2C_SPEED 0xc0
#define I2C_SPEED 60	// Hz

// functions for hardware I2C
void i2c_init(void){
	UCB0CTL1 |= UCSWRST;			// make sure I2C is stopped
	
	P1SEL |= BIT6 + BIT7;			// switch P1.6 P1.7 to I2C function
	P1SEL2 |= BIT6 + BIT7;
	
	UCB0CTL0 = UCMODE_3 + UCSYNC;	// slave mode, I2C, synchronous
	UCB0CTL1 = UCSSEL_2 + UCSWRST;	// clock from SMCLK, stay reset
// slave gets clock from master
//	UCB0BR0 = I2C_SPEED;			// 12 would be fSCL = SMCLK/12 = ~100kHz
//	UCB0BR1 = 0;					//
	UCB0I2COA = I2C_ADDR;			// Slave Address is 027h
	UCB0CTL1 &= ~UCSWRST;    		// **Initialize USCI state machine**
//	IE2 |= UCB0RXIE;				// enable i2c receive interrupt
	UCB0I2CIE |= UCSTPIE + UCSTTIE;           // Enable STT and STP interrupt
	IE2 |= UCB0RXIE;                          // Enable RX interrupt

//  UCB0BR0 = 0x40;                          // /64 = 250KHz
//  UCB0BR0 = 0x80;                          // /128 = 125KHz
}
