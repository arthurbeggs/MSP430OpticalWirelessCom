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
	char *ini_msg = "PRESSIONE M(MASTER) ou S(SLAVE): "; int msg_len = 33;
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
	___send_char_usci_A1(10);
	___delay_ms(3);
	___send_char_usci_A1(13);
	___delay_ms(3);
	return ini_state;
}

int receber_pc(char* msg_pc)
{
	char c;
	int cnt;
	int maquina_destino;
	char *ini_msgg = "DIGITE A MAQUINA DESTINO: "; int msg_len = 26;
	___send_msg_usci_A1(ini_msgg, msg_len);
	for(;;)
	{
		___receive_char_usci_A1(&c);
		if(c>='0' && c<='2')
			break;
		else
			continue;
	}
	___send_char_usci_A1(c);
	___delay_ms(3);
	___send_char_usci_A1(10);
	___delay_ms(3);
	___send_char_usci_A1(13);
	___delay_ms(3);

	maquina_destino = (uint8_t) c-'0';


	char *ini_msg = "DIGITE A MENSAGEM A SER ENVIADA: "; msg_len = 33;
	___send_msg_usci_A1(ini_msg, msg_len);
	for(cnt=0;;cnt++)
	{
		___receive_char_usci_A1(&c);
		if(c==13  || cnt >= 300)
			break;
		else
		{
			*(msg_pc+cnt)=c;
			___send_char_usci_A1(*(msg_pc+cnt));
		}
	}

	return maquina_destino;
}

void manda_frase_pc(char* msg2pc)
{
	int l = strlen(msg2pc);
	___send_msg_usci_A1(msg2pc, l);
	___delay_ms(3);
	___send_char_usci_A1(10);
	___delay_ms(3);
	___send_char_usci_A1(13);
	___delay_ms(3);
}
