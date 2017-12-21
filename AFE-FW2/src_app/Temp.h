/***********************************************************************************************//**
  @file         Temp.h
  @date         $Date:  $      
  @version      $Revision:  $    

  @brief        Header file for Temp.c - Read LM75B Temperature Sensor   

 **************************************************************************************************/

#ifndef __TEMP_H
#define __TEMP_H


/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

extern Result_t temp_readTemperature(int16_t * temp);
extern Result_t temp_printTemperature( void );

#endif //__TEMP_H
