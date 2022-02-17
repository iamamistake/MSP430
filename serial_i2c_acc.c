#include <msp430.h>
#define PUSH_BUTTON BIT3
//char received_data [3];
char received_data;
unsigned int i = 0;

void main (void) {
     WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting the clock rate to 1MHz

    P1IE |= PUSH_BUTTON;
    P1IES |= PUSH_BUTTON;
    P1IFG &= ~PUSH_BUTTON;
    _enable_interrupts();


    P1SEL = BIT6 | BIT7;
    P1SEL2 = BIT6 | BIT7;
    UCB0CTL1 |= UCSWRST + UCSSEL_2;
    UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;
    UCB0BR0 = 10;
    UCB0I2CSA =  0x51; // Slave address (without R/W bit)
    UCB0CTL1 &= ~UCSWRST;

    IE2 |= UCB0TXIE;
    IE2 |= UCB0RXIE;

    while (1) {
        UCB0CTL1 |= UCTR;
        UCB0CTL1 |= UCTXSTT;
        while (UCB0CTL1 & UCTXSTT);
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);


        UCB0CTL1 &= ~UCTR;
        UCB0CTL1 |= UCTXSTT;
        while (UCB0CTL1 & UCTXSTT);
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);
//        __delay_cycles(1000000);
    }
}

//#pragma vector=PORT1_VECTOR
//__interrupt void PORT1_ISR (void) {
//    UCB0CTL1 |= UCTR;
//    UCB0CTL1 |= UCTXSTT;
//    while (UCB0CTL1 & UCTXSTP);
//
//
//
//    UCB0CTL1 &= ~UCTR;
//    UCB0CTL1 |= UCTXSTT;
//
//    P1IFG &= ~PUSH_BUTTON;
//}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR (void) {

    if (IFG2 & UCB0RXIFG) {
        received_data = UCB0RXBUF;
//        IFG2 &= ~ UCB0RXIFG;
//        IFG2 &= ~ UCB0TXIFG;
    }

    else {
        UCB0TXBUF = 0x01;
//        IFG2 &= ~ UCB0TXIFG;
    }
    IFG2 &= ~ UCB0TXIFG;
    IFG2 &= ~ UCB0RXIFG;
}


//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCIAB0RX_ISR (void) {
////    received_data [i] = UCB0RXBUF;
////    if (sizeof(received_data)==3) {
////        i=0;
////        UCB0CTL1 |= UCTXSTP;
////    }
//    received_data = UCB0RXBUF;
//
//    IFG2 &= ~ UCB0RXIFG;
//}
