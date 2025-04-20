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
    SPI1CON1bits.CKP = 0;           // Clock idle low (CPOL = 0)
    SPI1CON1bits.CKE = 1;           // Data changes on transition from active to idle (CPHA = 0)
    SPI1CON1bits.SPRE = 0b110; // Secondary prescaler = 2:1
    SPI1CON1bits.PPRE = 0b10;  // Primary prescaler = 16:1
    SPI1STATbits.SPIEN = 1;         // Enable SPI

    // Step 1: Sleep mode
    LATDbits.LATD6 = 0;
    __delay_us(3);
    spi_write(0x4B & 0x7F);
    spi_write(0x01);  // Sleep
    LATDbits.LATD6 = 1;
    __delay_ms(3);

    // Step 3: Set Output Data Rate to 25Hz
    LATDbits.LATD6 = 0;
    __delay_us(3);
    spi_write(0x4C & 0x7F); // ODR register
    spi_write(0x00);        // 25Hz
    LATDbits.LATD6 = 1;
    __delay_ms(3);


}

unsigned int spi_write(unsigned int data) {
    while (SPI1STATbits.SPITBF == 1);  // Wait until transmit buffer is empty
    SPI1BUF = data;                    // Send data
    while (SPI1STATbits.SPIRBF == 0); // Wait for receive buffer full
    return SPI1BUF;                    // Return received data
}


unsigned char spi_read_register(unsigned char reg) {
    unsigned char val;

    LATDbits.LATD6 = 0;  // CS LOW
    //__delay_us(1);
    spi_write(reg | 0x80); // Set MSB for READ
    val = spi_write(0x00); // Dummy write to receive data
    LATDbits.LATD6 = 1;  // CS HIGH
    return val;
}

void bmx055_read_xyz(int16_t* x, int16_t* y, int16_t* z) {
    unsigned char x_lsb = spi_read_register(0x42);
    unsigned char x_msb = spi_read_register(0x43);
    *x = ((int16_t)(x_msb << 8) | x_lsb);

    unsigned char y_lsb = spi_read_register(0x44);
    unsigned char y_msb = spi_read_register(0x45);
    *y = ((int16_t)(y_msb << 8) | y_lsb);

    unsigned char z_lsb = spi_read_register(0x46);
    unsigned char z_msb = spi_read_register(0x47);
    *z = ((int16_t)(z_msb << 8) | z_lsb);
}
