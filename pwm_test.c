#include <msp430g2553.h>
#define PWM BIT2;

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDC0_1MHZ; // setting the clock rate to 1MHz

    P1DIR |= PWM; // setting P1.2 as output
    P1SEL |= PWM; // setting P1.2 as timer output pin

    while (1) {
        TACCR0 = 20000; // time period of 20ms
        TACCR1 = 1000; // pulse width of 1ms
        TACCTL1 |= OUTMOD_7 + CCIE; // try without CCIE
        TACTL |= TASSEL_2 + MC_1 + TAIE; // try without TAIE
    }
}