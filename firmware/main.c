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



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <unistd.h>

#include <avr/eeprom.h>


#define TOP		255
#define BOTTOM	0
#define MAX		250
#define MIN		125
#define RANGE	(MAX-MIN)


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


void initTimer_CTC(void)
{
	/**
	 * TCCR2A (pg 184 in ATmega2560 datasheet)
	 *		Output: OC2A = PB4
	 *
	 * Bits 7:6 – COM2A1:0: Compare Match Output A Mode
	 *	Compare Output Mode, non-PWM Mode
	 **		00	 Normal port operation, OC0A disconnected.
	 *		01	 Toggle OC2A on Compare Match
	 *		10	 Clear OC2A on Compare Match (sets TCNT2 = 0 and then counts up)
	 *		11	 Set OC2A on Compare Match
	 *
	 *	Compare Output Mode, Fast PWM Mode
	 *		00 	 Normal port operation, OC2A disconnected.
	 *		01 	 WGM22 = 0: Normal Port Operation, OC0A Disconnected. WGM22 = 1: Toggle OC2A on Compare Match.
	 *		10 	 Clear OC2A on Compare Match, set OC2A at TOP
	 *		11 	 Set OC2A on Compare Match, clear OC2A at TOP
	 *
	 *	Compare Output Mode, Phase Correct PWM Mode
	 *		00 	 Normal port operation, OC2A disconnected.
	 *		01 	 WGM22 = 0: Normal Port Operation, OC2A Disconnected. WGM22 = 1: Toggle OC2A on Compare Match.
	 *		10 	 Clear OC2A on Compare Match when up-counting. Set OC2A on Compare Match when down-counting.	example: "|_|"""
	 *		11 	 Set OC2A on Compare Match when up-counting. Clear OC2A on Compare Match when down-counting.	example: _____|"|__
	 *
	 * Bits 5:4 – COM2B1:0: Compare Match Output B Mode
	 *
	 * Bits 3, 2 – Res: Reserved Bits
	 *
	 * Bits 1:0 – WGM21:0: Waveform Generation Mode
	 *		Mode   	WGM2:0	Mode of Operation		TOP   	Update of OCRx at		TOV Flag Set on(1)(2)
	 *		 0   	 000   	 Normal					 0xFF    Immediate				 MAX
	 *		 1   	 001   	 PWM, Phase Correct   	 0xFF    TOP					 BOTTOM
	 **		 2   	 010   	 CTC					 OCRA    Immediate				 MAX
	 *		 3   	 011   	 Fast PWM				 0xFF    TOP					 MAX
	 *		 4   	 100	 Reserved				 –   	 –						 –
	 *		 5   	 101	 PWM, Phase Correct   	 OCRA    TOP					 BOTTOM
	 *		 6   	 110   	 Reserved				 –   	 –						 –
	 *		 7   	 111   	 Fast PWM				 OCRA    TOP					 TOP
	 *
	 * Clears OC2A on compare match and set OC2A at TOP
	 * Sets Fast PWM and TOP = 0xFF
	 */
	TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (0 << WGM20);
 	
	/**
	 * These outputs were based off of Timer/Counter 2, but the input pins are located on different
	 * registers
	 */
//	TCCR1A = (0 << COM1A1) | (1 << COM1A0) | (0 << WGM11) | (1 << WGM10);
//	TCCR3A = (0 << COM3A1) | (1 << COM3A0) | (0 << WGM31) | (1 << WGM30);
//	TCCR4A = (0 << COM4A1) | (1 << COM4A0) | (0 << WGM41) | (1 << WGM40);
//	TCCR5A = (0 << COM5A1) | (1 << COM5A0) | (0 << WGM51) | (1 << WGM50);
	
	/**
	 * TCCR2B (pg 187 in ATmega2560 datasheet)
	 *	Bit 7 – FOC2A: Force Output Compare A
	 *	Bit 6 – FOC2B: Force Output Compare B
	 *	Bits 5:4 – Res: Reserved Bits
	 *	Bit 3 – WGM22: Waveform Generation Mode
	 *	Bit 2:0 – CS22:0: Clock Select
	 *		000		No clock source (Timer/Counter stopped).
	 *		001		clkT2S/(No prescaling)			15,686.275Hz
	 *		010		clkT2S/8 (From prescaler)		 1,960.784Hz
	 *		011		clkT2S/32 (From prescaler)		   490.196Hz
	 *		100		clkT2S/64 (From prescaler)		   245.098Hz
	 *		101		clkT2S/128 (From prescaler)		   122.549Hz
	 *		110		clkT2S/256 (From prescaler)		    61.275Hz
	 **		111		clkT2S/1024 (From prescaler)	    15.319Hz
	 *
	 * Phase Correct PWM Mode: f_OCnx = f_clk / (N * 510)	where N is the prescaler
	 *
	 * Since the frequency of the motor will be 616.667Hz, the prescaler will be:
	 *		N = f_clk / (f_OCnx * 510) = 8,000,000Hz / (616.667Hz * 510) = 25.437188158 <= 32
	 * The number is rounded up to 32, because now the OC2A can be used to set the appropriate frequency.
	 */
	TCCR2B = (1 << CS22) |(1 << CS21) | (1 << CS20);	
	
//	/**
//	 * These outputs were based off of Timer/Counter 2, but the input pins are located on different
//	 * registers
//	 */
//	//	TCCR0B = (1 << WGM03) | (1 << CS02) |(0 << CS01) | (1 << CS00);
//	TCCR1B = (1 << WGM13) | (1 << CS12) |(0 << CS11) | (1 << CS10);
//	TCCR3B = (1 << WGM33) | (1 << CS32) |(0 << CS31) | (1 << CS30);
//	TCCR4B = (1 << WGM43) | (1 << CS42) |(0 << CS41) | (1 << CS40);
//	TCCR5B = (1 << WGM53) | (1 << CS52) |(0 << CS51) | (1 << CS50);
	
	/**
	 * CTC NOTES:
	 * Start the counter at 0
	 *	- DO NOT CHANGE THIS NUMBER DURING PROGRAM: WILL BLOCK ALL COMPARE MATCHES FOR ONE CLOCK CYCLE
	 *	- DO NOT SET TO BOTTOM WHEN COUNTING DOWN
	 *	- *NOT USED WITH PHASE CORRECT PWM MODE*
	 *	- TCNT used in CTC mode
	 *		- Continuously compared with OCR2A and OCR2B
	 *			- Match triggers Output Compare Flag (OCF2A or OCF2B) at next timer clock cycle
	 *	- Use Force Output Compare (FOC2x) to set Output Compare (OC2x)
	 *	- Frequency: f_OCnx = f_clk / (2 * N (1+OCRnx)), where N = 1, 8, 32, 64, 128, 256, 1024
	 *		- e.g. f_OCnx = 8MHz / (2 * 1024 (1 + 250)) = 15.563Hz
	 *	- SET THE OUTPUT COMPARE INTERRUPT ENABLE
	 *	- UPDATE OCR2A IN INTERRUPT
	 */
	TCNT2 = 10;
	
	/**
	 * OC2x should be set before setting Data Direction Register
	 * Change with FOC2x in Normal Mode
	 * Changing the COM2x1:0 bits will take effect immediately
	 *
	 * Since motor frequency is 616.667Hz, then the OCR2A needs to be:		// FALSE
	 *		OCR2A = (490.196Hz * 255) / 616.667Hz = 202.7027 ~ 203			// FALSE
	 *
	 * THE OCR2A IS USED TO DEFINE THE PULSE WIDTH
	 * THE FREQUENCY CAN ONLY BE CHANGED BY THE PRESCALER IN PHASE CORRECT PWM MODE OR CTC MODE
	 */
	OCR2A = MAX;	// 
//	//	OCR0A = 100 ;	// TOP 15.319Hz
//	OCR1A = 100 ;	// TOP 15.319Hz
//	OCR3A = 100 ;	// TOP 15.319Hz
//	OCR4A = 100 ;	// TOP 15.319Hz
//	OCR5A = 100 ;	// TOP 15.319Hz
	
	
	/**
	 * TIMSK2 – Timer/Counter2 Interrupt Mask Register
	 *	Bit 2 – OCIE2B: Timer/Counter2 Output Compare Match B Interrupt Enable
	 *	Bit 1 – OCIE2A: Timer/Counter2 Output Compare Match A Interrupt Enable
	 *	Bit 0 – TOIE2: Timer/Counter2 Overflow Interrupt Enable
	 */
	TIMSK2 = (1 << OCIE2A);

}


ISR (TIMER2_COMPA_vect){
//	OCR2A = 255;
	PORTA ^= 0x80;
}

#define STEP0 0x18 //0b0001 1000
#define STEP1 0x09 //0b0000 1001
#define STEP2 0x21 //0b0010 0001
#define STEP3 0x24 //0b0010 0100
#define STEP4 0x06 //0b0000 0110
#define STEP5 0x12 //0b0001 0010


#define SPEED_CONTROL_MAX	32767	/* largest number, slowest speed (approx 4Hz) */
#define SPEED_CONTROL_MIN	186		/* lowest number, fastest speed (approx 700Hz) */
#define LARGE_VARIANCE		1000
#define SMALL_VARIANCE		100


//#define ADC_MAX 1024
void placeholder(void);

int main(void)
{
	// PWM outputs (OCxn) must be set as outputs (even if connected to different port internally)
	DDRA = 0xFF;	// All outputs
	DDRB = 0xFF;	// All outputs	(PINB4 = OC2A)
	DDRC = 0xFF;	// All outputs
//	DDRD = 0x00;	// All inputs: Hardware: conneted to STK600 switches (buttons)
//	DDRE = 0xFF;
//	DDRF = 0xFF;	// All outputs
//	DDRG = 0xFF;	// All outputs
	DDRH = 0xFF;	// All outputs
//	DDRJ = 0xFF;	// all outputs
//	DDRL = 0xFF;
	PORTA = 0xA5;	// All ports off (output to LED's so LED's off)
	PORTB = 0xFF;
	PORTC = 0x0F;
//	PORTF = 0xFF;
//	PORTJ = 0xA5;
	PORTH = 0xFF;
	
	
	int o = MIN;
	//	int iPG5 = 0;	// OC0A
	int iPB5 = 0;	// OC1A
	int iPB4 = 0;	// OC2A 
	int iPE3 = 0;	// OC3A
	int iPH3 = 0;	// OC4A
	int iPL3 = 0;	// OC5A
	
	int PWM1 = 0;
	int PWM2 = 0;
	int PWM3 = 0;
	int PWM4 = 0;
	int PWM5 = 0;
	

//	initADC();
	initTimer_CTC();
	
	unsigned int count0 = 0;
	unsigned int count1 = 0;
	unsigned int count2 = 0;

//	unsigned int match0 = 65000;
	unsigned int match0 = TOP;
	unsigned int match1 = 255;
	
	unsigned int compareTimerMatch = 150;
	
	unsigned int prevTCNT2 = 0;
	sei();
	
	for(;;){

//		if ((PORTA & p7)){
//			count0++;
//		}

		
//		if ((TCNT2 == prevTCNT2) || (TCNT2 == 0)){
//			PORTA = ~TCNT2;
//			count0 = TCNT2;
//			/* used to make seconds */
//			if (count0 == match0){
//				count0 = 0;
//				count1++;
//			}
//			
//			if (count1 == match1){
//				count1 = 0;
//			}
//		
//			PORTC = ~count1;
//
//			
//		}
//		else {
//			prevTCNT2 = TCNT2 + 1;
//		}
		

//		PORTA = PORTB;
		PORTC = TCNT2;
		PORTB = PORTC;
		
//		if ((TCNT2%2) == 2){
//			TCCR2B |= (1 << FOC2A);
//			TIFR2 |= (1 << OCF2A);
//		}
		
//		switch (PIND){
//			case p0:
//				
//				compareTimerMatch = TOP;
//				break;
//			case p1:
//				compareTimerMatch = 100;
//				break;
//			case p2:
//				compareTimerMatch = BOTTOM + 1;
//				break;
//			default:
//				break;
//		}
		
//		OCR2A = compareTimerMatch;
		
		
	}
    return 0;   /* never reached */
}

/* 
 - find OutputCompare interrupt
 - 
 */
 

void placeholder(void){
//	if (ADC == ADC_MAX/2){
		// FORCE OUTPUT COMPARE MATCH
//		TCCR2B |= (1 << FOC2A); 
//	}
}














// ^--Space used to keep code of bottom of screen. It bugs me.
