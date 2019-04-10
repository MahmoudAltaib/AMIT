/*----------------------------------------------------------------------------
 * Name:    SVC.c
 * Purpose: SVC Handler
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 20009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

__asm void SVC_Handler (void) {
        PRESERVE8

                TST     LR,#4                   ; Called from Handler Mode?
                MRSNE   R12,PSP                 ; Yes, use PSP
                MOVEQ   R12,SP                  ; No, use MSP
                LDR     R12,[R12,#24]           ; Read Saved PC from Stack
                LDRH    R12,[R12,#-2]           ; Load Halfword
                BICS    R12,R12,#0xFF00         ; Extract SVC Number

                PUSH    {R4,LR}                 ; Save Registers
                LDR     LR,=SVC_Count
                LDR     LR,[LR]
                CMP     R12,LR
                BHS     SVC_Dead                ; Overflow
                LDR     LR,=SVC_Table
                LDR     R12,[LR,R12,LSL #2]     ; Load SVC Function Address
                BLX     R12                     ; Call SVC Function

                POP     {R4,LR}
                TST     LR,#4
                MRSNE   R12,PSP
                MOVEQ   R12,SP
                STM     R12,{R0-R3}             ; Function return values
                BX      LR                      ; RETI

SVC_Dead
                B       SVC_Dead                ; None Existing SVC

SVC_Cnt         EQU    (SVC_End-SVC_Table)/4
SVC_Count       DCD     SVC_Cnt

; Import user SVC functions here.
                IMPORT	__SVC_0
                IMPORT  __SVC_1
                IMPORT  __SVC_2
                IMPORT  __SVC_3

SVC_Table
; Insert user SVC functions here
                 DCD    __SVC_0 
                DCD     __SVC_1                 ; SVC 1 Function Entry
                DCD     __SVC_2                 ; SVC 2 Function Entry
                DCD     __SVC_3                 ; SVC 2 Function Entry

SVC_End

                ALIGN
}
