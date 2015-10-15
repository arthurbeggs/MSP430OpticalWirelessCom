/*
 * uart.h
 *
 *  Created on: 14/10/2015
 *      Author: jesse
 */

#ifndef UART_H_
#define UART_H_

//Pergunta ao usuário se esta máquina é Master ou Slave. Retorna ‘M’ para Master e ‘S’ para Slave.
char status_inicial(void);

//Pergunta ao usuário a frase de envio e a máquina destino.
int receber_pc(char*);

//Mandar para o usuário uma frase
void manda_frase_pc(char*);

#endif /* UART_H_ */
