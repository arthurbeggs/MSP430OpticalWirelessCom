//#include <msp430.h>
//#include <intrinsics.h>
//
//void sendArray(char *rx_buffer, int length);
//void sendChar(char rx);
//
//volatile unsigned char rx_buff = 0;
//volatile unsigned char send = 0;
//
//int main(void) {
//    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
//
//    // Configure GPIO
//    P1DIR |= BIT0;				// Enable LED output
//    P1REN |= BIT1;				// Enable internal resistor
//    P1OUT |= BIT1;				// Set internal pullup resistor
//    P1IE |= BIT1;				// Enable pin interrupts
//    P1IFG = 0x0;				// Clear interrupt flags
//
//    // Configure clock system
//    // ACLK = 32768HZ
//    // SMCLK = 1MHz
//    // MCLK = 16MHz
//    UCSCTL0 = 0x0000;			// Set lowest possible DCOx, MODx
//    							// These are controlled by FLL
//    UCSCTL1 = DCORSEL_5;		// Select DCO range to 16MHz
//    UCSCTL2 |= 243;				// DCOCLK = 2* (N+1) * 32.768kHz = 16MHz
//    UCSCTL3 = SELREF_0;         // Set DCO FLL reference = XT1
//    UCSCTL4 = SELA_0 | SELS_3 | SELM_3 ;	// Set ACLK = XT1,
//    										// SMCLK = DCOCLK, MCLK = DCOCLK
//    UCSCTL5 = DIVPA_0 | DIVA_0 | DIVS_4 | DIVM_0;	// Set SMCLK, divide by 16
//    __delay_cycles(250000);		// Wait for oscillator to stabilize
//
//    // Configure USCI module
//    UCA1CTL1 = UCSWRST;			// Keep module on reset state
//    UCA1CTL0 = 0x0;				// Async UART mode, no parity,
//    							// 8-bit data, one stop bit, LSB first
//    UCA1CTL1 |= UCSSEL_2; 		// BRCLK = SMCLK = 1MHz
//    UCA1BR0 = 104;				// 9600 bps with 1MHz input clock
//    UCA1BR1 = 0;
//    UCA1MCTL = UCBRF_0 | UCBRS_1;	// Refer to table 34-4
//    P4SEL |= BIT4 | BIT5;		// Configure I/O ports
//    UCA1CTL1 &= ~UCSWRST;		// Release module for operation
//    UCA1IE |= UCRXIE;			// Enable RX interrupts
//
//    __low_power_mode_0();
//
//    while(1){
//    	if(send){
//    		sendArray("MSP430\n\r", 8);
//    		send = 0;
//    	} else if(rx_buff == 'a'){
//    		P1OUT ^= BIT0;
//    	}
//    	__low_power_mode_0();
//    }
//	return 0;
//}
//
//void sendChar(char rx){
//	UCA1TXBUF = rx;
//	// Wait until each bit has been sent
//	while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
//}
//void sendArray(char *rx_buffer, int length){
//	unsigned int count = 0;
//
//	while(count < length){
//		UCA1TXBUF = *(rx_buffer+(count++));
//		// Wait until each bit has been sent
//		while(!(UCTXIFG==(UCTXIFG & UCA1IFG))&&((UCA1STAT & UCBUSY)==UCBUSY));
//	}
//}
//
//#pragma vector=PORT1_VECTOR
//__interrupt void P1_ISR(void){
//	send = 1;					// Send flag
//	P1IFG &= ~BIT1;				// Clear interrupt flag
//	__low_power_mode_off_on_exit();
//}
//
//#pragma vector=USCI_A1_VECTOR
//__interrupt void UART_ISR(void){
//	rx_buff = UCA1RXBUF;
//	__low_power_mode_off_on_exit();
//}
