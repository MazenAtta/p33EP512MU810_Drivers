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
    signed int val = 0, result = 0, mag_x[5] , mag_y[5], mag_z[5], avg_x = 0, avg_y =0 ,avg_z = 0;
    
    TRISGbits.TRISG9 = 0;
    
    INTCON2bits.GIE = 1;

    
    uart1_init();
    buffer_init(&uart_rx_buffer);
    
    spi_init();

    tmr_setup_period(TIMER1, 10);

    while (1) {

        algorithm(); //// simulate 7ms work
        
        //read the spi data every 40 ms
        if((tmr1_counter % 4) == 0)
        {
            if(index == 5){index = 0;}
            //acquire data from SPI
            bmx055_read_xyz(&mag_x[index], &mag_y[index], &mag_z[index]);
            index++;
        }

        //Blink the led every 500ms
        if((tmr1_counter % 50) == 0)
        {
            //blink led
            LATGbits.LATG9 ^= 1;
        }
        
        //send the avg Mag
        if (freq > 0 && (tmr1_counter %  (100/freq) == 0))
        {
            //Do the average of the last 5 measurements
            avg_x = (mag_x[0] + mag_x[1] + mag_x[2] +mag_x[3] +mag_x[4])/5;
            avg_y = (mag_y[0] + mag_y[1] + mag_y[2] +mag_y[3] +mag_y[4])/5;
            avg_z = (mag_z[0] + mag_z[1] + mag_z[2] +mag_z[3] +mag_z[4])/5;
            
            //Queue the avg Mag data "$MAG,x,y,z*" to be sent to the UART 

        }

        //send the angle every 5Hz (200ms)
        if ((tmr1_counter % 20) == 0)
        {
            //Compute the angle using atan2(y_avg, x_avg)
           
            //Queue the angle data "$YAW,x*" to be sent to the UART
        
        }
        
        //Parse Received UART message if any
        while(!buffer_is_empty(&uart_rx_buffer))
        {
            buffer_read(&uart_rx_buffer, &byte);
            result = parse_byte(&pstate, byte);
        }
        if ( result == NEW_MESSAGE && strcmp(pstate.msg_type, "RATE") == 0)
            {
                val =  extract_integer(pstate.msg_payload);

                //if data valid (0,1,2,4,5,10) update freq
                if ((val == 0 || val == 1 || val == 2 || val == 4 || val == 5 || val == 10))
                {
                    freq = val;
                }
                //if data non valid send $ERR,1 to 
                else
                {
                   //Queue the "$ERR,1*" message to be sent to the UART

                    result = NO_MESSAGE;
                }
            }
        
        tmr_wait_period(TIMER1);
        
        tmr1_counter++;

    }

    return (EXIT_SUCCESS);
}


void algorithm()
{
    tmr_wait_ms(TIMER2, 7);
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


void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;  // Clear interrupt flag    
}




