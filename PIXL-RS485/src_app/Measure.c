/************************************************************************************************//**
  @file         Measure.c			
  @date         $Date:  $        
  @version      $Revision:  $    

  @brief        Do Frequency Measurements

 **************************************************************************************************/

#include <xc.h>
#include "ComTypeDefs.h"
#include "PortDef.h"
#include "Tick.h"
#include "TimeDelay.h"
#include "Measure.h"

/**************************************************************************************************
*                                            TYPEDEFS
**************************************************************************************************/


typedef struct
{
    uint8_t en_50MHz:1;
    uint8_t en_10MHz:1;
    uint8_t s0:1;
    uint8_t s1:1;
} sOscSelectionTableEntry;


/**************************************************************************************************
*                                            LOCAL VARS
**************************************************************************************************/

static const sOscSelectionTableEntry OscSelectionTable[] = 
{ 
 //   50 10 s0 s1
    { 0, 0, 0, 0 },     // OscSelect_OFF
    { 1, 0, 0, 0 },     // OscSelect_50MHZ_CALIB
    { 1, 0, 0, 1 },     // OscSelect_50MHZ_MEAS_UPR
    { 1, 0, 1, 1 },     // OscSelect_50MHZ_MEAS_LWR
    { 0, 1, 0, 0 },     // OscSelect_10MHZ_CALIB
    { 0, 1, 1, 1 },     // OscSelect_10MHZ_MEAS
};



/**************************************************************************************************
*                                            PUBLIC VARS
**************************************************************************************************/

/**************************************************************************************************
*                                            PROTOTYPES
**************************************************************************************************/

static void waitForSamplingToComplete(void);

/**********************************************************************************************//**
 * @brief   measure_setVref - Set OSC VREG voltage regulator on or off
 *          
 * @param   bool: turnOn
 * 
 * @return  NONE
 ***************************************************************************************************/  
void measure_setVref( bool_t turnOn )
{
    measure_configOsc( OscSelect_OFF );   // make everything off
    if( turnOn )
    {
        OSCVREG_Enable();
    }
    else
    {
        OSCVREG_Disable();
    }
}	 


/**********************************************************************************************//**
 * @brief   measure_doMeasurement - Set OSC VREG voltage regulator on or off
 *          
 * @param   oscSelect: OscSelect_OFF...
 * 
 * @return  NONE
 ***************************************************************************************************/  
void measure_configOsc( eOscSelect oscSelect )
{
    // Configure OSC section (assumes measure_setVref already called)
    if( OscSelectionTable[oscSelect].en_50MHz ) 
        OSC50MHZ_Enable();
    else
        OSC50MHZ_Disable();
    
    if( OscSelectionTable[oscSelect].en_10MHz )     
        OSC10MHZ_Enable();
    else
        OSC10MHZ_Disable();

    if( OscSelectionTable[oscSelect].s0 )     
        S0_SetHigh();
    else
        S0_SetLow();
           
    if( OscSelectionTable[oscSelect].s1 )     
        S1_SetHigh();
    else
        S1_SetLow();
}

/**********************************************************************************************//**
 * @brief   measure_doMeasurement - Set OSC VREG voltage regulator on or off
 *          
 * @param   oscSelect: OscSelect_OFF...
 * 
 * @return  NONE
 ***************************************************************************************************/  
Result_t measure_doMeasurement( eOscSelect oscSelect, uint16_t osc_delay, uint8_t period, 
                                uint32_t *result_cal, uint32_t *result_meas )
{
    switch( oscSelect )
    {
        case OscSelect_50MHZ_MEAS_UPR:
        case OscSelect_50MHZ_MEAS_LWR:
            // measure calibration first
            measure_configOsc( OscSelect_50MHZ_CALIB );
            // Note:  In reality, delays will be a mult of 10ms due to
            // synching with tick 10ms counter
            DelayMs( osc_delay );
            TMR0_PrepareCounting( PRESCALE_50MHZ );
            tick_startFreqMeasurement( period );
            waitForSamplingToComplete();
            *result_cal = TMR0_ReadCount();
            
            // Now do measurement
            measure_configOsc( oscSelect );
            DelayMs( osc_delay );
            TMR0_PrepareCounting( PRESCALE_50MHZ );
            tick_startFreqMeasurement( period );
            waitForSamplingToComplete();
            *result_meas = TMR0_ReadCount();

            measure_configOsc( OscSelect_OFF );   // make everything off
            return RESULT_SUCCESS;
            //break;
           
        case OscSelect_10MHZ_MEAS:
            // measure calibration first
            measure_configOsc( OscSelect_10MHZ_CALIB );
            DelayMs( osc_delay );
            TMR0_PrepareCounting( PRESCALE_10MHZ );
            tick_startFreqMeasurement( period );
            waitForSamplingToComplete();
            *result_cal = TMR0_ReadCount();
            
            // Now do measurement
            measure_configOsc( oscSelect );
            DelayMs( osc_delay );
            TMR0_PrepareCounting( PRESCALE_10MHZ );
            tick_startFreqMeasurement( period );
            waitForSamplingToComplete();
            *result_meas = TMR0_ReadCount();
            
            measure_configOsc( OscSelect_OFF );   // make everything off
            return RESULT_SUCCESS;
            //break; 
            
        default:
            return RESULT_FAIL;
            //break;
    }
}

static void waitForSamplingToComplete(void)
{
    while( gTickEvents.u.b.tick_samp_complete == 0 )
        ;
}
