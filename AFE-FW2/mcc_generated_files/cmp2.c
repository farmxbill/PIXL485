/**
  CMP2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.c

  @Summary
    This is the generated driver implementation file for the CMP2 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for CMP2.
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "cmp2.h"

/**
  Section: CMP2 APIs
*/

void CMP2_Initialize(void)
{

    // set the CMP to the options selected in MPLAB(c) Code Configurator
    // C2SP hi_speed; C2ON enabled; C2POL not inverted; C2OE COUT_pin; C2R CVref; C2CH CIN1-;                          
    CM2CON0 = 0xAD;

    // set positive voltage reference to FVR_buf1
    CM2CON1bits.C2RSEL = 0x1; 

    // C2SYNC asynchronous
    CM2CON1bits.C2SYNC = 0x0; 

    // C2HYS enabled
    CM2CON1bits.C2HYS = 0x1; 
    //CM2CON1bits.C2HYS = 0x0; 
}

#if 0
bool CMP2_GetOutputStatus(void)
{
    return (CM2CON1bits.MC2OUT);
}
#endif

/**
 End of File
*/
