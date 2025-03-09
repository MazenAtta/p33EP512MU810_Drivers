/* 
 * File:   ECU_BUTTON.h
 * Author: Mazen.Atta
 *
 * Created on March 9, 2025, 1:39 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/* Section : includes */
#include "../../MCAL_Layer/GPIO/HAL_GPIO.h"
#include "ECU_BUTTON_CFG.h"


/* Section : Macro Declarations */
typedef enum{
BUTTON_PRESSED,
BUTTON_RELEASED
}button_state_t;

typedef enum{
BUTTON_ACTIVE_HIGH,
BUTTON_ACTIVE_LOW
}button_active_t;

typedef struct{
    
pin_config_t button_pin;
button_state_t button_state;
button_active_t button_connection;

}button_t;


/* Section : Macro Function Declarations */
Std_ReturnType button_intialize(const button_t *btn);
Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state);


/* Section : Datatype Declarations */

/* Section Function Declarations */

#endif	/* ECU_BUTTON_H */

