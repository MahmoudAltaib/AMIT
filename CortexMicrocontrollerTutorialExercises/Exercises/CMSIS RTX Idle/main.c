/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Idle Example

*----------------------------------------------------------------------------*/
#include "STM32F10x.h"
#include "cmsis_os.h"
#include "led.h"


void delay (unsigned int count)
{
unsigned int index;

	for(index =0;index<count;index++)
	{
		;
	}
}
/*----------------------------------------------------------------------------
  Task 1 'ledOn': switches the LED on
 *---------------------------------------------------------------------------*/
void ledOne (void const *argument) 
{
	for (;;) 
	{
		LED_On(1);                          // Toggle LED1                      
		osDelay(500);
		LED_Off(1);
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
void ledTwo (void const *argument) 
{
	for (;;) 
	{
    	LED_On(2);                          // Toggle LED2                       
		osDelay(100);
		LED_Off(2);
		osDelay(100);
	}
}




osThreadId main_ID;	
osThreadDef(ledOne, osPriorityNormal, 1, 0);
osThreadDef(ledTwo, osPriorityNormal, 1, 0);
/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
	LED_Init ();
	osThreadCreate(osThread(ledOne), NULL);
	osThreadCreate(osThread(ledTwo), NULL);
	main_ID = osThreadGetId ();
	osThreadTerminate(main_ID);
	for (;;);
}
