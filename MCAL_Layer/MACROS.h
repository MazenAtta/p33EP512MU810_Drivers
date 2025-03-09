/* 
 * File:   MACROS.h
 * Author: Mazen.Atta
 *
 * Created on February 22, 2025, 6:17 PM
 */

#ifndef MACROS_H
#define MACROS_H

/* Section : includes */
#include "MCAL_STD_TYPES.h"

/* Section : Macro Declarations */
/*------------- Null -------------*/
#ifndef NULL
#define NULL   	               ((void*) 0 )                  /* NULL Pointer */
#endif

/* Section : Macro Function Declarations */
                                                    /* Get Bits */
/*------------- Max-Min -------------*/
#define Max(x,y)                   (x>y)?(x):(y)                 /* Max Of Two Digits */
#define Min(x,y)                   (x>y)?(y):(x)                 /* Min Of Two Digits */
/*---------------- DELAY ---------------*/
#define delay_ms(time)                  {u32 i,j;for(i=0;i<time;i++)for(j=0;j<255;j++);}

/* Section : Datatype Declarations */
typedef enum{False=0,True,FALSE=0,TRUE,Disable=0,Enable,DISABLE=0,ENABLE}logic_t;
/*------------- Magic_Numbers -------------*/
typedef enum{ZERO=0,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN}numbers_t;

/* Section Function Declarations */


#endif /*MACROS_H*/