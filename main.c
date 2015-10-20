/* Univeridade de Brasilia - UnB
 * Departamento de Engenharia Eletrica - ENE
 * Projeto 2 - Comunicação serial com Código Morse
 * Alunos:  Luan Hackel Araujo      - 12/0125781
            Arthur Matos            - 12/0111089
            Jessé Barreto de Barros - 10/0106510
 * Data: 14/09/2015
 */

#include <msp430.h> 
#include <intrinsics.h>
#include "useful_lib.h"
#include "morse.h"
#include "uart.h"
#include <string.h>
#include "i2c.h"

volatile char frase_morse[1200];
volatile char frase_pt[300];
int cont_letra;

int main(void) {
    volatile int maquina_destino;
    int frase_recebida;
    volatile char status;   //variável para identificar se esta máquina é master ou slave
                //'M' - para master
                //'S' - para slave

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    
    // Configure GPIO
    P1DIR |= BIT0;              // Enable LED output
    P4DIR |= BIT7;              // On = master
    P4OUT &= ~BIT7;//apagando o led do master

    //iniciando a I2C
    ___initI2C();

    //Configure clock system
    ___setup_clk0(16000000);

    //Sets timerA0 using clock0
    ___setup_timerA0();

    //Sets Universal Serial Communication Interface (USCI) A1
    ___setup_usci_A1(9600);


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
        //acender led master
        P4OUT |= BIT7;
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
        P4OUT &= ~BIT7;//apagando led master 

    }

    //__low_power_mode_0();

    while(1){
       
       if(state == MASTER){//master
            maquina_destino = receber_pc(frase_pt);

            if(!strcmp(frase_pt,"mudar o master"))
            {
                converte_pt_morse(frase_pt, frase_morse);
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

                //mudar para slave
                ___delay_ms(200);

                state = SLAVE;
                UCB0CTL1 |= UCSWRST;    // Reset module
                UCB0CTL0 &= ~UCMST;     // Set as slave
                UCB0RXBUF = 0x0;        // Clear RX buffer
                UCB0TXBUF = 0x0;        // Clear TX buffer
                UCB0I2COA = MYID;        // Slave address = 0x2
                UCB0CTL1 &= ~UCTR & ~UCSWRST;   // Release module and disable transmitter
                UCB0IE |= UCRXIE;       // Enable interrupts
                status = 'S';
                P4OUT &= ~BIT7;//apagando led master


            }
            else
            {
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
       }
       else { //slave
            if(frase_recebida) {

                converte_morse_pt(frase_pt, frase_morse);

                if(!strcmp(frase_pt,"MUDAR  O  MASTER"))
                {
                    
                    state = MASTER;
                    UCB0CTL1 |= UCTR | UCSWRST; // Reset module and enable transmitter
                    UCB0CTL0 |= UCMST;      // Set as master
                    UCB0RXBUF = 0x0;        // Clear RX buffer
                    UCB0TXBUF = 0x0;        // Clear TX buffer
                    UCB0I2COA = MYID;        // Master Address = 0x1
                    UCB0CTL1 &= ~UCSWRST;   // Release module
                    UCB0IE |= UCRXIE;       // Enable interrupts
                    status = 'M';
                    //acender led master
                    P4OUT |= BIT7;
                }
                else
                {
                    pisca_morse(frase_morse);
                    manda_frase_pc(frase_pt);
                    frase_recebida = 0;
                }
            }
            else{
                __low_power_mode_0();
            }
        }
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
