#include <msp430.h>
#define PUSH_BUTTON BIT3
char packet[] = { <register address>, <data1>, <data2>, <data3>);
unsigned int i = 0;

//void serial_transmit (char *str);

void main (void) {
     WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting the clock rate to 1MHz

    P1IE |= PUSH_BUTTON;
    P1IES |= PUSH_BUTTON;
    P1IFG &= ~PUSH_BUTTON;
    _enable_interrupts();


    P2SEL = BIT1 | BIT2;
    P2SEL2 = BIT1 | BIT2;
    UCB0CTL1 |= UCSWRST + UCSSEL_2;
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL1 |= UCTR; // transmitter mode selected
    UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;
    UCB0BR0 = 10;
    UCB0I2CSA = ; // Slave address
    UCB0CTL1 &= ~UCSWRST;

    IE2 |= UCB0TXIE;
    IE2 |= UCB0RXIE;

    while (1);
}

//void serial_transmit (char *str) {
//    while (*str != 0) {
//        while (!(IFG2 & UCB0TXIFG));
//        UCB0TXBUF = *str++;
//    }
//}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void) {
    UCB0CTL1 |= UCTXSTT;
    P1IFG &= ~PUSH_BUTTON;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR (void) {
    UCB0TXBUF = ; // Data to be transmitted

    UCB0CTL1 |= UCTXSTP;
    IFG2 &= ~ UCB0TXIFG;
}
