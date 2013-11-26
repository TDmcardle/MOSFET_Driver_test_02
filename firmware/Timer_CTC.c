//
//  Timer_CTC.c
//  MOSFET_Driver_test_02
//
//  Created by Tommy McArdle on 11/26/13.
//
//

/*
 *		Output: OC2A = PB4
 */

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

/**
 * TCCR2A (pg 184 in ATmega2560 datasheet)
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
 */


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

/**
 * TIMSK2 – Timer/Counter2 Interrupt Mask Register
 *	Bit 2 – OCIE2B: Timer/Counter2 Output Compare Match B Interrupt Enable
 *	Bit 1 – OCIE2A: Timer/Counter2 Output Compare Match A Interrupt Enable
 *	Bit 0 – TOIE2: Timer/Counter2 Overflow Interrupt Enable
 */


#include "main.h"


void initTimer_CTC(void)
{

	TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (1 << WGM21) | (0 << WGM20);
 		

	TCCR2B = (1 << CS22) |(1 << CS21) | (1 << CS20);
	

	TCNT2 = 10;
	
	OCR2A = MAX;	

	
	

	TIMSK2 = (1 << OCIE2A);
	
}
