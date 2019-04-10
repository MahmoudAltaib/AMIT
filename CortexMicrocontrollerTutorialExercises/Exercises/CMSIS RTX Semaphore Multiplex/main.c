/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Multiplex Example

*----------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cmsis_os.h"

#include "LED.h"

void  multiplex_Thread (void const *argument);

osThreadDef(multiplex_Thread, osPriorityNormal, 6, 0);

osThreadId T_mux1;																			
osThreadId T_mux2;
osThreadId T_mux3;
osThreadId T_mux4;
osThreadId T_mux5;
osThreadId T_mux6;

osSemaphoreId semMultiplex;						//declare the Semaphore
osSemaphoreDef(semMultiplex);
osSemaphoreId semSignal;
osSemaphoreDef(semSignal);

/*******************************************************************
Create a thread and use the Semaphore to control the number of threads 
running simultaneously
********************************************************************/

void multiplex_Thread (void const *argument) 
{
	for (;;) 
	{
    	osSemaphoreWait(semMultiplex,osWaitForever);
		LED_Toggle((uint32_t)argument);
		osDelay(100);
		LED_Toggle((uint32_t)argument);
		osDelay(100);
		osSemaphoreRelease(semMultiplex);
	}
}

/*----------------------------------------------------------------------------
 Create the Semaphore and launch the threads
 *---------------------------------------------------------------------------*/
int main (void) 
{
	LED_Init ();
	semMultiplex = osSemaphoreCreate(osSemaphore(semMultiplex), 2);	
	T_mux1 = osThreadCreate(osThread(multiplex_Thread),(void *)1UL);
	T_mux2 = osThreadCreate(osThread(multiplex_Thread),(void *)2UL);                  
	T_mux3 = osThreadCreate(osThread(multiplex_Thread),(void *)4UL);
	T_mux4 = osThreadCreate(osThread(multiplex_Thread),(void *)8UL); 
	T_mux5 = osThreadCreate(osThread(multiplex_Thread),(void *)0x10UL);
	T_mux6 = osThreadCreate(osThread(multiplex_Thread),(void *)0x20UL); 	
	osDelay(osWaitForever);
}
