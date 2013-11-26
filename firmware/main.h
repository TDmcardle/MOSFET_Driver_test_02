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



void initPorts(void);
void initTimer_CTC(void);
void initADC(void);


#endif
