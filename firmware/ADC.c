/*
 * ADC.c
 *
 * Created: 9/13/2012 9:40:38 PM
 *  Author: Tom
 */


/*
 
 
 
 
 ADMUX – ADC Multiplexer Selection Register
	 Bit 7:6 – REFS1:0: Reference Selection Bits
		 REFS1:0	Voltage Reference Selection(1)
		  00			AREF, Internal VREF turned off
 		  01			AVCC with external capacitor at AREF pin
		  10			Internal 1.1V Voltage Reference with external capacitor at AREF pin
 *		  11			Internal 2.56V Voltage Reference with external capacitor at AREF pin

 *	 Bit 5 – ADLAR: ADC Left Adjust Result

	 Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits

 
 
 ADCSRB – ADC Control and Status Register B
	 Bit 7 – Res: Reserved Bit
 
	 Bit 3 – MUX5: Analog Channel and Gain Selection Bit
		 MUX5..0	Single Ended Input	Positive Differential Input	Negative Differential Input	Gain
		  000000	 ADC0				 N/A
		  000001	 ADC1
		  000010	 ADC2
		  000011	 ADC3
		  000100	 ADC4
		  000101	 ADC5
		  000110	 ADC6
		  000111	 ADC7
		  011110	 1.1V (VBG)			 N/A
		  011111	 0V (GND)
		  100000	 ADC8				 N/A
		  100001	 ADC9
		  100010	 ADC10
		  100011	 ADC11
		  100100	 ADC12
		  100101	 ADC13
		  100110	 ADC14
		  100111	 ADC15
 
	 Bit 2:0 – ADTS2:0: ADC Auto Trigger Source
		 ADTS2:0	Trigger Source
 *		  000		 Free Running mode
		  001		 Analog Comparator
		  010		 External Interrupt Request 0
		  011		 Timer/Counter0 Compare Match A
 		  100		 Timer/Counter0 Overflow
		  101		 Timer/Counter1 Compare Match B
 		  110		 Timer/Counter1 Overflow
		  111		 Timer/Counter1 Capture Event
 
 
 
 ADCSRA – ADC Control and Status Register A
 *	 Bit 7 – ADEN: ADC Enable
 
 *	 Bit 6 – ADSC: ADC Start Conversion
 
 *	 Bit 5 – ADATE: ADC Auto Trigger Enable
 
 	 Bit 4 – ADIF: ADC Interrupt Flag
 
 *	 Bit 3 – ADIE: ADC Interrupt Enable
 
	 Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
		 ADPS2:0	Division Factor
		  000		   2
		  001		   2
		  010		   4
		  011		   8
		  100		  16
		  101		  32
		  110		  64
 *		  111		 128
 
 */

#include <avr/io.h>


/* INITIALIZE ADC */
void initADC(void)
{
	/* AVCC used for Vref,MUX5:0 = 0b000000 single ended ADC0 (PF0), Left adjust */
	ADMUX = (0x3<<REFS0) | (1<<ADLAR);
	
	/* enable ADC, enable ADC interrupt, set pre-scaler to 32 */
	/* ADCclk = 8MHz/32 and sample rate = ADCclk/(prescaler*13) = 19.23kHz */
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (0x7<<ADPS0);
	
	/* Trigger Source = Free Running Mode */
	ADCSRB = (0x0<<ADTS0);
}


