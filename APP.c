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

pin_config_t led1 = {
.port = PORTA_INDEX,
.pin =  GPIO_PIN0,
.direction = GPIO_DIRECTION_OUTPUT,
.logic = GPIO_LOW
};

int main() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    //TRISEbits.TRISE8 = 1; //Button pin as input
    TRISGbits.TRISG9 = 0;
    LATGbits.LATG9 = 0;
    
    Std_ReturnType ret = E_NOT_OK;
    ret = gpio_pin_direction_intialize(&led1);
    ret = gpio_pin_write_logic(&led1, GPIO_HIGH);
    LATGbits.LATG9 = 1;
    
    while(1)
    {}

    
    /*while(1)
    {
        if(PORTEbits.RE8 == 0) //button pressed due to internal pull up
        {
            LATAbits.LATA0 = 1;
        }
        else if (PORTEbits.RE8 == 1){
            LATAbits.LATA0 = 0;
        }
    }*/
    /*uint32 button_pressed = 0;
    while(1){
      if(!PORTEbits.RE8 && !button_pressed)
        {
            //while(PORTEbits.RE8);
            button_pressed = 1;
            LATAbits.LATA0 = ~PORTAbits.RA0;
        }
      if (PORTEbits.RE8){
      button_pressed = 0;}
    }*/
    return (EXIT_SUCCESS);
}

