/***********************************************************************************************//**
  @file         Measure.h
  @date         $Date:  $      
  @version      $Revision:  $    

  @brief        Header file for Measure.C
   

 **************************************************************************************************/

#ifndef __MEASURE_H
#define __MEASURE_H

#define PRESCALE_50MHZ  3       // 1:16
#define PRESCALE_10MHZ  1       // 1:4
//#define PRESCALE_10MHZ  0       // 1:2 ***DEBUG

typedef enum
{ 
    OscSelect_OFF,              // 0
    OscSelect_50MHZ_CALIB,      // 1
    OscSelect_50MHZ_MEAS_UPR,   // 2
    OscSelect_50MHZ_MEAS_LWR,   // 3
    OscSelect_10MHZ_CALIB,      // 4
    OscSelect_10MHZ_MEAS        // 5
            
} eOscSelect;


/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
void measure_setVref( bool_t turnOn );
void measure_configOsc( eOscSelect oscSelect );
Result_t measure_doMeasurement( eOscSelect oscSelect, uint16_t osc_delay, uint8_t period, 
                                uint32_t *result_cal, uint32_t *result_meas );


#endif //__MEASURE_H
