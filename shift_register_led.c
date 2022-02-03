#include <msp430g2553.h>
#define SERIAL BIT0
#define SRCLK BIT1
#define RCLK BIT2

void SRCLK_pulse (void);
void RCLK_pulse (void);
void Send_Bit (bool x);

void main (void) {
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= SERIAL + SRCLK + RCLK;
    P1OUT = 0x00U;
    volatile int i=0;
    char bit_data = 0b01010101;
    while (i<8) {
        Send_Bit((bit_data & (0x01U << i)));
        SRCLK_pulse();
        i++;
    }
    RCLK_pulse();
    while (1);
}

void SRCLK_pulse (void) {
    P1OUT |= SRCLK;
    P1OUT ^= SRCLK;
}

void RCLK_pulse (void) {
    P1OUT |= RCLK;
    P1OUT ^= RCLK;
}

void Send_Bit (bool x) {
    if (x != 0) {
        P1OUT |= SERIAL;
    }
    else {
        P1OUT &= ~SERIAL;
    }
}