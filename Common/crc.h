/*!***************************************************************************************
  @file         crc.h
  @date         $Date$
  @version      $Revision$

  @brief	Functions for computing 8- and 16-bit CRCs.

****************************************************************************************/

#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>

//uint16_t compute_crc16(void* data, uint16_t length);

extern uint8_t compute_crc8( uint8_t crc, void* data, uint16_t length );

#endif
