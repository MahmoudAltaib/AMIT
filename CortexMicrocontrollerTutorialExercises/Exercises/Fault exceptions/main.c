/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Fault Exceptions Example

*----------------------------------------------------------------------------*/

#include "STM32F10x.h"              

volatile uint32_t *error_address;
volatile uint32_t op1;

/*******************************************************
Enable the divide by zero fault and the useage exception
Then cause a divide by zero exception
********************************************************/
int main (void)
{
int op2 = 0x1234,op3 = 0;

//	SCB->SHCSR = 0x00060000;					//Enable usage fault exception		
	SCB->CCR = 0x0000010;						//Enable divide by zero usage fault trap
	op1 = op2/op3;								//cause a div by zero error
	while(1)
	{
		;
	}
}

uint32_t faultInstruction;
/********************************************************
Trap the Usage exception and trace the fault
*********************************************************/
 /*
void UsageFault_Handler (void)
{
	
	error_address = (uint32_t *)(__get_MSP());		// load the current base address of the stack pointer
	error_address = error_address + 6;				// Locate the PC value in the last stack frame
	faultInstruction = *error_address;
	while(1)
	{
		;
	}
}

*/