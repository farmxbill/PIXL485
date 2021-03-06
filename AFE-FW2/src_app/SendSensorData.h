/***********************************************************************************************//**
  @file         SendSensorData.h
  @date         $Date:  $      
  @version      $Revision:  $    

  @brief        Header file for SendSensorData.c
   

 **************************************************************************************************/

#ifndef __SEND_SENSOR_DATA_H
#define __SEND_SENSOR_DATA_H


/**************************************************************************************************
*                                         DEFINES
**************************************************************************************************/

// Time to delay per slot$ increment
#define     DELAY_PER_SLOT_MS  500      // in ms 

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

extern Result_t DAFE_sensor_sampleAndSendData( void );

#endif //__SEND_SENSOR_DATA_H
