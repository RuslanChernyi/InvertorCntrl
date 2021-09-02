;// TI File $Revision: /main/4 $
;// Checkin $Date: July 30, 2007   10:28:57 $
;//###########################################################################
;//
;// FILE:  DSP2833x_usDelay.asm
;//
;// TITLE: Simple delay function
;//
;// DESCRIPTION:
;//  
;// This is a simple delay function that can be used to insert a specified
;// delay into code.  
;// 
;// This function is only accurate if executed from internal zero-waitstate
;// SARAM. If it is executed from waitstate memory then the delay will be
;// longer then specified. 
;// 
;// To use this function:
;//
;//  1 - update the CPU clock speed in the DSP2833x_Examples.h
;//    file. For example:
;//    #define CPU_RATE 6.667L // for a 150MHz CPU clock speed
;//    or #define CPU_RATE 10.000L // for a 100MHz CPU clock speed 
;//
;//  2 - Call this function by using the DELAY_US(A) macro
;//    that is defined in the DSP2833x_Examples.h file.  This macro
;//    will convert the number of microseconds specified
;//    into a loop count for use with this function.  
;//    This count will be based on the CPU frequency you specify.
;//
;//  3 - For the most accurate delay 
;//    - Execute this function in 0 waitstate RAM.  
;//    - Disable interrupts before calling the function
;//      If you do not disable interrupts, then think of
;//      this as an "at least" delay function as the actual
;//      delay may be longer. 
;//
;//  The C assembly call from the DELAY_US(time) macro will
;//  look as follows: 
;//
;//  extern void Delay(long LoopCount);                
;//
;//        MOV   AL,#LowLoopCount
;//        MOV   AH,#HighLoopCount
;//        LCR   _Delay
;//
;//  Or as follows (if count is less then 16-bits):
;//
;//        MOV   ACC,#LoopCount
;//        LCR   _Delay
;//
;//
;//###########################################################################
;// $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
;// $Release Date: June 8, 2012 $
;//###########################################################################	

WDKEY	.set	0x7025

       .def _usDelay
       ;.sect "ramfuncs"
       .text

        .global  _usDelay
_usDelay:
_DSP28x_usDelay:
                MOVB AH, #0                   ;Zero AH
        PUSH ST1                      ;Save ST1 to preserve EALLOW setting

DelayUs1:                             ;Outer loop

;Service the watchdog in case it is active
        EALLOW
        MOVZ DP, #(WDKEY>>6)
        MOV @WDKEY, #0x0055
        MOV @WDKEY, #0x00AA
        EDIS

;Proceed with the inner loop
        RPT #138                      ;Inner loop
     || NOP

        SUBB ACC,#1                   ;Decrement outer loop counter
        BF DelayUs1, GT               ;Branch for outer loop

;Finish up
        POP ST1                       ;Restore ST1
        LRETR                         ;Return

;There is a 9/10 cycle overhead and each loop
;takes five cycles. The LoopCount is given by
;the following formula:
;  DELAY_CPU_CYCLES = 9 + 5*LoopCount
; LoopCount = (DELAY_CPU_CYCLES - 9) / 5
; The macro DELAY_US(A) performs this calculation for you
;
;//===========================================================================
;// End of file.
;//===========================================================================

     .end

