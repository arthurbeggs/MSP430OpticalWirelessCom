/*
 * useful_lib.h
 *
 *  Created on: 16/09/2015
 *      Author: Arthur, Luan e Jesse
 */

#include "useful_lib.h"

//função de inicialização
void init(void)
{
	//Configurando as portas de saida
	P1DIR |= BIT0;				// P1.0 = LED
}


extern void ___send_msg_usci_A1(char *rx_buffer, int msg_length)
{
	unsigned int cnt = 0; //Cnt bytes
	for(cnt=0;cnt<msg_length;cnt++)
	{
		UCA1TXBUF = *(rx_buffer+cnt);
		//Wait until each bit has been sent
		while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
		___delay_ms(1);
	}
}

extern void ___send_char_usci_A1(char rx)
{
	UCA1TXBUF = rx;
	while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
}

extern void ___setup_usci_A1(int baud_rate)
{
	// Configure USCI module
    UCA1CTL1 = UCSWRST;			// Keep module on reset state
    UCA1CTL0 = 0x0;				// Async UART mode, no parity,
    							// 8-bit data, one stop bit, LSB first
    UCA1CTL1 |= UCSSEL_2; 		// BRCLK = SMCLK = 1MHz
    UCA1BR0 = (baud_rate == 9600 ? 104 : baud_rate == 4800 ? 208 : 417);	// 9600 bps with 1MHz input clock - 4800 and 2400
    UCA1BR1 = 0;
    UCA1MCTL = UCBRF_0 | UCBRS_1;	// Refer to table 34-4
    P4SEL |= BIT4 | BIT5;		// Configure I/O ports
    UCA1CTL1 &= ~UCSWRST;		// Release module for operation
    UCA1IE |= UCRXIE;			// Enable RX interrupts
}

extern void ___setup_clk0(long long int freq)
{
	//Configure clock system
	// ACLK = 32768HZ
    // SMCLK = 1MHz
    // MCLK = 16MHz
    UCSCTL0 = 0x0000;			// Set lowest possible DCOx, MODx
    							// These are controlled by FLL
    UCSCTL1 = DCORSEL_5;		// Select DCO range to 16MHz
    UCSCTL2 |= (int)((freq>>16) - 1);			// DCOCLK = 2* (N+1) * 32.768kHz (~= 16MHz with N=243)
    UCSCTL3 = SELREF_0;         // Set DCO FLL reference = XT1
    UCSCTL4 = SELA_0 | SELS_3 | SELM_3 ;	// Set ACLK = XT1,
    										// SMCLK = DCOCLK, MCLK = DCOCLK
    UCSCTL5 = DIVPA_0 | DIVA_0 | DIVS_4 | DIVM_0;	// Set SMCLK, divide by 16
    __delay_cycles(250000);		// Wait for oscillator to stabilize
}

extern void ___setup_timerA0()
{
	//Configure timer A0
	    TA0CTL = TASSEL_1 | MC_1 | TACLR;	// SMCLK = 32768Hz, Up Mode
	    TA0CCR0 = 255;
}

void ___delay_ms(int delay)
{
	const unsigned ONE_MS = 33;
	TA0CCR0 = delay*ONE_MS; //Add a different const in controller reg
	TA0CTL &= ~(TAIFG); //Resets the controller after the interrupt flag
	for(;!(TA0CTL & TAIFG););
}

void ___delay_s(int delay)
{
	const unsigned ONE_S = 32768;
	TA0CCR0 = delay*ONE_S; //Add a different const in controller reg
	TA0CTL &= ~(TAIFG); //Resets the controller after the interrupt flag
	for(;!(TA0CTL & TAIFG););
}
