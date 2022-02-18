#include <msp430.h>
#include <stdint.h>

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
#define DummyByte 0x00
#define READ BIT7

// Pin definitions
#define CS BIT4
#define SCLK BIT5
#define MISO BIT6
#define MOSI BIT7

void enableCS (void);
void disableCS (void);
char receiveByte (void);
void sendByte (char byte);
// char * readData (uint32_t addr, int size);
// void writeData (uint32_t addr, char *data);

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting the clock rate to 1MHz

    _enable_interrupts();

    P1DIR |= CS;
    disableCS();
    P1SEL = SCLK | MISO | MOSI;
    P1SEL2 = SCLK | MISO | MOSI;
    UCB0CTL1 |= UCSWRST + UCSSEL_2;
    UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC; // try including UCCKPH later
    UCB0BR0 = 10;
    UCB0CTL1 &= ~UCSWRST;

    IE2 |= UCB0RXIE;
    // IE2 |= UCB0TXIE;

    // read from WHO_AM_I register
    enableCS();
    sendByte((READ + WHO_AM_I));
    sendByte(DummyByte);
    disableCS();

    // write and read from register
//     char data = 0x45;
//     enableCS();
//     sendByte(CTRL_REG0);
//     sendByte(data);
//     disableCS();
//     __delay_cycles(1000000);
//     enableCS();
//     sendByte((READ+CTRL_REG0));
//     sendByte(DummyByte);
//     disableCS();

    while (1);
}

void enableCS (void) {
    P1OUT &= ~CS;
    __delay_cycles(1);
}

void disableCS (void) {
    __delay_cycles(1);
    P1OUT |= CS;
    __delay_cycles(1);
}

void sendByte (char byte) {
    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = byte;
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR (void) {
    value = UCB0RXBUF;
}






// char *readData (uint32_t addr, int size) {
//     char *data;
//     int i;
//     enableCS();
//     sendByte(ReadData);
//     sendByte((addr >> 16) & (0xFF));
//     sendByte((addr >> 8) & (0xFF));
//     sendByte(addr & 0xFF);
//     for (i=0; i < size; i++) {
//         *data = receiveByte();
//         data++;
//     }
//     disableCS();
//     return data;
// }

// void writeData (uint32_t addr, char *data) {
//     enableCS();
//     sendByte(WriteEnable);
//     sendByte(PageProgram);
//     sendByte((addr >> 16) & (0xFF));
//     sendByte((addr >> 8) & (0xFF));
//     sendByte(addr & 0xFF);
//     while (*data != 0) {
//         sendByte(*data++);
//         __delay_cycles(30000);
//     }
//     sendByte(WriteDisable);
//     disableCS();
// }
