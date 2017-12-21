/***********************************************************************************************//**
  @file         Monitor.h    
  @date         $Date:$        
  @version      $Revision:$    

  @brief        Serial Monitor

 **************************************************************************************************/

#ifndef __MONITOR_H
#define __MONITOR_H 

#include "ComTypeDefs.h"
#include "Console.h"

typedef struct cmd_def
{   
    const char *name;         // Command name
//    rom char (void)(*fptr)(void); // function pointer
    char (*fptr)(void); // function pointer
}CMDTABLE;

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
// Monitor
void Monitor_init(void);
void Monitor_update(void);

// Support routines
void Send_CR(void);
void Send_ROM_txt(const char *s);
void TX_printsRam( char *s );

//void TX_hex(uint8_t nibl);
void TX_hex2(uint8_t byte);
void TX_hex4(uint16_t word);
void TX_decUint32(uint32_t value);
void TX_decInt16(int16_t value);
void TX_decInt16rj(int16_t value);
void TX_decInt8(int8_t value);
void TX_printHours( uint32_t minutes );

//uint16_t hex2int(char *s);
//char char2hex (char digit);

#endif //__MONITOR_H
