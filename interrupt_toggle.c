#include <msp430g2553.h>
#define LED BIT0;
#define PUSH_BUTTON BIT3;

void main (void) {
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= LED;
    P1OUT |= LED;
    P1IE |= PUSH_BUTTON;
    P1IES |= PUSH_BUTTON;
    P1IFG &= ~PUSH_BUTTON;
    _enable_interrupts();
    while (1);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void) {
    P1OUT ^= LED;
    P1IFG &= ~PUSH_BUTTON;
}