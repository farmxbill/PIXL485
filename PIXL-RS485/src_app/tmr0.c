/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for TMR0.
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


// *** MODIFIED for PROFile Application -PS081816 ***

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr0.h"

/**
  Section: Global Variables Definitions
*/
//volatile uint16_t timer0ReloadVal;
volatile uint16_t timer0_hicount;

/**
  Section: TMR0 APIs
*/


void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0PS 1:16; T08BIT 16-bit; T0SE Increment_lo_hi; T0CS T0CKI; TMR0ON off; PSA assigned; 
    T0CON = 0x23;
    //T0CON = 0x28;       //No prescal ***DEBUG
    
}

void TMR0_PrepareCounting(uint8_t prescal_val)
{
    // Make sure it's stopped first
    T0CONbits.TMR0ON = 0;
    
    // Set prescaler
    T0CONbits.T0PS = prescal_val;
    T0CONbits.T0CS = 1;     // fer grins
    
    // Clear count
    timer0_hicount = 0;
    TMR0H = 0;
    TMR0L = 0;
    
    // Enable interrupt
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    
    // Do not start counting yet
    //T0CONbits.TMR0ON = 1;
}

void TMR0_StartCounting(void)
{
    TMR0L = 0;      // clear prescal again Just for grins...
    // Start the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 1;
}

void TMR0_StopCounting(void)
{
    // Stop the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 0;
    // Turn off IRQ
    INTCONbits.TMR0IE = 0;  
}

uint32_t TMR0_ReadCount(void)
{
    uint32_t readVal;
    uint8_t readValLow;
    uint8_t readValHigh;

    readValLow  = TMR0L;
    readValHigh = TMR0H;
    readVal  = ((uint32_t)timer0_hicount << 16) + ((uint32_t)readValHigh << 8) + readValLow;

    return readVal;
}


void TMR0_ISR(void)
{
    // clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    timer0_hicount++;
}


/**
  End of File
*/