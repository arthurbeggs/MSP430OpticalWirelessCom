/*
 * i2c.h
 *
 *  Created on: 19/10/2015
 *      Author: jesse
 				luan
 */

#ifndef I2C_H_
#define I2C_H_

volatile unsigned char rx_buff;

volatile enum _tx_state{
    TX_IDLE = 0,
    SEND
} tx_state;

volatile enum _rx_state{
    RX_IDLE = 0,
    RECEIVED
} rx_state;

volatile enum _state{
    MASTER = 0,
    SLAVE,
    GO_MASTER,
    GO_SLAVE
} state;

void ___initI2C(void);

#endif /* I2C_H_ */
