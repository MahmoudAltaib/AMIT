/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Bit band example

*----------------------------------------------------------------------------*/

#include "STM32F10x.h" 

/* Bit Band Macros used to calculate the alias address at run time*/
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, b) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))

/* Pointer using bit band alias address calculated at compile time*/
#define PortB_Bit8   (*((volatile unsigned long *)0x422181A0 ))
             
volatile unsigned long * bitAddress;

void dly (void)
{
uint32_t delay;	
	for( delay = 0;delay <0x100000;delay++)
	{
		;
	}
}
		
int main (void)
{
	while (1)
	{
		GPIOB->ODR |= 0x00000100; 	  							/* Set and Clear the bit using port wide AND and OR instructions - Slowest method */
		dly();
		GPIOB->ODR &=!0x00000100;	  
		dly();
		bitAddress = & BitBand(&GPIOB->ODR, 8);					/* Use a run time macro to calculate the bit band alias address - 2nd Fastest method */
		*bitAddress = 1;										/* Set and Clear the bit */ 
		dly();
		*bitAddress = 0;
		dly();
		PortB_Bit8 = 1;												/* Set and Clear the bit using a pointer calculated at compile time - Fastest method */
		dly();
		PortB_Bit8  = 0;  
		dly();		
	}
}


