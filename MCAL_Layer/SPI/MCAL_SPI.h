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
unsigned char spi_read_register(unsigned char reg);
void bmx055_read_xyz(int16_t* x, int16_t* y, int16_t* z);


#endif	/* MCAL_SPI_H */

