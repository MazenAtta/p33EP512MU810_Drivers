/* 
 * File:   HAL_GPIO.h
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:06 PM
 */

#include "HAL_GPIO.h"

/* Reference to the Data Control Registers */
volatile uint16 *tris_registers[] = {&TRISA, &TRISB, &TRISC, &TRISD, &TRISE, &TRISF, &TRISG};
/* Reference to the Data Latch Registers */
volatile uint16 *lat_registers[] = {&LATA, &LATB, &LATC, &LATD, &LATE, &LATF, &LATG};
/* Reference to the Data Port Registers */
volatile uint16 *port_registers[] = {&PORTA, &PORTB, &PORTC, &PORTD, &PORTE, &PORTF, &PORTG};


// Pin declarations functions

/**
 * @brief Initialize the direction of a specific GPIO pin.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    switch(_pin_config->direction){
        case GPIO_DIRECTION_OUTPUT:
            CLEAR_BIT(*tris_registers[_pin_config->port], _pin_config->pin);
            break;
        case GPIO_DIRECTION_INPUT:
            SET_BIT(*tris_registers[_pin_config->port], _pin_config->pin);
            break;
        default :
            ret = E_NOT_OK;
            break;
    }
    return ret;
}
#endif

/**
 * @brief Get the direction status of a specific GPIO pin.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @param _direction_status Pointer to the variable to store the direction status @ref direction_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *_direction_status)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || NULL == _direction_status || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    *_direction_status = READ_BIT(*tris_registers[_pin_config->port], _pin_config->pin); 
    
    return ret;
}
#endif

/**
 * @brief Write a logic level to a specific GPIO pin.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @param _logic Logic level to write to the pin @ref logic_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_write_logic(const pin_config_t * _pin_config, logic_t _logic)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    switch(_logic){
        case GPIO_LOW:
            CLEAR_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
            break;
        case GPIO_HIGH:
            SET_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
            break;
        default :
            ret = E_NOT_OK;
            break;
    }
    
    return ret;
}
#endif

/**
 * @brief Read the logic level from a specific GPIO pin.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @param _logic Pointer to the variable to store the logic level @ref logic_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_read_logic(const pin_config_t * _pin_config, logic_t *_logic)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || NULL == _logic || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    *_logic = READ_BIT(*port_registers[_pin_config->port], _pin_config->pin); 
    
    return ret;
}
#endif

/**
 * @brief Initialize a specific GPIO pin with the given configuration.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    ret = gpio_pin_direction_intialize(_pin_config);
    ret = gpio_pin_write_logic(_pin_config, _pin_config->logic);
    return ret;
}
#endif

/**
 * @brief Toggle the logic level of a specific GPIO pin.
 * @param _pin_config Pointer to the pin configuration structure @ref pin_index_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_PIN_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t * _pin_config)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _pin_config || _pin_config->pin > PORT_PIN_MAX_NUMBER - 1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    TOGGLE_BIT(*lat_registers[_pin_config->port], _pin_config->pin);
    return ret;
}
#endif

// Port declarations functions

/**
 * @brief Initialize the direction of a specific GPIO port.
 * @param _port Port index @ref port_index_t.
 * @param _direction Direction configuration for the port.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_port_direction_intialize(const port_index_t _port, uint8 _direction)
{
    Std_ReturnType ret = E_OK;
    if(_port > PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    *tris_registers[_port] = _direction;
    return ret;
}
#endif

/**
 * @brief Get the direction status of a specific GPIO port.
 * @param _port Port index @ref port_index_t.
 * @param _direction_status Pointer to the variable to store the direction status.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_port_get_direction_status(const port_index_t _port, uint8 *_direction_status)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _direction_status || _port > PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    *_direction_status = *tris_registers[_port];
    return ret;
}
#endif

/**
 * @brief Write a logic level to a specific GPIO port.
 * @param _port Port index @ref port_index_t.
 * @param _logic Logic level to write to the port.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_port_write_logic(const port_index_t _port, uint8 _logic)
{
    Std_ReturnType ret = E_OK;
    if(_port > PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    *lat_registers[_port] = _logic;
    return ret;
}
#endif

/**
 * @brief Read the logic level from a specific GPIO port.
 * @param _port Port index @ref port_index_t.
 * @param _logic Pointer to the variable to store the logic level.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_port_read_logic(const port_index_t _port, uint8 *_logic)
{
    Std_ReturnType ret = E_OK;
    if(NULL == _logic || _port > PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    *_logic = *port_registers[_port];
    return ret;
}
#endif

/**
 * @brief Toggle the logic level of a specific GPIO port.
 * @param _port Port index @ref port_index_t.
 * @return Status of the function.
 *         (E_OK) : The function completed successfully.
 *         (E_NOT_OK) : The function encountered an issue.
 */
#if GPIO_PORT_CONFIGRATION == CONFIG_ENABLE
Std_ReturnType gpio_port_toggle_logic(const port_index_t _port)
{
    Std_ReturnType ret = E_OK;
    if(_port > PORT_MAX_NUMBER-1)
    {
        ret = E_NOT_OK;
        return ret;
    }
    
    *lat_registers[_port] ^= 0xFFFF;   
    return ret;
}
#endif