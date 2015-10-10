#include <msp430.h> 

/* main.c
 * Univeridade de Brasilia - UnB
 * Departamento de Engenharia Eletrica - ENE
 * Projeto 2 - Comunicação serial com Código Morse
 * Alunos: 	Luan Hackel Araujo		-
			Arthur Matos			-
			Jessé Barreto de Barros - 10/0106510
 * Data: 14/09/2015
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	//Configure clock system
    // ACLK = 32768HZ
    // SMCLK = 1MHz
    // MCLK = 16MHz
    UCSCTL0 = 0x0000;			// Set lowest possible DCOx, MODx
    							// These are controlled by FLL
    UCSCTL1 = DCORSEL_5;		// Select DCO range to 16MHz
    UCSCTL2 |= 243;				// DCOCLK = 2* (N+1) * 32.768kHz = 16MHz
    UCSCTL3 = SELREF_0;         // Set DCO FLL reference = XT1
    UCSCTL4 = SELA_0 | SELS_3 | SELM_3 ;	// Set ACLK = XT1,
    										// SMCLK = DCOCLK, MCLK = DCOCLK
    UCSCTL5 = DIVPA_0 | DIVA_0 | DIVS_4 | DIVM_0;	// Set SMCLK, divide by 16
    __delay_cycles(250000);		// Wait for oscillator to stabilize
	
	
	return 0;
}
