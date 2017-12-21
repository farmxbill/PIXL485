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

//#define PrintfCommaSpace()          Printf(", ")
static void PrintfCommaSpace(void)  { Printf(", "); }

/**************************************************************************************************
*                                          GLOBAL VARS
**************************************************************************************************/

// Sampling Parameters
uint8_t gSamplingActive = 1;

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
    IO_RC0_SetDigitalOutput();
    DISABLE_TRANSMIT();

#if 0
#ifdef MONITOR_AUTODETECT	
    // Auto detect if FTDI debug cable is attached
//    if( Monitor_GetPin() == 1 )
#endif
#endif
    {
        PIXL_sensor_sampleAndSendData();
        // Our work is done.  Shut down and wait for sweet, sweet death
        INTERRUPT_GlobalInterruptDisable();
        INTERRUPT_PeripheralInterruptDisable();
        SLEEP();        // Stay here forever...   
    }

    //  If we get here, we must not have done single shot sensor measurement
    //  So crank everything up 
    OSCILLATOR_Initialize_HF();  
    tick_init();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable(); 
    
#ifdef DEBUGMONITOR
    EUSART_Initialize();
//	Monitor_init();
  
    // Print out hello message         
    Printf("\r\nPROFile ver ");
    PrintDec(VERSION_MAJOR);
    ConsolePut('.');
    PrintDec(VERSION_MINOR);
    ConsolePut('.');
    PrintDec(VERSION_REV);
    Send_CR();
#endif  
    
    // No watchdogs please
    //ClrWdt();

	//
	//  * * *     TOP LOOP      * * *   //
	//
    while( TRUE )            // Forever and ever amen
    {	
        uint32_t cal_val;       // For DEBUG
        uint32_t meas_val;        
         
        //if(gTickEvents.u.b.tick_100ms)
        //{   
        //    gTickEvents.u.b.tick_100ms = 0; 
        //}
       
        if(gTickEvents.u.b.tick_1s)
        {   
            gTickEvents.u.b.tick_1s = 0;   

            if( gSamplingActive )
            {
                measure_setVref( 1 );
#if 1                
                // 50HMZ upper
                measure_doMeasurement( OscSelect_50MHZ_MEAS_UPR, 
                                       gOscDelay, gMeasPeriod,
                                       &cal_val, &meas_val );
               
                TX_decUint32(cal_val);        
                PrintfCommaSpace();         
                TX_decUint32(meas_val);               
#endif
#if 1                
                // 50HMZ lower
                measure_doMeasurement( OscSelect_50MHZ_MEAS_LWR, 
                                       gOscDelay, gMeasPeriod,
                                       &cal_val, &meas_val );
               
                PrintfCommaSpace();         
                TX_decUint32(cal_val);        
                PrintfCommaSpace();         
                TX_decUint32(meas_val);        
#endif
#if 0                
                // 10HMZ 
                measure_doMeasurement( OscSelect_10MHZ_MEAS, 
                                       gOscDelay, gMeasPeriod,
                                       &cal_val, &meas_val );
               
                PrintfCommaSpace();         
                TX_decUint32(cal_val);        
                PrintfCommaSpace();         
                TX_decUint32(meas_val);        
#endif   
#if 1
                PrintfCommaSpace();         
                temp_printTemperature();
                Printf(" C"); 
#endif                
                Send_CR();         
                measure_setVref( 0 );
            }
        }                                                                      
       
#ifdef DEBUGMONITOR
        //Check if there's anything go on in the mon
//        Monitor_update();              
#endif        
    }  
    //
    // * * *   END TOPLOOP   * * *
    //
//#endif    
}

