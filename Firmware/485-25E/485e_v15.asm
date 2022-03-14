;**************************************************************************
; 485E.ASM
;
; High Speed Auto Enable, Auto Baud detect converter
;
; Version Info
; -------------
; Version 1.3
;   Changed from counting loop to Timer0 based timing
;
; Version 1.4
;   Added watchdog timeout support and commented some code.
;
; Version 1.5
;   Removed RS232_SLOW algorithm to removed power up deadlock.
;
;--------------------------------------------------------------------------
;
; Author
; ------
; Anthony A. Kempka
;
;**************************************************************************

        LIST P=12C508A, R=DEC

#include "p12c508.inc"

;
; Use the following config settings
	__CONFIG   _CP_ON & _WDT_ON & _MCLRE_OFF & _IntRC_OSC

;
; Handy bit defines
;
TX_EN_Bit       EQU     0x00
RS232_Bit       EQU     0x01
RTS_Bit         EQU     0x02
RS485_Bit       EQU     0x03
CTS_Bit         EQU     0x04
LED_Bit         EQU     0x05

TRIS_LED_OFF    EQU     0x2E
TRIS_LED_ON     EQU     0x0E

;--------------------------------------------------------------------------
;   Set ScratchPadRam here.
;--------------------------------------------------------------------------
ScratchPadRam   EQU     0x08

;--------------------------------------------------------------------------
; Variables
;--------------------------------------------------------------------------

Timeout         EQU     ScratchPadRam+0
Timeout_X10     EQU     ScratchPadRam+1
Timeout_485     EQU     ScratchPadRam+2
Count           EQU     ScratchPadRam+3

;--------------------------------------------------------------------------
; Program Code
;--------------------------------------------------------------------------

;--------------------------------------------------------------------------
; Main Program
;--------------------------------------------------------------------------
                ORG     0

Start
                MOVWF   OSCCAL          ; Load the oscillator calibration
                CLRF    GPIO            ; Clear the I/O outputs
                MOVLW   TRIS_LED_OFF    ; Configure I/O
                TRIS    GPIO            ; Load the TRIS
                BSF     GPIO, CTS_Bit   ; Make sure CTS inactive!

                MOVLW   0xC2            ; Option register prescale 1:8
                OPTION                  ; load into option register

;
; Start with the FAST prescale 9600-115200 since this is the most common.
; Use the timeout for 9600 BPS since it is the slowest.
;

                MOVLW   0x82            ; Starting timeout 130 count, 9600 BPS
                MOVWF   Timeout_X10
                MOVLW   0x0D            ; bit time timeout 13
                MOVWF   Timeout

Data_In_Loop
                CLRWDT                  ; Kick the dog
                BTFSS   GPIO, RS232_Bit ; Check RS232 active (0)
                CALL    RS232_TMT
                BTFSS   GPIO, RS485_Bit ; Check RS485 active (0)
                CALL    RS485_RCV
                BTFSS   GPIO, RTS_Bit   ; Check for RTS active (0)
                BCF     GPIO, CTS_Bit   ; CTS Active
                GOTO    Data_In_Loop    ; Repeat the whole thing


;--------------------------------------------------------------------------
;
; Handles the RS-485 receive (RS-232 transmit to PC)
;
RS485_RCV
                BSF     GPIO, CTS_Bit   ; Make sure CTS inactive!
                MOVLW   TRIS_LED_ON     ; LED ON state
                TRIS    GPIO
                BSF     GPIO, LED_Bit   ; LED Green

RS485_RCV_START
                MOVLW   0xFF            ; Set the timeout
                MOVWF   Timeout_485

RS485_RCV_LOOP
                CLRWDT                  ; Kick the dog
                BTFSS   GPIO, RS232_Bit ; Check RS232 active (0)
                GOTO    RS485_RCV_TMT
                BTFSS   GPIO, RS485_Bit ; Check RS485 active (0)
                GOTO    RS485_RCV_START
                DECFSZ  Timeout_485, F  ; Do timeout decrement
                GOTO    RS485_RCV_LOOP
                GOTO    RS485_RCV_END

RS485_RCV_TMT
                CALL    RS232_TMT
RS485_RCV_END
                MOVLW   TRIS_LED_OFF    ; LED OFF state
                TRIS    GPIO
                RETLW   0


;--------------------------------------------------------------------------
;
; Handles the RS-232 (HOST PC) transmitinng 
;
; When this routine is entered, we are seeing the START bit of
; the first byte being sent.
;
RS232_TMT
                CLRF    TMR0            ; Clear the TIMER0 count

                BSF     GPIO, TX_EN_Bit ; Enable RS485 transmit
                BCF     GPIO, CTS_Bit   ; CTS Active
                MOVLW   TRIS_LED_ON     ; LED ON state
                TRIS    GPIO
                BCF     GPIO, LED_Bit   ; LED Red

                INCF    Timeout, F      ; Increase Timeout every once in a while

                ;
                ; Check if we are to goto FAST timing loop
                ;

RS232_TMT_ON
                CLRWDT
                BTFSS   GPIO, RS232_Bit ; Check RS232 inactive (1)
                GOTO    RS232_TMT_ON

RS232_TMT_1                             ; Check minimum
                INCF    TMR0, W         ; Make sure count is at least 1
                CLRF    TMR0            ; Clear timer 0 for OFF loop
                MOVWF   Count
                SUBWF   Timeout, W      ; subtract current from current min.
                BTFSS   STATUS, C
                GOTO    RS232_TMT_OFF   ; no carry set (Timeout < W)
                MOVFW   Count           ; carry set (Timeout > W)
                MOVWF   Timeout         ; Save new timeout

RS232_TMT_2
                MOVWF   Timeout_X10     ; Multiply times 8
                INCF    Timeout_X10, F
                RLF     Timeout_X10, F
                RLF     Timeout_X10, F
                RLF     Timeout_X10, F

RS232_TMT_OFF
                CLRWDT
                BTFSS   GPIO, RS232_Bit ; Check RS232 active (0)
                GOTO    RS232_TMT_3
                MOVFW   TMR0            ; check timeout
                SUBWF   Timeout_X10, W
                BTFSS   STATUS, C
                GOTO    RS232_TMT_TIMEOUT
                GOTO    RS232_TMT_OFF

RS232_TMT_3
                INCF    TMR0, W         ; make sure count is at least 1
                CLRF    TMR0            ; clear Timer 0 for ON loop
                MOVWF   Count
                SUBWF   Timeout, W      ; subtract current from current min.
                BTFSS   STATUS, C
                GOTO    RS232_TMT_ON    ; no carry set (Timeout < W)
                MOVFW   Count           ; carry set (Timeout > W)
                MOVWF   Timeout         ; Save new timeout

RS232_TMT_4
                CLRWDT                  ; Kick the dog
                MOVWF   Timeout_X10     ; Multiply times 8
                INCF    Timeout_X10, F
                RLF     Timeout_X10, F
                RLF     Timeout_X10, F
                RLF     Timeout_X10, F
                GOTO    RS232_TMT_ON

RS232_TMT_TIMEOUT
                BCF     GPIO, TX_EN_Bit ; Disable RS485 transmit
                MOVLW   TRIS_LED_OFF    ; LED OFF state
                TRIS    GPIO
                BSF     GPIO, CTS_Bit   ; Make sure CTS inactive!
                RETLW   0


;----------------------------------------------------------------------------

                END
; End of program code
;****************************************************************************
;****************************************************************************


