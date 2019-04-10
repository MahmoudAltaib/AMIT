/*----------------------------------------------------------------------------
 * Name:    DSP_App.c
 * Purpose: 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "arm_math.h"
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "LED.h"
#include "Timer.h"
#include "ADC.h"
#include "DAC.h"
#include "DSP_IIR.h"



#define LED_CLK    7



/*----------------------------------------------------------------------------
   defines & typedefs for messages
 *----------------------------------------------------------------------------*/
typedef struct _DSP_MsgType {
  q31_t     Sample[DSP_BLOCKSIZE];
} DSP_MsgType;


osMailQDef(mbx_SigMod, 16, DSP_MsgType);
osMailQId mbx_SigMod;																				/* declare SigMod RTX mailbox */
osMailQDef(mbx_TimIrq, 16, DSP_MsgType);
osMailQId mbx_TimIrq;																				/* declare TimerIRQ RTX mailbox */
DSP_MsgType *pMsgTimIrqOut;
DSP_MsgType *pMsgTimIrqIn;
uint32_t     msgTimIrqOutIdx;
uint32_t     msgTimIrqInIdx;


/*----------------------------------------------------------------------------
   TaskIDs
 *----------------------------------------------------------------------------*/



#define OutAmpl      511UL                                /* Output Amplitute (Range  0 - 0.99)     */
#define TimerFreq  32000UL                                /* 32KHz Timer Frequency (used for AD/DA) */


float32_t tmpFilterIn;           /* 'global'  to display in LogicAnalyzer */
float32_t tmpFilterOut;          /* 'global'  to display in LogicAnalyzer */
/*----------------------------------------------------------------------------
   TIM2 IRQ Handler
 *----------------------------------------------------------------------------*/
void TIMER2_IRQHandler(void) {
uint32_t  adGdr;
osEvent evt;
q31_t     tmp;

    /* -- signal Input Section ------------------------------------------------- */
    adGdr = LPC_ADC->ADGDR;
    if (adGdr & (1UL << 31)) 								/* Data available ? */
	{						      			
      /* descale value and move it in positive/nagative range.   (12bit Ad = 0xFFF)
         filter in range is -1.0 < value < 1.0                */
		tmpFilterIn =  ((float32_t)((adGdr >> 4) & 0xFFF) / (0xFFF / 2)) - 1;
		arm_float_to_q31(&tmpFilterIn, &tmp, 1); 

		pMsgTimIrqOut->Sample[msgTimIrqOutIdx++] = tmp;

		if (msgTimIrqOutIdx >= DSP_BLOCKSIZE) 
		{
			osMailPut(mbx_SigMod, pMsgTimIrqOut);          			/* send the message */
			pMsgTimIrqOut = osMailAlloc (mbx_SigMod,0);         	/* allocate a message */
			msgTimIrqOutIdx = 0;
		}
    }
     
    /* -- signal Output Section ------------------------------------------------ */
    if (pMsgTimIrqIn == NULL)
	{
		evt = osMailGet(mbx_TimIrq,0); 
		if(evt.status == osEventMail)
		{
			pMsgTimIrqIn = evt.value.p;
			msgTimIrqInIdx = 0;	
		}
    }
    if (pMsgTimIrqIn != NULL) {

		tmp = pMsgTimIrqIn->Sample[msgTimIrqInIdx++];
		arm_q31_to_float(&tmp, &tmpFilterOut, 1);
      /* move value in positive range and scale it.   (10bit DA = 0x3FF)
         filter OUT range is -1.0 < value < 1.0                */
		LPC_DAC->DACR = (((uint32_t)((tmpFilterOut + 1) * (0x03FF / 2))) & 0x03FF) <<  6;

		if (msgTimIrqInIdx >= DSP_BLOCKSIZE) 
		{
  			osMailFree(mbx_TimIrq, pMsgTimIrqIn);
			pMsgTimIrqIn = NULL;
		}       
    }
	LPC_TIM2->IR |= (1UL <<  0);                          		/* clear MR0 Interrupt  flag   */
}


/*---------------------------------------------------------------------------
   Task 'Signal Modify'
 *---------------------------------------------------------------------------*/
  void SigMod (void) {
 
DSP_MsgType *pMsgIn;                                   		 	/* IN  message  */
DSP_MsgType *pMsgOut;                                   		/* OUT message */
osEvent evt;
		
	for (;;) 
	{
		evt = osMailGet (mbx_SigMod, osWaitForever);   			/* Wait for a message */
		if(evt.status == osEventMail)
		{
			pMsgIn = evt.value.p;
		}
		pMsgOut = osMailAlloc(mbx_TimIrq, osWaitForever);		/* allocate a message */
    
		iirExec_q31 (pMsgIn->Sample, pMsgOut->Sample);

		osMailPut(mbx_TimIrq, pMsgOut);
             
		osMailFree(mbx_SigMod, pMsgIn);							/* free memory allocated for message */
	}
}

/*----------------------------------------------------------------------------
   Task 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
  void Clock (void) {

	for (;;) 
	{
		osDelay (10);                                     	/* delay 60 clock ticks */
		LED_On (LED_CLK);
		osDelay (4);                                     	/* delay 40 clock ticks */
		LED_Off(LED_CLK);
	}
}


osThreadId main_t,tid_SigMod,tid_Clock;
osThreadDef(SigMod, osPriorityNormal, 1,0);
osThreadDef(Clock, osPriorityNormal, 1,0);
/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

	LED_Init ();                                            /* initialize the LEDs */
	TIM2_Init (TimerFreq);                                  /* initialize Timer 2 (used for DAC) */
	ADC_Init ();                                            /* initialize ADC Controller */
	DAC_Init ();                                            /* initialize DAC Controller */

	iirInit_q31 ();
  
	mbx_SigMod= osMailCreate(osMailQ(mbx_SigMod), NULL);	/* initialize SigMod mailbox */	
	mbx_TimIrq= osMailCreate(osMailQ(mbx_TimIrq), NULL);	/* initialize TimerIRQ mailbox */
          
	tid_Clock  = osThreadCreate(osThread(Clock), NULL);     /* start task clock */
	tid_SigMod = osThreadCreate(osThread(SigMod), NULL);    /* start task Signal Modify */ 

	pMsgTimIrqOut = osMailAlloc(mbx_SigMod, osWaitForever); /* allocate a message */
	pMsgTimIrqIn  = NULL;
	msgTimIrqOutIdx = 0;
	msgTimIrqInIdx  = 0;

	TIM2_Start ();                                          /* start Timer 2 (used for Signal IN/OUT) */

	osDelay(osWaitForever);                                     
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
