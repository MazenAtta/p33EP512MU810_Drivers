/* 
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */

#include "APP.h"

void algorithm();

CircularBuffer uart_rx_buffer;  // Create an instance of the buffer
CircularBuffer uart_tx_buffer;  // Create an instance of the buffer

int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // parser initialization
    parser_state pstate;
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;

    char byte;
    unsigned int freq = 5, index = 0, tmr1_counter = 0;
    signed int val = 0, result = 0, mag_x[5], mag_y[5], mag_z[5];
    double avg_x = 0, avg_y = 0, avg_z = 0, angle;
    
    TRISGbits.TRISG9 = 0;
    
    INTCON2bits.GIE = 1;

    uart1_init();
    buffer_init(&uart_rx_buffer);
    buffer_init(&uart_tx_buffer);
    
    spi_init();

    tmr_setup_period(TIMER1, 10);

    while (1) {
        algorithm(); //// simulate 7ms work
        
        // Read the SPI data every 40 ms
        if ((tmr1_counter % 4) == 0) {
            if (index == 5) { index = 0; }
            // Acquire data from SPI
            bmx055_read_xyz(&mag_x[index], &mag_y[index], &mag_z[index]);
            index++;
        }

        // Blink the LED every 500ms
        if ((tmr1_counter % 50) == 0) {
            LATGbits.LATG9 ^= 1;  // Toggle LED
        }
        
        // Send the average magnetometer data "$MAG,x,y,z*"
        if (freq > 0 && (tmr1_counter % (100 / freq)) == 0) {
            // Compute the average of the last 5 measurements
            avg_x = (mag_x[0] + mag_x[1] + mag_x[2] + mag_x[3] + mag_x[4]) / 5;
            avg_y = (mag_y[0] + mag_y[1] + mag_y[2] + mag_y[3] + mag_y[4]) / 5;
            avg_z = (mag_z[0] + mag_z[1] + mag_z[2] + mag_z[3] + mag_z[4]) / 5;
            
            // Queue the average magnetometer data "$MAG,x,y,z*" to be sent to the UART
            char mag_message[50];
            sprintf(mag_message, "$MAG,%.2f,%.2f,%.2f*", avg_x, avg_y, avg_z);
            for (int i = 0; mag_message[i] != '\0'; i++) {
                buffer_write(&uart_tx_buffer, mag_message[i]);
            }
        }

        // Send the angle data "$YAW,x*" every 200ms (5Hz)
        if ((tmr1_counter % 20) == 0) {
            // Compute the angle using atan2(y_avg, x_avg)
            angle = atan2((float)avg_y, (float)avg_x) * (180.0 / 3.14159);  // Convert radians to degrees
            
            // Queue the angle data "$YAW,x*" to be sent to the UART
            char yaw_message[30];
            sprintf(yaw_message, "$YAW,%.2f*", angle);
            for (int i = 0; yaw_message[i] != '\0'; i++) {
                buffer_write(&uart_tx_buffer, yaw_message[i]);
            }
        }
        
        // Parse received UART message if any
        while (!buffer_is_empty(&uart_rx_buffer)) {
            buffer_read(&uart_rx_buffer, &byte);
            result = parse_byte(&pstate, byte);
        }
        if (result == NEW_MESSAGE && strcmp(pstate.msg_type, "RATE") == 0) {
            val = extract_integer(pstate.msg_payload);

            // If data is valid (0, 1, 2, 4, 5, 10), update freq
            if (val == 0 || val == 1 || val == 2 || val == 4 || val == 5 || val == 10) {
                freq = val;
            }
            // If data is not valid, queue the "$ERR,1*" message to be sent to the UART
            else {
                char err_message[] = "$ERR,1*";
                for (int i = 0; err_message[i] != '\0'; i++) {
                    buffer_write(&uart_tx_buffer, err_message[i]);
                }
                result = NO_MESSAGE;
            }
        }
        // If TX interrupt was disabled, enable it and trigger first character
        if (!buffer_is_empty(&uart_tx_buffer)) {
            IEC0bits.U1TXIE = 1;
            char byte;
            buffer_read(&uart_tx_buffer, &byte);
            U1TXREG = byte;
        }
        tmr_wait_period(TIMER1);
        tmr1_counter++;
    }

    return (EXIT_SUCCESS);
}

void algorithm() {
    tmr_wait_ms(TIMER2, 7);
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;  // Clear interrupt flag
    unsigned char received_char = U1RXREG;

    // Write the received character to the circular buffer
    if (!buffer_write(&uart_rx_buffer, received_char)) {
        // Handle buffer overflow
        
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;  // Clear interrupt flag
   
    // Send the next character in the transmit buffer, if available
    char byte;
    if (!buffer_is_empty(&uart_tx_buffer)) {

        buffer_read(&uart_tx_buffer, &byte);
        U1TXREG = byte;
    }
    else {
        // No more data - disable TX interrupt until we have more data
        IEC0bits.U1TXIE = 0;
    }
}