/*
 * morse.h
 *
 *  Created on: 14/10/2015
 *      Author: Luan
 */

#ifndef MORSE_LIB_H_
#define MORSE_LIB_H_

 	//Matriz do caractere em binário onde o indice é o numero do caractere em ascii
	char* MatMorse[] = {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","e","","","","","","","","","","","","","","","","ttttt","ptttt","ppttt","ppptt","ppppt","ppppp","tpppp","ttppp","tttpp","ttttp","","","","","","","","pt","tppp","tptp","tpp","p","pptp","ttp","pppp","pp","pttt","tpt","ptpp","tt","tp","ttt","pttp","ttpt","ptp","ppp","t","ppt","pppt","ptt","tppt","tptt","ttpp","","","","","","","pt","tppp","tptp","tpp","p","pptp","ttp","pppp","pp","pttt","tpt","ptpp","tt","tp","ttt","pttp","ttpt","ptp","ppp","t","ppt","pppt","ptt","tppt","tptt","ttpp","","","","","",""};


	void dot(void);//função que pisca um ponto
	void dash(void);//função que pisca um traço
	void space(void);//função que pisca um espaço

	void converte(char* pt, char* morse);//função que converte uma string de pt para morse

	void pisca_morse(char* morse);//função para piscar uma frase em morse

#endif