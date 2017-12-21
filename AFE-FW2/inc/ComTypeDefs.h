/***********************************************************************************************//**
  @file         ComTypeDefs.h
  @date         $Date:$
  @version      $Revision:$

  @brief        General TypeDefs


 **************************************************************************************************/

#ifndef __COMTYPEDEFS_H
#define __COMTYPEDEFS_H

/**************************************************************************************************
 *                                           INCLUDES
 **************************************************************************************************/

// Microchip standard types
//#include "GenericTypeDefs.h"

/**************************************************************************************************
 *                                            TYPEDEFS
 **************************************************************************************************/

#if 0   // no longer needed
// Standard C99 Types from Software Coding Standards 0V0
typedef char                    char_t;                         
typedef unsigned char           bool_t;

typedef unsigned char           uint8_t;                        /* 8-bit unsigned  */
typedef unsigned short int      uint16_t;                       /* 16-bit unsigned */
typedef unsigned long           uint32_t;                       /* 32-bit unsigned */
/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
typedef unsigned long long      uint32_t;                       /* 64-bit unsigned */

typedef signed char             int8_t;                         /* 8-bit signed    */
typedef signed short int        int16_t;                        /* 16-bit signed   */
typedef signed long             int32_t;                        /* 32-bit signed   */
/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
typedef signed long long        int64_t;                        /* 64-bit signed   */
#endif

// Standard result type
typedef unsigned char           Result_t;

typedef unsigned char           bool_t;

enum
{
    RESULT_FAIL         = 0,
    RESULT_SUCCESS      = 1
};

#define TRUE    true
#define FALSE   false

//// Not the right place for this...
//#define Printf(x) ConsolePutROMString((ROM char*)x)
////#define printf(x) ConsolePutROMString((ROM char*)x)

#endif //__COMTYPEDEFS_H

