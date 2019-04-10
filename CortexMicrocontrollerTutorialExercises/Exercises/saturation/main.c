#include "stm32f10x.h"
#define Q_FLAG 0x08000000
int a,locked = 0,range = 300;
char c;
unsigned int xPSR;

int main (void)
{
register unsigned int apsr __asm("apsr");
	
	while (1)
	{
		for(a=0;a<range;a++)			//a goes from 0 to 300
		{
			c = a;
//			c = __SSAT	(a, 9);			//c will saturate at 255
		}
/*		xPSR = 	__get_APSR ();
		if(xPSR&Q_FLAG)	{				//Using Compiler optimisation level 3 the if, then, else statement 
			range--;					//will be constructed using conditional instructions
		}	
		apsr = apsr&~Q_FLAG;
*/	
	}
}

