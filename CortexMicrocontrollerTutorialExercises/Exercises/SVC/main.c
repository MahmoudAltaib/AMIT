/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Supervisor Call Exception Example

*----------------------------------------------------------------------------*/

#include "STM32F10x.h"
int res = 10;

/*----------------------------------------------------------------------------
 Define the add,mul4, div and mod function as svc routines
  Note:
       Software Interrupt Function accept up to four parameters
       and run in Supervisor Mode (Interrupt protected)
 *----------------------------------------------------------------------------*/

int __svc(3) add (int i1, int i2);
int __SVC_3      (int i1, int i2) 
{
	return (i1 + i2);
}

int __svc(2) mul4(int i);
int __SVC_2      (int i) 
{
	return (i << 2);
}

int __svc(1) div (int i1, int i2);
int __SVC_1      (int i1, int i2) 
{
	return (i1 / i2);
}

int __svc(0) mod (int i1, int i2);
int __SVC_0      (int i1, int i2) 
{
	return (i1 % i2);
}




/*----------------------------------------------------------------------------
 Call the div and mod SVC functions
 *----------------------------------------------------------------------------*/
void test_t (void) 
{
	res  = div (res, 10);                           /* Call SWI Functions       */
	res  = mod (res,  3);
}



/*----------------------------------------------------------------------------
  Call the add and mul SVC functions
 *----------------------------------------------------------------------------*/
void test_a (void) 
{
	res  = add (74,  27);                           /* Call SWI Functions       */
	res += mul4(res);
}

/*----------------------------------------------------------------------------
Calculate the inital value for the process stack   
Enable the Process Stack and set its start address
Give the processor thread mode unpriviliged access rights
Finally call the SVC test functions
 *----------------------------------------------------------------------------*/
#define USE_PSP_IN_THREAD_MODE 2
#define THREAD_MODE_IS_UNPRIVILIGED 1
#define PSP_STACK_SIZE 0x200

int main (void) 
{
uint32_t initalPSPValue;
	
	initalPSPValue = __get_MSP() + PSP_STACK_SIZE;
	__set_CONTROL(USE_PSP_IN_THREAD_MODE);
	__set_PSP(initalPSPValue);
	__set_CONTROL(THREAD_MODE_IS_UNPRIVILIGED|USE_PSP_IN_THREAD_MODE);
	test_a();
	test_t();
	while (1) 
	{
		;
	}
}

