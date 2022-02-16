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

// Pin definitions
#define BUTTON BIT3
#define CS BIT4
#define SCLK BIT5
#define MISO BIT6
#define MOSI BIT7


char receiveByte (void);
void sendByte (char byte);
char readSR (char readSRx);
void writeSR (uint16_t SR_data);
char * readData (uint32_t addr, int size);
void writeData (uint32_t addr, char *data);

void main (void) {
    WDTCTL = WDTPW | WDTHOLD; // disabling the watchdog timer

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ; // setting the clock rate to 1MHz

    P1IE |= BUTTON;
    P1IES |= BUTTON;
    P1IFG &= ~BUTTON;
    _enable_interrupts();

    P1DIR |= CS;
    P1OUT |= CS;
    __delay_cycles(40);
    P1SEL = SCLK | MISO | MOSI;
    P1SEL2 = SCLK | MISO | MOSI;
    UCB0CTL1 |= UCSWRST + UCSSEL_2;
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
    UCB0BR0 = 10;
    UCB0CTL1 &= ~UCSWRST;

//    IE2 |= UCB0RXIE;
//    IE2 |= UCB0TXIE;


    // HEHE
//    sendByte (ChipErase); // Erasing the flash
//    __delay_cycles(6000000);
//    char *value = "U";
//    writeData(0x000000, value);
//    char dataToSend [17] = "existence is pain";
//    writeData(0x000000, dataToSend);
    char *store = readData (0x000000, 1);

//    while (1) {
//        char *store = readData (0x000001, 1);
//        __delay_cycles(1000000);
//    }
      while (1);
}

char receiveByte (void) {
    sendByte(DummyByte);
    while (!(IFG2 & UCB0RXIFG));
    return UCB0RXBUF;
}

void sendByte (char byte) {
    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = byte;
}

char readSR (char readSRx) {
    char RSR = 0;
    P1OUT &= ~CS;
    __delay_cycles(40);
    sendByte(readSRx);
    RSR = receiveByte();
    P1OUT |= CS;
    __delay_cycles(40);
    return RSR;
}

void writeSR (uint16_t SR_data) {
    P1OUT &= ~CS;
    __delay_cycles(40);
    sendByte(WriteEnable);
    sendByte(WriteSR);
    sendByte((SR_data >> 8) & (0xFF));
    sendByte(SR_data & 0xFF);
    sendByte(WriteDisable);
    P1OUT |= CS;
    __delay_cycles(40);
}

char *readData (uint32_t addr, int size) {
    char *data;
    int i;
    P1OUT &= ~CS;
    __delay_cycles(40);
    sendByte(ReadData);
    sendByte((addr >> 16) & (0xFF));
    sendByte((addr >> 8) & (0xFF));
    sendByte(addr & 0xFF);
    for (i=0; i < size; i++) {
        *data = receiveByte();
        data++;
    }
    P1OUT |= CS;
    __delay_cycles(40);
    return data;
}

void writeData (uint32_t addr, char *data) {
    P1OUT &= ~CS;
    __delay_cycles(40);
    sendByte(WriteEnable);
    sendByte(PageProgram);
    sendByte((addr >> 16) & (0xFF));
    sendByte((addr >> 8) & (0xFF));
    sendByte(addr & 0xFF);
    while (*data != 0) {
        sendByte(*data++);
        __delay_cycles(30000);
    }
    sendByte(WriteDisable);
    P1OUT |= CS;
    __delay_cycles(40);
}
//
//#pragma vector=PORT1_VECTOR
//__interrupt void PORT1_ISR (void) {
////    char dataToSend [17] = "existence is pain";
////    writeData(0x000000, dataToSend);
////    char *store = readData (0x000000, 1);
//    P1IFG &= ~BUTTON;
//}