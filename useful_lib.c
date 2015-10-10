/*
 * useful_lib.h
 *
 *  Created on: 16/09/2015
 *      Author: jesse
 */

#include <useful_lib.h>

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
