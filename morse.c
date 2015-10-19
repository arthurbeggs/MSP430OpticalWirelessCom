/*
 * morse.h
 *
 *  Created on: 14/10/2015
 *      Author: Luan
 */

 #include "morse.h"
 #include <msp430.h>
 #include <string.h>

	    char* MatMorse[] = {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","e","","","","","","","","","","","","","","","","ttttt","ptttt","ppttt","ppptt","ppppt","ppppp","tpppp","ttppp","tttpp","ttttp","","","","","","","","pt","tppp","tptp","tpp","p","pptp","ttp","pppp","pp","pttt","tpt","ptpp","tt","tp","ttt","pttp","ttpt","ptp","ppp","t","ppt","pppt","ptt","tppt","tptt","ttpp","","","","","","","pt","tppp","tptp","tpp","p","pptp","ttp","pppp","pp","pttt","tpt","ptpp","tt","tp","ttt","pttp","ttpt","ptp","ppp","t","ppt","pppt","ptt","tppt","tptt","ttpp","","","","","",""};


 void dot(void)
{
	P1OUT |= BIT0;
	___delay_ms(250);
	P1OUT &= ~(BIT0);
	___delay_ms(250);

	return;
}

void dash(void)
{
	P1OUT |= BIT0;
	___delay_ms(750);
	P1OUT &= ~(BIT0);
	___delay_ms(250);

	return;
}

void space(void)
{
	___delay_ms(500);

	return;
}

void converte_pt_morse(char* pt, char* morse)
{
	int i, j, k = 0;

	for(i=0; pt[i] != '\0'; i++)//for para percorrer a frase
    	{
    		for(j=0; MatMorse[pt[i]][j] != '\0'; j++, k++)
    		{
    			morse[k] = MatMorse[pt[i]][j];
    		}
 	   		morse[k] = 'e';
 	   		k++;
 	   	}
 	   	morse[k] = '\0';
}

void converte_morse_pt(char* pt, char* morse)
{
	int i, j, k, achou;
	char letra[10];

	for(i=0, k=0; morse[i] != '\0'; i++)
	{
		j = 0;
		while(morse[i] != 'e')
			letra[j++] = morse[i++];
		letra[j] = '\0';

		if(letra[0] == '\0')
		{
			letra[0] = 'e';
			letra[1] = '\0';
		}

		for(j=0, achou=0; (j<129) && (!achou); j++)
		{
			if(!strcmp(letra, MatMorse[j]))
			{
				pt[k++] = j;
				achou = 1;
			}
		}

		pt[k] = '\0';

	}
}

void pisca_morse(char* morse){
	int i;

	for(i = 0; morse[i] != '\0'; i++)
		{
			if(morse[i] == 'p')
				dot();
			else if (morse[i] == 't')
				dash();
			else if(morse[i] == 'e')
				space();
		}
}
