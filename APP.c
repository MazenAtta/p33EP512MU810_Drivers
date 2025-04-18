/*
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */

#include <string.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include "MCAL_Layer//TIMER/HAL_TIMER.h"

void algorithm();
void uart1_init();
void uart_send_char(char c);
void uart_send_string(const char* str);
int uart_data_available();
char uart_read_char();
void process_uart_commands();
void parse_command(const char* cmd);
void send_magnetometer_data();
void send_yaw_data(float angle);

void spi_init();
unsigned int spi_write(unsigned int data);
void acquire_magnetometer_data();
void compute_average_magnetometer();
float compute_yaw_angle();

#define UART_RX_BUFFER_SIZE 128
#define MAGNETOMETER_SAMPLES 5

char command_buffer[32];
int cmd_index = 0;
volatile char uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile unsigned int uart_rx_head = 0;
volatile unsigned int uart_rx_tail = 0;
volatile int tmr1_counter = 0;
volatile int freq = 5; // Initialize system with 5 Hz

int magnetometer_data[3][MAGNETOMETER_SAMPLES] = {0}; // [x, y, z] samples
int sample_index = 0;
int average_data[3] = {0}; // [x_avg, y_avg, z_avg]

int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    TRISAbits.TRISA0 = 0; // Configure LD2 pin as output
    TRISGbits.TRISG9 = 0;

    tmr_setup_period(TIMER1, 10); // Set TIMER1 period to 10 ms
    uart1_init();
    spi_init();

    while (1) {
        // Simulate algorithm execution
        algorithm();

        // Process incoming UART commands
        process_uart_commands();

        // Wait for the next timer period
        tmr_wait_period(TIMER1);
    }

    return (EXIT_SUCCESS);
}

void algorithm() {
   tmr_wait_ms(TIMER2, 7); // Simulate 7 ms work
}

void uart1_init() {
    // Remap pins for UART1
    RPINR18bits.U1RXR = 75; // RX = RD11
    RPOR0bits.RP64R = 1;    // TX = RD0

    U1BRG = 468; // 9600 baud at Fcy = 72MHz
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    INTCON2bits.GIE = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1; // Enable UART1 RX interrupt
    IPC2bits.U1RXIP = 1; // Set priority
}

int uart_data_available() {
    return uart_rx_head != uart_rx_tail;
}

char uart_read_char() {
    if (uart_rx_head == uart_rx_tail) return 0;

    char c = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return c;
}

void uart_send_char(char c) {
    while (!U1STAbits.TRMT); // Wait until transmit buffer is empty
    U1TXREG = c;
}

void uart_send_string(const char* str) {
    while (*str) uart_send_char(*str++);
}

void parse_command(const char* cmd) {
    if (strncmp(cmd, "$RATE,", 6) == 0) {
        int val = atoi(&cmd[6]);

        // Validate rate and update frequency
        if ((val == 0 || val == 1 || val == 2 || val == 4 || val == 5 || val == 10) && cmd[strlen(cmd) - 1] == '*') {
            INTCON2bits.GIE = 0; // Disable global interrupts
            freq = val;
            INTCON2bits.GIE = 1; // Re-enable global interrupts
        } else {
            uart_send_string("$ERR,1*\r\n"); // Send error message
        }
    }
}

void process_uart_commands() {
    while (uart_data_available()) {
        char c = uart_read_char();

        if (cmd_index < sizeof(command_buffer) - 1) {
            command_buffer[cmd_index++] = c;
        }

        if (c == '*') {
            command_buffer[cmd_index] = '\0'; // Null-terminate the command
            parse_command(command_buffer);
            cmd_index = 0;
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0; // Clear interrupt flag
    char c = U1RXREG;

    // Read data into circular buffer
    unsigned int next_head = (uart_rx_head + 1) % UART_RX_BUFFER_SIZE;
    if (next_head != uart_rx_tail) {
        uart_rx_buffer[uart_rx_head] = c;
        uart_rx_head = next_head;
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; // Clear interrupt flag
    tmr1_counter++;

    if (tmr1_counter % 100 == 0) {
        LATAbits.LATA0 ^= 1; // Toggle LD2 every 1 second (100 Hz -> 1 Hz)
    }

    if (tmr1_counter % 40 == 0) {
        acquire_magnetometer_data(); // Acquire data at 25 Hz
    }

    if (tmr1_counter % ((100 / freq) == 0 ? 1 : (100 / freq)) == 0) {
        compute_average_magnetometer(); // Compute the average of the last 5 samples
        send_magnetometer_data();      // Send magnetometer data to UART
    }

    if (tmr1_counter % 20 == 0) { // 5 Hz -> 200 ms
        float angle = compute_yaw_angle();
        send_yaw_data(angle); // Send yaw angle to UART
    }
}

void acquire_magnetometer_data() {
    // Simulate SPI data acquisition
    magnetometer_data[0][sample_index] = spi_write(0x4C); // X-axis
    magnetometer_data[1][sample_index] = spi_write(0x4C); // Y-axis
    magnetometer_data[2][sample_index] = spi_write(0x4C); // Z-axis

    sample_index = (sample_index + 1) % MAGNETOMETER_SAMPLES; // Increment sample index
}

void compute_average_magnetometer() {
    for (int axis = 0; axis < 3; axis++) {
        average_data[axis] = 0;
        for (int i = 0; i < MAGNETOMETER_SAMPLES; i++) {
            average_data[axis] += magnetometer_data[axis][i];
        }
        average_data[axis] /= MAGNETOMETER_SAMPLES; // Compute average
    }
}

float compute_yaw_angle() {
    return atan2(average_data[1], average_data[0]) * (180.0 / M_PI); // Convert to degrees
}

void send_magnetometer_data() {
    char buffer[64];
    sprintf(buffer, "$MAG,%d,%d,%d*\r\n", average_data[0], average_data[1], average_data[2]);
    uart_send_string(buffer);
}

void send_yaw_data(float angle) {
    char buffer[32];
    sprintf(buffer, "$YAW,%.2f*\r\n", angle);
    uart_send_string(buffer);
}


void spi_init(){}