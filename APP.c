/* 
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */


#include "APP.h"



int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    RPINR18bits.U1RXR = 75;
    RPOR0bits.RP64R =1;
    
    char received_ch;
    
    U1BRG = 468; // (72000000) / (16 ? 9600)? 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)

    
    while(1){
        if(U1STAbits.URXDA)
        {
            received_ch = U1RXREG;
            while(!U1STAbits.TRMT);
                U1TXREG = received_ch;
        }
    }
    
    return (EXIT_SUCCESS);
}
