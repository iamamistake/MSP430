#include <msp430g2553.h>
#define SERIAL BIT0
#define SRCLK BIT4
#define RCLK BIT5

void SRCLK_pulse (void);
void RCLK_pulse (void);
 
void main(void) {
 
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
  P1DIR |= SERIAL + SRCLK + RCLK;
  P1OUT = 0x00U;
  while (1) {
    P1OUT |= SERIAL;
    SRCLK_pulse();
    P1OUT ^= SERIAL;
    for (i=8 ; i>0; i--) {
      RCLK_pulse();
      SRCLK_pulse();
      __delay_cycles(100000);
    }
  }
}

void SRCLK_pulse (void) {
  P1OUT |= SRCLK;
  P1OUT ^= SRCLK;
}

void RCLK_pulse (void) {
  P1OUT |= RCLK;
  P1OUT ^=RCLK;
}
