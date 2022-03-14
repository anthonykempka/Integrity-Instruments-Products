;----------------------------------------------------------------------------
;
; Integrity Instruments
; ---------------------
; PO Box 451   2642 20th Ave SW
; Pine River, MN 56474
; Tel: 800-450-2001
; Web: www.rs-485.com
;
; Description
; -----------
; Integrity Instruments ADC/DIG firmware
;
; Version:  2.0
;
; Note: Earlier versions were based on the Zilog Z8 processor.
;
; ------------
;
; Revision History
; -------------------------------------------------------
; A. Kempka     Original        11/18/98    v2.0
;
;


;
;----------------------------------------------------------------------------
; Specify processor type and output format.
;
        list p=16c65A, f=inhx8m

;----------------------------------------------------------------------------
; Include the Processor specific defines.
;
#INCLUDE "P16C65A.INC"

;
; MPU configuration options
;
__CONFIG   _CP_ALL & _WDT_ON & _HS_OSC & _PWRTE_ON & _BODEN_ON

;----------------------------------------------------------------------------
; General equates.
;----------------------------------------------------------------------------

VERSION_MAJOR       EQU     '2'
VERSION_MINOR       EQU     '1'

LED_ON              EQU     0x01

BAUD_9600           EQU     0x0B
BAUD_19200          EQU     0x05
BAUD_57600          EQU     0x01
BAUD_115200         EQU     0x00


;----------------------------------------------------------------------------
; Port bit defines
;----------------------------------------------------------------------------

LED_RED         EQU     0x01
LED_GREEN       EQU     0x00
RS485_EN        EQU     0x02
EEPROM_CS       EQU     0x01
ADC_CS          EQU     0x00

;--------------------------------------------------------------------------
;   Set Ram Bank 0 and 1 (Variable memory definitions)
;--------------------------------------------------------------------------
RamBank0       EQU     0x20

;--------------------------------------------------------------------------
; Variables
;--------------------------------------------------------------------------

W_TEMP          EQU     RamBank0+0x00
STATUS_TEMP     EQU     RamBank0+0x01
MODULE_ADDRESS  EQU     RamBank0+0x02
RUNNING_CNT_L   EQU     RamBank0+0x03
RUNNING_CNT_H   EQU     RamBank0+0x04
LED_CNT_L       EQU     RamBank0+0x05
BI_UNI_POLAR    EQU     RamBank0+0x06
LED_ON_FLAG     EQU     RamBank0+0x07
EEPROM_ADDRESS  EQU     RamBank0+0x08
DIP_SWITCH      EQU     RamBank0+0x09
OUR_SPBRG       EQU     RamBank0+0x0A
FAST_RESPONSE   EQU     RamBank0+0x0B
HEX_TEMP        EQU     RamBank0+0x0C
DEST_ADR        EQU     RamBank0+0x0D
SRC_ADR         EQU     RamBank0+0x0E
TRANSMIT_WAIT   EQU     RamBank0+0x0F
INPUT_REGISTER  EQU     RamBank0+0x10
SCRATCH         EQU     RamBank0+0x11
CONTROL_ANALOG  EQU     RamBank0+0x12
RCV_ERROR_CNT   EQU     RamBank0+0x13
RS232_FLAG      EQU     RamBank0+0x14
TMR2_RELOAD     EQU     RamBank0+0x15

OUR_TRISB       EQU     RamBank0+0x16
OUR_TRISD       EQU     RamBank0+0x17
OUR_PORTB       EQU     RamBank0+0x18
OUR_PORTD       EQU     RamBank0+0x19

ADC_NIBBLE1     EQU     RamBank0+0x1A
ADC_NIBBLE2     EQU     RamBank0+0x1B
ADC_NIBBLE3     EQU     RamBank0+0x1C
UNIPOLAR        EQU     RamBank0+0x1D
ASYNC_DEST      EQU     RamBank0+0x1E

RCV_BUFFER_PTR  EQU     RamBank0+0x1F
RCV_BUFFER      EQU     RamBank0+0x20
RCV_BUFFER_ST   EQU     RamBank0+0x30
RCV_LAST_CHAR   EQU     RamBank0+0x31
STREAM          EQU     RamBank0+0x32
ASYNC_CONFIG    EQU     RamBank0+0x33
ASYNC_TIME      EQU     RamBank0+0x34
ASYNC_COUNT     EQU     RamBank0+0x35
OUR_TMR0L       EQU     RamBank0+0x36
OUR_TMR0H       EQU     RamBank0+0x37
TMR0H           EQU     RamBank0+0x38
STREAM_ANALOG   EQU     RamBank0+0x39
STREAM_CH       EQU     RamBank0+0x3A

PREV_PORTB      EQU     RamBank0+0x40
PREV_PORTD      EQU     RamBank0+0x41
PREV_TMR0       EQU     RamBank0+0x42
PREV_TMR0H      EQU     RamBank0+0x43

CURRENT_PTR     EQU     RamBank0+0x44

;--------------------------------------------------------------------------
; Program Code
;--------------------------------------------------------------------------



;----------------------------------------------------------------------------
; Setup RESET and interrupt vectors.
;----------------------------------------------------------------------------
        org     0               ; Code start address = 0
        clrf    PCLATH
        goto    Start

        org     4               ; Irq address = 4
        goto    Main_Isr

;----------------------------------------------------------------------------



;----------------------------------------------------------------------------
; Begin code.
;----------------------------------------------------------------------------
;
        org     0x10

;----------------------------------------------------------------------------
; Converts a binary nibble in W to an ASCII Hexadecimal value
; returned in W
;
Nibble_To_Hex
        andlw   0x0F
        addwf   PCL, F
        retlw   '0'
        retlw   '1'
        retlw   '2'
        retlw   '3'
        retlw   '4'
        retlw   '5'
        retlw   '6'
        retlw   '7'
        retlw   '8'
        retlw   '9'
        retlw   'A'
        retlw   'B'
        retlw   'C'
        retlw   'D'
        retlw   'E'
        retlw   'F'

;----------------------------------------------------------------------------
; Initialization and Start-Up code.
;----------------------------------------------------------------------------
Start
; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0     ; select BANK 0
        clrf    PORTA           ; clear PORT A
        clrf    PORTB           ; clear PORT B
        clrf    PORTC           ; clear PORT C
        clrf    PORTD           ; clear PORT D
        clrf    PORTE           ; clear PORT E

; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
        movlw   0x30
        movwf   TRISA           ; set direction PORT A (0..3 OUT: 4-5 IN)
        movlw   0xFF
        movwf   TRISB           ; set direction PORT B (0..7 IN)
        movlw   0xFF
        movwf   TRISD           ; set direction PORT D (0..7 IN)
        movlw   0xD0
        movwf   TRISC           ; set direction PORT C (0..3 OUT: 4,6,7 IN)
        movlw   0x07
        movwf   TRISE           ; set direction port E (0..2 IN)

; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0     ; select BANK 0
        clrf    PORTA           ; clear PORT A
        clrf    PORTB           ; clear PORT B
        clrf    PORTC           ; clear PORT C
        clrf    OUR_SPBRG
        clrf    LED_ON_FLAG
        clrf    LED_CNT_L
        clrf    RUNNING_CNT_L
        clrf    RUNNING_CNT_H
        clrf    TRANSMIT_WAIT
        clrf    RCV_ERROR_CNT
        clrf    RCV_LAST_CHAR
        clrf    STREAM
        clrf    ASYNC_CONFIG
        clrf    ASYNC_TIME
        clrf    ASYNC_COUNT
        clrf    ASYNC_DEST
        clrf    UNIPOLAR
        clrf    TMR0H

        movfw   PORTA           ; Read DIP switch PORT A
        andlw   0x20            ; mask off everything but bit 5
        movwf   DIP_SWITCH
        bcf     STATUS, C       ; Clear the carry before rotate
        rrf     DIP_SWITCH, F   ; rotate to bit 3
        rrf     DIP_SWITCH, F
        movfw   PORTE           ; Read DIP switch PORT E
        andlw   0x07            ; Mask everything but bits 0..2
        iorwf   DIP_SWITCH, F

        movfw   DIP_SWITCH      ; Get the FAST_RESPONSE bit
        andlw   0x08            ; SW Pos 1
        movwf   FAST_RESPONSE

        movfw   DIP_SWITCH      ; Get the RS232_FLAG bit
        andlw   0x01            ; SW Pos 2
        movwf   RS232_FLAG

        movfw   DIP_SWITCH      ; Get the BAUD RATE bits
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x00            ; Val=0, Pos3=OFF Pos4= OFF, Baud=9600
        btfss   STATUS, Z
        goto    Start1
        movlw   BAUD_9600       ; 9600 BAUD
        movwf   OUR_SPBRG
        goto    Start4

Start1  movfw   DIP_SWITCH
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x02            ; Val=2, Pos3=ON Pos4=OFF, Baud=19200
        btfss   STATUS, Z
        goto    Start2
        movlw   BAUD_19200      ; 19200 BAUD
        movwf   OUR_SPBRG
        goto    Start4

Start2  movfw   DIP_SWITCH
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x04            ; Val=4, Pos3=OFF Pos4=ON, Baud=57600
        btfss   STATUS, Z
        goto    Start3
        movlw   BAUD_57600      ; 57600 BAUD
        movwf   OUR_SPBRG
        goto    Start4

Start3                          ; Val=6, Pos3=ON Pos4=ON, BAUD=115200
        movlw   BAUD_115200     ; 115200 BAUD
        movwf   OUR_SPBRG

Start4
        movlw   0x31            ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        movlw   0xF8            ; Load Timer1 counters
        movwf   TMR1H
        movlw   0xCC            ; 0xF8CC == 1 ms. interrupt
        movwf   TMR1L
        movlw   0x01            ; Timer1 prescale 1:1, enable Timer1
        movwf   T1CON
;        movfw   TMR2_RELOAD
;        movwf   TMR2            ; Load Timer2 counter
;        movlw   0x06            ; Timer 2 prescale 1:16, enable Timer 2
;        movwf   T2CON

        movfw   OUR_SPBRG       ; Load Baud Rate Generator
; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
        movwf   SPBRG           ; Load Baud Rate Generator
        bsf     PIE1, RCIE
        bsf     PIE1, TMR1IE    ; Enable Timer1 Interrupt
;        bsf     PIE1, TMR2IE    ; Enable Timer2 Interrupt
        movlw   0x20            ; TXEN=1, SYNC=0, BRGH=0
        movwf   TXSTA
        movlw   0xEF            ; OPTION REGISTER - TMR0 src (Capture Input)
        movwf   OPTION_REG

; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0         ; select BANK 0
        clrf    TMR0
        movlw   0x90                ; UART: SPEN=1, CREN=1
        movwf   RCSTA               ; RS-485/232 setup

        bcf     PORTA, RS485_EN     ; RS-485 transmit OFF
        bsf     PORTA, ADC_CS       ; ADC not selected
        bsf     PORTA, EEPROM_CS    ; EEPROM not selected

Start_LED_On
        clrwdt                      ; Kick the dog
        movlw   LED_ON              ; Set LED on flag
        movwf   LED_ON_FLAG
        bsf     PORTC, LED_GREEN    ; GREEN LED ON
        bcf     PORTC, LED_RED

        movf    RS232_FLAG, F   ; Is this RS-232 buffer pointer?
        btfss   STATUS, Z
        goto    Start_RS232
        movlw   RamBank0+0X20   ; Set the RCV buffer pointer for RS-485
        movwf   RCV_BUFFER_ST
        movwf   RCV_BUFFER_PTR
        goto    Start_Read_Eeprom
Start_RS232
        movlw   RamBank0+0X24   ; Set the RCV buffer pointer for RS-232
        movwf   RCV_BUFFER_ST
        movwf   RCV_BUFFER_PTR

        ; On any INITIALIZATION error, goto Start_LED_On

Start_Read_Eeprom
        movlw   0x00            ; Read EEPROM address 0
        call    Read_EEPROM
        movwf   MODULE_ADDRESS

        movlw   0x01            ; Read ASYNC DEST address
        call    Read_EEPROM
        movwf   ASYNC_DEST

        movlw   0x02            ; Read EEPROM address 2 - Direction PORT B
        call    Read_EEPROM
        movwf   OUR_TRISB
        bsf     STATUS, RP0     ; select BANK 1
        movwf   TRISB           ; set direction PORT B
        bcf     STATUS, RP0     ; select BANK 0

        movlw   0x03            ; Read EEPROM address 3 - Direction PORT D
        call    Read_EEPROM
        movwf   OUR_TRISD
        bsf     STATUS, RP0     ; select BANK 1
        movwf   TRISD           ; set direction PORT D
        bcf     STATUS, RP0     ; select BANK 0

        movlw   0x04            ; Read ASYNC UPDATE address 4
        call    Read_EEPROM
        movwf   ASYNC_CONFIG

        movf    RS232_FLAG, F   ; Async Only Stream if RS-232
        btfsc   STATUS, Z
        clrf    ASYNC_CONFIG

        movfw   PORTB           ; Establish the Previous values for ASYNC updates
        movwf   PREV_PORTB
        movfw   PORTD
        movwf   PREV_PORTD
        movfw   TMR0
        movwf   PREV_TMR0
        movfw   TMR0H
        movwf   PREV_TMR0H

        movfw   RCREG           ; Clear interrupts before enabling
        bcf     PIR1, TMR1IF    ; clear the Timer 1 Irq
        bcf     PIR1, TMR2IF
        movlw   0xe0            ; Enable interrupts, enable TMR0 irq
        movwf   INTCON

;----------------------------------------------------------------------------
; Main processing loop.
;
Main_Loop
        clrwdt                      ; Kick the dog
        movfw   RCV_LAST_CHAR       ; Check the last character RCVD
        xorlw   0x0D                ; Is this a CR?
        btfsc   STATUS, Z
        goto    Main_Loop_Parse

        movf    ASYNC_CONFIG, F     ; Are we in ASYNC mode
        btfsc   STATUS, Z
        goto    Main_Loop1

        movfw   ASYNC_CONFIG        ; Is this Mode 1 ? Update on change only
        xorlw   0x01
        btfss   STATUS, Z
        goto    Main_Loop_Async_Timed

        movfw   PORTB               ; Check if PORTB has changed
        xorwf   PREV_PORTB, W
        btfss   STATUS, Z
        goto    Main_Loop_Async_I

        movfw   PORTD               ; Check if PORTD has changed
        xorwf   PREV_PORTD, W
        btfss   STATUS, Z
        goto    Main_Loop_Async_I

        movfw   TMR0                ; Check if TMR0 has changed
        xorwf   PREV_TMR0, W
        btfss   STATUS, Z
        goto    Main_Loop_Async_N

        movfw   TMR0H               ; Check if TMR0H has changed
        xorwf   PREV_TMR0H, W
        btfss   STATUS, Z
        goto    Main_Loop_Async_N

        goto    Main_Loop1          ; Check stream mode

Main_Loop_Async_I
        movfw   ASYNC_DEST      ; Save ASYNC destination address
        movwf   SRC_ADR         ; Transmit_Delay uses SRC_ADR
        movfw   PORTB           ; Establish the Previous values for ASYNC updates
        movwf   PREV_PORTB
        movfw   PORTD
        movwf   PREV_PORTD
        call    Process_I
        goto    Main_Loop1      ; Check stream mode

Main_Loop_Async_N
        movfw   ASYNC_DEST      ; Save ASYNC destination address
        movwf   SRC_ADR         ; Transmit_Delay uses SRC_ADR
        movfw   TMR0
        movwf   PREV_TMR0
        movfw   TMR0H
        movwf   PREV_TMR0H
        call    Process_N
        goto    Main_Loop1          ; Check STEAM mode

 Main_Loop_Async_Timed
        movfw   ASYNC_CONFIG        ; Are we at the Timeout?
        xorwf   ASYNC_TIME, W
        btfss   STATUS, Z
        goto    Main_Loop1          ; Check STREAM mode
        movfw   ASYNC_DEST          ; Save ASYNC destination address
        movwf   SRC_ADR             ; Transmit_Delay uses SRC_ADR
        clrf    ASYNC_TIME          ; Clear the ASYNC timeout value
        goto    Main_Loop_Stream    ; Update with the Stream configuration

Main_Loop1
        movf    STREAM, F           ; Are we in STREAM Mode?
        btfsc   STATUS, Z
        goto    Main_Loop

Main_Loop_Stream
        movlw   0x19                ; Check stream DIGITAL
        call    Read_EEPROM
        andlw   0xFF
        btfsc   STATUS, Z
        goto    Main_Check_Cont1
        call    Process_I           ; Process DIGITAL

Main_Check_Cont1
        movlw   0x1A                ; Check stream CAPTURE
        call    Read_EEPROM
        andlw   0xFF
        btfsc   STATUS, Z
        goto    Main_Check_Cont2
        call    Process_N           ; Process CAPTURE

Main_Check_Cont2
        movlw   0x10                ; Check stream ANALOG
        call    Read_EEPROM
        andlw   0x0F                ; Limit total count
        movwf   STREAM_ANALOG
        clrf    STREAM_CH           ; Clear the current stream channel
Main_Check_Cont3
        movfw   STREAM_ANALOG       ; Are we done with ANALOG?
        btfsc   STATUS, Z
        goto    Main_Loop
        movfw   STREAM_CH           ; Compute the current channel
        addlw   0x11
        call    Read_EEPROM         ; Read the ANALOG control from EEPROM
        movwf   CONTROL_ANALOG
        andlw   0x80                ; Save unipolat/bipolar bit
        movwf   UNIPOLAR
        movlw   0x0F                ; Mask OFF unipolar/bipolar bit
        andwf   CONTROL_ANALOG, F
        call    Process_Analog
        decf    STREAM_ANALOG, F    ; Decr ANALOG query count
        incf    STREAM_CH, F        ; Next channel
        goto    Main_Check_Cont3


Main_Loop_Parse
;
; Parse incoming message
; If RS-485, then check dest. address and SAVE source address
;
        clrf    RCV_LAST_CHAR       ; Clear last char rcvd.
        movf    RS232_FLAG, F       ; Is this RS-232 data?
        btfss   STATUS, Z
        goto    Main_Loop_Check_V

        clrf    DEST_ADR        ; Clear address variables for conversion
        clrf    SRC_ADR

        movfw   RCV_BUFFER+0    ; Get the DEST address
        call    Hex_To_Nibble   ; Convert the HEX nibble
        movwf   DEST_ADR
        bcf     STATUS, C       ; Make sure C is cleared
        rlf     DEST_ADR, F     ; Rotate to high order nibble location
        rlf     DEST_ADR, F
        rlf     DEST_ADR, F
        rlf     DEST_ADR, F
        movfw   RCV_BUFFER+1
        call    Hex_To_Nibble   ; Convert the HEX nibble
        iorwf   DEST_ADR, F

        movfw   MODULE_ADDRESS  ; Is the packet Destination our address?
        xorwf   DEST_ADR, W
        btfsc   STATUS, Z
        goto    Main_Loop_Got_Address

        movfw   DEST_ADR        ; Is the packet Destination 0xFF?
        xorlw   0xFF
        btfss   STATUS, Z
        goto    Main_Loop_Rcv_End

Main_Loop_Got_Address
        movfw   RCV_BUFFER+2    ; Save the SRC address
        call    Hex_To_Nibble   ; Convert the HEX nibble
        movwf   SRC_ADR
        bcf     STATUS, C       ; Make sure C is cleared
        rlf     SRC_ADR, F      ; Rotate to high order nibble location
        rlf     SRC_ADR, F
        rlf     SRC_ADR, F
        rlf     SRC_ADR, F
        movfw   RCV_BUFFER+3
        call    Hex_To_Nibble   ; Convert the HEX nibble
        iorwf   SRC_ADR, F

        movfw   SRC_ADR         ; Is the source address 0?
        btfss   STATUS, Z
        goto    Main_Loop_Rcv_End

Main_Loop_Check_V
        movfw   RCV_BUFFER_PTR      ; Save off Current Buffer ptr (before it moves)
        movwf   CURRENT_PTR
        movlw   'V'                 ; VERSION command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_I
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'V'
        call    Transmit_Char
        movlw   VERSION_MAJOR       ; Respond with Firmware version
        call    Transmit_Char
        movlw   VERSION_MINOR       ; Respond with Firmware version
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_I
        movlw   'I'                 ; DIGITAL INPUTS command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_O
        movlw   RCV_BUFFER+0x06    ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Process_I
        goto    Main_Loop_Rcv_End

Process_I
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'I'
        call    Transmit_Char
        movfw   PORTB               ; PORT B - MSB
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTB               ; PORT B - LSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT D - MSB
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT D - LSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        return

Main_Loop_Check_O
        movlw   'O'                 ; DIGITAL OUTPUT command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_Q
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the PORT 1 (PORT B) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+6        ; Get the PORT 1 (PORT B) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   PORTB

        movfw   RCV_BUFFER+7        ; Get the PORT 2 (PORT D) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+8        ; Get the PORT 2 (PORT D) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   PORTD

        call    Transmit_Delay      ; Transmit Addresses
        movlw   'O'
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_Q
        movlw   'Q'                 ; BIPOLAR SAMPLE ANALOG command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_U
        movlw   RCV_BUFFER+0x07     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        clrf    UNIPOLAR            ; Bipolar sample
        movfw   RCV_BUFFER+5        ; Get the ANALOG CONTROL nibble
        call    Hex_To_Nibble
        movwf   CONTROL_ANALOG
        call    Process_Analog
        goto    Main_Loop_Rcv_End

Main_Loop_Check_U
        movlw   'U'                 ; UNIPOLAR SAMPLE ANALOG command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_N
        movlw   RCV_BUFFER+0x07     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movlw   0x01
        movwf   UNIPOLAR            ; Unipolar sample
        movfw   RCV_BUFFER+5        ; Get the ANALOG CONTROL nibble
        call    Hex_To_Nibble
        movwf   CONTROL_ANALOG
        call    Process_Analog
        goto    Main_Loop_Rcv_End

Process_Analog
        call    Transmit_Delay      ; Transmit Addresses
        call    Acquire_Sample      ; Acquire the sample from ADC
        movf    UNIPOLAR, F         ; Is the UNIPOLAR flag set?
        btfss   STATUS, Z
        goto    Process_Analog1
        movlw   'Q'                 ; BIPOLAR
        goto    Process_Analog2
Process_Analog1
        movlw   'U'                 ; UNIPOLAR
Process_Analog2
        call    Transmit_Char
        movfw   CONTROL_ANALOG      ; Return the Analog Control nibble
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   ADC_NIBBLE1         ; Here's the Analog Sample
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   ADC_NIBBLE2
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   ADC_NIBBLE3
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        return

Main_Loop_Check_N
        movlw   'N'                 ; CAPTURE COUNT command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_M
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Process_N
        goto    Main_Loop_Rcv_End

Process_N
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'N'
        call    Transmit_Char
        movfw   TMR0                ; Get TMR0 count
        movwf   OUR_TMR0L           ; save local copy so count does during output
        movfw   TMR0H
        movwf   OUR_TMR0H
        andlw   0xF0                ; TMR0H - MSB
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0H           ; TMR0H - LSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L           ; TMR0 Low order byte
        andlw   0xF0                ; TMR0L - MSB
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L           ; TMR0L - LSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        return

Main_Loop_Check_M
        movlw   'M'                 ; CLEAR CAPTURE COUNT command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_T
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        clrf    TMR0                ; Clear the Timer0 count - LSB
        clrf    TMR0H               ; Clear the Timer0 count - MSB
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'M'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_T
        movlw   'T'                 ; SET DIGITAL DIRECTION command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_G
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the PORT 1 (PORT B) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+6        ; Get the PORT 1 (PORT B) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   OUR_TRISB
        bsf     STATUS, RP0         ; select BANK 1
        movwf   TRISB               ; set direction PORT B
        bcf     STATUS, RP0         ; select BANK 0

        movfw   RCV_BUFFER+7        ; Get the PORT 2 (PORT D) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+8        ; Get the PORT 1 (PORT D) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   OUR_TRISD
        bsf     STATUS, RP0         ; select BANK 1
        movwf   TRISD               ; set direction PORT D
        bcf     STATUS, RP0         ; select BANK 0

        movlw   0x02                ; save TRIS registers in EEPROM
        movwf   EEPROM_ADDRESS
        movfw   OUR_TRISB
        call    Write_EEPROM
        movlw   0x03
        movwf   EEPROM_ADDRESS
        movfw   OUR_TRISD
        call    Write_EEPROM

        call    Transmit_Delay      ; Transmit Addresses
        movlw   'T'                 ; EEPROM Write delayed enough already
        call    Transmit_Char        
        movlw   0x0D                ; CR
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_G
        movlw   'G'                 ; GET DATA DIRECTION command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_W
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'G'
        call    Transmit_Char
        movfw   OUR_TRISB           ; Port 1 data direction (PORTB)
        andlw   0xF0                ; TMR0H - MSB
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TRISB           ; Port 1 data direction (PORTB)
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TRISD           ; Port 2 data direction (PORTD)
        andlw   0xF0                ; TMR0L - MSB
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TRISD           ; Port 2 data direction (PORTD)
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_W
        movlw   'W'                 ; WRITE EEPROM command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_R
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the EEPROM ADDRESS - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+6        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   EEPROM_ADDRESS

        movfw   RCV_BUFFER+7        ; Get the EEPROM VALUE - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+8        ; Get the EEPROM VALUE - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Write_EEPROM        ; Write value to EEPROM

        call    Transmit_Delay      ; Transmit Addresses
        movlw   'W'                 ; EEPROM Write delayed enough already
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char

        movlw   0x04                ; Read ASYNC address 4 - since maybe changed
        call    Read_EEPROM
        movwf   ASYNC_CONFIG
        clrf    ASYNC_TIME          ; Clear the ASYNC timeout value

        movlw   0x01                ; Read ASYNC DEST address - maybe changed
        call    Read_EEPROM
        movwf   ASYNC_DEST

        movlw   0x00                ; Read MODULE address - maybe changed
        call    Read_EEPROM
        movwf   MODULE_ADDRESS
        goto    Main_Loop_Rcv_End

Main_Loop_Check_R
        movlw   'R'                 ; READ EEPROM command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_J
        movlw   RCV_BUFFER+0x08     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the EEPROM ADDRESS - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+6        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Read_EEPROM
        movwf   SCRATCH             ; Save the data

        call    Transmit_Delay      ; Transmit Addresses
        movlw   'R'
        call    Transmit_Char
        movfw   SCRATCH             ; Get the saved Data
        andlw   0xF0                ; Send out the MSB
        movwf   EEPROM_ADDRESS
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     EEPROM_ADDRESS, F
        rrf     EEPROM_ADDRESS, F
        rrf     EEPROM_ADDRESS, F
        rrf     EEPROM_ADDRESS, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   SCRATCH             ; Get the saved Data
        andlw   0x0F                ; Send out the LSB
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_J
        movlw   'J'                 ; CLEAR RCV ERROR COUNT command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_S
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        clrf    RCV_ERROR_CNT
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'J'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_S
        movlw   'S'                 ; STREAM MODE command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_H
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movf    RS232_FLAG, F       ; Only Stream if RS-232
        btfsc   STATUS, Z
        goto    Main_Loop_Error_Response
        movlw   0x01                ; Set STREAM FLAG
        movwf   STREAM
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'S'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_H
        movlw   'H'                 ; HALT STREAM MODE command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_Y
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movf    RS232_FLAG, F       ; Only Stream if RS-232
        btfsc   STATUS, Z
        goto    Main_Loop_Error_Response
        clrf    STREAM              ; Clear Stream Flag
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'H'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_Y
        movlw   'Y'                 ; HOST ASYNC response
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_Z
        nop                         ; Don't respond!
        goto    Main_Loop_Rcv_End

Main_Loop_Check_Z
        movlw   'Z'                 ; RESET command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_K
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'Z'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
Main_Loop_Reset_Cpu                 ; Force a CPU reset by not kicking the DOG
        goto    Main_Loop_Reset_Cpu

Main_Loop_Check_K
        movlw   'K'                 ; RCV ERRORS command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'K'
        call    Transmit_Char
        movfw   RCV_ERROR_CNT       ; ERROR count - MSB
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   RCV_ERROR_CNT        ; ERROR count - LSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Error_Response
        call    Transmit_Delay      ; Transmit Addresses
        movlw   'X'                 ; ERROR Response
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char

Main_Loop_Rcv_End
        bcf     PORTA, RS485_EN     ; Disable RS-485 driver
        movfw   RCV_BUFFER_ST       ; Reset the RCV buffer pointer
        movwf   RCV_BUFFER_PTR
        goto    Main_Loop           ; Ready for new message


;----------------------------------------------------------------------------
; Waits 1-2 character times and transmitss SRC DEST addresses
;
Transmit_Delay
        bcf     PORTC, LED_GREEN        ; Turn RED LED ON
        bsf     PORTC, LED_RED
        movlw   LED_ON                  ; set the ON flag
        movwf   LED_ON_FLAG
        clrf    LED_CNT_L

        movf    RS232_FLAG, F       ; Is this RS-232 data?
        btfss   STATUS, Z
        return

        movf    FAST_RESPONSE, F    ; Is this a FAST RESPONSE?
        btfss   STATUS, Z
        goto    Transmit_No_Wait

        movlw   0xCC                ; 0xF8CC == 1 ms. interrupt
        movwf   TMR1L
        movlw   0xF8                ; Re-Load Timer1 counters
        movwf   TMR1H
        clrf    TRANSMIT_WAIT

Transmit_Wait                   ; Delay for appprox. 4 milliseconds
        movfw   TRANSMIT_WAIT
        xorlw   0x04
        btfss   STATUS, Z
        goto    Transmit_Wait

Transmit_No_Wait
        bsf     PORTA, RS485_EN     ; Enable the RS-485
        movfw   SRC_ADR
        andlw   0xF0
        movwf   HEX_TEMP
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        movfw   HEX_TEMP
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   SRC_ADR
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char

        movfw   MODULE_ADDRESS
        andlw   0xF0
        movwf   HEX_TEMP
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        movfw   HEX_TEMP
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   MODULE_ADDRESS
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        return

;----------------------------------------------------------------------------
; Transmits a single character
;
Transmit_Char
        movwf   TXREG
        bsf     STATUS, RP0     ; select BANK 1
Transmit_Pending
        clrwdt                  ; Kick the dog
        btfss   TXSTA, TRMT
        goto    Transmit_Pending
        bcf     STATUS, RP0     ; select BANK 0
        return



;----------------------------------------------------------------------------
; Acquires an ADC sample from the LTC1294/1296
;
; ADC config in CONTROL_ANALOG
;
; Data return in ADC_NIBBLE1/2/3
;
Acquire_Sample
        movlw   0x31                    ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        bcf     PORTA, ADC_CS           ; Select the ADC
        movfw   CONTROL_ANALOG          ; Restore CONTROL in W
        iorlw   0x10                    ; OR in  the START bit
        call    Spi_Send                ; Send part 1 ADC command
        movf    UNIPOLAR, F             ; Is the UNIPOLAR flag set?
        btfss   STATUS, Z
        goto    Acquire_Sample1
        movlw   0x60                    ; UNI=0, MSBF=1, PS=1
        goto    Acquire_Sample2
Acquire_Sample1
        movlw   0xE0                    ; UNI=1, MSBF=1, PS=1
Acquire_Sample2
        call    Spi_Send                ; Send part 2 ADC command - rcv 1st Nibble
        andlw   0x0F                    ; Mask off MSB nibble
        movwf   ADC_NIBBLE1
        call    Spi_Send                ; rcv 2nd 3rd Nibbles (send crap)
        movwf   SCRATCH                 ; save off
        andlw   0x0F                    ; Mask off LSB nibble (3rd Nibble)
        movwf   ADC_NIBBLE3
        movlw   0xF0                    ; Mask off MSB nibble (2nd nibble)
        andwf   SCRATCH, F
        bcf     STATUS, C               ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, F
        rrf     SCRATCH, W
        movwf   ADC_NIBBLE2
        bsf     PORTA, ADC_CS           ; De-Select the ADC
        return


;----------------------------------------------------------------------------
; Reads the EEPROM
;
; Address to read in W
; Value returned in W
;
Read_EEPROM
        movwf   SCRATCH                 ; Save off address
        movlw   0x30                    ; SPI Master mode, CKP=1, Fosc/4
        movwf   SSPCON
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x03                    ; READ command, A8=0
        call    Spi_Send
        movfw   SCRATCH                 ; Send ADDRESS byte
        call    Spi_Send
        movlw   0xFF                    ; Send DUMMY just to receive
        call    Spi_Send
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM
        return

;----------------------------------------------------------------------------
; Write the EEPROM
;
; Address in EEPROM_ADDRESS
; Value in W
;
;
Write_EEPROM
        movwf   SCRATCH                 ; Save off DATA value
        movlw   0x30                    ; SPI Master mode, CKP=1, Fosc/4
        movwf   SSPCON
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x06                    ; WRITE ENABLE command
        call    Spi_Send
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM to set WRITE ENABLE
        nop
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x02                    ; WRITE command, A8=0
        call    Spi_Send
        movfw   EEPROM_ADDRESS          ; ADDRESS byte
        call    Spi_Send
        movfw   SCRATCH                 ; DATA byte
        call    Spi_Send
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM
        nop
Write_EEPROM_Done
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x05                    ; READ STATUS REGISTER
        call    Spi_Send
        movlw   0xFF                    ; Send DUMMY just to receive
        call    Spi_Send
        movwf   SCRATCH
        btfsc   SCRATCH, 0
        goto    Write_EEPROM_Done
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM
        return


;----------------------------------------------------------------------------
; Sends/Receives a byte via the SPI interface
;
; Data to transmit in W
; Received data returned in W
;
Spi_Send
        movwf   SSPBUF                  ; transmit/receive byte
        bsf     STATUS, RP0             ; select BANK 1
Spi_Pending
        btfss   SSPSTAT, BF             ; waiting for byte to send
        goto    Spi_Pending
        bcf     STATUS, RP0             ; select BANK 0
        movfw   SSPBUF                  ; read to empty - return in W
        return


;----------------------------------------------------------------------------
; Converts a ASCII Hexadecimal value in W to a binary nibble
; returned in W. Returns 0 if ASCII outside of Hexdecimal range.
;
Hex_To_Nibble
        movwf   HEX_TEMP
        xorlw   '0'
        btfsc   STATUS, Z
        retlw   0x00
        movfw   HEX_TEMP
        xorlw   '1'
        btfsc   STATUS, Z
        retlw   0x01
        movfw   HEX_TEMP
        xorlw   '2'
        btfsc   STATUS, Z
        retlw   0x02
        movfw   HEX_TEMP
        xorlw   '3'
        btfsc   STATUS, Z
        retlw   0x03
        movfw   HEX_TEMP
        xorlw   '4'
        btfsc   STATUS, Z
        retlw   0x04
        movfw   HEX_TEMP
        xorlw   '5'
        btfsc   STATUS, Z
        retlw   0x05
        movfw   HEX_TEMP
        xorlw   '6'
        btfsc   STATUS, Z
        retlw   0x06
        movfw   HEX_TEMP
        xorlw   '6'
        btfsc   STATUS, Z
        retlw   0x06
        movfw   HEX_TEMP
        xorlw   '7'
        btfsc   STATUS, Z
        retlw   0x07
        movfw   HEX_TEMP
        xorlw   '8'
        btfsc   STATUS, Z
        retlw   0x08
        movfw   HEX_TEMP
        xorlw   '9'
        btfsc   STATUS, Z
        retlw   0x09
        movfw   HEX_TEMP
        xorlw   'A'
        btfsc   STATUS, Z
        retlw   0x0A
        movfw   HEX_TEMP
        xorlw   'a'
        btfsc   STATUS, Z
        retlw   0x0A
        movfw   HEX_TEMP
        xorlw   'B'
        btfsc   STATUS, Z
        retlw   0x0B
        movfw   HEX_TEMP
        xorlw   'b'
        btfsc   STATUS, Z
        retlw   0x0B
        movfw   HEX_TEMP
        xorlw   'C'
        btfsc   STATUS, Z
        retlw   0x0C
        movfw   HEX_TEMP
        xorlw   'c'
        btfsc   STATUS, Z
        retlw   0x0C
        movfw   HEX_TEMP
        xorlw   'D'
        btfsc   STATUS, Z
        retlw   0x0D
        movfw   HEX_TEMP
        xorlw   'd'
        btfsc   STATUS, Z
        retlw   0x0D
        movfw   HEX_TEMP
        xorlw   'E'
        btfsc   STATUS, Z
        retlw   0x0E
        movfw   HEX_TEMP
        xorlw   'e'
        btfsc   STATUS, Z
        retlw   0x0E
        movfw   HEX_TEMP
        xorlw   'F'
        btfsc   STATUS, Z
        retlw   0x0F
        movfw   HEX_TEMP
        xorlw   'f'
        btfsc   STATUS, Z
        retlw   0x0F
        retlw   0


;----------------------------------------------------------------------------
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
;
; Interrupt Service Routines Follow.
;
;
;----------------------------------------------------------------------------
; Timer1_Isr
;
; Bump the running time variables
;----------------------------------------------------------------------------
;
Timer1_Isr
        movlw   0xF8            ; Re-Load Timer1 counters
        movwf   TMR1H
        movlw   0xBC            ; 0xF8CC - 10 instructions
        movwf   TMR1L
        bcf     PIR1, TMR1IF    ; clear the Timer 1 Irq

        incf    TRANSMIT_WAIT, F    ; Incr transmit delay counter

        incf    ASYNC_COUNT, F      ; Incr the ASYNC counter
        movfw   ASYNC_COUNT
        xorlw   0x64                ; Have we gone 100 milliseconds?
        btfss   STATUS, Z
        goto    Timer1_Isr_Cnt
        incf    ASYNC_TIME, F
        clrf    ASYNC_COUNT

Timer1_Isr_Cnt
        incfsz  RUNNING_CNT_L, F        ; Bump the LSB counter
        goto    Timer1_Isr_End
        incf    RUNNING_CNT_H, F        ; Every 255 milliseconds gets here

Timer1_Isr_LED
        movf    LED_ON_FLAG, F          ; Handle the LED blink
        btfsc   STATUS, Z
        goto    Timer1_Led_Off
        incf    LED_CNT_L, F
        movfw   LED_CNT_L
        xorlw   0x02
        btfss   STATUS,Z
        goto    Timer1_Isr_End
        bcf     PORTC, LED_GREEN        ; Turn GREEN LED OFF
        bcf     PORTC, LED_RED
        clrf    LED_ON_FLAG             ; clear the ON flag
        goto    Timer1_Isr_End

Timer1_Led_Off
        incf    LED_CNT_L, F
        movfw   LED_CNT_L
        xorlw   0x07
        btfss   STATUS,Z
        goto    Timer1_Isr_End
        bsf     PORTC, LED_GREEN        ; Turn GREEN LED ON
        bcf     PORTC, LED_RED
        movlw   LED_ON                  ; set the ON flag
        movwf   LED_ON_FLAG
        clrf    LED_CNT_L

Timer1_Isr_End
        return



;----------------------------------------------------------------------------
; Timer2_Isr
;
; Bump the running time variables
;----------------------------------------------------------------------------
;
Timer2_Isr
        movfw   TMR2_RELOAD
        movwf   TMR2            ; Load Timer2 counter
        bcf     PIR1, TMR2IF    ; clear the Timer 2 Irq

        return



;
;----------------------------------------------------------------------------
; Rcv_Isr
;
; Asynchronous data reception ISR
;----------------------------------------------------------------------------
;
Rcv_Isr
        btfsc   RCSTA, FERR
        goto    Rcv_Isr_Frame_Error
        btfsc   RCSTA, OERR
        goto    Rcv_Isr_Over_Error
        goto    Rcv_Isr_Read

Rcv_Isr_Over_Error
        bcf     RCSTA, CREN         ; Clear CREN to reset receive logic
        nop
        bsf     RCSTA, CREN
Rcv_Isr_Frame_Error
        incf    RCV_ERROR_CNT
        movfw   RCREG               ; read data byte into W and DUMP
        clrf    RCV_LAST_CHAR
        return

Rcv_Isr_Read
        movfw   RCREG               ; read data byte into W
        movwf   RCV_LAST_CHAR
        movlw   0x0A                ; Ignore line feeds
        xorwf   RCV_LAST_CHAR, W
        btfsc   STATUS, Z
        return                      ; return without processing

        movfw   RCV_BUFFER_PTR      ; Get RCV buffer pointer
        movwf   FSR
        movfw   RCV_LAST_CHAR
        movwf   INDF                ; Save off in RCV buffer

        bsf     PORTC, LED_GREEN    ; GREEN LED ON
        bcf     PORTC, LED_RED
        clrf    LED_CNT_L
        movlw   LED_ON              ; Set LED on flag
        movwf   LED_ON_FLAG

        movfw   RCV_BUFFER_PTR      ; Get RCV buffer pointer
        incf    RCV_BUFFER_PTR, W   ; Bump RCV buffer pointer for next read
        andlw   0x4F                ; Prevent Buffer overrun
        movwf   RCV_BUFFER_PTR
        return

;
;----------------------------------------------------------------------------
; Tmr0_Isr
;
; Timer 0 ISR - Capture Port
;----------------------------------------------------------------------------
;
Tmr0_Isr
        bcf     INTCON, T0IF        ; Clear the interrupt source
        incf    TMR0H               ; Bump the High order byte count
        return
;
;----------------------------------------------------------------------------
; Main_Isr
;
; Main Interrupt Service routine. Interrupt sources are checked and routed
; to the proper specific ISR. W and STATUS are saved and restored.
;----------------------------------------------------------------------------
;
Main_Isr
        movwf   W_TEMP          ; save W in either BANK 0 or BANK 1
        swapf   STATUS, W       ; swap STATUS into W
        bcf     STATUS, RP0     ; select BANK 0
        movwf   STATUS_TEMP     ; save STATUS in temp

        btfsc   PIR1, TMR1IF    ; Is TMR1IF Set?
        call    Timer1_Isr

        btfsc   PIR1, TMR2IF    ; Is TMR2IF Set?
        call    Timer2_Isr

        btfsc   PIR1, RCIF      ; is bit 5 (RCIF) Set?
        call    Rcv_Isr

        btfsc   INTCON, T0IF    ; Is Timer0 Set?
        call    Tmr0_Isr

        swapf   STATUS_TEMP, W  ; move old STATUS into W
        movwf   STATUS          ; restore status
        swapf   W_TEMP, F       ; swap it so it will be right in the next instruction.
        swapf   W_TEMP, W       ; move old W into W
        retfie
;
;----------------------------------------------------------------------------
;

Copyright_Notice
        db      "Copyright 1997-1998 Integrity Instruments"

;
; End of Code
;
        end

