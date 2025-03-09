/* 
 * File:   ECU_LED.c
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:19 PM
 */
#include "ECU_LED.h"

/**
 * 
 * @param led_t
 * @return 
 */
Std_ReturnType led_intialize(const led_t *led){

    Std_ReturnType ret = E_OK;
    if(NULL == led)
    {
        ret = E_NOT_OK;
        return ret;
    }
    pin_config_t led_cfg = {
    .port = led->port_name,
    .pin =  led->pin,
    .direction = GPIO_DIRECTION_OUTPUT,
    .logic = led->led_status
    };
    ret = gpio_pin_intialize(&led_cfg);
    return ret;
}


/**
 * 
 * @param led_t
 * @return 
 */
Std_ReturnType led_turn_on(const led_t *led)
{
    Std_ReturnType ret = E_OK;
    if(NULL == led)
    {
        ret = E_NOT_OK;
        return ret;
    }
    pin_config_t led_cfg = {
    .port = led->port_name,
    .pin =  led->pin,
    .direction = GPIO_DIRECTION_OUTPUT,
    .logic = GPIO_HIGH
    };
    ret = gpio_pin_write_logic(&led_cfg, led_cfg.logic);

    return ret;
}


/**
 * 
 * @param led_t
 * @return 
 */
Std_ReturnType led_turn_off(const led_t *led)
{
        Std_ReturnType ret = E_OK;
    if(NULL == led)
    {
        ret = E_NOT_OK;
        return ret;
    }
    pin_config_t led_cfg = {
    .port = led->port_name,
    .pin =  led->pin,
    .direction = GPIO_DIRECTION_OUTPUT,
    .logic = GPIO_LOW
    };
    ret = gpio_pin_write_logic(&led_cfg, led_cfg.logic);

    return ret;
}


/**
 * 
 * @param led_t
 * @return 
 */
Std_ReturnType led_toggle(const led_t *led)
{
        Std_ReturnType ret = E_OK;
    if(NULL == led)
    {
        ret = E_NOT_OK;
        return ret;
    }
    pin_config_t led_cfg = {
    .port = led->port_name,
    .pin =  led->pin,
    .direction = GPIO_DIRECTION_OUTPUT,
    .logic = led->led_status
    };
    ret = gpio_pin_toggle_logic(&led_cfg);

    return ret;
}