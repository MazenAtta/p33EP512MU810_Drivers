#include "MCAL_UART.h"

void uart1_init() {
    // Remap pins for UART1
    RPINR18bits.U1RXR = 75; // RX = RD11
    RPOR0bits.RP64R = 1;    // TX = RD0

    U1BRG = 468; // 9600 baud at Fcy = 72MHz
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    //Enable UART RX interrupt
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF =0;
    IEC0bits.U1RXIE = 1;        // Enable UART RX
    IEC0bits.U1TXIE=1;
}


void uart_send_char(char data) {
    while (U1STAbits.UTXBF); // Wait until the transmit buffer is not full
    U1TXREG = data;          // Transmit the character
}

void uart_send_string(const char *str) {
    while (*str) { // Loop until the null character is encountered
        uart_send_char(*str); // Send the current character
        str++; // Move to the next character
    }
}
// Initialize the buffer
void buffer_init(CircularBuffer *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
}

// Add a character to the buffer
int buffer_write(CircularBuffer *buf, char data) {
    if (buf->count == UART_BUFFER_SIZE) {
        // Buffer is full
        return 0;
    }

    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) % UART_BUFFER_SIZE;
    buf->count++;
    return 1;
}

// Read a character from the buffer
int buffer_read(CircularBuffer *buf, char *data) {
    if (buf->count == 0) {
        // Buffer is empty
        return 0;
    }

    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) % UART_BUFFER_SIZE;
    buf->count--;
    return 1;
}

// Check if the buffer is empty
int buffer_is_empty(CircularBuffer *buf) {
    return (buf->count == 0);
}

// Check if the buffer is full
int buffer_is_full(CircularBuffer *buf) {
    return (buf->count == UART_BUFFER_SIZE);
}

void uart_send_buffer(CircularBuffer *buf) {
    char data;
    while (buffer_read(buf, &data)) {
        uart_send_char(data); // Send each character in the buffer
    }
}

void send_mag_data(int16_t x, int16_t y, int16_t z) {
    char uart_msg[64];
    sprintf(uart_msg, "$MAG,%d,%d,%d*\n", x, y, z);
    uart_send_string(uart_msg);
}
