#include "msp430g2553.h"
#include "tests.h"
#include "main.h"

// test individual segments of display to make sure they are connected to correct pins
void test1(){
	volatile unsigned int i;
	volatile unsigned int j;
	volatile unsigned int k;
	for (i=0;i<8;i++){
		P2OUT=~(1<<i);
		delay_ms(500);
	}
}

// test digits of display to make sure they are cnnected to correct pins
void test2(){
	volatile unsigned int i;
	volatile unsigned int j;
	volatile unsigned int k;
	P2OUT=~1;
	for (i=0;i<4;i++){
		P1OUT=(P1OUT & 0xf0) | (1<<i);
		delay_ms(500);
	}
}

void test3(){
	volatile unsigned int i;
	volatile unsigned int j;
	volatile unsigned int k;
	for (i=0;i<4;i++){
		P1OUT=(P1OUT & 0xf0) | (0<<i);	// turn off display
		P2OUT=~(bcd27seg(i));						// set digit to display
		P1OUT=(P1OUT & 0xf0) | (1<<i);	// turn on digit
		delay_ms(500);
	}
}

void test4(unsigned int d){
	volatile unsigned int i;
	volatile unsigned int j;
	volatile unsigned int k;
	for (i=0;i<4;i++){
		P1OUT=(P1OUT & 0xf0) | (0<<i);	// turn off display
		P2OUT=~(bcd27seg(d>>(4*i)&0x0f));						// set digit to display
		P1OUT=(P1OUT & 0xf0) | (1<<i);	// turn on digit
		delay_ms(5);
	}
}
