/***********************************************************************************************//**
  @file         NvmData.c
  @date         $Date:$
  @version      $Revision:$

  @brief        Nvm Data Definitions

 **************************************************************************************************/

#include <stdint.h>
#include <xc.h>
#include "Versions.h"
#include "NvmData.h"
#include "memory.h"
 
/**************************************************************************************************
*              NV DATA DEFINITIONS
**************************************************************************************************/

// Initialize EEPROM defaults in Hex file
__EEPROM_DATA( (DEFAULT_HWID & 0xff), (DEFAULT_HWID >> 8), 
                DEFAULT_SW_MAJOR,
                DEFAULT_SW_MINOR,
                DEFAULT_SW_REV,
                DEFAULT_OSC_DELAY,
                DEFAULT_MEAS_PERIOD,
                SENS_TYPE
             );
__EEPROM_DATA(        
                'N', 'o', ' ', 'S', 'e', 'r', 'i', 'a'
             );
__EEPROM_DATA(        
                'l', '#', 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
             );

/**************************************************************************************************
*              GLOBAL NV SHADOW VARS
**************************************************************************************************/

uint8_t gOscDelay;
uint8_t gMeasPeriod;
uint8_t gSw_major;
uint8_t gSw_minor;
uint8_t gSw_rev;
uint8_t gSensorType;
uint16_t gHw_ID;
uint8_t gSN0;
uint8_t gSN1;
uint8_t gSN2;
uint8_t gSN3;
uint8_t gSN4;
uint8_t gSN5;
uint8_t gSN6;
uint8_t gSN7;
uint8_t gSN8;
uint8_t gSN9;

/**********************************************************************************************//**
 * @brief   Initialize global variables from NVRAM
 *          
 * @param   NONE
 * 
 * @return  NONE
 ***************************************************************************************************/  
void NVM_ReadGlobals( void )
{
    gOscDelay = DATAEE_ReadByte(NVADRS_OSC_DELAY);
    gMeasPeriod = DATAEE_ReadByte(NVADRS_MEAS_PERIOD);  
    gHw_ID = (uint16_t) (DATAEE_ReadByte(NVADRS_HWID) + (DATAEE_ReadByte(NVADRS_HWID+1)<<8));
    gSensorType = (uint8_t)(DATAEE_ReadByte(NVADRS_SENS_TYPE));
    gSw_major = (uint8_t) (DATAEE_ReadByte(NVADRS_SW_MAJOR));
    gSw_minor = (uint8_t) (DATAEE_ReadByte(NVADRS_SW_MINOR));
    gSw_rev   = (uint8_t) (DATAEE_ReadByte(NVADRS_SW_REV));
    gSN0      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN));
    gSN1      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+1));
    gSN2      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+2));
    gSN3      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+3));
    gSN4      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+4));
    gSN5      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+5));
    gSN6      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+6));
    gSN7      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+7));
    gSN8      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+8));
    gSN9      = (uint8_t) (DATAEE_ReadByte(NVADRS_SN+9));
}

