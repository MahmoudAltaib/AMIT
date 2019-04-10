/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	software Testing using the Instrumentation Trace Example

*----------------------------------------------------------------------------*/

#include "defines.h"
#include "cmsis_os.h"
#include "LPC17xx.H"
#include <stdio.h>
 
void errorHandler (void const *argument) {
	osEvent errorCode;
	uint32_t errorNumber;
	while(1)
	{
		errorCode = osSignalWait(0,osWaitForever);		// Wait here until error condition occurs.
		errorNumber = errorCode.value.signals;				
		switch(errorNumber)								// Jump to the error handeling code
		{								
			case (RTX_STARTED):
				printf("RTX started\n\r");
			break;
			case (LED_PARAMETER_INVALID):
				printf("Invalid Lower nibble LED Parameter\n\r");
			break;
			case (MAILBOX_OVERFLOW):
				printf("Task2 Mail queue overflow\n\r");
			break;
		
			default	:
			
			break;
		}
	}
}
