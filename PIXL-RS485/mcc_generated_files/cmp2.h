/**
  CMP2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.h

  @Summary
    This is the generated header file for the CMP2 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for CMP2.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC18F13K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _CMP2_H
#define _CMP2_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: CMP2 APIs
*/

/**
  @Summary
    Initializes the CMP2

  @Description
    This routine initializes the CMP2.
    This routine must be called before any other CMP2 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    CMP2_Initialize();
    </code>
*/
void CMP2_Initialize(void);

/**
  @Summary
    Gets the CMP2 output status.

  @Description
    This routine gets the CMP2 output status.

  @Preconditions
    The CMP2 initializer routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    high  - if the CMP2 output is high.
    low   - if the CMP2 output is low.

  @Example
    <code>
    #define LED_On  LATAbits.LATA0=1
    #define LED_Off LATAbits.LATA0=0

    CMP2_Initialize()

    while(1)
    {
        if(CMP2_GetOutputStatus())
        {
            LED_On;
        }
        else
        {
             LED_Off;
        }
    }
    </code>
*/
bool CMP2_GetOutputStatus(void);



#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _CMP2_H
/**
 End of File
*/

