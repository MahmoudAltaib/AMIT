/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	First Exception Example

*----------------------------------------------------------------------------*/

#include "stm32f10x.h" 

#define SYSTICK_COUNT_ENABLE		1
#define SYSTICK_INTERRUPT_ENABLE	2


/************************************************************
Configure GPIO Port B and SysTick Timer
************************************************************/
int main (void)
{
	GPIOB->CRH = 0x33333333;								//Configure the Port B LED pins
	SysTick->VAL   = 0x9000;								//Start value for the sys Tick counter
	SysTick->LOAD  = 0x9000;								//Reload value 
	SysTick->CTRL = SYSTICK_INTERRUPT_ENABLE
					|SYSTICK_COUNT_ENABLE;					//Start and enable interrupt
	while(1)
	{
		;
	}
}

/************************************************************
SysTick Timer ISR - Toggle the LED pins
************************************************************/

void SysTick_Handler ( void)
{
static unsigned char count = 0;
	
	if(count++>0x60)
	{
		GPIOB->ODR ^=0xFFFFFFFF;							//Toggle LED pins
		count = 0;
	}
}





 

