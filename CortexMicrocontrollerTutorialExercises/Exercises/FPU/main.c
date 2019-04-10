/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	Floating point execution Example

*----------------------------------------------------------------------------*/

#include "ARMCM4.h"
#include <math.h>
volatile int a1;

int main (void)
{
float a,b,c,d,e;
int f,g = 100;

	while(1)
	{
		a= 10.1234;
		b=100.2222;
		c= a*b;
		d= c-a;
		e= d+b;
		f =(int)a;
		f = f*g;
		a1 = (unsigned int) a;
		a=__sqrtf(e); 			//Square root by Hardware FPU 
	//	a= sqrt(e);				//Square root by software library
		a = c/f;	
		e = a/0;				//Raise divide by zero error
	}
}
