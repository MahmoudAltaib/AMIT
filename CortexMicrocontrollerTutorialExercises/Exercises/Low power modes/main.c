/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Low Power Example

*----------------------------------------------------------------------------*/

#include "stm32f10x.h" 
void init_ADC (void);
#define SYSTICK_COUNT_ENABLE		1
#define SYSTICK_INTERRUPT_ENABLE	2
unsigned char BACKGROUND =1;
unsigned char ADC =0;
unsigned char SYSTICK=0;
unsigned char SLEEP=0;

int main (void)
{
	GPIOB->CRH = 0x33333333;					//Configure the Port B LED pins
	SysTick->VAL = 0x9000;						//Start value for the sys Tick counter
	SysTick->LOAD = 0x9000;						//Reload value 
	SysTick->CTRL = SYSTICK_INTERRUPT_ENABLE 
					|SYSTICK_COUNT_ENABLE;		//Start and enable interrupt
	init_ADC();
	ADC1->CR1 |= ( 1UL <<  5);          		// enable for EOC Interrupt         
	NVIC->ISER[0] = ( 1UL << 18);				// enable ADC Interrupt             
	ADC1->CR2 |= ( 1UL <<  0);            		// ADC enable                       
 
//	SCB->SCR = 0x00000002;						//Configure Sleep on exit mode

	while(1)
	{
		SLEEP = 1;
		BACKGROUND = 0;
		__wfe();	
		BACKGROUND = 1;
		SLEEP = 0;
		
	}
}

void ADC1_2_IRQHandler (void)
{
uint32_t i;
	
	BACKGROUND = 0;	
	SYSTICK = 0;	
	for (i=0;i<0x1000;i++)
	{
		ADC =1;	
		SLEEP = 0;
	}	
	ADC1->SR &= ~(1 << 1);              // clear EOC interrupt    	
	ADC = 0;
}

void SysTick_Handler ( void)
{
int i;
	
	BACKGROUND = 0;
	ADC = 0;
	ADC1->CR2 |= (1UL<<22);	
	for (i=0;i<0x1000;i++)
	{
		SYSTICK = 1;	
		SLEEP = 0;
	}	
	SYSTICK = 0;
}

void init_ADC (void)
{
	RCC->APB2ENR |= (1UL<<2);           	/* enable periperal clock for GPIOA */
	GPIOA->CRL &= ~0x000000F0;              /* set PIN1 as analog input         */
	/* Setup and initialize ADC converter                                       */
	RCC->CFGR    |= ( 3UL<<14);           	/* ADC clk = PCLK2 / 8              */
	RCC->APB2ENR |= ( 1UL<<9);           	/* enable periperal clock for ADC1  */
	ADC1->SQR1    =  0;                     /* Regular chn. Sequence length = 1 */
	ADC1->SQR2    =  0;                     /* Clear register                   */
	ADC1->SQR3    = ( 1UL<<0);           	/* 1. conversion = channel 1        */
	ADC1->SMPR2   = ( 5UL<<3);           	/* sample time channel 1  55,5 cyc. */
	ADC1->CR1     = ( 1UL<<8);           	/* Scan mode on                     */
	ADC1->CR2     = ( 7UL<<17)|           	/* select SWSTART                   */
					( 1UL<<20);          	/* enable external Trigger          */
}



 

