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


int mosfets = 0;
int ADC_phase = 0;

void changeState(int state){
	
	switch(state){
		case 0:
			mosfets = (A_HIGH | B_LOW);
			ADC_phase = phase_C;
			break;
		case 1:
			mosfets = (A_HIGH | C_LOW);
			ADC_phase = phase_B;
			break;
		case 2:
			mosfets = (B_HIGH | C_LOW);
			ADC_phase = phase_A;
			break;
		case 3:
			mosfets = (A_LOW | B_HIGH);
			ADC_phase = phase_C;
			break;
		case 4:
			mosfets = (A_LOW | C_HIGH);
			ADC_phase = phase_B;
			break;
		case 5:
			mosfets = (B_LOW | C_HIGH);
			ADC_phase = phase_A;
			break;
		default:
			mosfets = (A_HIGH | A_LOW | B_LOW | C_LOW);
			ADC_phase = phase_C;
			break;
			
	}
	
	
}





ISR (TIMER2_COMPA_vect){
	/* Static variables only initialize the value at functions first call */
	static int state = 0;
	
	/* Transition States */
	if (state == 5)
		state = 0;
	else
		state++;
	
	changeState(state);
	
	/* Set the phase the ADC will be detecting a zero-crossing on (ADC_phase set in main) */
	ADMUX |= (ADC_phase << MUX0);
	
	/* Set the start counter to 0 */
	TCNT2 = 0;
}



ISR (TIMER2_COMPB_vect){
	OCR2A = 2 * TCNT2;
}





int main(void)
{

	initPorts();
	initADC();
	initTimer_CTC();
	
	sei();
	
	mosfets = (A_HIGH | A_LOW | B_LOW | C_LOW);
	ADC_phase = phase_C;
	
	
	for(;;){
		PORTA = mosfets;
	}
	
    return 0;   /* never reached */
}
























// ^--Space used to keep code of bottom of screen. It bugs me.
