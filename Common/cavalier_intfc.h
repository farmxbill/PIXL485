/**********************************************************************************************//**
  @file       cavalier_intfc.h
  @date       $Date$
  @revision   $Revision$

  @brief      PROfile soil sensor interface
**************************************************************************************************/

#ifndef _CAVALIER_INTFC_H
#define _CAVALIER_INTFC_H

/***************************************************************************************************
 *                                            INCLUDES
 ***************************************************************************************************/
#include <stdint.h>
#include "ComTypeDefs.h"
#include "pixl_intfc.h"


/***************************************************************************************************
 *                                            MACROS
 ***************************************************************************************************/

// Should this be tied to max radio packet length (NP)?
#define MAX_CAV_PACKET_SIZE	256

#define MAX_ANALOG_SENSORS	6  		// includes both external and internal sensors

// Analog Sensor ID's
//    External
#define	ANASEN_ID_ANA1		0
#define	ANASEN_ID_ANA2		1
#define	ANASEN_ID_ANA3		2
#define	ANASEN_ID_VBATT		3

//   Internal
#define	ANASEN_ID_VDD		4
#define	ANASEN_ID_TEMP		5

#define	ANASEN_ID_INVALID	255

#define MAX_HALF_RDIV_ID	ANASEN_ID_VDD

#ifndef __XC__
#pragma pack(push,1)
#endif


/***************************************************************************************************
 *                                       Cavalier OTA Message Structs
 ***************************************************************************************************/

enum cavalier_cmds
{
	SUBTYPE_SoilSensorData 					= 0x11,
	SUBTYPE_AnalogData 						= 0x22
};

typedef struct CavalierOTAHeader
{
	uint16_t		fw_ver;				// Cavalier FW Version: maj|min|rev|bld
	uint8_t			seq_num;			// Packet Sequence number
	uint8_t			hw_ver;				// Cavalier HW Version: maj|min

} CavalierOTAHeader_t;

// *** Soil Sensor SubPacket ***

typedef struct SoilSensorDataSubPacketHdr
{
	uint8_t			subpkt_type;		// Sub Packet Type (==SUBTYPE_SoilSensorData)
	uint8_t			subpkt_len;			// Remaining Sub Packet Length, not including type & len
	uint8_t			num_sensors;		// expected & reporting
	uint8_t			samp_delay;			// # samples delayed (0= current)
	uint16_t		sss_fwver;			// Soil Sensor FW Ver
    uint16_t        sss_hwver;			// Soil Sensor HW Ver

	//DetailedSensorDataWHdr_t sensor[PIXL_MAX_NUM_SENSORS];

} SoilSensorDataSubPacketHdr_t;


//typedef struct SoilSensorAggregatedDataWHdr
//{
//	CavalierOTAHeader_t			h;
//	SoilSensorAggregatedData_t	d;
//
//} SoilSensorAggregatedDataWHdr_t;



// *** Analog Data SubPacket ***

typedef struct AnalogDataSubPacketHdr
{
	uint8_t				subpkt_type;		// Sub Packet Type (==SUBTYPE_AnalogData)
	uint8_t				subpkt_len;			// Remaining Sub Packet Length, not including type & len
	uint8_t				num_points;			// Number of analog sensor points
	uint8_t				samp_delay; 		// # samples delayed (0= current)

} AnalogDataSubPacketHdr_t;

typedef struct AnalogDataPoint
{
	uint16_t	ana_data:13;		// Analog data in mV's
	uint16_t	ana_id:3;			// Analog ID

} AnalogDataPoint_t;

//typedef struct AnalogDataSubPacket
//{
//	AnalogDataSubPacketHdr_t 	hdr;
//	AnalogDataPoint_t			data[MAX_ANALOG_SENSORS];	// packet may have less than max points
//
//} AnalogDataWHdr_t;


#ifndef __XC__
#pragma pack(pop)
#endif


#endif // _CAVALIER_INTFC_H
