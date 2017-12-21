/***********************************************************************************************//**
  @file         Monitor.c    
  @date         $Date:$        
  @version      $Revision:$    

  @brief        Serial Monitor

 **************************************************************************************************/

#include "ComTypeDefs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Monitor.h"
#include "Versions.h"
#include "Console.h"
#include "Measure.h"
#include "Temp.h"
#include "Tick.h"
#include "memory.h"
#include "NvmData.h"

#ifdef DEBUGMONITOR

#define Usart_RX_rdy()      ConsoleIsGetReady()
#define Usart_RX()          ConsoleGet()
#define Usart_TX(x)         ConsolePut(x)
#define Usart_TX_busy()     !ConsoleIsPutReady()

#define CMD_SIZE 20     // Max Input buffer size

#define MONITOR_OK 0
#define ERROR_SYN -1
#define ERROR_RANGE -2
#define ERROR_FAIL -3  

// Monitor states
//
enum cmd_states
{
    cmd_idle,
    cmd_collect,
    cmd_parse,
    cmd_exec,
};    

/**************************************************************************************************
*                                          EXTERNALS
**************************************************************************************************/
// From 'MainLoop.c'
extern uint8_t gSamplingActive;

/**************************************************************************************************
*                                          FUNCTIONS - LOCAL VARS
**************************************************************************************************/

static uint8_t  cmd_state;  
static uint8_t  cmd_idx;    //command line index

static char     *arg_ptr;   //command line argument pointer
static char     cmd_line[CMD_SIZE+1];

/**************************************************************************************************
*                                          CONST DATA
**************************************************************************************************/

#if 0
const char help_text[] =
{
    "Commands:\n"  
    "status\n"
    "reset\n"
    /*"EV {start} {end}\n"
    "EM <start> {dat8_1} {dat8_2} {dat8_3}...\n"
    "RV {start} {end}\n" */
    "start\n"
    "stop\n"
    "set {config} {# of samps}\n"
    "temp\n"
};    
#endif

const char CR_text[] =    {"\n"};    
const char pconstpt_text[]= {"\n=>"};

/* Error messages */
//const char cmd_error_text[]=  {"\nUnknown Err\n"};
//const char syn_error_text[]=  {"\nSyntax Err\n"};
//const char range_error_text[]={"\nRange Err\n"};
//const char fail_error_text[]= {"\nFAIL\n"};

#define cmd_error_text  fail_error_text
#define syn_error_text  fail_error_text
#define range_error_text  fail_error_text
const char fail_error_text[]= {"\nERR\n"};

/**************************************************************************************************
*                                          FUNCTIONS - LOCAL PROTOTYPES
**************************************************************************************************/
 
//char excmd_help(void);
//char excmd_ev(void);
//char excmd_rv(void);
char excmd_status(void);
//char excmd_reset(void);
//char excmd_em(void);
char excmd_startsamp(void);
char excmd_stop(void);
char excmd_set(void);
//char excmd_temp(void);

// Command jump table
//    
const CMDTABLE command_table[] =
{
//    { "?",          excmd_help          },
//    { "HELP",       excmd_help          },
    { "STATUS",     excmd_status        },
//    { "EV",         excmd_ev            }, 
//    { "EM",         excmd_em            },
//    { "RESET",      excmd_reset         },
//    { "RV",         excmd_rv            },
    { "START",      excmd_startsamp     },  
    { "STOP",       excmd_stop          },  
    { "SET",        excmd_set           },  
//    { "TEMP",       excmd_temp          },  
    { 0, 0} 		     
};     

/**************************************************************************************************
*                                          FUNCTIONS - PUBLIC
**************************************************************************************************/

/**********************************************************************************************//**
 * @brief   Initilaize the monitor
 *
 * @param   none
 * 
 * @return  none
 ***************************************************************************************************/    
void Monitor_init(void)
{
    cmd_state = cmd_idle;
    cmd_idx = 0;
    //Send_ROM_txt(startup_text);
}

/**********************************************************************************************//**
 * @brief   This is the monitor state machine. It should be called periodicaly fconst the main loop.   
 *     
 * @param   none
 * 
 * @return  none
 ***************************************************************************************************/
void Monitor_update(void)
{
    switch(cmd_state)
    {
        case cmd_idle:
//            Send_ROM_txt(pconstpt_text);
            cmd_idx = 0;
            cmd_state = cmd_collect;               
            break;
        
        case cmd_collect:
            if(Usart_RX_rdy())
            {
                uint8_t cmd_char;   //temp char
                cmd_char = Usart_RX();       
                
                /* test for special chars... */
                     
                if(cmd_char == '\b')
                {
                    if(cmd_idx)
                    {
                        Usart_TX('\b'); //back up   
                        Usart_TX(' ');  //Over strike
                        Usart_TX('\b'); //back up again
                        cmd_idx--;
                    }    
                }
                else if(cmd_char == '\r')
//            else if(cmd_char == '\n')
                {
                    cmd_line[cmd_idx] = '\0';   //make eol
                    strupr (cmd_line);
                    
                    if(!cmd_idx)                    //Blank line?            
                        cmd_state = cmd_idle;
                    else                    
                        cmd_state = cmd_parse;                
                }
                else if(cmd_char == '\x1B')     //Escape
                    cmd_state = cmd_idle;        
                    
                else if (cmd_idx > CMD_SIZE)
                    Usart_TX(0x07); //bell
                else 
                {       
                    cmd_line[cmd_idx++] = cmd_char;
                    Usart_TX(cmd_char);     //Echo the char
                }
            }//No char ready        
            break;
        
        case cmd_parse: //parsing & call the exec routines
        {
            const CMDTABLE *cmdptr = command_table;
            char matched = 0;
            int8_t result = 0;     //Assume all ok
        		// Search for new command
            if((arg_ptr = strtok(cmd_line, (const char *)", ")) != 0)
            {
		// Search command table
                while(cmdptr->name != 0)
                {
                    if(strcmp(arg_ptr, (const char *)cmdptr->name) == 0)
                    {
                        matched = 1;
                        break;
                    }
                cmdptr++;
                }
            } //No command found
            Send_CR();
		    if(matched)
                result = (cmdptr->fptr)();  // Call function, return flag            		
            else	// no match
                Send_ROM_txt(cmd_error_text);   // Unknown command			
                
/* After execution, print an error message if there was a return */                

            if (result == ERROR_SYN)
                Send_ROM_txt(syn_error_text);   // syntax error	
                
            if (result == ERROR_RANGE)
                Send_ROM_txt(range_error_text);   // bad range	
                
            if (result == ERROR_FAIL)
                Send_ROM_txt(fail_error_text);   // called routine  fails                
                
            cmd_state = cmd_idle;
		}		
			break;
    } // End Switch
}    


/**************************************************************************************************
*                     Support helper routines for the monitor & debug
**************************************************************************************************/

//       
// Send a const based string to the terminal 
// (prepending CR to LF if enabled)
//
void Send_ROM_txt(const char *s)
{
    while(*s)
    {
#if 1   /* Add a cr to every linefeed */       
        if(*s == '\n')
        {
            Usart_TX('\r');
        }
#endif            
        Usart_TX(*s++);
    }    
}     
//
// Send CR,LF to the terminal 
//        
void Send_CR(void)
{
    Send_ROM_txt(CR_text);
} 
// 
// send a hex nibble
//
/**********************************************************************************************//**
 * @brief   Print out single hex digit (nibble)
 *
 * @param   value - int8_t to print
 ***************************************************************************************************/
void TX_hex(uint8_t nibl)
{
    Usart_TX(nibl + (nibl > 9 ? ('A'-10) : '0'));
}    

/**********************************************************************************************//**
 * @brief   Print out 2 digit hex byte (nibble)
 *
 * @param   value - int8_t to print
 ***************************************************************************************************/
void TX_hex2(uint8_t byte)
{
    TX_hex(byte >> 4);
    TX_hex(byte & 0x0F);
}    

/**********************************************************************************************//**
 * @brief   Print out four digit hex word
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_hex4(uint16_t word)
{
    TX_hex2(word >> 8);
    TX_hex2(word & 0xFF);
}      

//static char charbuf[7];
/**********************************************************************************************//**
 * @brief   Print out uint16 decimal 
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_decUint32(uint32_t value)
{
    char charbuf[12];
    
    ultoa( charbuf, value, 10 );
    TX_printsRam( charbuf );
}

/**********************************************************************************************//**
 * @brief   Print out int16 decimal 
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_decInt16(int16_t value)
{
    char charbuf[7];
    
    itoa( charbuf, value, 10 );
    TX_printsRam( charbuf );
}

/**********************************************************************************************//**
 * @brief   Print out int16 decimal right had justified
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_decInt16rj(int16_t value)
{
    char charbuf[7];
    uint8_t i;
    
    itoa( charbuf, value, 10 );
    i = strlen(charbuf);
    for(;i<4;i++)
        Usart_TX(' ');
    TX_printsRam( charbuf );
}

/**********************************************************************************************//**
 * @brief   Print out int8 decimal 
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_decInt8(int8_t value)
{
    char charbuf[5];
    
    itoa( charbuf, value, 10 );
    TX_printsRam( charbuf );
}
  
/**********************************************************************************************//**
 * @brief   Print out RAM based string
 *
 * @param   value - int16_t to print
 ***************************************************************************************************/
void TX_printsRam( char *s )
{    
    while(*s)
    {
#if 0   /* Add a cr to every linefeed */       
        if(*s == '\n')
        {
            Usart_TX('\r');
        }
#endif            
        Usart_TX(*s++);
    }    
}

//
// convert a hex (ascii) digit to an int8 (0..15) 
//   or return -1 if invalid digit
//
char char2hex (char digit)
{
    if(isxdigit(digit))
        return (digit - (digit < 'A' ? '0' : ('A' -10)));
    return(-1);
}
//
// Convert a string of hex to an int
// return -1 if error
//
uint16_t hex2int(char *s)
{
    uint16_t value = 0;
    int8_t digit;
    
    if(!isxdigit(*s))   //Must start with a digit
        return(-1);
    
    while(*s)
    {
        if(value > 0x0FFF)  //Overflow?    
            return(-1);        
        
        if((digit = char2hex(*s++)) == -1)
            return(-1);
            
        value = (value << 4) + digit;
    }    
    return(value);    
} 
   
     
/**************************************************************************************************
*                                          FUNCTIONS - LOCAL MONITOR FUNCTIONS
**************************************************************************************************/

#if 0    
//////////////////////////////////////////////////////
//
// Read a series of eepconst bytes
//
char excmd_ev(void)
{
    uint16_t start_adr;
    uint16_t end_adr;
    uint8_t i;


    if(!(arg_ptr = strtok(0, (const char *)", ")))
        return(ERROR_SYN); //Error syntax
        
    if((start_adr = hex2int(arg_ptr)) == -1)
        return(ERROR_SYN);
        
    if(!(arg_ptr = strtok(0, (const char *)", ")))
        end_adr = start_adr + 15;
    else
    {
        if((end_adr = hex2int(arg_ptr)) == -1)
            return(ERROR_SYN); 
            
        if((start_adr > end_adr) || (end_adr - start_adr > 0x8000))
            return(ERROR_RANGE); //range error      
    } 
    while(start_adr <= end_adr)
    {
//        if((i) || ((start_adr & 0x07) == 0))   //On 1st line & every 8 bytes, start a new line
        if((i) || ((start_adr & 0x0F) == 0))    //On 1st line & every 8 bytes, start a new line
        {
            Send_CR();
            TX_hex4(start_adr);
            Usart_TX(':');           
        }
        Usart_TX(' ');
        NVMRead( &i, start_adr++, 1 );
        TX_hex2(i);
        i = 0;      //Also used as a first line flag
    }   
    return(MONITOR_OK);
}
#endif  

#if 0
///////////////////////////////////////////
//
// Write a series of eepconst bytes
//
char excmd_em(void)  
{
    uint16_t start_adr;
    uint16_t data;
    uint8_t i;
  
    if(!(arg_ptr = strtokpgmram(0, (MEM_MODEL const char *)", ")))
        return(ERROR_SYN); //Error syntax
        
    if((start_adr = hex2int(arg_ptr)) == -1)
        return(ERROR_SYN);
        
    while(1)
    {        
        if(!(arg_ptr = strtokpgmram(0, (MEM_MODEL const char *)", ")))
            return(MONITOR_OK);  //All OK
    
        if((data = hex2int(arg_ptr)) == -1)
            return(ERROR_SYN); 
            
        if(data >256)           
            return(ERROR_RANGE); //range error   
            
        i = data;            
       
        NVMWrite( &i, start_adr, 1 );        
        start_adr++;
    }  
    return(MONITOR_OK);
}
#endif

#if 0
//////////////////////////////////////////////////////
//
// Display a series of ram bytes
//
char excmd_rv(void)
{
    uint16_t start_adr;
    uint16_t end_adr;
    uint8_t i;
    uint8_t *ram_ptr;
    
    if(!(arg_ptr = strtok(NULL, (const char *)", ")))
        return(ERROR_SYN); //Error syntax
        
    if((start_adr = hex2int(arg_ptr)) == -1)
        return(ERROR_SYN);
        
    if(!(arg_ptr = strtok(NULL, (const char *)", ")))
        end_adr = start_adr + 15;
    else
    {
        if((end_adr = hex2int(arg_ptr)) == -1)
            return(ERROR_SYN); 
            
        if((start_adr > end_adr) || (end_adr - start_adr > 0x8000))
            return(ERROR_RANGE); //range error      
    } 
    while(start_adr <= end_adr)
    {
//        if((i) || ((start_adr & 0x07) == 0))   //On 1st line & every 8 bytes, start a new line
        if((i) || ((start_adr & 0x0F) == 0))    //On 1st line & every 8 bytes, start a new line
        {
            Send_CR();
            TX_hex4(start_adr);
            Usart_TX(':');           
        }
        Usart_TX(' ');
//        if(NvReadData( start_adr++, &i, 1) != RESULT_SUCCESS) 
  //          return(ERROR_FAIL);
        ram_ptr = (uint8_t *)start_adr++;
        TX_hex2(*ram_ptr);
        i = 0;      //Also used as a first line flag
    } 
    return(MONITOR_OK);
}
#endif

#if 0
//////////////////////////////////////////////
//
// Send the help text to the terminal 
//
char excmd_help(void)
{
    //printFwVersion();
    Send_ROM_txt(help_text);
    return(MONITOR_OK);
}    
#endif


char excmd_status(void)
{
    //Send_ROM_txt("SP:");
    //TX_hex2(STKPTR);
    Send_ROM_txt("\r\nOsc Delay: ");
    TX_decInt8(gOscDelay);
    Send_ROM_txt("ms, Meas Period: ");
    TX_decInt8(gMeasPeriod*10);
    Send_CR(); 
            
    return(MONITOR_OK);
}    

#if 0
/*****************************************/
/* Reset command */
const char Reset_text[] = {"Reseting...\n"};     
char excmd_reset(void)
{
    Send_ROM_txt(Reset_text);
    while (Usart_TX_busy())
    ;
    Reset();    
    return(MONITOR_OK);
}
#endif

/*****************************************/
/* startsamp command                     */
/*****************************************/ 
const char StartSamp_text[] = {"Sampling Started\n"};     
char excmd_startsamp(void)
{
    Send_ROM_txt(StartSamp_text);
    gSamplingActive = 1;

    return(MONITOR_OK);
}

/*****************************************/
/* stop sampling command                     */
/*****************************************/ 
  
char excmd_stop(void)
{
    gSamplingActive = 0;
    measure_setVref( 0 );       // for good measure

    return(MONITOR_OK);
}

#if 1
/*****************************************/
/* set command                    */
/*****************************************/     
char excmd_set(void)
{
    int8_t  osc;
    int8_t  num_samps;
    uint8_t i;

    if(!(arg_ptr = strtok(0, (const char *)", ")))
        return(ERROR_SYN); //Error syntax
        
    if((osc = atoi(arg_ptr)) == -1)
        return(ERROR_SYN);
        
    if(!(arg_ptr = strtok(0, (const char *)", ")))
    {
        num_samps = 0;
    }
    else
    {
        if((num_samps = atoi(arg_ptr)) == -1)
            return(ERROR_SYN);        
    }
    
    if( osc == OscSelect_OFF )
    {
        measure_setVref( 0 ); 
        return(MONITOR_OK);
    }
   
    if(osc > OscSelect_10MHZ_MEAS)
        return(ERROR_RANGE);
        
    measure_setVref( 1 );   // turn on VREF (& leave it on)
    
    if( num_samps == 0 )
    {
        // Just configure and leave it
        measure_configOsc( osc );
    }
    else if((osc==OscSelect_50MHZ_CALIB) || (osc==OscSelect_10MHZ_CALIB))
    {
        return(ERROR_RANGE);
    }
    else
    {
        // run samples measurement
        for( i=0; i<num_samps; i++ )
        {
            uint32_t meas_val;
            uint32_t cal_val;
 
            measure_doMeasurement( osc, 
                       gOscDelay, gMeasPeriod,
                       &cal_val, &meas_val );
            
            TX_decUint32(cal_val);        
            Printf(", ");         
            TX_decUint32(meas_val);             
            Send_CR();
        }
    }
    
    return(MONITOR_OK);
}
#endif

/*****************************************/
/* stop sampling command                     */
/*****************************************/ 
  
#if 0
char excmd_temp(void)
{
    //uint8_t i;
    
    //for(i=0; i<100; i++)
    //{
        if( temp_printTemperature() != RESULT_SUCCESS )
        {
            return(ERROR_FAIL);
        }   
     
        //while(gTickEvents.u.b.tick_1s == 0)
        //    ;
        //gTickEvents.u.b.tick_1s = 0;
    //}
        
    return(MONITOR_OK);
}
#endif

#endif // DEBUGMONITOR