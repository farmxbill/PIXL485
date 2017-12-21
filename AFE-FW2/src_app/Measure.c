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
#include "Monitor.h"

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
        IO_RC6_SetLow()
        ;
//        OSC50MHZ_Enable();
    else
        ;
//        OSC50MHZ_Disable();
    
//    if( OscSelectionTable[oscSelect].en_10MHz )     
//        OSC10MHZ_Enable();
//    else
//        OSC10MHZ_Disable();

//    if( OscSelectionTable[oscSelect].s0 )     
//        S0_SetHigh();
//    else
//        S0_SetLow();
           
//    if( OscSelectionTable[oscSelect].s1 )     
//        S1_SetHigh();
//    else
//        S1_SetLow();
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

/**********************************************************************************************//**
 * @brief   measure_readADC - Read ADC and return 32 bit result
 *          
 * @param   NONE
 * 
 * @return  32 bit ADC result
 ***************************************************************************************************/  
void measure_readADC( uint32_t *result_meas, uint16_t ADC_config )
{
    uint16_t i;
    uint32_t datain, config_bit;
    uint32_t reg;
  
// config values: ((1 << 12) | (en << 10) | (sgl << 9) | (odd << 8) | (addr << 5) | (en2 << 4) | (im << 3) | (fa << 2) | (fb << 1) | (spd << 0));  

    // read measurement from ADC
    // need to implement a bit-banging SDI routine
    // first need to program the ADC for the right channels, etc.
    // then read the programming back to confirm it
    // then read the ADC conversion
    //
    // then use these four pins as a SDI UART:
    //    CS_N  : RC6  (chip select)
    //    SCK   : RC4  (clocking)
    //    MISO  : RB6  (input from ADC)
    //    MOSI  : RC5  (output to ADC)
    //
    // start out with having CS_N always active (low), and have this happen prior to power up.
    
    ADC_ChipSel_Enable();  // turn on chipselect
    send_a_SCK; // make sure SCK starts out low when power applied to ADC  
    DelayMs ( 5 );  // let the chip select settle
    
    // turn on ADC power and let power to settle
    Enable_ADC_Power();  // after the chip select is low - means we control SCKclock to the ADC
    DelayMs( 25 );
    
    // read the Data Input to see if the conversion has completed (it is low)
    waitForADCSamplingToComplete();

    IO_RC5_SetHigh();    
    // now read the next 31 bits by toggling SCK    
    reg = 0;
    i = 0;
    ADC_ChipSel_Enable();
    while (i <= 31)
    {
        if (i <= 12)
        {
            config_bit = ((ADC_config)>>(12-i)) & 1;
            if ((config_bit & 1) == 1)
            {
                IO_RC5_SetHigh();
            } else {
                IO_RC5_SetLow();
            }
        }
        send_a_SCK();
        i++;
    }

    waitForADCSamplingToComplete();  
    // now read the next 31 bits by toggling SCK
    
    reg = 0;
    i = 0;
    while (i <= 31)
    {
        datain = PORTBbits.RB6;
        reg |= (datain << (31-i));
        if (i <= 12)
        {
            config_bit = ((ADC_config)>>(12-i)) & 1;
            if ((config_bit & 1) == 1)
            {
                IO_RC5_SetHigh();
            } else {
                IO_RC5_SetLow();
            }
        }
        send_a_SCK();
        i++;
    }

    // turn off chip select and turn off ADC Power
    ADC_ChipSel_Disable();
    Disable_ADC_Power();
    
    *result_meas = reg;
}

void measure_readTemp( uint32_t *result_meas )
{
    uint32_t reg;
    
    measure_readADC( &reg, ((1<<12)|(1<<10)|(1<<4)|(1<<3)) );
    *result_meas = reg;
}

void measure_readDiff12( uint32_t *result_meas )
{
    uint32_t reg;
    
    measure_readADC( &reg, ((1<<12)|(1<<10)|(1<<4)) );
    *result_meas = reg;
}

void measure_readSingle3( uint32_t *result_meas )
{
    uint32_t reg;
    // to get the full 2.5V range for the single ended measurement
    // the PCB has a 1.25V input on channel 4 of the device, and we
    // measure channel 3 relative to this.  It will return a range
    // from -1.25V to 1.25V.  To get the single ended voltage normalized
    // to ground, then add 1.25V to the read result (do that in the application)
    
    measure_readADC( &reg, ((1<<12)|(1<<10)|(1<<5)|(1<<4)) );
    *result_meas = reg;
}

void send_a_SCK(void)
{
    ADC_SCK_rising();
    DelayMs ( 1 );
    ADC_SCK_falling();
    DelayMs ( 1 );
}

void waitForADCSamplingToComplete(void)
{
    uint32_t datain;
    
    // read the Data Input to see if the conversion has completed (it is low)
    datain = 1;
    while ((datain) == 1)
    {
        datain = PORTBbits.RB6;
        DelayMs(1);
    }
}

static void waitForSamplingToComplete(void)
{
    while( gTickEvents.u.b.tick_samp_complete == 0 )
        ;
}
