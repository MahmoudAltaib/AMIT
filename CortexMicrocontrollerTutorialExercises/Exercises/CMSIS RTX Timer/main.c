
/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS User Timer Example

*----------------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "cmsis_os.h"
#include "LED.h"

void callback(void const *param);
void ledThread1 (void const *argument);
void ledThread2 (void const *argument);

osThreadDef(ledThread1, osPriorityNormal, 1, 0);
osThreadDef(ledThread2, osPriorityNormal, 1, 0);

osThreadId T_ledThread1;
osThreadId T_ledThread2;

osTimerDef(timer0_handle, callback);
osTimerDef(timer1_handle, callback);
osTimerDef(timer2_handle, callback);
osTimerDef(timer3_handle, callback);

/*----------------------------------------------------------------------------
  Timer callback function. Toggle the LED associated with the timer
 *---------------------------------------------------------------------------*/
void callback(void const *param)
{
	switch( (uint32_t) param)
	{
		case 0:
			GPIOB->ODR ^= 0x2;
		break;

		case 1:
			GPIOB->ODR ^= 0x4;
		break;

		case 2:
			GPIOB->ODR ^= 0x8;	
		break;

		case 3:
			GPIOB->ODR ^= 0x10;
		break;
	}
}
/*----------------------------------------------------------------------------
  Flash LED 1 when signaled by thread 2
 *---------------------------------------------------------------------------*/
void ledThread1 (void const *argument) 
{
	for (;;) 
	{
		osSignalWait(0x01,osWaitForever);
		LED_On(1);                          
		osSignalWait(0x01,osWaitForever);	
		LED_Off(1);
	}
}
/*----------------------------------------------------------------------------
  Flash LED 2 and signal thread 1
 *---------------------------------------------------------------------------*/
void ledThread2 (void const *argument) 
{
	for (;;) 
	{
		LED_On(2);		
		osSignalSet	(T_ledThread1,0x01);
		osDelay(500);
		LED_Off(2);
		osSignalSet	(T_ledThread1,0x01);
		osDelay(500);
	}
}




/*----------------------------------------------------------------------------
 Create and start the timers then initilise the LED, Start the threads
 *---------------------------------------------------------------------------*/
int main (void) 
{
	osTimerId timer0 = osTimerCreate(osTimer(timer0_handle), osTimerPeriodic, (void *)0);	
	osTimerId timer1 = osTimerCreate(osTimer(timer1_handle), osTimerPeriodic, (void *)1);	
	osTimerId timer2 = osTimerCreate(osTimer(timer2_handle), osTimerPeriodic, (void *)2);	
	osTimerId timer3 = osTimerCreate(osTimer(timer3_handle), osTimerPeriodic, (void *)3);	
                  
	osTimerStart(timer0, 500);	
	osTimerStart(timer1, 100);	
	osTimerStart(timer2, 300);	
	osTimerStart(timer3, 200);	

	LED_Init ();	
	T_ledThread1 = osThreadCreate(osThread(ledThread1), NULL);
	T_ledThread2 = osThreadCreate(osThread(ledThread2), NULL);	
	for (;;);
}
