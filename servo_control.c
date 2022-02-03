#include <msp430g2553.h>
#define POT BIT0;
#define PWM BIT2;
volatile int value;

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting clock rate to 1MHz

    P1DIR |= PWM; // setting P1.2 as output
    P1SEL |= PWM; // setting P1.2 as timer output pin

    ADC10CTL0 |= SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE; // try without ADC10IE
    ADC10CTL1 |= INCH_0 + SHS_0 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_0;
    ADC10AE0 |= POT;
    ADC10CTL0 |= ENC;

    while (1) {
        ADC10CTL0 |= ADC10SC;
        while (ADC10CTL1 & ADC10BUSY);
        value = ADC10MEM;

        TACCR0 = 20000; // time period of 20ms
        TACCR1 = 1000 + (int) (1000./1023)*value;
        TACCTL1 = OUTMOD_7 + CCIE; // try without CCIE
        TACTL = TASSEL_2 + MC_1 + TAIE; // try without TAIE
    }
}