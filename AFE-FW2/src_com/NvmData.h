/***********************************************************************************************//**
  @file         NvmData.h
  @date         $Date:$
  @version      $Revision:$

  @brief        Nvm Data Definitions

 **************************************************************************************************/

#ifndef __NVM_DATA_H
#define __NVM_DATA_H 

#include <stdint.h>

/**************************************************************************************************
 *                                        DEFINES
 **************************************************************************************************/

//              ** Default Settings **
// 16b: HW ID
#define DEFAULT_HWID            0x0001
// 24b: SW Version MAJOR, MINOR, REV
#define DEFAULT_SW_MAJOR        VERSION_MAJOR
#define DEFAULT_SW_MINOR        VERSION_MINOR
#define DEFAULT_SW_REV          VERSION_REV
// 8b: Osc Startup Delay in ms
#define DEFAULT_OSC_DELAY       1
// 8b: Measurement sampling period in 10ms units
#define DEFAULT_MEAS_PERIOD     18
// 8b: DAFE Sensor Type: 0 is unknown, 1 is Pressure, 2 is Dendrometer, 3 is IR, 4 is Frost
#define SENS_TYPE               0x00

// EEPROM Address of info
#define NVADRS_HWID             0
#define NVADRS_SW_MAJOR         2
#define NVADRS_SW_MINOR         3
#define NVADRS_SW_REV           4
#define NVADRS_OSC_DELAY        5
#define NVADRS_MEAS_PERIOD      6
#define NVADRS_SENS_TYPE        7
#define NVADRS_SN               8

/**************************************************************************************************
 *                                        GLOBALS
 **************************************************************************************************/

extern uint8_t gOscDelay;
extern uint8_t gMeasPeriod;
extern uint16_t gHw_ID;
extern uint8_t gSw_major;
extern uint8_t gSw_minor;
extern uint8_t gSw_rev;
extern uint8_t gSensorType;
extern uint8_t gSN0;
extern uint8_t gSN1;
extern uint8_t gSN2;
extern uint8_t gSN3;
extern uint8_t gSN4;
extern uint8_t gSN5;
extern uint8_t gSN6;
extern uint8_t gSN7;
extern uint8_t gSN8;
extern uint8_t gSN9;

/**************************************************************************************************
 *                                        API
 **************************************************************************************************/

extern void NVM_ReadGlobals( void );

#endif //__NVM_DATA_H
