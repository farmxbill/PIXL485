/******************************************************************************

File Name:       TimeDelay.c
Dependencies:    None
Processor:       PIC18/PIC24/dsPIC30/dsPIC33/PIC32
Compiler:        C30 v3.12
Company:         Microchip Technology, Inc.

Copyright (C) 2010 Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute 
Software only when embedded on a Microchip microcontroller or digital signal 
controller that is integrated into your product or third party product 
(pursuant to the sublicense terms in the accompanying license agreement).  

You should refer to the license agreement accompanying this Software for 
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF 
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. 
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER 
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR 
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES 
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR 
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF 
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES 
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Author          Date    Comments
--------------------------------------------------------------------------------
AKN	2009.10.14	FILE CREATED
AKN	2009.10.15	CHANGED C18 DELAY ROUTINE TO DECREMENT ENTIRE NUMBER OF CYCLES
AKN	2009.10.19	CHANGED C30 DELAY ROUTINE TO MATCH C18 IMPLEMENTATION
AKN	2009.10.26	ADDED C32 DELAY ROUTINE TO MATCH C18 IMPLEMENTATION
AKN	2009.10.27	CONSOLIDATED C30 AND C32 IMPLEMENTATIONS, ADDED PADDING TO
                MAKE C30 DELAYS MORE ACCURATE
PAT	2010.01.26	CONVERTED LOCALS TO VOLATILE 
PAT	2010.03.07	ADDED include "Compiler.h"
*******************************************************************************/

#include <xc.h>
//#include "pic18.h"
#include "stdint.h"
#include "ComTypeDefs.h"
#include "mcc.h"
#include "TimeDelay.h" 


#if 0
/****************************************************************************
  Function:
    void Delay10us( UINT32 tenMicroSecondCounter )

  Description:
    This routine performs a software delay in intervals of 10 microseconds.

  Precondition:
    None

  Parameters:
    UINT32 tenMicroSecondCounter - number of ten microsecond delays
    to perform at once.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void Delay10us( uint32_t tenMicroSecondCounter )
{
    volatile uint32_t cyclesRequiredForEntireDelay;    
        

    if (OSCILLATOR_GetInstructionClock <= 2500000) //for all FCY speeds under 2MHz (FOSC <= 10MHz)
    {
        //26 cycles burned through this path (includes return to caller).
        //For FOSC == 1MHZ, it takes 104us.
        //For FOSC == 4MHZ, it takes 26us
        //For FOSC == 8MHZ, it takes 13us.
        //For FOSC == 10MHZ, it takes 10.5us.
    }
    else
    {
        //14 cycles burned to this point.

        //We want to pre-calculate number of cycles required to delay 10us * tenMicroSecondCounter using a 1 cycle granule.
        cyclesRequiredForEntireDelay = (int32_t)(OSCILLATOR_GetInstructionClock()/100000) * tenMicroSecondCounter;

        //We subtract all the cycles used up until we reach the while loop below, where each loop cycle count is subtracted.
        //Also we subtract the 22 cycle function return.
        cyclesRequiredForEntireDelay -= (153 + 22);

        if (cyclesRequiredForEntireDelay <= 45)
        {
            // If we have exceeded the cycle count already, bail! Best compromise between FOSC == 12MHz and FOSC == 24MHz.
        }    
        else
        {
            //Try as you may, you can't get out of this heavier-duty case under 30us. ;]

            while (cyclesRequiredForEntireDelay>0) //153 cycles used to this point.
            {
                Nop(); //Delay one instruction cycle at a time, not absolutely necessary.
                cyclesRequiredForEntireDelay -= 42; //Subtract cycles burned while doing each delay stage, 42 in this case.
            }
        }
    }
    

}
#endif

/****************************************************************************
  Function:
    void DelayMs( uint16_t ms )

  Description:
    This routine performs a software delay in intervals of 1 millisecond.

  Precondition:
    None

  Parameters:
    UINT16 ms - number of one millisecond delays to perform at once.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void DelayMs( uint16_t ms )
{       
    uint32_t cycles;
    
    cycles = OSCILLATOR_GetOscFreq();
    if( cycles == _LF_OSC_FREQ )
    {
        //LF Osc
        ms /= 10;
        while(ms--) 
        { 
            _delay( (unsigned long)(_LF_OSC_FREQ/400.0) );   // 10ms delay
        }
    }
    else
    {
        //HF Osc
        while(ms--) 
        { 
            _delay( (unsigned long)(_HF_OSC_FREQ/4000.0) );   // 1ms delay
        }
    }
}
