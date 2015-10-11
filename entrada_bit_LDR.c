/*
 * Entrada de bit através do LDR ligado com divisor de tensão
 *   
 *       LDR     R=56Kohm
 *   _____O________[]___
 *   |        |        |
 *   |        |        |
 *  3.3v     P8.2     GND
 */

#include "msp430_lib3.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	
    // Configure outputs
    P2DIR |= BIT0;					// Set pino do led
    P1DIR |= BIT0;                  // Set P1.0 to output direction

    // Configure inputs
    P8DIR &= ~BIT2;					//setando o pino de entrada do LDR

    P1DIR &= ~BIT1;					// Set P4.7 to input direction
    P1REN |= BIT1;					// Enable pull-up or pull-down resistor
    P1OUT |= BIT1;					// Set resistor to pull-up


    for(;;) {
    	volatile unsigned int i;	// volatile to prevent optimization

    	

    	//Trocar estado do led quando o botão 1 for precionado
    	if (!(P1IN&BIT1)){			// If button S1 is pressed
    		while(!(P1IN&BIT1)){} 
            P2OUT ^= BIT0;          // Toggle P1.0 using exclusive-OR  
    	}

        //mostrar estado da porta do LDR no LED da placa
        if(P8IN&BIT2)
            P1OUT =| BIT0;
        else
            P1OUT =& (~BIT0)


    	//do i--;						// Delay
    	//while(i != 0);
    }

	return 0;
}
