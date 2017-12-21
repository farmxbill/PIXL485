/**
  CMP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp1.c

  @Summary
    This is the generated driver implementation file for the CMP1 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for CMP1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.3
        Device            :  PIC18LF13K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "cmp1.h"
/**
  Section: CMP1 APIs
*/

void CMP1_Initialize(void)
{

    // set the CMP to the options selected in MPLAB(c) Code Configurator
    // C1SP hi_speed; C1ON enabled; C1POL not inverted; C1OE COUT_internal; C1R CIN+; C1CH CIN0-;                          
    CM1CON0 = 0x88;


    // C1SYNC asynchronous
    CM2CON1bits.C1SYNC = 0x0; 

    // C1HYS disabled
    CM2CON1bits.C1HYS = 0x0; 

}

bool CMP1_GetOutputStatus(void)
{
    return (CM2CON1bits.MC1OUT);
}


/**
 End of File
*/
