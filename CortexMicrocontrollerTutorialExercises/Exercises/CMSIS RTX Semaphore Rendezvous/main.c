/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Semaphore Rendezvous Example

*----------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cmsis_os.h"

#include "LED.h"

void led_Thread1 (void const *argument);
void led_Thread2 (void const *argument);
osThreadDef(led_Thread1, osPriorityNormal, 1, 0);
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);

osThreadId T_led_Thread1;
osThreadId T_led_Thread2;
	
osSemaphoreId semArrived1;									//define the semaphores
osSemaphoreDef(semArrived1);
osSemaphoreId semArrived2;
osSemaphoreDef(semArrived2);

/*----------------------------------------------------------------------------
  Synchronise the LED's using a semaphore rendezvous
 *---------------------------------------------------------------------------*/
void led_Thread1 (void const *argument) 
{
	for (;;)
	{
		LED_Off(1);
		osDelay(100);
		//osSemaphoreRelease(semArrived2);					//The semaphores ensure both tasks arrive here
		//osSemaphoreWait(semArrived1,osWaitForever);		//before continuing
		LED_On(1);
		osDelay(500);
  }
}
/*----------------------------------------------------------------------------
   Synchronise the LED's using a semaphore rendezvous
 *---------------------------------------------------------------------------*/
void led_Thread2 (void const *argument) 
{
	for (;;) 
	{
		LED_Off(2); 
		osDelay(500);
		//osSemaphoreRelease(semArrived1);					//The semaphores ensure both tasks arrive here
		//osSemaphoreWait(semArrived2,osWaitForever);		//before continuing
		LED_On(2);
		osDelay(500);
	}
}




/*----------------------------------------------------------------------------
 Create the semaphores and start the threads
 *---------------------------------------------------------------------------*/
int main (void) 
{
	LED_Init ();
	semArrived1 = osSemaphoreCreate(osSemaphore(semArrived1), 0);				
	semArrived2 = osSemaphoreCreate(osSemaphore(semArrived2), 0);		
	T_led_Thread1 =	osThreadCreate(osThread(led_Thread1), NULL);
	T_led_Thread2 =	osThreadCreate(osThread(led_Thread2), NULL);
	for (;;);
}
