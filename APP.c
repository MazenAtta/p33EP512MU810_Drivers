/* 
 * File:   APP.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:33 PM
 */


#include "APP.h"

/*
 * 
 */
volatile int counter = 0;  // updated in ISR
void blink_LD2();


int main() {
    // Disable all analog pins
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    RPINR0bits.INT1R = 88;
    INTCON2bits.GIE = 1;
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
    IEC0bits.T1IE = 0;
    IEC0bits.T2IE = 0;

    //tmr1_set_callback(blink_LD2);
    
    tmr_setup_period(TIMER2, 200);
    tmr_setup_period(TIMER1,100);
    while(1)
    {
        LATAbits.LATA0 ^= 1;
        tmr_wait_period(TIMER2);
        LATAbits.LATA0 ^= 1;
        tmr_wait_period(TIMER2);        
    }
    
    return (EXIT_SUCCESS);
}



void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(void)
{
        IFS1bits.INT1IF = 0; // Clear interrupt flag
        LATGbits.LATG9 ^= 1;  // Toggle LD2   
}


void blink_LD2()
{
    static int counter = 0;

    counter++;
    if (counter >= 5) {
        LATGbits.LATG9 ^= 1;  // Toggle LD2
        counter = 0;
    }
}