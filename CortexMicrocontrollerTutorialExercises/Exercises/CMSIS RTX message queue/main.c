/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Message Queue Example

*----------------------------------------------------------------------------*/
#include "STM32F10x.h"
#include "cmsis_os.h"

#include "LED.h"

osMessageQId Q_LED;													//define the message queue
osMessageQDef (Q_LED,0x16,unsigned char);
osEvent  result;


void led_Thread1 (void const *argument);																
void led_Thread2 (void const *argument);

osThreadDef(led_Thread1, osPriorityNormal, 1, 0);
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);

osThreadId T_led_Thread1;
osThreadId T_led_Thread2;
osThreadId	T_main; 



/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
void led_Thread1 (void const *argument) 
{
	for (;;) 
	{
		result = 	osMessageGet(Q_LED,osWaitForever);				//wait for a message to arrive
		LED_On(result.value.v);                          			// write the data to the LED's
	}
}
/*----------------------------------------------------------------------------
  Task 1 'ledOn': switches the LED on
 *---------------------------------------------------------------------------*/
void led_Thread2 (void const *argument) 
{
	for (;;) 
	{
		osMessagePut(Q_LED,0x10,osWaitForever);                   	//Place a value in the message queue
		osDelay(100);												//Wait 100 ms
		LED_Off(0x10);												//switch off the LED
		osMessagePut(Q_LED,0x20,osWaitForever);						//Repeat for other bit patterns
		osDelay(100);
		LED_Off(0x20);
		osMessagePut(Q_LED,0x40,osWaitForever);
		osDelay(100);
		LED_Off(0x40);
		osMessagePut(Q_LED,0x80, osWaitForever);
		osDelay(100);
		LED_Off(0x80);
	}
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/

int main (void) 
{
	LED_Init ();
	Q_LED = osMessageCreate(osMessageQ(Q_LED),NULL);					//create the message queue
	T_led_Thread1 = osThreadCreate(osThread(led_Thread1), NULL);
	T_led_Thread2 =	osThreadCreate(osThread(led_Thread2), NULL);
	osDelay(osWaitForever);
	for (;;);
}
