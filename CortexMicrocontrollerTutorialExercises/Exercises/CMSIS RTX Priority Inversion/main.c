/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Priority Inversion Example

*----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cmsis_os.h"
#include "LED.h"

void phaseA (void const *argument);
void phaseB (void const *argument);
void phaseC (void const *argument);
void phaseD (void const *argument); 
void Delay(__IO uint32_t nCount);

osThreadDef(phaseA, osPriorityHigh, 1, 0);
osThreadDef(phaseB, osPriorityNormal, 1, 0);
osThreadDef(phaseC, osPriorityNormal, 1, 0);
osThreadDef(phaseD, osPriorityBelowNormal, 1, 0);

osThreadId t_main,t_phaseA,t_phaseB,t_phaseC,t_phaseD;

void phaseA (void const *argument) 
{
	for (;;) 
	{
		LED_On(3);
	//	osThreadSetPriority(t_phaseD, osPriorityHigh);
		osDelay(500);
		osSignalSet(t_phaseD,0x0001);							//Wake up Phase D by setting one of its signal flags.
		osSignalWait(0x00,osWaitForever);						//ERROR!! wait for Phase D to finish its task	
	//	osThreadSetPriority(t_phaseD,osPriorityBelowNormal);
	}
}

/*----------------------------------------------------------------------------
 *        Task 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
void phaseB (void const *argument) 
{
	for (;;) 
	{
		Delay(100);
		LED_On(2); 
		Delay(100);
		osSignalSet(t_phaseC,0x01);	 
	}
}

void phaseC (void const *argument) 
{
	for (;;) 
	{
		osSignalWait(0x01,osWaitForever);  
		LED_Off(2);
	}
}

 void phaseD (void const *argument) 
{
	for (;;) 
	{
		osSignalWait(0x01,osWaitForever);
		Delay(100);
		LED_Off(3); 
		osSignalSet(t_phaseA,0x02);   
	}
}

int main(void)
{
	LED_Init();
	t_main = osThreadGetId ();
	osThreadSetPriority(t_main,osPriorityHigh);
	t_phaseA = osThreadCreate(osThread(phaseA), NULL);
	t_phaseB = osThreadCreate(osThread(phaseB), NULL);
	t_phaseC = osThreadCreate(osThread(phaseC), NULL);
	t_phaseD = osThreadCreate(osThread(phaseD), NULL);
	osThreadTerminate(t_main);
	while(1)
	{
		;
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
 void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



