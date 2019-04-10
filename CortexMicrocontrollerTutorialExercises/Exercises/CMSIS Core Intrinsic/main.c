/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS Core Intrinsics Example

*----------------------------------------------------------------------------*/

#include "STM32F10x.h"              

volatile unsigned int input=0x00112233,outputREV=0,outputREV16=0,outputRBIT=0,outputROR=0,outputCLZ=0;
volatile int outputREVSH=0;

int main (void)
{
	outputREV 	= __REV(input);							//Reverse Byte Order (32 bit)
	outputREV16 = __REV16(input);						//Reverse byte order (16 bit)
	outputREVSH = __REVSH((int)input);					//Reverse byte order is signed short value
	outputRBIT 	= __RBIT(input);						//Reverse bit order (32 bit value)
	outputROR 	= __ROR(input,8);						//Rotate right (32 bit any number of bits)
	outputCLZ 	= __CLZ(input);							//Count leading zeros
						
	while (1)
	{
		;
	}

}

