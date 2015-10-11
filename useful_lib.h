/*
 * useful_lib.h
 *
 *  Created on: 16/09/2015
 *      Author: jesse
 */

#ifndef USEFUL_LIB_H_
#define USEFUL_LIB_H_

#include <msp430.h>
#include <stdlib.h>

//Sets Universal Serial Communication Interface (USCI) A1
extern void ___setup_usci_A1(int baud_rate);

//Sets clock0
extern void ___setup_clk0(long long int freq);

//Sets timerA0 using clock0
extern void ___setup_timerA0();

//Delay function using timerA0 in clock0
void ___delay_ms(int delay);

//Delay function using timerA0 in clock0 in seconds
void ___delay_s(int delay);

//Sets the USCI in A1
extern void ___setup_usci_A1(int baud_rate);

//Communication function to send a msg by usci A1
extern void ___send_msg_usci_A1(char *rx_buffer, int msg_length);

//Communication function to send a char by usci A1 
extern void ___send_char_usci_A1(char rx);




#endif /* USEFUL_LIB_H_ */
