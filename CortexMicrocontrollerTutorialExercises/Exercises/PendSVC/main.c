/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS Multiple Interrupts Example

*----------------------------------------------------------------------------*/

#include "stm32f10x.h" 

void init_ADC (void);

void __svc(0) systemCode (void);
void Do_System_Code(void);

void PendSV_Handler (void){

	Do_System_Code();									//Resume the pended exception and run the main system code
}


void Do_System_Code(void){
unsigned int i;
	
	for(i=0;i<100;i++);									//Loop to simulate system code
	
}



void __SVC_0      (void) {
unsigned int i, pending;

	for(i=0;i<100;i++);									//Delay to simulate critical code
	pending = NVIC_GetPendingIRQ(ADC1_2_IRQn);			//check to see if an interrupt is pending
	if(pending == 1){											
		SCB->ICSR |= 1<<28;								//If the ADC is pending set the pend exception pend bit and quit
	}else{
		Do_System_Code();								//Else do the system code and quit
	}
}

void ADC1_2_IRQHandler (void)
{
int16_t i;
	
	for (i=0;i<0x100;i++)
	ADC1->SR &= ~(1 << 1);              				// Clear the end of conversion status bit         	
}

int main (void)
{
	init_ADC();
	NVIC_SetPriority(PendSV_IRQn,2);
	NVIC_SetPriority(ADC1_2_IRQn,1);					// Set the Systick Priority to the same as the ADC
	NVIC_EnableIRQ(ADC1_2_IRQn); 						// Enable the ADC Interrupt
	ADC1->CR1    |= ( 1UL <<  5);          				// Enable for EOC Interrupt         
	ADC1->CR2    |= ( 1UL <<  0);            		   	// ADC enable      
	ADC1->CR2 	 |=  (1UL << 22);						//Start the ADC conversion
	systemCode();
	while(1)
	{
		;
	}
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



 

