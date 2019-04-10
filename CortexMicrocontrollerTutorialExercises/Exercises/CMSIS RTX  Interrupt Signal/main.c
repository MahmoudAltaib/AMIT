


#include "stm32f10x.h"
#include "cmsis_os.h"

#include "LED.h"

void led_Thread1 (void const *argument);
void led_Thread2 (void const *argument);
void adc_Thread  (void const *argument);
void init_ADC (void);						//Define the ADC initilising code as a supervisor function

osThreadDef(led_Thread1, osPriorityNormal, 1, 0);
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);
osThreadDef(adc_Thread, osPriorityAboveNormal, 1, 0);

osThreadId T_led_ID1;
osThreadId T_led_ID2;	
osThreadId T_adc_ID;


/*----------------------------------------------------------------------------
 ADC interrupt handler. On Conversion set ADC thread signal
 *---------------------------------------------------------------------------*/
void ADC1_2_IRQHandler (void)
{
	osSignalSet(T_adc_ID,0x01);
	ADC1->SR &= ~(1 << 1);              
}

void adc_Thread (void const *argument)
{
	for (;;) 
	{	
		osSignalWait(0x01,osWaitForever);	
		GPIOB->ODR = ADC1->DR;
	}	
}

void led_Thread1 (void const *argument) 
{
	for (;;)
	{
		osSignalWait(0x01,osWaitForever);
		LED_On(1);                          
		osSignalWait(0x01,osWaitForever);	
		LED_Off(1);
	}
}

void led_Thread2 (void const *argument) 
{
	for (;;) 
	{
		LED_On(2);		
		osSignalSet(T_led_ID1,0x01);
		osDelay(500);
		ADC1->CR2 |=  (1UL << 22); 				//Start ADC conversion
		LED_Off(2);
		osSignalSet(T_led_ID1,0x01);
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
  Initilise the ADC and create the threads
 *---------------------------------------------------------------------------*/
int main (void) {
	LED_Init();
	init_ADC();		
	T_led_ID1 = osThreadCreate(osThread(led_Thread1), NULL);
	T_led_ID2 = osThreadCreate(osThread(led_Thread2), NULL);
	T_adc_ID = osThreadCreate(osThread(adc_Thread), NULL);	
                  
	for (;;);
}

/*----------------------------------------------------------------------------
  ADC initilisation code defined as a SVC function. Must be in priviliged mode to enable the NVIC interrupt
 *---------------------------------------------------------------------------*/
void init_ADC(void){
	RCC->APB2ENR |= ( 1UL <<  2);           /* enable periperal clock for GPIOA */
	GPIOA->CRL &= ~0x000000F0;              /* set PIN1 as analog input         */


  /* Setup and initialize ADC converter                                       */
	RCC->CFGR |= ( 3UL << 14);           	/* ADC clk = PCLK2 / 8              */

	RCC->APB2ENR |= ( 1UL <<  9);           /* enable periperal clock for ADC1  */

	ADC1->SQR1 = 0;                     	/* Regular chn. Sequence length = 1 */
	ADC1->SQR2 = 0;                     	/* Clear register                   */
	ADC1->SQR3 = (1UL << 0);           		/* 1. conversion = channel 1        */
	ADC1->SMPR2 = (5UL << 3);           	/* sample time channel 1  55,5 cyc. */
	ADC1->CR1 = (1UL << 8);           		/* Scan mode on                     */
	ADC1->CR2 = (7UL << 17)|           		/* select SWSTART                   */
				(1UL << 20) ;          		/* enable external Trigger          */
	ADC1->CR1 |= (1UL << 5);          		/* enable for EOC Interrupt         */
	NVIC->ISER[0] = (1UL << 18);			/* To Access the NVIC we must be in priviliged mode    */
	
}

