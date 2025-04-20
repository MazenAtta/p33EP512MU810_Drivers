/* 
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */


#include "string.h"
#include "APP.h"


CircularBuffer uart_rx_buffer;  // Create an instance of the buffer


void algorithm();

volatile int tmr1_counter = 0, tmr2_counter = 0;
volatile int freq = 5;


int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // parser initialization
    parser_state pstate;
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;
    
    char byte;
    TRISGbits.TRISG9 = 0;
    INTCON2bits.GIE = 1;
    
    IFS0bits.T1IF = 0;  // Clear interrupt flag for Timer1
    IEC0bits.T1IE = 1;    // Enable Timer1 interrupt

    
    tmr_setup_period(TIMER1, 10);
    tmr_setup_period(TIMER2,7);
    
    uart1_init();
    
    spi_init();
    buffer_init(&uart_rx_buffer);
    
    while (1) {
        // Main loop does nothing; could add more debug here
        algorithm(); //// simulate 7ms work
        
        //disable interrupt for critical region
        INTCON2bits.GIE = 0;
        //Parse UART message
        while(!buffer_is_empty(&uart_rx_buffer))
        {
            buffer_read(&uart_rx_buffer, &byte);
            int result = parse_byte(&pstate, byte);
        
        
        if (strcmp(pstate.msg_type, "RATE") == 0 && result == NEW_MESSAGE)
        {
            int val =  extract_integer(pstate.msg_payload);
            
            //if data valid (1,2,4,5,10) update freq
            if ((val == 0 || val == 1 || val == 2 || val == 4 || val == 5 || val == 10))
            {
                freq = val;
            }
            //if data non valid send $ERR,1 to 
            else
            {
                uart_send_string("$ERR,1*");
            }
        }
        }

        //enable interrupt after critical region
        INTCON2bits.GIE = 1;

        tmr_wait_period(TIMER1);
    }

    return (EXIT_SUCCESS);
}


void algorithm()
{
    tmr_wait_period(TIMER2);
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;  // Clear interrupt flag
    unsigned char received_char = U1RXREG;

    // read data from UART and parse data
    
    // Write the received character to the circular buffer
    if (!buffer_write(&uart_rx_buffer, received_char)) {
        // Buffer overflow: handle as needed (e.g., log an error or discard data)
    }
    

}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;  // Clear interrupt flag
    tmr1_counter++;
    
    if((tmr1_counter % 4) == 0)
    {
        //acquire data from SPI 
    }
    if((tmr1_counter % 50) == 0)
    {
        //blink led
        LATGbits.LATG9 ^= 1;
    }
    
    //Do the average of the last 5 measurements and compute the angle using atan2(y_avg, x_avg)
    
    //send the avg data to UART
    if ((tmr1_counter %  (1/freq*1000)/10) == 0)
    {
       //send the avg x,y,z to the UART
    }
    //send the angle every 5Hz (200ms)
    if ((tmr1_counter % 20) == 0)
    {
        //send angle to UART
    }
}




