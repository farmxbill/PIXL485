/***********************************************************************************************//**
  @file         PortDef.h
  @date         $Date: $      
  @version      $Revision: $    

  @brief        Defines project ports & hardware

  Based on PIC18F14K22Port Definition and Assignment for PROFile 
  
 **************************************************************************************************/

#ifndef __PORTDEF_H
#define __PORTDEF_H

#include <xc.h>
#include "pin_manager.h"
#include "mcc.h"
//
// RC Oscillator runs at 16Mhz (Don't forget the "L" eLse you'll be screwed!)
//
#define FOSC  _HF_OSC_FREQ      
#define FOSC4 (FOSC / 4L)   //peripheral clock is 1/4 main osc.

//Port definitions

// Protection
//#define S0_SetHigh()    IO_RA4_SetHigh()
//#define S1_SetHigh()    IO_RA5_SetHigh()
//#define S0_SetLow()     IO_RA4_SetLow()
//#define S1_SetLow()     IO_RA5_SetLow()

// Prototype (S0, S1 reversed))
#define S0_SetHigh()    IO_RA5_SetHigh()
#define S1_SetHigh()    IO_RA4_SetHigh()
#define S0_SetLow()     IO_RA5_SetLow()
#define S1_SetLow()     IO_RA4_SetLow()
// RA.1 - PGC
// RA.0 - PGD


// Port B:
//#define TXD_H       LATBbits.LATB7  //Rs232 Tx data
//#define RXD_H       LATBbits.LATB5	//RS232 Rx data

//#define TXD_Hn      7               //Rs232 Tx data
//#define RXD_Hn      5	            //RS232 Rx data

#define RXD_GetPin()        PORTBbits.RB5

#define ENABLE_TRANSMIT()   Enable_RS485_Tx()
#define DISABLE_TRANSMIT()  Disable_RS485_Tx()

// Port C:
#define ADR0_GetPin()   IO_RC2_GetValue()
#define ADR1_GetPin()   IO_RC3_GetValue()

#define DBG_SetHigh()   IO_RA5_SetHigh()
#define DBG_SetLow()    IO_RA5_SetLow()
#define DBG_Toggle()    IO_RA5_Toggle()

#define OSCVREG_Enable()       IO_RC0_SetHigh()
#define OSCVREG_Disable()      IO_RC0_SetLow()

#define OSC50MHZ_Disable()     IO_RC6_SetDigitalInput()
#define OSC50MHZ_Enable()      IO_RC6_SetDigitalOutput()
#define OSC10MHZ_Disable()     IO_RC7_SetDigitalInput()
#define OSC10MHZ_Enable()      IO_RC7_SetDigitalOutput()

#define Enable_ADC_Power()     IO_RB4_SetDigitalInput()
#define Disable_ADC_Power()    IO_RB4_SetDigitalOutput()
#define ADC_ChipSel_Enable()   IO_RC6_SetLow()
#define ADC_ChipSel_Disable()  IO_RC6_SetHigh()
#define ADC_SCK_rising()  IO_RC4_SetHigh()
#define ADC_SCK_falling() IO_RC4_SetLow()

#define Enable_RS485_Tx()      IO_RC1_SetHigh()
#define Disable_RS485_Tx()     IO_RC1_SetLow()
#define Enable_RS485_Rx()      IO_RC0_SetLow()
#define Disable_RS485_Rx()     IO_RC0_SetHigh()

#endif //__PORTDEF_H