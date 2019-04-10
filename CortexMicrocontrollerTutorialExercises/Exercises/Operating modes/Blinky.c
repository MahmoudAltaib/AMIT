/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Advanced Operating Modes Example

*----------------------------------------------------------------------------*/

#include <stdio.h>
#include "STM32F10x.h"
#include "LCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

char text[10];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;

/* variable demonstrating trace  capabilities */
uint16_t AD_DbgVal;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
#define USE_PSP_IN_THREAD_MODE (1<<1)
#define THREAD_MODE_IS_UNPRIVILIGED 1
#define PSP_STACK_SIZE 0x200											/* Define the Process stack space size in bytes */ 

int main (void) 
{
uint32_t initalPSPValue;
uint32_t basePri;
uint32_t ad_avg = 0;
uint16_t ad_val = 0, ad_val_ = 0xFFFF;
	
	initalPSPValue = __get_MSP() + PSP_STACK_SIZE;						/* 	Calculate the starting address for the Process Stack */
	__set_CONTROL(USE_PSP_IN_THREAD_MODE);								/*	Enable the Process Stack */
	__ISB();															/*	Instruction Barrier to ensure PSP is enables before proceeding */
	__set_PSP(initalPSPValue);											/*	Set the process stack to the calculated start address	*/
	__ISB();															/* 	Again use an instruction barrier to ensure instuction completes before proceeding */
  
	LED_Init();                                							/*	Now configure the user peripherals and sys tick interrupt */
	SER_Init();                                
	ADC_Init();                                
	SysTick_Config(SystemCoreClock / 100);  
	
	__set_CONTROL(USE_PSP_IN_THREAD_MODE								/*	Once the interrupts have been configured give Thread mode execution unpriviliged access */
				|THREAD_MODE_IS_UNPRIVILIGED);
	__ISB();															/* 	Instruction barrier to ensure instuction completes before proceeding */
	basePri = __get_BASEPRI (); 										/*	Read the current value of the Base Priority register */
	
	while (1) {                                

                                                    
    if (AD_done) {                           
      AD_done = 0;

      ad_avg += AD_last << 8;                
      ad_avg ++;
      if ((ad_avg & 0xFF) == 0x10) {        
        ad_val = (ad_avg >> 8) >> 4;        
        ad_avg = 0;
      }
    }

    if (ad_val ^ ad_val_) {                  
      ad_val_ = ad_val;

      AD_DbgVal = ad_val;

      sprintf(text, "0x%04X", ad_val);       

    }

    
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
