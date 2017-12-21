/************************************************************************************************//**
  @file         SendSensorData.c			
  @date         $Date:  $        
  @version      $Revision:  $    

  @brief        Send Sensor Data Upstream

 **************************************************************************************************/

#include <xc.h>
#include "ComTypeDefs.h"
#include "PortDef.h"
#include "Tick.h"
#include "Measure.h"
#include "pixl_intfc.h"
#include "eusart.h"
#include "crc.h"
#include "Versions.h"
#include "NvmData.h"
#include "TimeDelay.h"
#include "Temp.h"
#include "SendSensorData.h"

/**************************************************************************************************
*                                            TYPEDEFS
**************************************************************************************************/

#define PREAMBLE_BYTES  2
#define PREAMBLE_CHAR   0

/**************************************************************************************************
*                                            LOCAL VARS
**************************************************************************************************/

DetailedDAFESensorData_t DAFE_sens_data;
ProfileHeader_t      sens_hdr;
uint32_t    meas_val;

/**************************************************************************************************
*                                            PUBLIC VARS
**************************************************************************************************/


/**************************************************************************************************
*                                            PROTOTYPES
**************************************************************************************************/

void pack32_to_24( uint32_t in_val, PckdDetailedDataPoint_t *out_val );
void frame_header_init( ProfileHeader_t *data, uint8_t frame_slot, uint8_t frame_len );
void frame_send( ProfileHeader_t *hdr_data, void *frm_data );

/**********************************************************************************************//**
 * @brief   DAFE (Digital to Analog Front End) Sample and Send Sensor Data 
 *          
 * @param   
 * 
 * @return  NONE
 ***************************************************************************************************/  
Result_t DAFE_sensor_sampleAndSendData( void )
{      
    
// Sample and collect data  
    frame_header_init( &sens_hdr, (0x10 | gSensorType), sizeof(DetailedDAFESensorData_t) );
    
    measure_readTemp( &meas_val );
    DAFE_sens_data.celsius = meas_val;

    measure_readDiff12( &meas_val );
    DAFE_sens_data.diff12 = meas_val;

    measure_readSingle3( &meas_val );
    DAFE_sens_data.single3 = meas_val;

    ENABLE_TRANSMIT();
    EUSART_Initialize();
    frame_send( &sens_hdr, &DAFE_sens_data );
    DelayMs(20);
    EUSART_Disable();    
    DISABLE_TRANSMIT();
    
    return RESULT_SUCCESS;
}

/**********************************************************************************************//**
 * @brief   Initialize Frame Header
 *          
 * @param   bool: turnOn
 * 
 * @return  NONE
 ***************************************************************************************************/ 
void frame_header_init( ProfileHeader_t *data, uint8_t frame_slot, uint8_t frame_len )
{
    data->length = (uint8_t) (frame_len + PROF_FRAME_HEADER_LEN-1);
    data->fwver.maj = gSw_major;  // From EEPROM
    data->fwver.min = gSw_minor;  // From EEPROM 
    data->fwver.rev = gSw_rev;    // From EEPROM  
    data->hwver = gHw_ID;         // From EEPROM
    data->frame_slot = frame_slot;
    data->sn0 = gSN0;
    data->sn1 = gSN1;
    data->sn2 = gSN2;
    data->sn3 = gSN3;
    data->sn4 = gSN4;
    data->sn5 = gSN5;
    data->sn6 = gSN6;
    data->sn7 = gSN7;
    data->sn8 = gSN8;
    data->sn9 = gSN9;
}

/**********************************************************************************************//**
 * @brief   Initialize Frame Header
 *          
 * @param   bool: turnOn
 * 
 * @return  NONE
 ***************************************************************************************************/ 
void frame_send( ProfileHeader_t *hdr_data, void *frm_data )
{
    uint8_t     i;
    uint8_t     *data_pt;
    uint8_t     len;
    uint8_t     crc8;
    
    // Turn on Serial intfc
    
    // Send preamble bytes
    for( i=0; i<PREAMBLE_BYTES; i++ )
    {
        EUSART_Write( PREAMBLE_CHAR );
    }

    // Send frame start
    EUSART_Write( FRAME_START_DELIMITER );
     // Send length byte
    EUSART_Write( hdr_data->length );
   
    // Send Header
    data_pt = (uint8_t *)&(hdr_data->fwver);
    crc8 = compute_crc8( 0, data_pt, PROF_FRAME_HEADER_LEN-1 );    
    for( i=0; i<PROF_FRAME_HEADER_LEN-1; i++ )
    {
        EUSART_Write( *data_pt++ );   
    }
    
     // Send Frame
    data_pt = frm_data;
    len = hdr_data->length - (PROF_FRAME_HEADER_LEN-1);
    crc8 = compute_crc8( crc8, data_pt, len );    
    for( i=0; i<len; i++ )
    {
        EUSART_Write( *data_pt++ );   
    }
    
    // Send CRC8
    EUSART_Write( crc8 );
    // flush the channel
    EUSART_Write( 0x00 ); 
    EUSART_Write( 0x00 ); 
}