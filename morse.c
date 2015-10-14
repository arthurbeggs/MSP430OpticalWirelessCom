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