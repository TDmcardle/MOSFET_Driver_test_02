//
//  IOPorts.c
//  MOSFET_Driver_test_02
//
//  Created by Tommy McArdle on 11/26/13.
//
//

#include "main.h"


void initPorts(void){
	DDRA = 0xFF;	// All outputs
	DDRB = 0xFF;	// All outputs	(PINB4 = OC2A)
	DDRC = 0xFF;	// All outputs
	
	PORTA = 0xA5;	// All ports off (output to LED's so LED's off)
	PORTB = 0xFF;
	PORTC = 0x0F;
}