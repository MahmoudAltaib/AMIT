#include "stm32f10x.h"

void LED_Init (void)
{
GPIOB->CRH = 0x33333333;	
}
void LED_On(uint32_t output){
GPIOB->ODR |= output << 8;
}

void LED_Off(uint32_t output){
GPIOB->ODR &= ~(output << 8);	
	
}

