/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Bootloader Example

*----------------------------------------------------------------------------*/
  
#include <stm32F10x.h>

#define APPLICATION_ADDRESS 0x2000			//Start address for the user code. This should normally be at the start of a flash sector

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t  JumpAddress;

int main(void) 
{
uint32_t bootFlags;
	/* Add your code here to perform the following functions */
	/* Check the integrity of the application code */
	/* Check if an update is available */
	/* if either case is true set a bit in the bootflags register */
	bootFlags =0;
	
	if (bootFlags != 0) 
	{ 
		/*enter the flash update code here */
		 NVIC_SystemReset();	//When you are finished you can force a processor reset	
	}
	else 
	{
		if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000) 		/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
		{ 	
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);					/* Calculate the address of the user code reset vector */
			Jump_To_Application = (pFunction) JumpAddress;								/* Set the pointer with the jump address */
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);							/* Read the inital stack pointer value from the user code and program it into the Main Stack pointer  */
			Jump_To_Application();														/* Now jump to the application */
		}
		while (1) 
		{
			;
		}
	}
}
