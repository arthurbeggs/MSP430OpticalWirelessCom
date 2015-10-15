/*
 * morse.h
 *
 *  Created on: 14/10/2015
 *      Author: Luan
 */

#ifndef MORSE_LIB_H_
#define MORSE_LIB_H_

 	//Matriz do caractere em binário onde o indice é o numero do caractere em ascii


	void dot(void);//função que pisca um ponto
	void dash(void);//função que pisca um traço
	void space(void);//função que pisca um espaço

	void converte_pt_morse(char* pt, char* morse);//função que converte uma string de pt para morse
	void converte_morse_pt(char* pt, char* morse);//função que converte uma string de pt para morse

	void pisca_morse(char* morse);//função para piscar uma frase em morse

#endif
