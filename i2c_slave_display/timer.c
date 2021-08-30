// slave with display

#include "msp430g2553.h"
#include "timer.h"
#include "main.h"

void timer_init(void){
	CCTL0 = CCIE;	// TACCTLx, Capture/Compare Control Reg = interrupt enable
//	CCR0 = 0x9ac0;	// = 100.0Hz TACCRx, Timer_A Capture/Compare period Register x
	CCR0 = 0x0f76;	// = 1.000KHz TACCRx, Timer_A Capture/Compare period Register x
//	CCR1 = 0x0001;
	TACTL = TASSEL_2+ID_0+MC_1;	// Enable timer A
//	IE1 &= ~(WDTIE+OFIE);	// Disable WDT interrupt
}
