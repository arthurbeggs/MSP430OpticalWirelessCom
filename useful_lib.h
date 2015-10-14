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
#include <stdint.h>
#include <string.h>

//Variables
 volatile unsigned char rx_byte_buff = 0;


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

//Sets the USCI (I2C) in B0
extern void ___setup_usci_B0();

//Switch to Master
extern void ___switch_to_MASTER();

//Switch to Slave
extern void ___switch_to_SLAVE();

//Slave select
extern void ___select_SLAVE(uint8_t address);

//Stop transmission
extern void ___stop_transmission();

//Send byte
extern void ___send_byte(uint8_t txByte);

//Receive byte
extern void ___read_byte(char *rxBuffer);

#endif /* USEFUL_LIB_H_ */
