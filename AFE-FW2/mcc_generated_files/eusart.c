/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART.
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
#include "eusart.h"
#include "PortDef.h"
#include "TimeDelay.h"

/**
  Section: EUSART APIs
*/

void EUSART_Initialize(void)
{
    // Set the EUSART module to the options selected in the user interface.

    // CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA = 0x24;

    // Baud Rate = 9600; divisor is 0x01A0: SPBRG 160; SPBRGH 1
    SPBRGH = 0x01;
    SPBRG = 0xA0;
    
    // Baud Rate = 1200; divisor is 0x0D04: SPBRG 4; SPBRGH 13
//    SPBRGH = 0x0D;
//    SPBRG = 0x04;
    
//    Disable_RS485_Tx();
//    Enable_RS485_Rx();
}

void EUSART_Disable(void)
{
    // Set the EUSART module to the options selected in the user interface.

    // CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON = 0;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA = 0;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA = 0;
    
//    Disable_RS485_Tx();
//    Disable_RS485_Rx();
}
uint8_t EUSART_Read(void)
{

   RCSTAbits.SREN = 1;
    while(!PIR1bits.RCIF)
    {
    }
    
    if(1 == RCSTAbits.OERR)
    {
        // EUSART error - restart

        RCSTAbits.SPEN = 0; 
        RCSTAbits.SPEN = 1; 
    }

    return RCREG;
}

void EUSART_Write(uint8_t txData)
{
// wait until character transmitted before turning off external transceiver
    while(0 == PIR1bits.TXIF)
    {
    }
// and then send the character
    TXREG = txData;    // Write the data byte to the USART.
    while(0 == PIR1bits.TXIF)
    {
    }
}
/**
  End of File
*/
