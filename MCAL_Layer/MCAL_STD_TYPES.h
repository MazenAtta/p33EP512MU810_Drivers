/* 
 * File:   MCAL_STD_TYPES.h
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:16 PM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/* Section : includes */
#include "STD_LIBRARIES.h"
#include "COMPILER.h"


/* Section : Macro Declarations */
#define STD_HIGH      0x0001
#define STD_LOW       0x0000

#define STD_ON        0x0001
#define STD_OFF       0x0000

#define STD_ACTIVE    0x0001
#define STD_IDLE      0x0001

#define E_OK          0x0001
#define E_NOT_OK      0x0000

/* Section : Macro Function Declarations */



/* Section : Datatype Declarations */

/* unSigned Data Types */
typedef unsigned char           uint8;
typedef unsigned short int      uint16;
typedef unsigned long int       uint32;
typedef unsigned long long int  uint64;

typedef uint8 Std_ReturnType;


/* signed Data Types */
typedef signed char             sint8;
typedef signed short int        sint16;
typedef signed long int         sint32;
typedef signed long long int    sint64;

/* Floating point Types */
typedef float                   f32;
typedef double                  f64;

/* Address NULL */
#define NULL ( (void *) 0x00)

/* Section Function Declarations */


#endif	/* MCAL_STD_TYPES_H */

