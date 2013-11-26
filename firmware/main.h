//
//  main.h
//  MOSFET_Driver_test_02
//
//  Created by Tommy McArdle on 11/26/13.
//
//

#ifndef MOSFET_Driver_test_02_main_h
#define MOSFET_Driver_test_02_main_h


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>


#define TOP		255
#define BOTTOM	0
#define MAX		250
#define MIN		125
#define RANGE	(MAX-MIN)


#define A_HIGH	 0x01
#define A_LOW	 0x02
#define B_HIGH	 0x04
#define B_LOW	 0x08
#define C_HIGH	 0x10
#define C_LOW	 0x20

#define phase_A 0
#define phase_B 1
#define phase_C 2



void initPorts(void);
void initTimer_CTC(void);
void initADC(void);


#endif
