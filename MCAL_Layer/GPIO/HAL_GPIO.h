/* 
 * File:   HAL_GPIO.h
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:06 PM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/* Section : includes */
#include "p33EP512MU810.h"
#include "../MCAL_STD_TYPES.h"
#include "../DEVICE_CONFIG.h"
#include "HAL_GPIO_CFG.h"
/* Section : Macro Declarations */
#define BIT_MASK    (uint16)1

#define PORT_PIN_MAX_NUMBER 16
#define PORT_MAX_NUMBER 7

#define GPIO_PORT_PIN_CONFIGRATION CONFIG_ENABLE
#define GPIO_PORT_CONFIGRATION CONFIG_ENABLE


/* Section : Macro Function Declarations */
/*------------- Bit Math -------------*/
#define HWREG16(_x)                                      (*((volatile uint16 *)(_X)))
#define WRITE_BITS(Reg,Start_Bit,Number_Bits,Val)        ((Reg)=((Reg)&~((((uint16)1<<(Number_Bits))-(uint16)1)<<(Start_Bit)))|((uint16)(Val)<<(Start_Bit)))         /* Write Bits */
#define SET_BIT(Reg,Bit_Num)                             (Reg|=BIT_MASK<<Bit_Num)			                                                                      /* Set Bit */
#define CLEAR_BIT(Reg,Bit_Num)                           (Reg&=~(BIT_MASK<<Bit_Num))		                                                                      /* Clear Bit */
#define TOGGLE_BIT(Reg,Bit_Num)                          (Reg^=(BIT_MASK<<Bit_Num))		                                                                      /* Toggle Bit */
#define READ_BIT(Reg,Bit_Num)                            ((Reg>>Bit_Num)&BIT_MASK)		                                                                           /* Get Bit */
#define READ_BITS(Reg,Start_Bit,Number_Bits)             ((Reg>>Start_Bit)&(~((~0UL)<<Number_Bits)))    


/* Section : Datatype Declarations */
typedef enum{
    GPIO_LOW = 0,
    GPIO_HIGH
}logic_t;

typedef enum{
    GPIO_DIRECTION_OUTPUT = 0,
    GPIO_DIRECTION_INPUT
}direction_t;

typedef enum{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN014,
    GPIO_PIN015
}pin_index_t;

typedef enum{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX,
    PORTF_INDEX,
    PORTG_INDEX
}port_index_t;


typedef struct{
    uint16 port : 3;        /* @ref port_index_t */
    uint16 pin : 4;         /* @ref pin_index_t */
    uint16 direction : 1;   /* @ref direction_t */
    uint16 logic : 1;       /* @ref logic_t */
    uint16 :7;
}pin_config_t;


/* Section : Function Declarations */
// Pin declarations functions
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t * _pin_config);
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t * _pin_config, direction_t *_direction_status);
Std_ReturnType gpio_pin_write_logic(const pin_config_t * _pin_config, logic_t _logic);
Std_ReturnType gpio_pin_read_logic(const pin_config_t * _pin_config, logic_t *_logic);
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t * _pin_config);
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config);

// Port declarations functions
Std_ReturnType gpio_port_direction_intialize(const port_index_t _port, uint8 _direction);
Std_ReturnType gpio_port_get_direction_status(const port_index_t _port, uint8 *_direction_status);
Std_ReturnType gpio_port_write_logic(const port_index_t _port, uint8 logic);
Std_ReturnType gpio_port_read_logic(const port_index_t _port, uint8 *logic);
Std_ReturnType gpio_port_toggle_logic(const port_index_t _port);



#endif	/* HAL_GPIO_H */

