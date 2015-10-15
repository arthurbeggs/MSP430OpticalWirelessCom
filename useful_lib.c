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
	P4DIR |= BIT7;				// P4.7 = LED VERDE DE COMMUNICACAO
	P4OUT &= ~BIT7;
}



extern void ___receive_char_usci_A1(char *tx)
{
	while (!(UCRXIFG==(UCRXIFG & UCA1IFG)));
	*tx = UCA1RXBUF;
}

extern void ___send_msg_usci_A1(char *rx_buffer, int msg_length)
{
	unsigned int cnt = 0; //Cnt bytes
	for(cnt=0;cnt<msg_length;cnt++)
	{
		UCA1TXBUF = *(rx_buffer+cnt);
		//Wait until each bit has been sent
		while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
		___delay_ms(2);
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

extern void ___setup_usci_B0()
{
    UCB0CTL1 = UCSWRST;             // Keep module on reset state
    UCB0CTL0 = UCMODE_3 | UCSYNC;   // I2C mode, 8-bit data, slave mode
    UCB0CTL1 |= UCSSEL_2;           // BRCLK = SMCLK
    UCB0BR1 = 0;
    UCB0BR0 = 4;                    // Prescaler = 4
    P3SEL |= BIT0 | BIT1;           // Configure I/O ports
    UCB0I2COA = (uint8_t) MYID;     // Set own address using MYID defined in main.c
    UCB0CTL1 &= ~UCSWRST;           // Release module for operation
    UCB0IE |= UCSTPIE | UCRXIE;     // Enable STOP and RX interrupts
}

extern void ___switch_to_MASTER()
{
    UCB0CTL1 |= UCTR | UCSWRST; // Reset module and enable transmitter
    UCB0CTL0 |= UCMST;      // Set as master
    UCB0RXBUF = 0x0;        // Clear RX buffer
    UCB0TXBUF = 0x0;        // Clear TX buffer
    UCB0CTL1 &= ~UCSWRST;   // Release module
    UCB0IE |= UCRXIE;       // Enable interrupts
}

extern void ___switch_to_SLAVE()
{
    UCB0CTL1 |= UCSWRST;    // Reset module
    UCB0CTL0 &= ~UCMST;     // Set as slave
    UCB0RXBUF = 0x0;        // Clear RX buffer
    UCB0TXBUF = 0x0;        // Clear TX buffer
    UCB0CTL1 &= ~UCTR & ~UCSWRST;   // Release module and disable transmitter
    UCB0IE |= UCSTPIE | UCRXIE;       // Enable interrupts
}

extern void ___select_SLAVE(uint8_t address)
{
    UCB0I2CSA = (uint8_t) address;
}

extern void ___stop_transmission()
{
    UCB0CTL1 |= UCTXSTP;
}

extern void ___start_transmission()
{
    
    UCB0CTL1 |= UCTR;               // Start transmitter
    UCB0CTL1 |= UCTXSTT;            // Start condition

    __delay_cycles(1000);
    __no_operation();
}

extern void ___send_byte(uint8_t txByte)
{
    P4OUT ^= BIT7;                  // Flag de depuração
    while(!(UCB0IFG & UCTXIFG)){}   // TX Buffer ready?
    UCB0TXBUF = txByte;             // Send byte
    while(!(UCB0IFG & UCTXIFG)){}   // TX Buffer ready?

    __delay_cycles(1000);
    __no_operation();
}

extern void ___read_byte(char *rxBuffer)
{
	P4OUT ^= BIT7;
    rx_byte_buff = 0;
    rx_byte_buff = (char) UCB0RXBUF;
    UCB0RXBUF = 0x0;
    strncat(rxBuffer, rx_byte_buff, 1);
}
