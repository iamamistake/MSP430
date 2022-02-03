#include <msp430g2553.h>
#define ADC_IN BIT0; 
volatile int value;

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDC0_1MHZ; // setting the clock rate to 1MHz
    
    ADC10CTL0 |= SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;
    ADC10CTL1 |= INCH_0 + SHS_0 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_0;
    ADC10AE0 |= ADC_IN;
    ADC10CTL0 |= ENC;

    while (1) {
        ADC10CTL0 |= ADC10SC;
        while (ADC10CTL1 & ADC10BUSY);
        value = ADC10MEM; // returns value ranging 0-1023
    }
}