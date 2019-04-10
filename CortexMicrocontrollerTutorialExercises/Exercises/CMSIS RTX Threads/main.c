/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Threads Example

*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Include the microcontroller header for register defenitions and CMSIS core functions
	Include the CMSIS RTOS header for the RTOS API
 *---------------------------------------------------------------------------*/

#include "STM32F10x.h"
#include "cmsis_os.h"
#include "led.h"

/*----------------------------------------------------------------------------
  Simple delay routine
 *---------------------------------------------------------------------------*/
void delay (unsigned int count)
{
unsigned int index;

	for(index =0;index<count;index++)
	{
		;
	}
}

/*----------------------------------------------------------------------------
  Flash LED 1
 *---------------------------------------------------------------------------*/
void led_thread1 (void const *argument) 
{

	for (;;) 
	{
		LED_On(1);                          
		delay(500);
		LED_Off(1);
		delay(500);
	}
}

/*----------------------------------------------------------------------------
 Flash LED 2
 *---------------------------------------------------------------------------*/
void led_thread2 (void const *argument) 
{
	for (;;) 
	{
		LED_On(2);                          
		delay(500);
		LED_Off(2);
		delay(500);
	}
}

/*----------------------------------------------------------------------------
 Define the thread handles and thread parameters
 *---------------------------------------------------------------------------*/

osThreadId main_ID,led_ID1,led_ID2;	
osThreadDef(led_thread2, osPriorityNormal, 1, 0);
osThreadDef(led_thread1, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 Initilise the LED's, get a handle for main, start the threads and terminate main
 *---------------------------------------------------------------------------*/

int main (void) {
	LED_Init ();
	main_ID = osThreadGetId ();
	//osThreadSetPriority(main_ID,osPriorityAboveNormal);	
	led_ID2 = osThreadCreate(osThread(led_thread2), NULL);
	led_ID1 = osThreadCreate(osThread(led_thread1), NULL);
	osThreadTerminate(main_ID);
	for (;;);
}
