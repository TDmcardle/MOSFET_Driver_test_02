/* Name: main.c
 * Author: Tom McArdle
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

/**
 * OC0 PG5
 * OC1B	PB6
 * OC2B PH5
 * OC3B	PE4
 * OC4B	PH4
 * OC5B PL4
 */



#include "main.h"


enum portPins{
	p0 = 0xFE,
	p1 = 0xFD,
	p2 = 0xFB,
	p3 = 0xF7,
	p4 = 0xEF,
	p5 = 0xDF,
	p6 = 0xBF,
	p7 = 0x7F
};



ISR (TIMER2_COMPA_vect){
//	OCR2A = 255;
	PORTA ^= 0x80;
}






int main(void)
{

	initPorts();
//	initADC();
	initTimer_CTC();
	
	sei();
	
	
	
	for(;;){



		
		
	}
    return 0;   /* never reached */
}
























// ^--Space used to keep code of bottom of screen. It bugs me.
