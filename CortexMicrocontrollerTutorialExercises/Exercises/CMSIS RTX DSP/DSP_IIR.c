/*----------------------------------------------------------------------------
 * Name:    DSP_IIR.c
 * Purpose: 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>
#include <math.h>
#include "arm_math.h"
#include "DSP_IIR.h"





#define F2Q31(x)  ((q31_t)((float32_t)x * 2147483648UL))
#define F2Q15(x)  ((q15_t)((float32_t)x *      32768UL))

#define F2Q30(x)  ((q31_t)((float32_t)x * 1073741824UL))
#define F2Q14(x)  ((q15_t)((float32_t)x *      16384UL))


/*----------------------------------------------------------------------------
   Declare IIR state buffers and structure  
 *---------------------------------------------------------------------------*/



q31_t                         iirState_q31[NUMSTAGES * 4];  
arm_biquad_casd_df1_inst_q31  iirInstance_q31; 
q31_t                         iirCoeff_q31[NUMSTAGES * 5] =
                                 {
  F2Q30( 0.4938164725899697E-01), /* section  1  B0 */
  F2Q30( 0.8426557574421167E-01), /* section  1  B1 */
  F2Q30( 0.4938164725899697E-01), /* section  1  B2 */
  F2Q30( 0.1253551801666617E+01), /* section  1 -A1 */
  F2Q30(-0.4365806737914682E+00), /* section  1 -A2 */
  F2Q30( 0.1536796065047383E+00), /* section  2  B0 */
  F2Q30( 0.1158730611205101E+00), /* section  2  B1 */
  F2Q30( 0.1536796065047383E+00), /* section  2  B2 */
  F2Q30( 0.1318815267644823E+01), /* section  2 -A1 */
  F2Q30(-0.7469483343884349E+00)  /* section  2 -A2 */
                                 };


/*----------------------------------------------------------------------------
   Initialize the IIR data structure  
 *---------------------------------------------------------------------------*/

void iirInit_q31 (void) {

  arm_biquad_cascade_df1_init_q31(&iirInstance_q31, NUMSTAGES, iirCoeff_q31, iirState_q31, 1); 
}

/*----------------------------------------------------------------------------
   Execute the IIR processing function  
 *---------------------------------------------------------------------------*/

void iirExec_q31 (q31_t *pSrc, q31_t *pDst) {

  arm_biquad_cascade_df1_q31(&iirInstance_q31, pSrc, pDst, DSP_BLOCKSIZE); 
}

