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
.port = PORTG_INDEX,
.pin =  GPIO_PIN9,
.direction = GPIO_DIRECTION_OUTPUT,
.logic = GPIO_LOW
};

led_t led2 = {
.port_name = PORTA_INDEX,
.pin = GPIO_PIN0,
.led_status = GPIO_LOW
};

button_t but1 = {
.button_pin.port = PORTE_INDEX,
.button_pin.pin = GPIO_PIN8,
.button_pin.direction = GPIO_DIRECTION_INPUT,
.button_pin.logic = GPIO_LOW,
.button_connection = BUTTON_ACTIVE_LOW,
.button_state = BUTTON_RELEASED
};

int main() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    button_state_t btn_status = BUTTON_RELEASED;
    led_intialize(&led2);
    //led_turn_on(&led2);        
    button_intialize(&but1);
    
    
    
    while(1)
    {
        button_read_state(&but1, &btn_status);
        
        if (btn_status == BUTTON_PRESSED){
            __delay_ms(150);
            led_toggle(&led2); 
        }               
    }
    return (EXIT_SUCCESS);
}

