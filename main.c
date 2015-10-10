#include <msp430.h> 

/*
 * main.c
 *jesse isso é um teste
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	return 0;
}
