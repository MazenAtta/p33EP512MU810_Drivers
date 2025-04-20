#include "MCAL_SPI.h"


void spi_init()
{
    TRISAbits.TRISA1 = 1;  // MISO (RPI17)
    TRISFbits.TRISF12 = 0; // SCK  (RP108)
    TRISFbits.TRISF13 = 0; // MOSI (RP109)
    
    TRISDbits.TRISD6 = 0;  // CS3 for magnetometer
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB3 = 0;

    // === Default CS States ===
    LATDbits.LATD6 = 1;    // CS3 HIGH (not selected)
    LATBbits.LATB3 = 1;    // CS2 HIGH (not used)
    LATBbits.LATB4 = 1;   // CS1 HIGH (not used)

    // === Remap SPI1 Pins ===
    RPINR20bits.SDI1R = 0b0010001;  // SDI1 (MISO) <- RPI17 (RA1)
    RPOR12bits.RP109R = 0b000101;   // SDO1 (MOSI) -> RP109 (RF13)
    RPOR11bits.RP108R = 0b000110;   // SCK1 -> RP108 (RF12)

    // === Configure SPI1 ===
    SPI1STATbits.SPIEN = 0;         // Disable SPI before config
    SPI1CON1bits.MSTEN = 1;         // Master mode
    SPI1CON1bits.MODE16 = 0;        // 8-bit mode
    SPI1CON1bits.CKP = 1;           // Clock idle low (CPOL = 0)
    SPI1CON1bits.CKE = 0;           // Data changes on transition from active to idle (CPHA = 0)
    SPI1CON1bits.SPRE = 0b100;      // Secondary prescaler 1:1
    SPI1CON1bits.PPRE = 0b01;       // Primary prescaler 1:1
    SPI1STATbits.SPIEN = 1;         // Enable SPI

    // Put magnetometer to sleep
    LATDbits.LATD6 = 0;
    __delay_ms(3);                 // Ensure timing requirements for CS
    spi_write(0x4B);  // Power control register
    spi_write(0x00);  // Sleep mode
    LATDbits.LATD6 = 1;

    // Wake up magnetometer
    LATDbits.LATD6 = 0;
    __delay_ms(3);                 // Ensure timing requirements for CS
    spi_write(0x4B);
    spi_write(0x01);  // Active mode
    LATDbits.LATD6 = 1;
}

unsigned int spi_write(unsigned int data) {
    while (SPI1STATbits.SPITBF == 1);  // Wait until transmit buffer is empty
    SPI1BUF = data;                    // Send data
    while (SPI1STATbits.SPIRBF == 0); // Wait for receive buffer full
    return SPI1BUF;                    // Return received data
}