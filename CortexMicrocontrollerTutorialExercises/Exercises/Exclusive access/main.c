/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Exclusive Access Example

*----------------------------------------------------------------------------*/
#include "STM32F10x.h"


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
#define USE_PSP_IN_THREAD_MODE 2
#define THREAD_MODE_IS_UNPRIVILIGED 1
#define PSP_STACK_SIZE 0x200

volatile unsigned char lock_bit,semaphore = 0;
volatile uint32_t result;

void __svc(0) thread (void);
void __SVC_0      (void) 
{
	__LDREXB( &lock_bit);	
	if(__LDREXB( &lock_bit)==0)
	{
		result = 	__STREXB(1,&lock_bit);
	}	
}

int main (void)
{
	if(__LDREXB( &lock_bit)==0)
	{
		if (__STREXB(1,&lock_bit) == 0)
		{
			semaphore++;
			lock_bit = 0;
		}
	}
	
	if(__LDREXB( &lock_bit)==0)
	{
		thread();
		if (!__STREXB(1,&lock_bit))
		{
			semaphore++;
		}
	}	

  while (1) 
	{
		;
	}
}
