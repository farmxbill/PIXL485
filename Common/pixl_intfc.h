/**********************************************************************************************//**
  @file       pixl_intfc.h
  @date       $Date$
  @revision   $Revision$

  @brief      Pixl soil sensor interface
**************************************************************************************************/

#ifndef _PIXL_INTFC_H
#define _PIXL_INTFC_H

/***************************************************************************************************
 *                                            INCLUDES
 ***************************************************************************************************/
#include <stdint.h>
#include "ComTypeDefs.h"


/***************************************************************************************************
 *                                            MACROS
 ***************************************************************************************************/

#define PIXL_MAX_NUM_SENSORS		4

#define FRAME_START_DELIMITER      0x7e

#define DETAIL_DATA_PCKD_LEN	3

#define REF_CNT_IDX		0
#define MEAS_CNT_IDX	1

#define PIXL_FRAME_HEADER_LEN    2		// sizeof(PixlHeader_t)

#define PIXL_SLOT_MASK		0x03
#define PIXL_FRAME_MASK		0xf0

// Max frame type size, not including header & CRC
#define PIXL_MAX_FRAME_SIZE		20

#define PROF_FRAME_HEADER_LEN    sizeof(ProfileHeader_t)


/***************************************************************************************************
 *                                            PIXL Structs
 ***************************************************************************************************/
// Message formats

#ifndef __XC__
#pragma pack(push,1)
#endif

enum pixl_types
{
	PIXL_DetailedSensor 		= 0x01,
	PIXL_CompressedSensor 		= 0x02
};

enum profile_types
{
	PROFTYPE_DetailedSensor 		= 0x01
};

typedef struct PixlFWVer
{
	uint8_t		maj:4;
	uint8_t		min:4;
	uint8_t		rev;
} sPixlFWVer;

typedef struct PixlHeader
{
	uint8_t			length;			// data field length
	//sPixlFWVer		fwver;
    //uint16_t        hwver;
    uint8_t         frame_slot;

	//uint8_t			frame[0];		// Start of Frame data
} PixlHeader_t;

typedef struct ProfileHeader
{
	uint8_t		length;			// data field length
	sPixlFWVer  fwver;
    uint16_t    hwver;
    uint8_t     frame_slot;
    uint8_t     sn0;
    uint8_t     sn1;
    uint8_t     sn2;
    uint8_t     sn3;
    uint8_t     sn4;
    uint8_t     sn5;
    uint8_t     sn6;
    uint8_t     sn7;
    uint8_t     sn8;
    uint8_t     sn9;    
	//uint8_t			frame[0];		// Start of Frame data
} ProfileHeader_t;

typedef struct PckdDetailedDataPoint
{
	uint8_t		data[DETAIL_DATA_PCKD_LEN];		// 2x 24b values
} PckdDetailedDataPoint_t;

typedef struct DetailedDAFESensorData
{
	uint32_t	celsius;
    uint32_t    diff12;
    uint32_t    single3;
} DetailedDAFESensorData_t;

typedef struct DetailedPIXLSensorData
{
	uint16_t				temper;			// Temperature (units of 0.125 degC)
	PckdDetailedDataPoint_t	f10mhz[2];		// 0: ref, 1: meas
	PckdDetailedDataPoint_t	f50mhz_up[2];
	PckdDetailedDataPoint_t	f50mhz_low[2];    
} DetailedPIXLSensorData_t;

typedef struct DetailedPIXLSensorDataWHdr
{
	PixlHeader_t			hdr;
	DetailedPIXLSensorData_t	data;
} DetailedPIXLSensorDataWHdr_t;

typedef struct DetailedDAFESensorDataWHdr
{
	PixlHeader_t			hdr;
	DetailedDAFESensorData_t	data;
} DetailedDAFESensorDataWHdr_t;

#ifndef __XC__
#pragma pack(pop)
#endif

#endif // _PIXL_INTFC_H
