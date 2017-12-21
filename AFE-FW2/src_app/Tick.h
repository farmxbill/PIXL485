/***********************************************************************************************//**
  @file         Tick.h
  @date         $Date:  $      
  @version      $Revision:  $    

  @brief        Header file for tick.C
   

 **************************************************************************************************/

#ifndef __TICK_H
#define __TICK_H


/*********************************************************/

extern volatile uint16_t    ticks;      //Increment @ 100Hz (10 ms) [sb the raw tick rate?]

extern volatile uint8_t    cnt_100ms;  //mod 10
extern volatile uint8_t    cnt_1000ms; //mod 100


typedef struct
{
    union
    {
        struct
        {
            uint8_t tick_10ms:1;
            uint8_t tick_100ms:1;   // 100ms Tick occurred
            uint8_t tick_1s:1;      // 1s Tick occurred            
            uint8_t tick_samp_complete:1;       // Freq sampling is complete
            
            uint8_t samp_state:3;     // Sampling state
            uint8_t unused:1;       // TBD
        } b;
        
        uint8_t tick_bits;          // For setting/getting all bits
    } u;
} sTickEvents;
extern volatile sTickEvents gTickEvents;


/* tick & Reload timing calculations */

#define T1_PRE  8L      //T1 prescale (Directly sets T2CKPSv bits)
#define T1_FREQ 100L    //T1 tick frequency

#define T2_FREQ 100L    //T1 tick frequency
#define T2_PRE  16L
#define T2_POST 10L
#define T2_PR_REG (FOSC4/T2_PRE/T2_POST/T2_FREQ)

#define T1_RELOAD_REG        (65536 - (FOSC4/T1_PRE/T1_FREQ))
#define T1_RELOAD_REG_H      (T1_RELOAD_REG >> 8)
#define T1_RELOAD_REG_L      (T1_RELOAD_REG % 256)

/* Lookup the T1 prescale reg setting */
#if T1_PRE == 1
#define T1CKPSv 0
#elif T1_PRE == 2
#define T1CKPSv 1
#elif T1_PRE == 4
#define T1CKPSv 2
#elif T1_PRE == 8
#define T1CKPSv 3
#else
#error  "==> Invalid T1 prescaler setting (T1CKPS) <=="         
#endif

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
extern void tick_init(void);
extern void tick_startFreqMeasurement( uint8_t period );
//extern void tick_doDelay( uint16_t period );

void TMR2_ISR(void);

#endif //__TICK_H
