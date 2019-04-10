/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Semaphore Example
*----------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cmsis_os.h"

#include "LED.h"

void led_Thread1 (void const *argument);
void led_Thread2 (void const *argument);
osThreadDef(led_Thread1, osPriorityAboveNormal, 1, 0);			//note the raised priority for led_thread 1
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);

osThreadId T_ledOn;
osThreadId T_ledOff;
/*----------------------------------------------------------------------------
  Define the semaphore
 *---------------------------------------------------------------------------*/	
osSemaphoreId sem1;									
osSemaphoreDef(sem1);
/*----------------------------------------------------------------------------
  Wait to acquire a semaphore token from sem1 then flash LED 1
 *---------------------------------------------------------------------------*/
void led_Thread1 (void const *argument) 
{
	for (;;) 
	{
		osSemaphoreWait(sem1, osWaitForever);
		LED_On(1);                          
		osDelay(500);
		LED_Off(1);
	}
}
/*----------------------------------------------------------------------------
  Flash LED 2 and 'release' a semaphore token to sem1
 *---------------------------------------------------------------------------*/
void led_Thread2 (void const *argument) 
{
	for (;;) 
	{
		LED_On(2);		
		osSemaphoreRelease(sem1);
		osDelay(500);
		LED_Off(2);
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
  Initilise the LED's, Create the semaphore and start the threads
 *---------------------------------------------------------------------------*/
int main (void) 
{
	LED_Init ();
	sem1 = osSemaphoreCreate(osSemaphore(sem1), 0);	
	T_ledOff = osThreadCreate(osThread(led_Thread2), NULL);
	T_ledOn = osThreadCreate(osThread(led_Thread1), NULL);
	for (;;);
}
