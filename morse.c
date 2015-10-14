/*
 * morse.h
 *
 *  Created on: 14/10/2015
 *      Author: Arthur, Luan e Jesse
 */

 #include "morse.h"

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

void converte(char* pt, char* morse)
{
	int i, j, k = 0;

	for(i=0; pt[i] != '\0'; i++)//for para percorrer a frase
    	{
    		for(j=0; MatMorse[frase[i]][j] != '\0'; j++, k++)
    		{
    			morse[k] = MatMorse[frase[i]][j];
    		}
 	   		moser[k] = 'e';
 	   		k++;
 	   	}
 	   	morse[k] = '\0';
}

void pisca_morse(char* morse){
	int i;

	for(i = 0; morse[i] != '\0'; i++)
		{
			if(letra[i] == 'p')
				dot();
			else if (letra[i] == 't')
				dash();
			else if(letra[i] == 'e')
				space();
		}
}
