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
; RCE-1 Firmware
; Remote Contact Extender
;
; Based on Integrity Instruments ADC/DIG firmware
;
; Version:  2.5
;
; ------------
;
; Revision History
; -------------------------------------------------------
;   A. Kempka   Original    11/18/98    v2.0
;   A. Kempka   v2.3        11/1/99     Added delta Digital and Delta counter
;                                       to the Async changed update.
;   A. Kempka   v2.5        5/5/2002    Converted v2.3 ADC-DIG code to
;                                       RCE-1 v2.5
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
VERSION_MINOR       EQU     '5'

LED_ON              EQU     0x01

TRUE                EQU     1
FALSE               EQU     0

;----------------------------------------------------------------------------
; EEPROM addresses
;----------------------------------------------------------------------------
EE_MOD_ADDR         EQU     0x00    ; Module address
EE_DEST_ADDR        EQU     0x01    ; Async destination address
EE_PORT1_DIR        EQU     0x02    ; Port 1 data direction (PORTB)
EE_PORT2_DIR        EQU     0x03    ; Port 2 data direction (PORTD)
EE_ASYNC_CONFIG     EQU     0x04    ; Async configuration
EE_ASYNC_CONFIG2    EQU     0x05    ; Async configuration byte 2
EE_PORT1_DEF        EQU     0x06    ; Port 1 default power up setting (PORTB)
EE_PORT2_DEF        EQU     0x07    ; Port 2 default power up setting (PORTD)
EE_EXPANDER         EQU     0x08    ; TRUE (1) if expander Opto-22 modules in use
EE_DAC0_MSB         EQU     0x09    ; DAC Channel 0 power up setting
EE_DAC0_LSB         EQU     0x0A    ; DAC Channel 0 power up setting
EE_DAC1_MSB         EQU     0x0B    ; DAC Channel 1 power up setting
EE_DAC1_LSB         EQU     0x0C    ; DAC Channel 1 power up setting
EE_OFFSET_CALIB     EQU     0x0F    ; A/D Offset calibration (2's compliment)
EE_STREAM_ANALOG    EQU     0x10    ;
EE_ANALOG_CFG_BASE  EQU     0x11
EE_STREAM_DIG       EQU     0x19    ;
EE_STREAM_COUNT     EQU     0x1A    ;

;----------------------------------------------------------------------------
;
; Baud Rate divisors
; Clock = 7.3728 Mhz with prescale of 64
;
;SPBRG_600           EQU     0xBF
;SPBRG_1200          EQU     0x5F
;SPBRG_2400          EQU     0x2F
;SPBRG_4800          EQU     0x17
;SPBRG_9600          EQU     0x0B
;SPBRG_19200         EQU     0x05
;SPBRG_38400         EQU     0x02
;SPBRG_57600         EQU     0x01
;SPBRG_115200        EQU     0x00
;
; Clock = 14.7456 Mhz with prescale of 64
;
SPBRG_1200          EQU     0xBF
SPBRG_2400          EQU     0x5F
SPBRG_4800          EQU     0x2F
SPBRG_9600          EQU     0x17
SPBRG_19200         EQU     0x0B
SPBRG_38400         EQU     0x05
SPBRG_57600         EQU     0x02
SPBRG_115200        EQU     0x01
SPBRG_230400        EQU     0x00

;----------------------------------------------------------------------------
;
; Timer 1 reload values
;
; Clock = 7.3728 Mhz with prescale of 1
; 0xF8CC - instruction count for ISR (0x10) = 0xF8BC
;TMR1_RELOAD_H       EQU     0xF8
;TMR1_RELOAD_L       EQU     0xBC
; Clock = 14.7456 Mhz with prescale of 1
; 0xF199 - instruction count for ISR (0x10) = 0xF189
TMR1_RELOAD_H       EQU     0xF1
TMR1_RELOAD_L       EQU     0x89

;
; TRIS value for Master - even bits = inputs,  odd bits = outputs
; TRIS value for Slave  - even bits = outputs, odd bits = inputs
MASTER_TRIS     EQU     0x55
SLAVE_TRIS      EQU     0xAA

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
MASTER_UNIT     EQU     RamBank0+0x06

LED_ON_FLAG     EQU     RamBank0+0x07
EEPROM_ADDRESS  EQU     RamBank0+0x08
DIP_SWITCH      EQU     RamBank0+0x09
OUR_SPBRG       EQU     RamBank0+0x0A

MASTER_START    EQU     RamBank0+0x0B

HEX_TEMP        EQU     RamBank0+0x0C
DEST_ADR        EQU     RamBank0+0x0D
SRC_ADR         EQU     RamBank0+0x0E
TRANSMIT_WAIT   EQU     RamBank0+0x0F

EXPANDER        EQU     RamBank0+0x10

SCRATCH         EQU     RamBank0+0x11

IDLE_TIMEOUT    EQU     RamBank0+0x12

RCV_ERROR_CNT   EQU     RamBank0+0x13

ASYNC_DEST      EQU     RamBank0+0x1E

RCV_BUFFER_PTR  EQU     RamBank0+0x1F
RCV_BUFFER      EQU     RamBank0+0x20
RCV_BUFFER_ST   EQU     RamBank0+0x30
RCV_LAST_CHAR   EQU     RamBank0+0x31
OUR_TMR0L       EQU     RamBank0+0x36
OUR_TMR0H       EQU     RamBank0+0x37
TMR0H           EQU     RamBank0+0x38

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
        clrf    PR2             ; PWM period = 0

; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0     ; select BANK 0
        clrf    OUR_SPBRG
        clrf    LED_ON_FLAG
        clrf    LED_CNT_L
        clrf    RUNNING_CNT_L
        clrf    RUNNING_CNT_H
        clrf    TRANSMIT_WAIT
        clrf    RCV_ERROR_CNT
        clrf    RCV_LAST_CHAR
        clrf    TMR0H

        call    Read_DIP        ; Read DIP switch settings and set BAUD rate

        movlw   0x31            ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        movlw   TMR1_RELOAD_H   ; Load Timer1 counters
        movwf   TMR1H
        movlw   TMR1_RELOAD_L   ; 0xF8CC == 1 ms. interrupt
        movwf   TMR1L
        movlw   0x01            ; Timer1 prescale 1:1, enable Timer1
        movwf   T1CON

        clrf    CCPR1L          ; PWM LSB = 0
        movlw   0x0C            ; PWM mode, PWM MSB = 0
        movwf   CCP1CON         ; Initially PWM = 0%
        clrf    TMR2
        movlw   0x05            ; Timer 2 prescale 1:4, enable Timer 2
        movwf   T2CON

; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
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

        movlw   RamBank0+0X20       ; Set the RCV buffer pointer for RS-485
        movwf   RCV_BUFFER_ST
        movwf   RCV_BUFFER_PTR

        ;
        ; NOTE: On any INITIALIZATION error, goto Start_LED_On
        ;

        call    Read_Config     ; Read EEPROM configuration data

        movlw   0xFF
        movwf   EXPANDER        ; EXPANDER Opto-22 inversion

        ;
        ; Do the Master/Slave settings
        ;
        clrf    MASTER_START
        clrf    MASTER_UNIT
        btfss   MODULE_ADDRESS, 0   ; If odd number, then MASTER unit
        goto    Set_TRIS_Slave
Set_TRIS_Master
        movlw   TRUE
        movwf   MASTER_UNIT
        movlw   MASTER_TRIS
        goto    Set_TRIS

Set_TRIS_Slave
        movlw   SLAVE_TRIS

Set_TRIS
        bsf     STATUS, RP0     ; Select BANK 1
; ------------- BANK 1 ------------------------------------------------------
        movwf   TRISB           ; set direction PORT B
        movwf   TRISD           ; set direction PORT D
; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0     ; select BANK 0

        movlw   0xFF            ; Set default values for PORTB/PORTD
        movwf   PORTB           ; all off (the expander needs the inversion)
        movwf   PORTD

Start_Final
        movfw   RCREG           ; Clear interrupts before enabling
        bcf     PIR1, TMR1IF    ; clear the Timer 1 Irq
        bcf     PIR1, TMR2IF
        movlw   0xe0            ; Enable interrupts, enable TMR0 irq
        movwf   INTCON

        movfw   RUNNING_CNT_H   ; Get current 255 ms count
        addlw   0x04
        movwf   IDLE_TIMEOUT    ; Timeout approx. 1 second

;----------------------------------------------------------------------------
; Main processing loop.
;
Main_Loop
        clrwdt                      ; Kick the dog
        bcf     PORTA, RS485_EN     ; Disable RS-485 driver - RECEIVE
        movfw   RCV_LAST_CHAR       ; Check the last character RCVD
        xorlw   0x0D                ; Is this a CR?
        btfsc   STATUS, Z
        goto    Main_Loop_Parse

        movf    MASTER_UNIT, F      ; Do we initiate the Sxxyy command?
        btfsc   STATUS, Z
        goto    Main_Loop_DIP
        movf    MASTER_START, F     ; Is the DIP switch set to start?
        btfsc   STATUS, Z
        goto    Main_Loop_DIP

        ;
        ; Send the Sxxyy command
        ;
Send_Remote_Update
        movfw   ASYNC_DEST          ; Save ASYNC destination address
        movwf   SRC_ADR             ; Transmit_Delay() uses SRC_ADR
        call    Transmit_Delay      ; Transmit addresses
        movlw   'S'
        call    Transmit_Char
        movfw   PORTB               ; PORT 1 (PORT B) - MSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTB               ; PORT 1 (PORT B) - LSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT 2 (PORT D) - MSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT 2 (PORT D) - LSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char

        ;
        ; Delay 8 ms for Slave unit to respond
        ;
        bcf     PORTA, RS485_EN     ; Disable RS-485 driver
        movfw   RUNNING_CNT_L       ; Get current 1 ms count
        addlw   0x08
        movwf   TRANSMIT_WAIT       ; Wait for 11 ms. (actually statistically 10.5 ms)

Send_Remote_Update_Delay
        movfw   RUNNING_CNT_L
        subwf   TRANSMIT_WAIT, W
        btfss   STATUS, Z
        goto    Send_Remote_Update_Delay

Main_Loop_DIP
        call    Read_DIP            ; Read the DIP switch in case it changed

Main_Loop_Check_Timeout
        movfw   RUNNING_CNT_H
        subwf   IDLE_TIMEOUT, W
        btfss   STATUS, Z
        goto    Main_Loop

Main_Loop_Timeout        
        movlw   0xFF            ; Set default values for PORTB/PORTD
        movwf   PORTB           ; all off (the expander needs the inversion)
        movwf   PORTD
        goto    Main_Loop


Main_Loop_Parse
;
; Parse incoming message
; If RS-485, then check dest. address and SAVE source address
;
        clrf    RCV_LAST_CHAR   ; Clear last char rcvd.
        clrf    DEST_ADR        ; Clear address variables for conversion
        clrf    SRC_ADR

        movfw   RCV_BUFFER+0    ; Get the DEST address
        call    Hex_To_Nibble   ; Convert the HEX nibble
        movwf   DEST_ADR
        swapf   DEST_ADR, F
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
        ;
        ; Address 0xFF fall through here
        ;
Main_Loop_Got_Address
        movfw   RCV_BUFFER+2    ; Save the SRC address
        call    Hex_To_Nibble   ; Convert the HEX nibble
        movwf   SRC_ADR
        swapf   SRC_ADR, F
        movfw   RCV_BUFFER+3
        call    Hex_To_Nibble   ; Convert the HEX nibble
        iorwf   SRC_ADR, F

Main_Loop_Check_V
        movfw   RCV_BUFFER_PTR      ; Save off Current Buffer ptr (before it moves)
        movwf   CURRENT_PTR
        movlw   'V'                 ; VERSION command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_S
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

Main_Loop_Check_S
        movlw   'S'                 ; REMOTE CONTACT EXTENDER command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_W
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the PORT 1 (PORT B) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+6        ; Get the PORT 1 (PORT B) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        movwf   PORTB

        movfw   RCV_BUFFER+7        ; Get the PORT 2 (PORT D) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+8        ; Get the PORT 2 (PORT D) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        movwf   PORTD
        
        movfw   RUNNING_CNT_H       ; Get current 255 ms count
        addlw   0x04
        movwf   IDLE_TIMEOUT        ; Timeout approx. 1 second

        ;
        ; Only Slave sends response
        ; Send our PORTB/D data back to Master
        ;
        movf    MASTER_UNIT, F      ; Are we master?
        btfss   STATUS, Z
        goto    Main_Loop_Rcv_End
        
        call    Transmit_No_Delay   ; Transmit addresses - No delay!
        movlw   'S'
        call    Transmit_Char
        movfw   PORTB               ; PORT 1 (PORT B) - MSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTB               ; PORT 1 (PORT B) - LSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT 2 (PORT D) - MSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTD               ; PORT 2 (PORT D) - LSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
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

        call    Read_Config         ; Read EEPROM configuration since it may have changed

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
        goto    Main_Loop_Check_Z
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


Main_Loop_Check_Z
        movlw   'Z'                 ; RESET command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_X
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


Main_Loop_Check_X
        movlw   'X'                 ; ERROR acknowledgedment
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_K

        ;
        ; Don't send anything back!
        ;
        goto    Main_Loop_Rcv_End


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
        movfw   RUNNING_CNT_L       ; Get current 1 ms count
        addlw   0x02
        movwf   TRANSMIT_WAIT       ; Wait for 2 ms. (actually statistically 1.5 ms)

Transmit_Delay1
        movfw   RUNNING_CNT_L
        subwf   TRANSMIT_WAIT, W
        btfss   STATUS, Z
        goto    Transmit_Delay1

Transmit_No_Delay
        bcf     PORTC, LED_GREEN    ; Turn RED LED ON
        bsf     PORTC, LED_RED
        movlw   LED_ON              ; set the ON flag
        movwf   LED_ON_FLAG
        clrf    LED_CNT_L

        bsf     PORTA, RS485_EN     ; Enable the RS-485
        movfw   SRC_ADR             ; Packet Source address to return
        andlw   0xF0                ; Do high order nibble
        movwf   HEX_TEMP
        swapf   HEX_TEMP, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   SRC_ADR
        andlw   0x0F                ; Do low order nibble
        call    Nibble_To_Hex
        call    Transmit_Char

        movfw   MODULE_ADDRESS      ; Module address
        andlw   0xF0                ; Do high order nibble
        movwf   HEX_TEMP
        swapf   HEX_TEMP, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   MODULE_ADDRESS
        andlw   0x0F                ; Do low order nibble
        call    Nibble_To_Hex
        call    Transmit_Char
        return


;----------------------------------------------------------------------------
;
; Reads current configuration from EEPROM
;
Read_Config
        movlw   EE_MOD_ADDR     ; Get Module address
        call    Read_EEPROM
        movwf   MODULE_ADDRESS

        movlw   EE_DEST_ADDR    ; Get ASYNC Destination address
        call    Read_EEPROM
        movwf   ASYNC_DEST
        return

;----------------------------------------------------------------------------
;
; Read DIP switch settings and update BAUD rate
;
Read_DIP
        movfw   PORTA           ; Read DIP switch PORT A
        andlw   0x20            ; mask off everything but bit 5
        movwf   DIP_SWITCH      ; DIP position 1 = bit 5
        movfw   PORTE           ; Read DIP switch PORT E
        andlw   0x07            ; Mask everything but bits 0..2
        iorwf   DIP_SWITCH, F

        btfss   DIP_SWITCH, 5   ; DIP Position 1 = MASTER_START
        goto    DIP_Off
        movlw   TRUE
        movwf   MASTER_START
        goto    DIP0

DIP_Off
        clrf    MASTER_START

DIP0
        movfw   DIP_SWITCH      ; Get the BAUD RATE bits
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x00            ; Val=0, Pos3=OFF Pos4= OFF, Baud=9600
        btfss   STATUS, Z
        goto    DIP1
        movlw   SPBRG_9600      ; 9600 BAUD
        movwf   OUR_SPBRG
        goto    DIP4

DIP1    movfw   DIP_SWITCH
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x02            ; Val=2, Pos3=ON Pos4=OFF, Baud=19200
        btfss   STATUS, Z
        goto    DIP2
        movlw   SPBRG_19200     ; 19200 BAUD
        movwf   OUR_SPBRG
        goto    DIP4

DIP2    movfw   DIP_SWITCH
        andlw   0x06            ; SW Pos 3_4
        xorlw   0x04            ; Val=4, Pos3=OFF Pos4=ON, Baud=57600
        btfss   STATUS, Z
        goto    DIP3
        movlw   SPBRG_57600     ; 57600 BAUD
        movwf   OUR_SPBRG
        goto    DIP4

DIP3                            ; Val=6, Pos3=ON Pos4=ON, BAUD=115200
        movlw   SPBRG_115200    ; 115200 BAUD
        movwf   OUR_SPBRG

DIP4
        ;
        ; BUGBUG: The only time we update is if the value has changed
        ;
        movfw   OUR_SPBRG       ; Load Baud Rate Generator
; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
        movwf   SPBRG           ; Load Baud Rate Generator
; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0         ; select BANK 0
        return

;----------------------------------------------------------------------------
; Transmits a single character in W
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
;                   1 millisecond timer ISR
;                   Bump the running time variables
;----------------------------------------------------------------------------
;
Timer1_Isr
        movlw   TMR1_RELOAD_H       ; Re-Load Timer1 counters
        movwf   TMR1H
        movlw   TMR1_RELOAD_L       ; 0xF8CC - 10 instructions
        movwf   TMR1L
        bcf     PIR1, TMR1IF        ; clear the Timer 1 Irq

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
; Timer 0 ISR - Counter Input is directed to Timer 0
;               ISR fires on roll-over (Capture Port)
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
        db      "Copyright 1997-2002 Integrity Instruments"

;
; End of Code
;
        end

