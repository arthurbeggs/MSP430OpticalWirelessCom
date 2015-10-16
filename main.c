/* Univeridade de Brasilia - UnB
 * Departamento de Engenharia Eletrica - ENE
 * Projeto 2 - Comunicação serial com Código Morse
 * Alunos: 	Luan Hackel Araujo		- 12/0125781
			Arthur Matos			- 12/0111089
			Jessé Barreto de Barros - 10/0106510
 * Data: 14/09/2015
 */

//Libraries
#include <msp430.h> //Common used library
#include "useful_lib.h" //Useful functions
#include "morse.h"//morse function
#include <string.h>//funções para tratar strings
#include "uart.h"

extern int frase_recebida;
extern unsigned char rx_byte_buff, TXDataSize, *TXDataPtr, RXDataSize, *RXDataPtr;

//Variáveis globais
volatile char frase_morse[400];
volatile char frase_pt[100];
volatile char status;	//variável para identificar se esta máquina é master ou slave
			//'M' - para master
			//'S' - para slave
volatile unsigned char rx_buffer = 0;


void manda_frase(uint8_t address);

/*
 * main.c
*/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

	//variáveis locais
	volatile int maquina_destino;

    frase_recebida = 0;

    //SETUP


	//Configure clock system
    ___setup_clk0(16000000);

    //Sets timerA0 using clock0
	___setup_timerA0();

	//Sets Universal Serial Communication Interface (USCI) A1
	___setup_usci_A1(9600);

    //Sets Universal Serial Communication Interface (USCI) B0
    ___setup_usci_B0();


    //setando variáveis iniciais
    init();

    status = status_inicial();//função que se comunica com o PC via UART pergunta o usuário se essa maquina será master ou slave, retorna 'M' para master ou 'S' para slave

    if (status == 'M'){  ___switch_to_MASTER();  }
    else {  ___switch_to_SLAVE();  }


    //LOOP
    while(1) {
    	//caso esta maquina seja o master
    	if(status == 'M')
    	{
    		maquina_destino = receber_pc(frase_pt);//função que pede e recebe uma frase do PC e guarda na variável global frase, e pedi o PC destino (número de 0 à 2) e retrna esse número
    		converte_pt_morse(frase_pt, frase_morse);
    		___send_char_usci_A1(10);
    		___send_char_usci_A1(13);
    		___send_msg_usci_A1("MORSE: ", 7);
    		___send_msg_usci_A1(frase_morse, strlen(frase_morse));
            if(maquina_destino == MYID)//caso esteja mandando uma frase para se mesmo
    		{
    			if(strcmp(frase_pt, "mudar master"))//caso não seja o comendo de troca de mestre
    			{
    				pisca_morse(frase_morse);//pisca a frase em código morse
    			}
    			//caso seja o comando de troca de master não fazer nada, pois esta maquina já é o master
    		}
    		else {//caso esteja mandando para uma outra máquina
    			if(strcmp(frase_pt, "mudar master"))//caso não seja o comando de troca de mestre
    			{
    				pisca_morse(frase_morse);//pisca a frase em código morse
    				manda_frase(maquina_destino);//função que manda a frase via I2C para a maquina detino
    				P4OUT &= ~BIT7;                                                                                 // Flag de depuração
    			}
    			else { //caso seja o comando de troca de master
    				manda_frase(maquina_destino); //manda a frase "mudar master" para o PC destino via I2C
                    ___switch_to_SLAVE();
    				status = 'S';//muda seu status para Slave
    			}
    		}
    	}
    	else if (status == 'S') {//caso esta maquina seja um Slave
            ___read_message(frase_morse);
    		P4OUT &= ~BIT7;                                                                                 // Flag de depuração

			___send_msg_usci_A1("MORSE: ", 7);
			___send_msg_usci_A1(frase_morse, strlen(frase_morse));
			___send_char_usci_A1(10);
			___send_char_usci_A1(13);
            converte_morse_pt(frase_pt, frase_morse);
            ___send_msg_usci_A1("FRASE: ", strlen(frase_pt));
            ___send_msg_usci_A1(frase_pt, strlen(frase_pt));
    		if(strcmp(frase_pt, "mudar master"))//caso não seja o camando de troca de mestre
			{
				pisca_morse(frase_morse);//pisca a frase em código morse
				manda_frase_pc(frase_pt);//função que manda a frase via UART para o PC
			}
			else { //caso seja o comando de troca de master
                ___switch_to_MASTER();
				status = 'M';//muda seu status para Master
			}

    	}
    	else {
    		//variável de status indefinida tratar erro!!
    	}

    }


	return 0;
}


void manda_frase(uint8_t address)
{
    ___send_message(frase_morse, address);
}



#pragma vector=USCI_B0_VECTOR
__interrupt void I2C_ISR(void)
{
    switch(__even_in_range(UCB0IV,12))
    {
        case  0: break;                           // Vector  0: No interrupts
        case  2: break;                           // Vector  2: ALIFG
        case  4: break;                           // Vector  4: NACKIFG
        case  6:                                  // Vector  6: STTIFG
            UCB0IFG &= ~UCSTTIFG;
            break;
        case  8:                                  // Vector  8: STPIFG
            UCB0IFG &= ~UCSTPIFG;
            if (RXDataSize)                       // Check RX byte counter
                __bic_SR_register_on_exit(LPM0_bits);
            break;
        case 10:                                  // Vector 10: RXIFG
        	P4OUT |= BIT7;                                                                                 // Flag de depuração
            *RXDataPtr++ = UCB0RXBUF;
            RXDataSize++;
            break;
        case 12:                                  // Vector 12: TXIFG
        	P4OUT |= BIT7;                                                                                 // Flag de depuração
            if (TXDataSize)                       // Check TX byte counter
            {
                UCB0TXBUF = *TXDataPtr++;         // Load TX buffer
                TXDataSize--;                     // Decrement TX byte counter
            }
            else
            {
                UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
                UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
                __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
            }

            break;
        default: break;
    }
}

