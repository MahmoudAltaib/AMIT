/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
  Software Testing using the Instrumentation Trace Example

*----------------------------------------------------------------------------*/

#include <RTL.h>
#include "LPC17xx.H"
#include <stdio.h>
#include "cmsis_os.h"

extern osMessageQId Q_LED;

extern void Thread1 (void const *argument);
extern void Thread2 (void const *argument);

extern osThreadDef_t os_thread_def_Thread1;
extern osThreadDef_t os_thread_def_Thread2;

extern osThreadId T_Thread1;
extern osThreadId T_Thread2;
extern osThreadId	T_main; 

extern int SER_GetChar (void);


/***********************************************************************************/
/* A test function to place a message into the Task2 mail queue                    */
/* this allows the test task to inject messages in place of Task1                  */
/***********************************************************************************/

 void injectTestMessage(uint32_t ledData){
 
		osMessagePut(Q_LED,ledData,osWaitForever);
		printf("LED %d on \n\r",(ledData+1));
}

/************************************************************************************/				
/* The test task runs at a low priority so it does not efect the application tasks  */
/* It receives a character from the ITM and then runs a test function               */				
/************************************************************************************/
				
 void testTask(void const *argument) {
	unsigned int command;

	while(1) 
	{	
		command = SER_GetChar();											//wait for an the ITM to receive a character from the debugger
		switch(command){													//Jump to the test function
			case ('1') :
				osThreadTerminate(T_Thread1);								//Stop Task1 running
				osSignalSet	(T_main,0x01);
				printf("Task1 Halted, LED's Off \n\r");
				break;
			case ('2') :
				injectTestMessage(0x0000);									// Place a message into the Task2 mail queue
				break;
			case ('3') :
				injectTestMessage(0x0001);
				break;
			case ('4') :
				injectTestMessage(0x0002);
				break;
			case ('5') :
				injectTestMessage(0x0003);
				break;
			case ('6') :
				injectTestMessage(0x0004);	
				break;
			case ('7') :
				injectTestMessage(0x0100);									//send a message with some invalid parameters
				break;
			case ('8') :
				osSignalSet	(T_main,0x01);									//Trigger Task3 to clear the LED bank
			  printf("Task3 Triggered to clear LED bank \n\r");
				break;
			case ('9') :
				T_Thread1 = osThreadCreate(osThread(Thread1), NULL);	
				printf("Task1 Started \n\r");
				break;
			default:
					break;
		}
	}
}

