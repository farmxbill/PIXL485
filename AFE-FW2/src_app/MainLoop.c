/***********************************************************************************************//**
  @file         MainLoop.c    
  @date         $Date: 2012-03-19 18:57:02 -0700 (Mon, 19 Mar 2012) $
  @version      $Revision: 226 $    

  @brief        Main & TOPLOOP

 **************************************************************************************************/

//#include "Configuration.h"	// Contains all "pragma config" configurations
#include "PortDef.h"
#include "ComTypeDefs.h"
#include "mcc.h"
#include "Monitor.h"
#include "Versions.h"
#include "Tick.h"
#include "Measure.h"
#include "Temp.h"
#include "NvmData.h"
#include "SendSensorData.h"
#include "TimeDelay.h"
#include "float_to_char.h"

//#define PrintfCommaSpace()          Printf(", ")
//static void PrintfCommaSpace(void)  { Printf(", "); }

/**************************************************************************************************
*                                          GLOBAL VARS
**************************************************************************************************/

// Sampling Parameters
uint8_t gSamplingActive = 0;

/**************************************************************************************************
*                                          FUNCTIONS - PUBLIC
**************************************************************************************************/

/**********************************************************************************************//**
 * @brief   Main entry point and top loop
 *
 * @param   None 
 * 
 * @return  None    
 ***************************************************************************************************/
void main(void)
{
// Initialize the device
    SYSTEM_Initialize();    // Start up with slow LF Osc
    NVM_ReadGlobals();
    ENABLE_TRANSMIT();
    EUSART_Initialize();
    
    OSCILLATOR_Initialize_HF();  
    tick_init();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

// this code is for the non-MODBUS version of the PIC code.  Keeping for reference.
    
//    uint8_t i, j;
    while ( TRUE )   // bypass this, and enter monitor: be nice to autodetect this somehow
    {   
        DBG_Toggle();
        DelayMs(100);
#if 0
        for (i = 0; i < 30; i++)
        {
            for (j = 0; j < 16; j++)
            {
                EUSART_Write('\x00');
                DelayMs (1);
            }
            for (j = 0; j < 16; j++)
            {
                EUSART_Write('\xFF');
                DelayMs (1);
            }
            for (j = 0; j < 16; j++)
            {
                EUSART_Write('\xAA');
                DelayMs (1);
            }
            for (j = 0; j < 16; j++)
            {
                EUSART_Write('\x55');
                DelayMs (1);
            }
        }
    }
#endif
#if 1
        DBG_Toggle();
        DelayMs(500);
        DBG_Toggle();
        DelayMs(500);
        
        DAFE_sensor_sampleAndSendData();
        DelayMs(250);
        // Our work is done.  Shut down and wait for sweet, sweet death
        
        INTERRUPT_GlobalInterruptDisable();
        INTERRUPT_PeripheralInterruptDisable();
        SLEEP();
        while ( TRUE )
        {
            DBG_Toggle();
            DelayMs(200);
        }
#endif
    }
    // No watchdogs please
    //ClrWdt();
    
	//
	//  * * *     TOP LOOP      * * *   //
	//
    
    //  If we get here, we must not have done single shot sensor measurement
    //  So crank everything up

#if 0
	Monitor_init();
  
    // Print out hello message         
    Printf("\r\nLarry's DAFE version for MODBUS development ");
    PrintDec(gSw_major);
    ConsolePut('.');
    PrintDec(gSw_minor);
    ConsolePut('.');
    PrintDec(gSw_rev);
    Send_CR();
#endif
#if 0
// until the MODBUS code is available, cranking up the monitor code, and printing values    
    while( FALSE )            // Forever and ever amen
    {
        uint32_t meas_val;
        uint32_t range_and_sign;
        float temp_c, temp_k, diff_v, sing_v;
        char  diff_buf[10], temp_buf[10], sing_buf[10];

        gSamplingActive = TRUE;
        if(gTickEvents.u.b.tick_1s)
        {   
            gTickEvents.u.b.tick_1s = 0;         
            DBG_Toggle();
            if( gSamplingActive )
            {
                measure_readTemp( &meas_val );
                meas_val &=0x0FFFFFFF;  // ignore sign bits and overflow bits
                meas_val >>=5;  // ignore lower five bits
                temp_k = meas_val * 0.00159235668789809; // convert to Kelvin
                temp_c = temp_k - 271.15;  // convert to Celsius from Kelvin
                float_to_char(temp_c, temp_buf, 2);
                
// reads differential voltage input on channels 1 and 2 of the ADC
                measure_readDiff12( &meas_val );
                range_and_sign = ((meas_val >> 28) & 0x3);
                meas_val = (meas_val & 0x0FFFFFFC) >> 5;

                if ((range_and_sign == 0x1) | (range_and_sign == 0x2))
                {
                    // in range measurement: both positive and negative values determined here
                    diff_v = meas_val * 0.0000001490116119;  // volts = signbit *(ADC value * 1/(2^23)*1.25
                }
                if (range_and_sign == 0x3)
                {
                    // positive out of range value: max out at +1.25V
                    diff_v = 1.25;
                }
                if (range_and_sign == 0x0)
                {
                    // negative out of range value: max out at -1.25V
                    diff_v = -1.25;
                }
                float_to_char(diff_v, diff_buf, 5); 

// reads the single ended third channel of the ADC
                measure_readSingle3( &meas_val ); 
                range_and_sign = ((meas_val >> 28) & 0x3);
                meas_val = (meas_val & 0x0FFFFFFC) >> 5;
                
                if ((range_and_sign == 0x1) | (range_and_sign == 0x2))
                {
                    // in range measurement: both positive and negative values determined here
                    sing_v = meas_val * 0.0000001490116119;  // volts = signbit *(ADC value * 1/(2^23)*1.25
                }
                if (range_and_sign == 0x3)
                {
                    // positive out of range value: max out at +1.25V
                    sing_v = 1.25;
                }
                if (range_and_sign == 0x0)
                {
                    // negative out of range value: max out at -1.25V
                    sing_v = -1.25;
                }
                float_to_char(sing_v, sing_buf, 5);
//
// Temperature in Celsius
                if (temp_c >= 0)
                {
                    Printf("+");
                }
                Printf(temp_buf);
                Printf(" C, ");
// differential voltage (for Pressure, Dendrometer, IR Sensor)
                if (diff_v >= 0)
                {
                    Printf("+");
                }
                Printf(diff_buf);
                Printf(" V, ");
// single-ended voltage (for IR sensor)
                if (sing_v >= 0)
                {
                    Printf("+");
                }
                Printf(sing_buf);
                Printf(" V");                    
                Send_CR();
            }
        }                                                                             
        //Check if there's anything go on in the mon
        Monitor_update();
    }
#endif
    //
    // * * *   END TOPLOOP   * * *
    //
    }

