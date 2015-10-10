/* Univeridade de Brasilia - UnB
 * Departamento de Engenharia Eletrica - ENE
 * Projeto 2 - Comunicação serial com Código Morse
 * Alunos: 	Luan Hackel Araujo		-
			Arthur Matos			-
			Jessé Barreto de Barros - 10/0106510
 * Data: 14/09/2015
 */


//Libraries
#include <msp430.h> //Common used library
#include <useful_lib.h> //Useful functions


/*
 * main.c
*/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//Configure clock system
    ___setup_clk0(16000000);




	return 0;
}
