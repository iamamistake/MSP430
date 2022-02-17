#include <msp430.h>

// LIS3DH definitions (W25Q80DVSNIG)
#define STATUS_REG_AUX 0x07
#define OUT_ADC1_L 0x08
#define OUT_ADC1_H 0x09
#define OUT_ADC2_L 0x0A
#define OUT_ADC2_H 0x0B
#define OUT_ADC3_L 0x0C
#define OUT_ADC3_H 0x0D
#define WHO_AM_I 0x0F
#define CTRL_REG0 0x1E
#define TEMP_CFG_REG 0x1F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define CTRL_REG6 0x25
#define REFERENCE 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_THS 0x32
#define INT1_DURATION 0x33
#define INT2_CFG 0x34
#define INT2_SRC 0x35
#define INT2_THS 0x36
#define INT2_DURATION 0x37
#define CLICK_CFG 0x38
#define CLICK_SRC 0x39
#define CLICK_THS 0x3A
#define TIME_LIMIT 0x3B
#define TIME_LATENCY 0x3C
#define TIME_WINDOW 0x3D
#define ACT_THIS 0x3E
#define ACT_DUR 0x3F
#define READ BIT7

// Pin definitions
#define SCLK BIT6
#define SDA BIT7

char received_data;

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting the clock rate to 1MHz

    P1SEL = SCLK | SDA;
    P1SEL2 = SCLK | SDA;
    UCB0CTL1 |= UCSWRST + UCSSEL_2;
    UCB0CTL0 |= UCMST + UCMODE_3 + UCSYNC;
    UCB0BR0 = 10;
    UCB0I2CSA =  0x18; // Slave address (without R/W bit)
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

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR (void) {

    if (IFG2 & UCB0RXIFG) {
        received_data = UCB0RXBUF;
//        IFG2 &= ~ UCB0RXIFG;
//        IFG2 &= ~ UCB0TXIFG;
    }

    else {
        UCB0TXBUF = WHO_AM_I;
//        IFG2 &= ~ UCB0TXIFG;
    }
    IFG2 &= ~ UCB0TXIFG;
    IFG2 &= ~ UCB0RXIFG;
}
