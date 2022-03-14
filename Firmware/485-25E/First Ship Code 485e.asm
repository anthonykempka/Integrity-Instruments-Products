;**************************************************************************
; 485E.ASM
;
; High Speed Auto Enable, Auto Bauad detect converter
;
;
; Variable    Initial    Description
;              Value
;--------------------------------------------------------------------------
;
;
;**************************************************************************

        LIST P=12C508, R=DEC

#include "p12c508.inc"

;
; Use the following for production code
	__CONFIG   _CP_ON & _WDT_ON & _MCLRE_OFF & _IntRC_OSC

;
; Use the following for Debugging
;	__CONFIG   _CP_OFF & _WDT_OFF & _MCLRE_OFF & _IntRC_OSC

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
Timeout_X8      EQU     ScratchPadRam+1
Temp            EQU     ScratchPadRam+2
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

                MOVLW   0xFF            ; Starting timeout 0xFF
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
; Handles the RS-485 receive
;
RS485_RCV
                BSF     GPIO, CTS_Bit   ; Make sure CTS inactive!
                MOVLW   TRIS_LED_ON     ; LED ON state
                TRIS    GPIO
                BSF     GPIO, LED_Bit   ; LED Green

RS485_RCV_START
                MOVLW   0xFF            ; Set the timeout
                MOVWF   Timeout

RS485_RCV_LOOP
                CLRWDT                  ; Kick the dog
                BTFSS   GPIO, RS232_Bit ; Check RS232 active (0)
                GOTO    RS485_RCV_TMT
                BTFSS   GPIO, RS485_Bit ; Check RS485 active (0)
                GOTO    RS485_RCV_START
                DECFSZ  Timeout, F      ; Do timeout decrement
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
; Handles the RS-232 (HOST PC) transmit
;
; When this routine is entered, we are seeing the START bit of
; the first byte being sent.
;
RS232_TMT
                BSF     GPIO, TX_EN_Bit ; Enable RS485 transmit
                BCF     GPIO, CTS_Bit   ; CTS Active
                MOVLW   TRIS_LED_ON     ; LED ON state
                TRIS    GPIO
                BCF     GPIO, LED_Bit   ; LED Red

 ;               MOVLW   0xFF            ; Starting timeout 0xFF
 ;               MOVWF   Timeout
 ;               MOVLW   0x48            ; Timeout for 19200 Bps
 ;               MOVWF   Timeout_X8

                MOVLW   0x02
                ADDWF   Timeout, F      ; Increase each char received

RS232_TMT_CLR
                CLRF    Count           ; Clear Count for timing

RS232_TMT_ON
                INCFSZ  Count, F
                GOTO    RS232_TMT_ON1
                MOVLW   0xFF            ; 0xFF maximum!
RS232_TMT_ON1
                BTFSC   GPIO, RS232_Bit ; Check RS232 inactive (1)
                GOTO    RS232_TMT_1
                NOP                     ; match same as TMT_OFF loop
                NOP
                NOP
                NOP
                GOTO    RS232_TMT_ON

RS232_TMT_1                             ; Check minimum
                MOVFW   Count
                SUBWF   Timeout, W      ; subtract current from current min.
                BTFSS   STATUS, C
                GOTO    RS232_TMT_2     ; no carry set
                MOVFW   Count           ; carry set
                MOVWF   Timeout

RS232_TMT_2
                MOVLW   2
                ADDWF   Timeout, W
                MOVWF   Timeout_X8      ; Multiply times 8
                RLF     Timeout_X8, F
                RLF     Timeout_X8, F
                RLF     Timeout_X8, F

                CLRF    Count                    ; Clear W for timing

RS232_TMT_OFF
                INCFSZ  Count, F
                GOTO    RS232_TMT_OFF1
                MOVLW   0xFF            ; 0xFF maximum!
RS232_TMT_OFF1
                BTFSS   GPIO, RS232_Bit ; Check RS232 active (0)
                GOTO    RS232_TMT_3
                MOVFW   Count            ; check timeout
                XORWF   Timeout_X8, W
                BTFSC   STATUS, Z
                GOTO    RS232_TMT_TIMEOUT
                GOTO    RS232_TMT_OFF

RS232_TMT_3
                MOVFW   Count
                SUBWF   Timeout, W      ; subtract current from current min.
                BTFSS   STATUS, C
                GOTO    RS232_TMT_4     ; no carry set
                MOVFW   Count           ; carry set
                MOVWF   Timeout

RS232_TMT_4
                CLRWDT                  ; Kick the dog
                MOVLW   2
                ADDWF   Timeout, W
                MOVWF   Timeout_X8      ; Multiply times 8
                RLF     Timeout_X8, F
                RLF     Timeout_X8, F
                RLF     Timeout_X8, F
                GOTO    RS232_TMT_CLR

RS232_TMT_TIMEOUT
                BCF     GPIO, TX_EN_Bit ; Disable RS485 transmit
                MOVLW   TRIS_LED_OFF    ; LED OFF state
                TRIS    GPIO
                BSF     GPIO, CTS_Bit   ; Make sure CTS inactive!
                RETLW   0

RS232_LONG_TIMEOUT
;----------------------------------------------------------------------------
;
                END

