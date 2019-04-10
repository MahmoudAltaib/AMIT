/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS mailbox Example
*----------------------------------------------------------------------------*/
#include "STM32F10x.h"
#include "cmsis_os.h"
#include "LED.h"

typedef struct {																//define the mail slot format
	uint8_t LED0; 
	uint8_t LED1; 
	uint8_t LED2; 
	uint8_t	LED3;
} mail_format;

osMailQDef(mail_box, 16, mail_format);											//define the mailbox 
osMailQId  mail_box;															//define the mailbox id

void LED_Producer (void const *argument) 
{						
uint8_t led0[12] = {1,0,0,0,1,1,1,1,0,0,0,0};									//create the patterns to send to the mailbox
uint8_t led1[12] = {0,2,0,0,0,2,2,2,2,0,0,0};
uint8_t led2[12] = {0,0,4,0,0,0,4,4,4,4,0,0};
uint8_t led3[12] = {0,0,0,8,0,0,0,8,8,8,8,0};
uint8_t index;
mail_format *LEDtx;																//define a pointer in the mailslot format

    while (1) 
	{
		for(index=0;index<12;index++)
		{
			LEDtx = (mail_format*)osMailAlloc(mail_box, osWaitForever);			//allocate a mailslot
			LEDtx->LED0 = led0[index]; 											//Populate it with data
			LEDtx->LED1 = led1[index];
			LEDtx->LED2 = led2[index];
			LEDtx->LED3 = led3[index];
			osMailPut(mail_box, LEDtx);											//Post the mail to the mailbox
			osDelay(100);
		}
    }
}


void LED_Consumer (void const *argument) 
{
osEvent evt;																	//declare an osEvent variable
mail_format *LEDrx;																//define a pointer in the mailslot format
	
	while (1) 
	{
		evt = osMailGet(mail_box, osWaitForever);								//wait until a message arrives
		if(evt.status == osEventMail) 											//Check for a valid message
		{
			LEDrx = (mail_format*)evt.value.p;									//Set the receive pointer to the mailslot
			LED_Out((LEDrx->LED0|LEDrx->LED1|LEDrx->LED2|LEDrx->LED3)<<8);		//Use the data
			osMailFree(mail_box, LEDrx);										//Free the mailslot
		}
	}	
}


osThreadDef(LED_Producer, osPriorityNormal, 1,0);
osThreadDef(LED_Consumer, osPriorityNormal, 1,0);

int main (void) 
{
	LED_Init();
	mail_box = osMailCreate(osMailQ(mail_box), NULL);							// Create the mailbox
    osThreadCreate(osThread(LED_Producer), NULL);
	osThreadCreate(osThread(LED_Consumer), NULL);
	osDelay(osWaitForever);
	
}
