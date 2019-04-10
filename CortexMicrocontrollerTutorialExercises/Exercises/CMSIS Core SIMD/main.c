

#include "ARMCM4.h"
#include <math.h>

union _test{
uint16_t	Arry_halfword[100];
uint32_t	Arry_word[50];
};

union _test op1;
union _test op2;
uint32_t Result;

int main (void){
int n;

	for(n=0;n<100;n++)
	{
		op1.Arry_halfword[n]= op2.Arry_halfword[n] = n;
	}

	for(n=0;n<50;n++)
	{	
		Result = __SMLAD(op1.Arry_word[n],op2.Arry_word[n],Result);
	
	}
	Result = 0;

	for(n = 0;n<100;n++)
	{
		Result = Result+(op1.Arry_halfword[n]*op2.Arry_halfword[n]);
	}
	while(1)
	{
		;
	}
}
