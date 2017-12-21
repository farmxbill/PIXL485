/************************************************************************************************//**
  @file         Temp.c			
  @date         $Date:  $        
  @version      $Revision:  $    

  @brief        Read LM75B Temperature Sensor

 **************************************************************************************************/

#include <xc.h>
#include "ComTypeDefs.h"
#include "PortDef.h"
#include "Tick.h"
#include "Temp.h"
#include "Monitor.h"     

/**************************************************************************************************
*                                            TYPEDEFS
**************************************************************************************************/

#define LM75B_I2C_ADR   0x48
//#define LM75B_I2C_ADR   0x90

#define LM75B_RETRY_MAX 3  // define the max retry count
// Registers
#define LM75B_REG_CONF  0x01
#define LM75B_REG_TEMP  0x00
#define LM75B_REG_TOS   0x03
#define LM75B_REG_THYST 0x02


/**************************************************************************************************
*                                            LOCAL VARS
**************************************************************************************************/



/**************************************************************************************************
*                                            PUBLIC VARS
**************************************************************************************************/

/**************************************************************************************************
*                                            PROTOTYPES
**************************************************************************************************/



/**********************************************************************************************//**
 * @brief   Read Temperature from LM75B
 *          
 * @param   *temp - pointer to temp data returned - in units of 0.125C
 * 
 * @return  NONE
 ***************************************************************************************************/
Result_t temp_readTemperature( int16_t * temp ) 
{
    I2C_MESSAGE_STATUS status = 0;
    uint8_t    timeOut = 0;
    uint8_t    repeat = 1;  // read two samples quickly - throw away first
    uint8_t    dataout[2];

    while( status != I2C_MESSAGE_FAIL ) 
    {
        // read 16b temp register
        I2C_MasterRead( dataout,
                        2,
                        LM75B_I2C_ADR,
                        &status);

        // wait for the message to be sent or status has changed.
        while( status == I2C_MESSAGE_PENDING )
            ;           // ugg, danger - infinite loop city

        if( status == I2C_MESSAGE_COMPLETE )
        {
            if( (repeat--) > 0)
            {
                // Kludge - throw away first sample because of strange
                // MSB vs. LSB mismatch issues
                continue;
            }

            //TX_hex2(dataout[0]);      // EDBUG only
            //TX_hex2(dataout[1]);
            //Printf(": "); 
            
            // 11b sensor data.  LSB = 0.125C
            *temp = (int16_t)(((uint16_t)dataout[0])*8 + ((uint16_t)dataout[1])/32);
            return RESULT_SUCCESS;
        }

        // if status is  I2C_MESSAGE_ADDRESS_NO_ACK,
        //               or I2C_DATA_NO_ACK,
        // The device may be busy and needs more time for the last
        // write so we can retry writing the data, this is why we
        // use a while loop here

        // check for max retry and skip this byte
        if (timeOut == LM75B_RETRY_MAX)
            break;
        else
            timeOut++;
    }
    
    return RESULT_FAIL;
}
	 
#ifdef DEBUGMONITOR
/**********************************************************************************************//**
 * @brief   Print Temperature from LM75B to Debug USART
 *          
 * @param   none
 * 
 * @return  NONE
 ***************************************************************************************************/
Result_t temp_printTemperature( void )
{    
    int16_t temp;

    if( temp_readTemperature(&temp) == RESULT_SUCCESS )
    {
        TX_decInt8(temp/8);        
        Printf("."); 
        TX_decInt16( (temp%8)*125 );     
        return RESULT_SUCCESS;
    }
    else
    {
        return RESULT_FAIL;
    }
}
#endif // DEBUGMONITOR
