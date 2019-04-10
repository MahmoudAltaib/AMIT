/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Software Testing using the Instrumentation Trace Example

*----------------------------------------------------------------------------*/

#include "LPC17xx.H"
#include <stdio.h>
#include "LED.h"
#include "cmsis_os.h"
#include "defines.h"
/**************************************************************************/
/* Task1 is used to send a mail message to Task2. 							*/
/* The message contains two variables which are used to switch the 			*/
/*upper and lower nibbles of the LED bank. Once all the LED's are			*/
/* on an event flag is set on Task 3 which switches off the LED's 			*/
/* The process then restarts												*/
/**************************************************************************/

osMessageQId Q_LED;
osMessageQDef (Q_LED,0x16,unsigned char);

osEvent  result;
extern void errorHandler (void const *argument);
extern void testTask(void const *argument);

void Thread1 (void const *argument);
void Thread2 (void const *argument);

osThreadDef(Thread1, osPriorityNormal, 1, 0);
osThreadDef(Thread2, osPriorityNormal, 1, 0);

osThreadDef(testTask, osPriorityNormal, 1, 0);
osThreadDef(errorHandler, osPriorityNormal, 1, 0);

osThreadId T_Thread1;
osThreadId T_Thread2;
osThreadId	T_main; 

osThreadId T_testTask;
osThreadId T_errorTask;

void Thread1 (void const *argument)
{
uint32_t ledData,i;
osStatus status;

	 while(1)
	{
		ledData=0;	
		for(i=0;i<8;i++)
		{																											
			osDelay(100);
			status = osMessagePut(Q_LED,ledData,0);
			if(status != osOK)
			{
				osSignalSet(T_errorTask,MAILBOX_OVERFLOW);
			}
			ledData = ledData+1;							//Update the led data for the next message
		}
		osDelay(100);
		osSignalSet	(T_main,0x01);							//Signal task3 to clear the LED's
	}
}

/******************************************************************************/
/* Task2 receives a mail message, checks that the LED data is within range    */
/* and then writes the data to the bank of LED's. If the data is out of range */
/* the error task is signaled with an error code                              */
/******************************************************************************/

 void Thread2 (void const *argument)
{
	while(1) 
	{
		result = osMessageGet(Q_LED,osWaitForever);
		if(result.value.v>0x0000FF)
		{
			osSignalSet(T_errorTask,LED_PARAMETER_INVALID);
		}
		else
		{
			LED_On(result.value.v); 
		}
	}
}

/*************************************************************************************/
/* The init task creates the mail queues and the tasks                               */
/* It also signals the error tas to print a sign on message and then deletes itself  */
/*************************************************************************************/

int main (void) 
{
	T_main = osThreadGetId ();
	LED_Init ();
	Q_LED = osMessageCreate(osMessageQ(Q_LED),NULL);
	T_Thread1 = osThreadCreate(osThread(Thread1), NULL);	
	T_Thread2 = osThreadCreate(osThread(Thread2), NULL);
	T_testTask = osThreadCreate(osThread(testTask), NULL);	
	T_errorTask = osThreadCreate(osThread(errorHandler), NULL);
	osSignalSet(T_errorTask,RTX_STARTED);
	while(1)
	{
		osSignalWait(0x01,osWaitForever);
		LED_Out(0x00);								//Switch off all the LEDs
	}																		
}
