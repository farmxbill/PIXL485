/********************************************************************
* FileName:		Console.h
* Dependencies: none
* Processor:	PIC18

* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement). 
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*  This header file allows usage of the console functions located
*   in Console.c
*
* Change History:
*  Rev   Date         Description

********************************************************************/
#ifndef  _CONSOLE_H_
#define  _CONSOLE_H_

/************************ HEADERS **********************************/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define ENABLE_CONSOLE

//#define BAUD_RATE 19200

/************************ DEFINITIONS ******************************/
/************************ FUNCTION PROTOTYPES **********************/

    #if defined(ENABLE_CONSOLE)   // Useful for disabling the console (saving power)
        void ConsoleInit(void);
        void ConsolePut(uint8_t c);
        void ConsolePutString(uint8_t *s);
        void ConsolePutROMString(const char* str);
    
        uint8_t ConsoleGet(void);
        uint8_t ConsoleGetString(char *buffer, uint8_t bufferLen);
        void PrintChar(uint8_t);
        void PrintDec(uint8_t);
        char * strupr(char *s);
        // Hmm these should be abstracted...
        #define ConsoleIsGetReady()     (PIR1bits.RCIF)  //For Shame!
        #define ConsoleIsPutReady()     (TXSTAbits.TRMT)

    #else
        #define ConsoleInit()
        #define ConsoleIsPutReady() 1
        #define ConsolePut(c)
        #define ConsolePutString(s)
        #define ConsolePutROMString(str)
    
        #define ConsoleIsGetReady() 1
        #define ConsoleGet()        'a'
        #define ConsoleGetString(buffer, bufferLen) 0
        #define PrintChar(a)
        #define PrintDec(a)
    #endif

#define Printf(x) ConsolePutROMString((const char*)x)
//#define printf(x) ConsolePutROMString((ROM char*)x)
#endif


