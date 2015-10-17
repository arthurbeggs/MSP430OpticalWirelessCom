/*********************************
 * I2C master-slave example
 *
 * Button S2 changes MCU personality
 * Button S1 transmits master byte
 *
 * LED2 on = master
 * LED1 toggles when received master command
 *
 * P3.0 = SDA
 * P3.1 = SCL
 * Remember to pullup both SCA and SCL lines
 *********************************/

#include <msp430.h> 
#include <intrinsics.h>
#include "useful_lib.h"
#include "morse.h"
#include "uart.h"
#include <string.h>

volatile unsigned char rx_buff = 0;

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

volatile int maquina_destino;
int frase_recebida;
int cont_letra;
volatile char status;   //variável para identificar se esta máquina é master ou slave
            //'M' - para master
            //'S' - para slave
volatile char frase_morse[1200];
volatile char frase_pt[300];


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    
    // Configure GPIO
    P1DIR |= BIT0;              // Enable LED output
    P4DIR |= BIT7;              // On = master
   // P1REN |= BIT1;              // Enable internal resistor
   // P1OUT |= BIT1;              // Set internal pullup resistor
  //  P1IE |= BIT1;               // Enable pin interrupts
  //  P1IFG = 0x0;                // Clear interrupt flags
 //   P2REN |= BIT1;              // Enable internal resistor
 //   P2OUT |= BIT1;              // Set internal pullup resistor
 //   P2IE |= BIT1;               // Enable pin interrupts
 //   P2IFG = 0x0;                // Clear interrupt flags

    //Configure clock system
    ___setup_clk0(16000000);

    //Sets timerA0 using clock0
    ___setup_timerA0();

    //Sets Universal Serial Communication Interface (USCI) A1
    ___setup_usci_A1(9600);


    // Configure clock system
    // ACLK = 32768HZ
    // SMCLK = 1MHz
    // MCLK = 16MHz
    //UCSCTL0 = 0x0000;           // Set lowest possible DCOx, MODx
                                // These are controlled by FLL
    //UCSCTL1 = DCORSEL_5;        // Select DCO range to 16MHz
    //UCSCTL2 |= 243;             // DCOCLK = 2* (N+1) * 32.768kHz = 16MHz
    //UCSCTL3 = SELREF_0;         // Set DCO FLL reference = XT1
    //UCSCTL4 = SELA_0 | SELS_3 | SELM_3 ;    // Set ACLK = XT1,
                                            // SMCLK = DCOCLK, MCLK = DCOCLK
    //UCSCTL5 = DIVPA_0 | DIVA_0 | DIVS_4 | DIVM_0;   // Set SMCLK, divide by 16
    //__delay_cycles(250000);     // Wait for oscillator to stabilize

    // Configure USCI module
    UCB0CTL1 = UCSWRST;             // Keep module on reset state
    UCB0CTL0 = UCMODE_3 | UCSYNC;   // I2C mode, 8-bit data, slave mode
    UCB0CTL1 |= UCSSEL_2;           // BRCLK = SMCLK = 1MHz
    UCB0BR1 = 0;
    UCB0BR0 = 4;                    // Prescaler = 4
    P3SEL |= BIT0 | BIT1;           // Configure I/O ports
    //UCB0STAT |= UCLISTEN;         // Loopback
    UCB0CTL1 &= ~UCSWRST;           // Release module for operation
    UCB0IE |= UCRXIE;               // Enable RX interrupts

    // Deafult state = SLAVE
    state = SLAVE;
    UCB0I2COA = MYID;                // slave address = 0x2
    P4OUT &= ~BIT7;
    
    frase_recebida = 0;
    cont_letra = 0;

    status = status_inicial();//função que se comunica com o PC via UART pergunta o usuário se essa maquina será master ou slave, retorna 'M' para master ou 'S' para slave

    if (status == 'M'){
        state = MASTER;
        UCB0CTL1 |= UCTR | UCSWRST; // Reset module and enable transmitter
        UCB0CTL0 |= UCMST;      // Set as master
        UCB0RXBUF = 0x0;        // Clear RX buffer
        UCB0TXBUF = 0x0;        // Clear TX buffer
        UCB0I2COA = MYID;        // Master Address = 0x1
        UCB0CTL1 &= ~UCSWRST;   // Release module
        UCB0IE |= UCRXIE;       // Enable interrupts
        status = 'M';
    }
    else {  

        state = SLAVE;
        UCB0CTL1 |= UCSWRST;    // Reset module
        UCB0CTL0 &= ~UCMST;     // Set as slave
        UCB0RXBUF = 0x0;        // Clear RX buffer
        UCB0TXBUF = 0x0;        // Clear TX buffer
        UCB0I2COA = MYID;        // Slave address = 0x2
        UCB0CTL1 &= ~UCTR & ~UCSWRST;   // Release module and disable transmitter
        UCB0IE |= UCRXIE;       // Enable interrupts
        status = 'S';

    }

    //__low_power_mode_0();

    while(1){
       
       if(state == MASTER){//master
            maquina_destino = receber_pc(frase_pt);
            converte_pt_morse(frase_pt, frase_morse);
            pisca_morse(frase_morse);

            UCB0I2CSA = maquina_destino;    // Slave address
            UCB0CTL1 |= UCTXSTT;            //Start condition

            while(frase_morse[cont_letra] != '\0'){
                while(!(UCB0IFG & UCTXIFG)){}           // Buffer ready?
                UCB0TXBUF = frase_morse[cont_letra++];  // Send char
            }
            while(!(UCB0IFG & UCTXIFG)){}           // Buffer ready?
            UCB0TXBUF = frase_morse[cont_letra];
            while(!(UCB0IFG & UCTXIFG)){}   // Buffer ready?
            UCB0CTL1 |= UCTXSTP;            // Stop transmission
            tx_state = TX_IDLE;
            cont_letra = 0;
       }
       else { //slave
            if(frase_recebida) {
                pisca_morse(frase_morse);
                converte_morse_pt(frase_pt, frase_morse);
                manda_frase_pc(frase_pt);
                frase_recebida = 0;
            }
            else{
                __low_power_mode_0();
            }
        }


       /*
        switch(state){
        case GO_MASTER:
            state = MASTER;
            UCB0CTL1 |= UCTR | UCSWRST; // Reset module and enable transmitter
            UCB0CTL0 |= UCMST;      // Set as master
            UCB0RXBUF = 0x0;        // Clear RX buffer
            UCB0TXBUF = 0x0;        // Clear TX buffer
            UCB0I2COA = 0x1;        // Master Address = 0x1
            UCB0CTL1 &= ~UCSWRST;   // Release module
            UCB0IE |= UCRXIE;       // Enable interrupts
            break;
        case GO_SLAVE:
            state = SLAVE;
            UCB0CTL1 |= UCSWRST;    // Reset module
            UCB0CTL0 &= ~UCMST;     // Set as slave
            UCB0RXBUF = 0x0;        // Clear RX buffer
            UCB0TXBUF = 0x0;        // Clear TX buffer
            UCB0I2COA = 0x2;        // Slave address = 0x2
            UCB0CTL1 &= ~UCTR & ~UCSWRST;   // Release module and disable transmitter
            UCB0IE |= UCRXIE;       // Enable interrupts
            break;
        }
        */
        /*
        switch(rx_state){
        case RECEIVED:
            if(rx_buff == 'm'){
                P1OUT ^= BIT0;
            }
            rx_state = RX_IDLE;
            break;
        }
        */
        /*
        switch(tx_state){
        case SEND:
            UCB0I2CSA = 0x2;                // Slave address
            UCB0CTL1 |= UCTXSTT;            //Start condition
            while(!(UCB0IFG & UCTXIFG)){}   // Buffer ready?
            UCB0TXBUF = 'm';                // Send char
            while(!(UCB0IFG & UCTXIFG)){}   // Buffer ready?
            UCB0CTL1 |= UCTXSTP;            // Stop transmission
            tx_state = TX_IDLE;
            break;
*/
        }

        
    return 0;
}

#pragma vector=USCI_B0_VECTOR
__interrupt void SPI_ISR(void){
    rx_buff = UCB0RXBUF;
    UCB0RXBUF = 0x0;//crear RX buffer
    rx_state = RECEIVED;
    frase_morse[cont_letra++] = rx_buff;
    if(rx_buff == '\0'){
        frase_recebida = 1;
        cont_letra = 0;
    }
    __low_power_mode_off_on_exit();
}
/*
#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(void){
    switch(state){
    case MASTER:
        tx_state = SEND;
        break;
    case SLAVE:
        break;
    }
    P2IFG &= ~BIT1;             // Clear interrupt flag
    __low_power_mode_off_on_exit();
}
*/
/*
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void){
    switch(state){
    case MASTER:
        state = GO_SLAVE;
        P4OUT &= ~BIT7;
        break;
    case SLAVE:
        state = GO_MASTER;
        P4OUT |= BIT7;
        break;
    }
    P1IFG &= ~BIT1;                 // Clear interrupt flag
    __low_power_mode_off_on_exit(); // Execute main loop
}
*/
