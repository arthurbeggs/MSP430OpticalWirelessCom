/*
 * useful_lib.h
 *
 *  Created on: 16/09/2015
 *      Author: jesse
 */

#ifndef USEFUL_LIB_H_
#define USEFUL_LIB_H_

#include <msp430.h>

//Sets the clock0
extern void ___setup_clk0(long long int freq)

//Sets an timerA0 using clock0
extern void ___setup_timerA0();

//Delay function using timerA0 in clock0
void ___delay_ms(int delay);

//Delay function using timerA0 in clock0 in seconds
void ___delay_s(int delay);



#endif /* USEFUL_LIB_H_ */
