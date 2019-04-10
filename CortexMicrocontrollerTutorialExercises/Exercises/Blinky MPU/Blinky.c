/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): possible defines set in "options for target - C/C++ - Define"
 *            __USE_LCD   - enable Output on LCD
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */

#include "Serial.h"
#include "LED.h"
#include "ADC.h"
                                                                         
char text[10];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;

#define SIZE_FIELD 							1
#define ATTRIBUTE_FIELD 					16
#define ACCESS_FIELD 						24
#define ENABLE 								1
#define ATTRIBUTE_FLASH 					0x4
#define ATTRIBUTE_SRAM						0x5
#define ATTRIBUTE_PERIPHERAL 				0x3
#define PRIV_RW_UPRIV_RW 					3
#define PRIV_RO_UPRIV_NONE 					5
#define PRIV_RO_UPRIV_RO 					6
#define PRIV_RW_UPRIV_RO 					2
#define USE_PSP_IN_THREAD_MODE 				2
#define THREAD_MODE_IS_UNPRIVILIGED 		1
#define PSP_STACK_SIZE 						0x200
#define TOP_OF_THREAD_RAM 					0x10007FF0
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) 
{
uint32_t ad_avg = 0;
uint16_t ad_val = 0, ad_val_ = 0xFFFF;
	
	/*Thread code */
	MPU->RNR = 0;
	MPU->RBAR=0x0000000;
	MPU->RASR |= (PRIV_RO_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_FLASH<<ATTRIBUTE_FIELD)|(17<<SIZE_FIELD)|ENABLE;
	
	/*Handler code*/
	MPU->RNR = 1;
	MPU->RBAR=0x00040000;
	MPU->RASR |=(PRIV_RO_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_FLASH<<ATTRIBUTE_FIELD)|(17<<SIZE_FIELD)|ENABLE;
	
	/*Thread RAM */
	MPU->RNR = 2;
	MPU->RBAR=0x10000000;
	MPU->RASR |=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_SRAM<<ATTRIBUTE_FIELD)|(14<<SIZE_FIELD)|ENABLE;
	
	/*Handler RAM */
	MPU->RNR = 3;
	MPU->RBAR=0x2007C000;
	MPU->RASR |=(PRIV_RW_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_SRAM<<ATTRIBUTE_FIELD)|(14<<SIZE_FIELD)|ENABLE;

	/*GPIO*/
	MPU->RNR = 4;
	MPU->RBAR=LPC_GPIO_BASE;
	MPU->RASR=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_PERIPHERAL<<ATTRIBUTE_FIELD)|(13<<SIZE_FIELD)|ENABLE;
	
	/*PERIPHERALS*/
	MPU->RNR = 5;
	MPU->RBAR=LPC_APB0_BASE;
	MPU->RASR=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_PERIPHERAL<<ATTRIBUTE_FIELD)|(19<<SIZE_FIELD)|ENABLE;
	
//	NVIC_EnableIRQ (MemoryManagement_IRQn);	Wrong !	Do not use this for cortex Processor exceptions 
	SCB->SHCSR = 0x01<<16;							//The Fault exceptions are enabled in the System Control Block
	MPU->CTRL = ENABLE;
	
	__set_CONTROL(USE_PSP_IN_THREAD_MODE);
	__set_PSP(TOP_OF_THREAD_RAM);
	
	LED_Init();                                /* LED Initialization            */
	SER_Init();                                /* UART Initialization           */
	ADC_Init();                                /* ADC Initialization            */

	SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */
	__set_CONTROL(USE_PSP_IN_THREAD_MODE|THREAD_MODE_IS_UNPRIVILIGED);

	while (1) {                                /* Loop forever                  */

    /* AD converter input                                                     */
    if (AD_done) {                           /* If conversion has finished    */
      AD_done = 0;

      ad_avg += AD_last << 8;                /* Add AD value to averaging     */
      ad_avg ++;
      if ((ad_avg & 0xFF) == 0x10) {         /* average over 16 values        */
        ad_val = (ad_avg >> 8) >> 4;         /* average devided by 16         */
        ad_avg = 0;
      }
    }

    if (ad_val ^ ad_val_) {                  /* AD value changed              */
      ad_val_ = ad_val;

      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */

    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;													//Error! Attempt to write to Priviliged memory from unpriviliged mode

      printf("AD value: %s\r\n", text);
    }
  }
}
