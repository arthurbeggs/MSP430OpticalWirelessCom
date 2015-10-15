/*
 * uart.c
 *
 *  Created on: 14/10/2015
 *      Author: jesse
 */


#include"uart.h"

char status_inicial(void)
{
	char ini_state;
	char *ini_msg = "PRESSIONE M(MASTER) ou S(SLAVE): "; int msg_len = 40;
		___send_msg_usci_A1(ini_msg, msg_len);

		for(;;)
		{
			___receive_char_usci_A1(&ini_state);
			if(ini_state == 'm' || ini_state == 's')
			{
				ini_state = (ini_state=='m' ? 'M' : 'S');
				break;
			}
			else
				continue;
		}
		___delay_ms(3);
		___send_char_usci_A1(ini_state);
		___delay_ms(3);
		___send_char_usci_A1('\n');
		___delay_ms(3);
		___send_char_usci_A1('\r');
		___delay_ms(3);

		return ini_state;
}
