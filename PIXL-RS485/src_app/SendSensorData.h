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
// in ms
// 4 measurements taken with 180 ms per measurement 
// + transmit time for the packet (40 ms))
// + turnoff time for transmitter (20 ms)
// with 15% margin
// then add 75 ms of inter-controller quiet time (no coupling between channels)
// #define     DELAY_PER_SLOT_MS  (((4*DEFAULT_MEAS_PERIOD*10)+40+20)*1.15)
#define DELAY_PER_SLOT_MS 950

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

extern Result_t DAFE_sensor_sampleAndSendData( void );
extern Result_t PIXL_sensor_sampleAndSendData( void );
extern Result_t FAKE_PIXL_sensor_sampleAndSendData( void );

void measure_readTemp ( uint32_t *meas_val );
void measure_readDiff12 ( uint32_t *meas_val );
void measure_readSingle3 ( uint32_t *meas_val );

#endif //__SEND_SENSOR_DATA_H
