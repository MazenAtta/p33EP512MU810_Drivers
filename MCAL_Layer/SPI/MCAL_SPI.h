/* 
 * File:   MCAL_SPI.h
 * Author: Mazen Atta
 *
 * Created on April 19, 2025, 4:04 PM
 */

#ifndef MCAL_SPI_H
#define	MCAL_SPI_H

#include "../../MCAL_Layer/GPIO/HAL_GPIO.h"

void spi_init();
unsigned int spi_write(unsigned int data);


#endif	/* MCAL_SPI_H */

