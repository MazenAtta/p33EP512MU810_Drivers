/* 
 * File:   ECU_LED.h
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:19 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/* Section : includes */
#include "../../MCAL_Layer/GPIO/HAL_GPIO.h"
#include "ECU_LED_CFG.h"


/* Section : Macro Declarations */
typedef enum{
    LED_OFF = 0,
    LED_ON
}led_status;

typedef struct{
    uint16 port_name: 3;
    uint16 pin: 4;
    uint16 led_status: 1;
    uint16 :8;

    
}led_t;

/* Section : Macro Function Declarations */
Std_ReturnType led_intialize(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_toggle(const led_t *led);


/* Section : Datatype Declarations */

/* Section Function Declarations */

#endif	/* ECU_LED_H */

