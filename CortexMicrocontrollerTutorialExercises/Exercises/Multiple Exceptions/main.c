/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS Multiple Interrupts Example

*----------------------------------------------------------------------------*/

#include "stm32f10x.h" 

void init_ADC (void);

#define SYSTICK_COUNT_ENABLE			1
#define SYSTICK_INTERRUPT_ENABLE		2

unsigned int BACKGROUND =0;
unsigned int ADC =0;
unsigned int SYSTICK=0;
uint32_t Temp;

int main (void)
{

	GPIOB->CRH = 0x33333333;							// Configure the Port B LED pins
	SysTick->VAL = 0x9000;
	SysTick->LOAD = 0x9000;
	SysTick->CTRL = SYSTICK_INTERRUPT_ENABLE|SYSTICK_COUNT_ENABLE;
	init_ADC();
	ADC1->CR1    |= ( 1UL <<  5);          				// Enable for EOC Interrupt    
	NVIC->ISER[0] = (1UL<<18);
	ADC1->CR2    |= ( 1UL <<  0);						// ADC enable  
/*	
	NVIC->IP[18] = (2<<4);
	SCB->SHP[11] = (3<<4);
*/
/*
	Temp  =  SCB->AIRCR;                                                   
	Temp &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             
	Temp  =  (Temp|((uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos) |(5 << 8));                                     
	SCB->AIRCR =  Temp;
*/
	while(1)
	{
		BACKGROUND = 1;									// Set the background and sleep toggle bits
														// Place the CPU in sleep mode
	}
}

void ADC1_2_IRQHandler (void)
{
int16_t i;

	BACKGROUND 	= 0;									// Clear all the toggle bits
	SYSTICK = 0;	
	for (i=0;i<0x1000;i++)
	{
		ADC = 1;										// Set the ADC toggle bit
	}	
	ADC1->SR &= ~(1 << 1);              				// Clear the end of conversion status bit         	
	ADC =0;												// Clear the ADC toggle bit
}

void SysTick_Handler ( void)
{
int i;

	BACKGROUND = 0;										// Clear the toggle bits
	ADC = 0;
	ADC1->CR2 |=  (1UL << 22);							//Start the ADC conversion
	for (i=0;i<0x1000;i++)
	{
		SYSTICK = 1;									//Set the systick toggle bit
	}	
	SYSTICK =0;											// Clear the systick toggle bit
}

void init_ADC (void)
{
	RCC->APB2ENR |= ( 1UL <<  2);           			/* enable periperal clock for GPIOA */
	GPIOA->CRL &= ~0x000000F0;              			/* set PIN1 as analog input         */


	/* Setup and initialize ADC converter                                       */
	RCC->CFGR    |= ( 3UL << 14);           			/* ADC clk = PCLK2 / 8              */

	RCC->APB2ENR |= ( 1UL <<  9);           			/* enable periperal clock for ADC1  */

	ADC1->SQR1    =  0;                     			/* Regular chn. Sequence length = 1 */
	ADC1->SQR2    =  0;                     			/* Clear register                   */
	ADC1->SQR3    = ( 1UL <<  0);          				 /* 1. conversion = channel 1        */
	ADC1->SMPR2   = ( 5UL <<  3);           			/* sample time channel 1  55,5 cyc. */
	ADC1->CR1     = ( 1UL <<  8);           			/* Scan mode on                     */
	ADC1->CR2     = ( 7UL << 17)|           			/* select SWSTART                   */
					( 1UL << 20) ;          			/* enable external Trigger          */
	
}



 

