/* 
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */



#include "APP.h"


CircularBuffer uart_rx_buffer;  // Create an instance of the buffer


void algorithm();

volatile int tmr1_counter = 0, tmr2_counter = 0;
volatile int freq = 5;


int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    tmr_setup_period(TIMER1, 10);
    tmr_setup_period(TIMER2,50);
    uart1_init(&uart_rx_buffer);
    while (1) {
        // Main loop does nothing; could add more debug here
        algorithm(); //// simulate 7ms work
        
        
        tmr_wait_period(TIMER1);
    }

    return (EXIT_SUCCESS);
}


void algorithm()
{
   __delay_ms(7);
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;  // Clear interrupt flag
    tmr1_counter++;
    
    if(tmr1_counter == 4)
    {
        //acquire data from SPI 
    }
    if(tmr1_counter == 50)
    {
        //blink led
    }
    
    //Do the average of the last 5 measurements and compute the angle using atan2(y_avg, x_avg)
    
    //send the avg data to UART
    if (tmr1_counter ==  (1/freq*1000)/10)
    {
       //send the avg x,y,z to the UART
    }
    //send the angle every 5Hz (200ms)
    if (tmr1_counter ==  20)
    {
        //send angle to UART
    }
}


void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;  // Clear interrupt flag
    unsigned char received_char = U1RXREG;

    // read data from UART and parse data
    
    // Write the received character to the circular buffer
    if(received_char == 'm')
    {
        uart_send_buffer(&uart_rx_buffer);
    }
    else{
    if (!buffer_write(&uart_rx_buffer, received_char)) {
        // Buffer overflow: handle as needed (e.g., log an error or discard data)
    }}
    
    //if data valid (1,2,4,5,10) update freq
    
    //if data non valid send $ERR,1 to UART
}

