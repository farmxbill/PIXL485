/************************************************************************************************//**
  @file         Tick.c			
  @date         $Date:  $        
  @version      $Revision:  $    

  @brief        Provide RTC & timer support

 **************************************************************************************************/

#include <xc.h>
#include "ComTypeDefs.h"
#include "PortDef.h"
#include "Tick.h"

/**************************************************************************************************
*                                            TYPEDEFS
**************************************************************************************************/

typedef enum
{
    SampState_OFF = 0,            // 00: START
    SampState_PRETRIGGER,         // 01: MFG_TEST
    SampState_SAMPLING,           // 02: DAY_RSU
    SampState_DONE,               // 03: DAY_USD

} eSampState;

/**************************************************************************************************
*                                            LOCAL VARS
**************************************************************************************************/

static volatile uint8_t samp_state;
static volatile uint8_t samp_period;

/**************************************************************************************************
*                                            PUBLIC VARS
**************************************************************************************************/

/* Tick related counters */
volatile uint16_t   ticks;      //Increment @ 100Hz (10 ms)

volatile uint8_t    cnt_100ms;  //mod 10
volatile uint8_t    cnt_1000ms; //mod 100

volatile sTickEvents gTickEvents;


/**********************************************************************************************//**
 * @brief tick_init() - Initialize Tick & hardware & routines
 *          
 * @param   NONE
 *
 * 
 * @return  NONE
 ***************************************************************************************************/  
void tick_init(void)
{
    //T2CON: 0100 1011:  PRE=16, POST=10
    T2CON = 0x4B;
    PR2 = T2_PR_REG;
    TMR2 = 0;

/* Timer 2 IRQ setup */	

    //IPR1bits.TMR2IP = 1;    //This is a high priority
    PIR1bits.TMR2IF = 0;    //Clear possible pending IRQ
    PIE1bits.TMR2IE = 1;    //Enable the IRQ
    
    T2CONbits.TMR2ON = 1;   //Enable the timer
    
    gTickEvents.u.tick_bits = 0;

    ticks = 0;
    cnt_100ms = 10;
    cnt_1000ms = 10;     
    
    samp_state = SampState_OFF;
}	 
  
/**********************************************************************************************//**
 * @brief tick_startFreqMeasurement - Begin synchronized frequency measurement period
 *          
 * @param   period     uint8_t - Measurement period in 10ms units
 *                                                                                                 
 * @prereq  TMR0_PrepareCounting() must be called prior 
 *         
 ***************************************************************************************************/
void tick_startFreqMeasurement( uint8_t period )
{
    samp_state = SampState_OFF;         // this is just incase an IRQ occurs
    gTickEvents.u.b.tick_samp_complete = 0;
    samp_period = period;
    samp_state = SampState_PRETRIGGER;  // Let's do this
}                                

#if 0
/**********************************************************************************************//**
 * @brief   tick_doDelay - Do Dumb Delay
 *          
 * @param   period     uint16_t - Measurement period in 10ms units
 *          - Note first delay period will be from 0ms-10ms                                                                                       
 *         
 ***************************************************************************************************/
void tick_doDelay( uint16_t period )
{
    while( (period--) > 0)
    {
        gTickEvents.u.b.tick_10ms = 0;
        while(gTickEvents.u.b.tick_10ms == 0)
            ;
    }
}   
#endif

/**********************************************************************************************//**
 * @brief Tick_isr(void) - IRQ service for the tick TMR1 routines
 *
 * @param   NONE
 *          
 * @return  NONE
 ***************************************************************************************************/ 
void TMR2_ISR(void)
{
    // Clear the TMR2 interrupt flag
    PIR1bits.TMR2IF = 0;
    
    //// Write to the Timer1 register (reload)
    //TMR1H = T1_RELOAD_REG_H;
    //TMR1L = T1_RELOAD_REG_L;
    
    ticks++;                    //Keep track of total ticks (for now)        

    /* things that happen at 10ms */        
    
    gTickEvents.u.b.tick_10ms = 1;  //Tell the App		           
   
    // Sampling state machine
    switch(samp_state)
    {
        case SampState_PRETRIGGER:
            TMR0_StartCounting();
            DBG_SetHigh();
            samp_state = SampState_SAMPLING;
            break;
            
        case SampState_SAMPLING:
            if((--samp_period) == 0)
            {
                TMR0_StopCounting();
                DBG_SetLow();
                samp_state = SampState_DONE;
                gTickEvents.u.b.tick_samp_complete = 1;
            }
            break;
           
        default:
            break;            
    }
    
    //DBG_Toggle();
    
    /* ===== test for 100ms tick period ===== */
    cnt_100ms--;
    if (!cnt_100ms)
    {
        cnt_100ms = 10;
        gTickEvents.u.b.tick_100ms = 1;   // 100ms Tick occurred    
        
        /***** 100 ms events here ****/
        //DBG_Toggle();
        
    /* ===== test for 1000ms tick period ===== */        
        cnt_1000ms--;
        if(!cnt_1000ms)
        {
            cnt_1000ms = 10;
            gTickEvents.u.b.tick_1s = 1;      // 1s Tick occurred 
            
            /**** 1000ms events here *****/
        }
    }
} //End of tick ISR
