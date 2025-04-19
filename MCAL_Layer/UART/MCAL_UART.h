/* 
 * File:   MCAL_UART.h
 * Author: Mazen Atta
 *
 * Created on April 19, 2025, 1:19 PM
 */

#ifndef MCAL_UART_H
#define	MCAL_UART_H

#include "../../MCAL_Layer/GPIO/HAL_GPIO.h"


#define UART_BUFFER_SIZE 64  // Define the size of the buffer


typedef struct {
    unsigned char buffer[UART_BUFFER_SIZE];
    unsigned char head;  // Points to the next position to write
    unsigned char tail;  // Points to the next position to read
    volatile unsigned char count;  // Tracks how many characters are in the buffer
} CircularBuffer;

void uart1_init();
void buffer_init(CircularBuffer *buf);
void uart_send_char(char data);
void uart_send_string(const char *str);

int buffer_write(CircularBuffer *buf, char data);
int buffer_read(CircularBuffer *buf, char *data);
int buffer_is_empty(CircularBuffer *buf);
int buffer_is_full(CircularBuffer *buf);
void uart_send_buffer(CircularBuffer *buf);

#endif	/* MCAL_UART_H */

