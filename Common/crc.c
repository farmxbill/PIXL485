/*!***************************************************************************************
  @file         crc.c
  @date         $Date$
  @version      $Revision$

  @brief	Code for computing 8- and 16-bit CRCs.

****************************************************************************************/

#include <stdint.h>
#include "crc.h"

#if 0
/*!***************************************************************************************
 *
 * Update a 16-bit CRC using polynomial x^16 + x^15 + x^2 + 1 (0xA001)
 *
 * @param crc [in] Previous CRC value, or initial value (0xFFFF)
 *
 * @param data [in] Byte to incorporate in the CRC
 *
 * @return The new CRC with `data` incorporated.
 *
 ****************************************************************************************/
static uint16_t _crc16_update(uint16_t crc, uint8_t data)
{
	uint8_t i;
	crc ^= data << 8;
	for (i = 0; i < 8; i++)
	{
		if (crc & 0x8000)
		{
			crc = (crc << 1) ^ 0x8005;
		}
		else
		{
			crc <<= 1;
		}
	}
	return crc;
}

/*!***************************************************************************************
 *
 * Compute a 16-bit CRC on a block of memory.
 *
 * The algorithm used is CRC-16-IBM using the polynomial x^16 + x^15 + x^2 + 1 (0xA001) with initial value 0xFFFF.
 *
 * @return The 16-bit CRC of the program Flash and the interrupt vector table.
 *
 ****************************************************************************************/
uint16_t compute_crc16(void* data, uint16_t length)
{
	uint16_t crc = 0x00;//FFFF;
	uint8_t* cursor = data;
	//uint8_t* max_cursor = (uint8_t*)0;
	uint16_t i;
	for (i = 0; i < length; i++)
	{
		crc = _crc16_update(crc, *cursor);
		cursor++;
		/*if ((uint16_t)cursor > (uint16_t)max_cursor)
		{
			max_cursor = cursor;
		}*/
	}
	return crc;
}
#endif

/*!***************************************************************************************
 *
 * @brief Update the CRC with a byte of data.
 *
 * @param crc [in] The CRC returned for the previous byte, or the algorithm's initial value if this is the first byte.
 *
 * @param data [in] The byte with which to update the CRC.
 *
 * @return The updated CRC.
 *
 ****************************************************************************************/
static uint8_t _crc8_update(uint8_t crc, uint8_t data)
{
	uint8_t i;
	data = (uint8_t)(crc ^ data);
	for ( i = 0; i < 8; i++ )
	{
		if (( data & 0x80 ) != 0 )
		{
			data <<= 1;
			data ^= 0x07;
		}
		else
		{
			data <<= 1;
		}
	}
	return data;
}

/*!***************************************************************************************
 *
 * @brief Compute the 8-bit CRC of a sequence of data.
 *
 * The algorithm used is CRC-8-CCITT using the polynomial x^8 + x^2 + x + 1 (0x07) with initial value 0x00.
 *
 * @param data [in] The block of data to be CRCed.
 *
 * @param length [in] The number of bytes in `data` to be CRCed.
 *
 * @return The CRC of the sequence
 *
 ****************************************************************************************/
uint8_t compute_crc8( uint8_t crc, void* data, uint16_t length )
{
	uint16_t i;
	//uint8_t crc = 0;
	uint8_t* _data = (uint8_t*)data;
    
	for (i = 0; i < length; i++)
	{
		crc = _crc8_update(crc, _data[i]);
	}
	return crc;
}
