/*----------------------------------------------------------------------------
	
	Designers Guide to the Cortex-M Family
	DSP Optimisation Example

*----------------------------------------------------------------------------*/

#define __NVIC_PRIO_BITS          4       /*!< STM32F4XX uses 4 Bits for the Priority Levels */

typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
} IRQn_Type;

#include "core_cm4.h"             

#define BLOCKSIZE 	32
#define NUM_TAPS 	10
#define FILTERLEN 	10
#define q31_t int

int state[BLOCKSIZE];
int coefficents[NUM_TAPS];
int Instance;
int in[32];
int out[32];
int stateIndex;
int state_step2[BLOCKSIZE+NUM_TAPS];

void fir(q31_t *in, q31_t *out, q31_t *coeffs, int *stateIndexPtr, 
		int filtLen, int blockSize)
{
int sample;
int k;
q31_t sum;
int stateIndex = *stateIndexPtr;
	
	for(sample=0; sample < blockSize; sample++)
    {
		state[stateIndex++] = in[sample];
		sum=0;
		for(k=0;k<filtLen;k++)
		{
			sum += coeffs[k] * state[stateIndex];
			stateIndex--;
			if (stateIndex < 0) 
			{
				stateIndex = filtLen-1;
			}
		}
		out[sample]=sum;
    }
	*stateIndexPtr = stateIndex;
}

void fir_block(q31_t *in, q31_t *out, q31_t *coeffs, int *stateIndexPtr, 
		int filtLen, int blockSize)
{
int sample;
int k;
q31_t sum;
int stateIndex = *stateIndexPtr;
	
	for(sample=0; sample < blockSize; sample++)
    {
		state_step2[stateIndex++] = in[sample];
		sum=0;
		for(k=0; k<filtLen; k++)
		{
			sum += coeffs[k] * state_step2[stateIndex];
			stateIndex++;
		}
		out[sample]=sum;
    }
	*stateIndexPtr = stateIndex;
}

void fir_unrolling(q31_t *in, q31_t *out, q31_t *coeffs, int *stateIndexPtr, 
		int filtLen, int blockSize)
{
int sample;
int k;
q31_t sum;
int stateIndex = *stateIndexPtr;
	
	for(sample=0; sample < blockSize; sample++)
    {
		state[stateIndex++] = in[sample];
		sum=0;
		k = filtLen >>2;
		for(k=0;k<filtLen;k++)
		{
			sum += coeffs[k] * state_step2[stateIndex];
			stateIndex++;
			sum += coeffs[k] * state_step2[stateIndex];
			stateIndex++;
			sum += coeffs[k] * state_step2[stateIndex];
			stateIndex++;
			sum += coeffs[k] * state_step2[stateIndex];
			stateIndex++;
		}
		out[sample]=sum;
		*stateIndexPtr = stateIndex;
	}
}

void fir_SIMD(q31_t *in, q31_t *out, q31_t *coeffs, int *stateIndexPtr, 
		int filtLen, int blockSize)
{
int sample,I;
int k,c,s;
q31_t sum;
int stateIndex = *stateIndexPtr;
	
	for(sample=0; sample < blockSize; sample++)
    {
		state[stateIndex++] = in[sample];
		sum=0;
		I = 0;
		k = filtLen >>2;
		for(k=0;k<filtLen;k++)
		{
			c = *coeffs++;				
			s = state[I++];				
			sum = __SMLALD(c, s, sum);		
			c = *coeffs++;				
			s = state[I++];					
			sum = __SMLALD(c, s, sum);		
			c = *coeffs++;				
			s = state[I++];					
			sum = __SMLALD(c, s, sum);		
			c = *coeffs++;				
			s = state[I++];					
			sum = __SMLALD(c, s, sum);		
		}					
	}
	out[sample]=sum;
    *stateIndexPtr = stateIndex;
}


void fir_SuperUnrolling(q31_t *in, q31_t *out, q31_t *coeffs, int *stateIndexPtr, 
		int filtLen, int blockSize)
{
int sample,i;
int c0,x0,x1,x2,x3,sum0,sum1,sum2,sum3;
int stateIndex = *stateIndexPtr;
int * coeffPtr;
	
	sample = blockSize/4;
	do
	{
		sum0 = sum1 = sum2 = sum3 = 0;
		stateIndex = *stateIndexPtr;
		coeffPtr = coeffs;
		x0 = *(q31_t *)(stateIndex++);
		x1 = *(q31_t *)(stateIndex++);
		i = NUM_TAPS>>2;	
		do	
		{
			c0 = *(coeffPtr++);
			x2 = *(q31_t *)(stateIndex++);
			x3 = *(q31_t *)(stateIndex++);
			sum0  = __SMLALD(x0, c0, sum0);
			sum1  = __SMLALD(x1, c0, sum1);
			sum2  = __SMLALD(x2, c0, sum2);
			sum3  = __SMLALD(x3, c0, sum3);
			c0 = *(coeffPtr++);
			x0 = *(q31_t *)(stateIndex++);
			x1 = *(q31_t *)(stateIndex++);
			sum0  = __SMLALD(x0, c0, sum0);
			sum1  = __SMLALD(x1, c0, sum1);
			sum2  = __SMLALD (x2, c0, sum2);
			sum3  = __SMLALD (x3, c0, sum3);
			} while(--i);	
		out[sample] = sum0;		
		out[sample+1] = sum1; 	
		out[sample+2] = sum2;
		out[sample+3] = sum3;
		stateIndexPtr= stateIndexPtr + 4;
	} while(--sample);

}
    

int main (void)
{
	fir(in,out,coefficents,&stateIndex,FILTERLEN, BLOCKSIZE);
	fir_block(in,out,coefficents,&stateIndex,FILTERLEN, BLOCKSIZE);	
	fir_unrolling(in,out,coefficents,&stateIndex,FILTERLEN, BLOCKSIZE);
	fir_SIMD(in,out,coefficents,&stateIndex,FILTERLEN, BLOCKSIZE);
	fir_SuperUnrolling(in,out,coefficents,&stateIndex,FILTERLEN, BLOCKSIZE);
	while(1)
	{
		;
	}
}

