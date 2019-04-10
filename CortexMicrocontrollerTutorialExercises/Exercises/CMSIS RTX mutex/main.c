/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	CMSIS RTOS Mutex Example

*----------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"

void uart_Thread1 (void const *argument);
void uart_Thread2 (void const *argument);
osThreadDef(uart_Thread1, osPriorityNormal, 1, 0);
osThreadDef(uart_Thread2, osPriorityNormal, 1, 0);

osThreadId T_uart1;
osThreadId T_uart2;
	
osMutexId uart_mutex;
osMutexDef(uart_mutex);

/*----------------------------------------------------------------------------
 Thread two writes the character '1' to UART 1
 *---------------------------------------------------------------------------*/
void uart_Thread1 (void const *argument) 
{
uint32_t i;

	for (;;) 
	{
	//	osMutexWait(uart_mutex, osWaitForever); 
		for( i=0;i<10;i++)
		{
			SendChar('1');
		}
		SendChar('\n');
		SendChar('\r');
	//	osMutexRelease(uart_mutex);	
	}
}

/*----------------------------------------------------------------------------
  Thread two writes the character '2' to UART 1
 *---------------------------------------------------------------------------*/
void uart_Thread2 (void const *argument) 
{
uint32_t i;

	for(;;)
	{
	//	osMutexWait(uart_mutex, osWaitForever);
		for( i=0;i<10;i++)
		{
			SendChar('2');
		}
		SendChar('\n');
		SendChar('\r');
	//	osMutexRelease(uart_mutex);
	}
}




/*----------------------------------------------------------------------------
 Initilise the USART, create the mutex and start the Threads
 *---------------------------------------------------------------------------*/
int main (void) 
{
	USART1_Init ();
	uart_mutex = osMutexCreate(osMutex(uart_mutex));	
	T_uart1 = osThreadCreate(osThread(uart_Thread1), NULL);
	T_uart2 = osThreadCreate(osThread(uart_Thread2), NULL);
 	for (;;);
}
