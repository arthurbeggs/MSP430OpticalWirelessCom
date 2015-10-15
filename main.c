/* Univeridade de Brasilia - UnB
 * Departamento de Engenharia Eletrica - ENE
 * Projeto 2 - Comunicação serial com Código Morse
 * Alunos: 	Luan Hackel Araujo		- 12/0125781
			Arthur Matos			- 12/0111089
			Jessé Barreto de Barros - 10/0106510
 * Data: 14/09/2015
 */

//identidade para diferenciar um PC do outro
#define MYID 0

//Libraries
#include <msp430.h> //Common used library
#include "useful_lib.h" //Useful functions
#include "morse.h"//morse function
#include <string.h>//funções para tratar strings
#include <uart.h>

//Variáveis globais
volatile char frase_morse[1200];
volatile char frase_pt[300];
volatile char status;	//variável para identificar se esta máquina é master ou slave
			//'M' - para master
			//'S' - para slave
volatile unsigned char rx_buffer = 0;

void manda_frase(uint8_t address);

/*
 * main.c
*/
int main(void) {
	//variáveis locais
	volatile int maquina_destino;

    //SETUP
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//Configure clock system
    ___setup_clk0(16000000);

    //Sets timerA0 using clock0
	___setup_timerA0();

	//Sets Universal Serial Communication Interface (USCI) A1
	___setup_usci_A1(9600);


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
    			}
    			else { //caso seja o comando de troca de master
    				manda_frase(maquina_destino); //manda a frase "mudar master" para o PC destino via I2C
                    ___switch_to_SLAVE();
    				status = 'S';//muda seu status para Slave
    			}
    		}
    	}
    	else if (status == 'S') {//caso esta maquina seja um Slave
    		while(!frase_recebida) {}//aguardar receber uma frase
    		frase_recebida = 0;
            converte_morse_pt(frase_pt, frase_morse);
    		if(strcmp(frase_pt, "mudar master"))//caso não seja o comendo de troca de mestre
			{
				pisca_morse(frase_morse);//pisca a frase em código morse
				manda_frase_pc();//função que manda a frase via UART para o PC
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
    volatile int dummy;
    volatile int len = strlen(frase_morse);
    ___select_SLAVE(address);                           //Sets SLAVE address
    for (dummy = 0 ; dummy < len ; dummy++)
    {
        ___send_byte((uint8_t) frase_morse[dummy]);     //Send individual bytes
    }
}

