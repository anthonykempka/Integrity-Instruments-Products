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
; Integrity Instruments 232M200 Module Firmware
;
; Version:  3.2
;
; Note: Based on ADC-DIG v2.3 code (ala Tony, not Tim's)
;
; ------------
;
; Revision History
; -------------------------------------------------------
;   A. Kempka   Original    11/18/98    v2.0
;   A. Kempka   v2.3        11/1/99     Added delta Digital and Delta counter
;                                       to the Async changed update.
;   A. Kempka   v3.0        2/25/2002   Converted to 232M300 module
;   A. Kempka   v3.0        10/3/2002   Converted from 232M300 module source code
;   A. Kempka   RTM         12/16/2002
;   A. Kempka   v3.1        2/18/2003   Fixed problem where the stream flags for
;                                       Digital, Analog, and Counter were not read
;                                       during reboot
;   A. Kempka   v3.2        3/23/2003   Fixed PORT2 (PORTB) power up default and TRIS
;                                       data direction
;


;
;----------------------------------------------------------------------------
; Specify processor type and output format.
; BUGBUG: Use 16C63A in the final release
;
        list p=16c63A, f=inhx8m

;----------------------------------------------------------------------------
; Include the Processor specific defines.
;
#INCLUDE "P16C63A.INC"

;
; MPU configuration options
;
  __CONFIG   _CP_ALL & _WDT_ON & _HS_OSC & _PWRTE_ON & _BODEN_ON

;----------------------------------------------------------------------------
; General equates.
;----------------------------------------------------------------------------

VERSION_MAJOR       EQU     '3'
VERSION_MINOR       EQU     '2'

TRUE                EQU     1
FALSE               EQU     0

;----------------------------------------------------------------------------
; EEPROM addresses
;----------------------------------------------------------------------------
EE_MOD_ADDR         EQU     0x00    ; Module address
EE_DEST_ADDR        EQU     0x01    ; Async destination address
EE_PORT1_DIR        EQU     0x02    ; Port 1 data direction (PORTB)
EE_PORT2_DIR        EQU     0x03    ; Port 2 data direction (PORTD)
EE_ASYNC_CONFIG_MSB EQU     0x04    ; Async configuration MSB
EE_ASYNC_CONFIG_LSB EQU     0x05    ; Async configuration LSB
EE_PORT1_DEF        EQU     0x06    ; Port 1 default power up setting (PORTB)
EE_PORT2_DEF        EQU     0x07    ; Port 2 default power up setting (PORTD)
EE_EXPANDER         EQU     0x08    ; 0xFF if expander Opto-22 modules in use
EE_DAC0_MSB         EQU     0x09    ; DAC Channel 0 power up setting
EE_DAC0_LSB         EQU     0x0A    ; DAC Channel 0 power up setting
EE_DAC1_MSB         EQU     0x0B    ; DAC Channel 1 power up setting
EE_DAC1_LSB         EQU     0x0C    ; DAC Channel 1 power up setting
EE_SLOW_ADC         EQU     0x0D    ; 0xFF slows A/D clock for high impedance inputs
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
SPBRG_57600         EQU     0x03
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


;----------------------------------------------------------------------------
; Port bit defines
;----------------------------------------------------------------------------

; PORT A bits
EEPROM_CS       EQU     0x00
RTC_CS          EQU     0x01
ADC_CS          EQU     0x02
DAC_CS          EQU     0x03
LED_BIT         EQU     0x05


;--------------------------------------------------------------------------
; Variables
; Ram BANK 0
;--------------------------------------------------------------------------
RamBank0        EQU     0x20
W_TEMP          EQU     RamBank0+0x00
STATUS_TEMP     EQU     RamBank0+0x01
STRING_INDEX    EQU     RamBank0+0x02
RUNNING_CNT_H   EQU     RamBank0+0x03
RUNNING_CNT_L   EQU     RamBank0+0x04
LED_CNT_L       EQU     RamBank0+0x05
BI_UNI_POLAR    EQU     RamBank0+0x06
LED_ON_FLAG     EQU     RamBank0+0x07
EEPROM_ADDRESS  EQU     RamBank0+0x08
DIP_SWITCH      EQU     RamBank0+0x09
OUR_SPBRG       EQU     RamBank0+0x0A
ADC_CLOCK       EQU     RamBank0+0x0B
HEX_TEMP        EQU     RamBank0+0x0C

INPUT_REGISTER  EQU     RamBank0+0x10
SCRATCH         EQU     RamBank0+0x11
CONTROL_ANALOG  EQU     RamBank0+0x12
RCV_ERROR_CNT   EQU     RamBank0+0x13

OUR_TRISB       EQU     RamBank0+0x16
OUR_TRISD       EQU     RamBank0+0x17
OUR_PORTB       EQU     RamBank0+0x18
OUR_PORTD       EQU     RamBank0+0x19

ADC_NIBBLE1     EQU    RamBank0+0x1A
ADC_NIBBLE2     EQU    RamBank0+0x1B
ADC_NIBBLE3     EQU    RamBank0+0x1C

RCV_BUFFER_PTR  EQU     RamBank0+0x1F
RCV_BUFFER      EQU     RamBank0+0x20
RCV_BUFFER_ST   EQU     RamBank0+0x30
RCV_LAST_CHAR   EQU     RamBank0+0x31

STREAM          EQU     RamBank0+0x32
ASYNC_CONFIG    EQU     RamBank0+0x33
ASYNC_TIME_H    EQU     RamBank0+0x34
ASYNC_TIME_L    EQU     RamBank0+0x35

STREAM_ANALOG_CNT EQU     RamBank0+0x39
STREAM_CH       EQU     RamBank0+0x3A

STREAM_ANALOG   EQU     RamBank0+0x3B
STREAM_DIG      EQU     RamBank0+0x3C
STREAM_COUNT    EQU     RamBank0+0x3D
EXPANDER        EQU     RamBank0+0x3E

PREV_PORTB      EQU     RamBank0+0x40
PREV_PORTD      EQU     RamBank0+0x41
PREV_TMR0       EQU     RamBank0+0x42
PREV_TMR0H      EQU     RamBank0+0x43

CURRENT_PTR     EQU     RamBank0+0x44

PWM_LSB         EQU     RamBank0+0x45
PWM_MSB         EQU     RamBank0+0x46
PWM_DIVISOR     EQU     RamBank0+0x47

DAC0_MSB        EQU     RamBank0+0x50
DAC0_LSB        EQU     RamBank0+0x51
DAC1_MSB        EQU     RamBank0+0x52
DAC1_LSB        EQU     RamBank0+0x53

OUR_TMR0H_MSB   EQU     RamBank0+0x54
OUR_TMR0L_MSB   EQU     RamBank0+0x55
OUR_TMR0H       EQU     RamBank0+0x56
OUR_TMR0L       EQU     RamBank0+0x57
TMR0H_MSB       EQU     RamBank0+0x58
TMR0L_MSB       EQU     RamBank0+0x59
TMR0H           EQU     RamBank0+0x5A

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
        org     0x20

;----------------------------------------------------------------------------
; Initialization and Start-Up code.
;----------------------------------------------------------------------------
Start
; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0     ; select BANK 0
        clrf    PORTA           ; clear PORT A
        clrf    PORTB           ; clear PORT B
        clrf    PORTC           ; clear PORT C

; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
        movlw   0x10
        movwf   TRISA           ; set direction PORT A (0,1,2,3,5 OUT: 4 IN)
        movlw   0xFF
        movwf   TRISB           ; set direction PORT B (0..7 IN)
        movlw   0xD3
        movwf   TRISC           ; set direction PORT C (0,1 IN: 2,3,5 OUT: 4,6,7 IN)
        clrf    PR2             ; PWM period = 0

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
        clrf    ASYNC_TIME_L
        clrf    ASYNC_TIME_H
        clrf    RCV_ERROR_CNT
        clrf    RCV_LAST_CHAR
        clrf    STREAM
        clrf    ASYNC_CONFIG
        clrf    TMR0H
        clrf    PWM_LSB
        clrf    PWM_MSB
        clrf    PWM_DIVISOR
        clrf    DIP_SWITCH
        clrf    ADC_CLOCK

        clrf    DAC0_MSB
        clrf    DAC0_LSB
        clrf    DAC1_MSB
        clrf    DAC1_LSB

        clrf    STREAM_DIG
        clrf    STREAM_COUNT
        clrf    STREAM_ANALOG

        clrf    OUR_TMR0H_MSB
        clrf    OUR_TMR0L_MSB
        clrf    OUR_TMR0H
        clrf    OUR_TMR0L
        clrf    TMR0H_MSB
        clrf    TMR0L_MSB
        clrf    TMR0H

        bsf     PORTA, ADC_CS       ; ADC not selected
        bsf     PORTA, EEPROM_CS    ; EEPROM not selected
        bsf     PORTA, DAC_CS       ; DAC not selected
        bsf     PORTA, RTC_CS       ; RTC not selected

Start4
        movlw   0x31            ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        movlw   0xF8            ; Load Timer1 counters
        movwf   TMR1H
        movlw   0xCC            ; 0xF8CC == 1 ms. interrupt
        movwf   TMR1L
        movlw   0x01            ; Timer1 prescale 1:1, enable Timer1
        movwf   T1CON

        clrf    CCPR1L          ; PWM LSB = 0
        movlw   0x0C            ; PWM mode, PWM MSB = 0
        movwf   CCP1CON         ; Initially PWM = 0%
        clrf    TMR2
        movlw   0x04            ; Timer 2 prescale 1:1, enable Timer 2
        movwf   T2CON

        movlw   SPBRG_9600      ; 9600 BAUD default
        movwf   OUR_SPBRG

        call    Check_DIP_Switch

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

Start_LED_On
        clrwdt                      ; Kick the dog
        movlw   TRUE                ; Set LED on flag
        movwf   LED_ON_FLAG
        bsf     PORTA, LED_BIT      ; LED ON

Start_RS232
        movlw   RamBank0+0X24   ; Set the RCV buffer pointer for RS-232
        movwf   RCV_BUFFER_ST
        movwf   RCV_BUFFER_PTR

        ; On any INITIALIZATION error, goto Start_LED_On

Start_Read_Eeprom
        movlw   EE_PORT2_DIR    ; Direction PORT2 (PORTB)
        call    Read_EEPROM
        movwf   OUR_TRISB
        bsf     STATUS, RP0     ; select BANK 1
        movwf   TRISB           ; set direction PORT B
        bcf     STATUS, RP0     ; select BANK 0

        ;
        ; Read the EEPROM configuration registers
        ;
        call    Read_EEPROM_Config

        movlw   EE_STREAM_DIG       ; Get stream DIGITAL EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_DIG
        movlw   EE_STREAM_COUNT     ; Get stream COUNT EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_COUNT
        movlw   EE_STREAM_ANALOG    ; Get stream ANALOG EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_ANALOG

        movlw   EE_PORT2_DEF    ; Default power up output values for PORT2 (PORTB)
        call    Read_EEPROM
        xorwf   EXPANDER, W     ; EXPANDER Opto-22 inversion
        movwf   PORTB

        movlw   EE_DAC0_LSB     ; DAC Channel 0 power up default settings
        call    Read_EEPROM
        movwf   DAC0_LSB
        movlw   EE_DAC0_MSB
        call    Read_EEPROM
        movwf   DAC0_MSB
        movlw   EE_DAC1_LSB     ; DAC Channel 1 power up default settings
        call    Read_EEPROM
        movwf   DAC1_LSB
        movlw   EE_DAC1_MSB
        call    Read_EEPROM
        movwf   DAC1_MSB
        call    Update_DAC      ; Update the DAC with the new values

        movfw   PORTB           ; Establish the Previous values for ASYNC updates
        movwf   PREV_PORTB
        movfw   TMR0
        movwf   PREV_TMR0
        movfw   TMR0H
        movwf   PREV_TMR0H

        movfw   RCREG           ; Clear interrupts before enabling
        bcf     PIR1, TMR1IF    ; clear the Timer 1 Irq
        bcf     PIR1, TMR2IF
        movlw   0xe0            ; Enable interrupts, enable TMR0 irq
        movwf   INTCON

        call    Update_DAC      ; Update the default Power On values

        clrf    STRING_INDEX
Printf_Startup                      ; Display the startup string
        movfw   STRING_INDEX
        call    Startup_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Startup_end
        call    Transmit_Char
        incf    STRING_INDEX, F
        goto    Printf_Startup
Printf_Startup_end


;----------------------------------------------------------------------------
; Main processing loop.
;
Main_Loop
        clrwdt                      ; Kick the dog
        call    Check_DIP_Switch    ; Check the DIP switch in case some smart ass changed it
        movfw   RCV_LAST_CHAR       ; Check the last character RCVD
        xorlw   0x0D                ; Is this a CR?
        btfsc   STATUS, Z
        goto    Main_Loop_Parse

Main_Loop_Check_Async
        movf    ASYNC_CONFIG, F     ; Are we in ASYNC mode?
        btfsc   STATUS, Z
        goto    Main_Loop_Check_Stream

        movfw   ASYNC_CONFIG        ; Is this Mode 1?
        xorlw   0x01                ; ASYNC UPDATE ON CHANGE (DELTA)
        btfss   STATUS, Z
        goto    Main_Loop_Async_Timed

Main_Loop_Async_Delta
        movfw   PORTB               ; Check if PORTB has changed
        xorwf   PREV_PORTB, W
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

        goto    Main_Loop_Check_Stream

Main_Loop_Async_I
        movfw   PORTB           ; Establish the Previous values for ASYNC updates
        movwf   PREV_PORTB
        call    Process_I
        goto    Main_Loop_Check_Stream

Main_Loop_Async_N
        movfw   TMR0
        movwf   PREV_TMR0
        movfw   TMR0H
        movwf   PREV_TMR0H
        call    Process_N
        goto    Main_Loop_Check_Stream

Main_Loop_Async_Timed

        movfw   ASYNC_TIME_H
        subwf   RUNNING_CNT_H, W
        btfss   STATUS, C
        goto    Main_Loop_Check_Stream
        movfw   ASYNC_TIME_L
        subwf   RUNNING_CNT_L, W
        btfsS   STATUS, C
        goto    Main_Loop_Check_Stream

        bcf     INTCON, GIE         ; Disable interrupts so these clears work
        clrf    RUNNING_CNT_H
        clrf    RUNNING_CNT_L
        bsf     INTCON, GIE
        goto    Main_Loop_Stream    ; ASYNC Update with the Stream configuration


Main_Loop_Check_Stream
        movf    STREAM, F           ; Are we in STREAM Mode?
        btfsc   STATUS, Z
        goto    Main_Loop

Main_Loop_Stream
        movf    STREAM_DIG, F       ; Check stream DIGITAL
        btfsc   STATUS, Z
        goto    Main_Check_Cont1
        call    Process_I           ; Process DIGITAL

Main_Check_Cont1
        movf    STREAM_COUNT, F     ; Check stream COUNT
        btfsc   STATUS, Z
        goto    Main_Check_Cont2
        call    Process_N           ; Process CAPTURE

Main_Check_Cont2
        movf    STREAM_ANALOG, F    ; Check stream ANALOG
        btfsc   STATUS, Z
        goto    Main_Loop
        movlw   EE_STREAM_ANALOG    ; Get stream ANALOG EEPROM setting
        call    Read_EEPROM
        andlw   0x03                ; Limit count to 3
        movwf   STREAM_ANALOG_CNT
        clrf    STREAM_CH           ; Clear the current stream channel
Main_Check_Cont3
        movfw   STREAM_ANALOG_CNT   ; Are we done with ANALOG?
        btfsc   STATUS, Z
        goto    Main_Loop
        movfw   STREAM_CH           ; Compute the current channel
        addlw   EE_ANALOG_CFG_BASE
        call    Read_EEPROM         ; Read the ANALOG control from EEPROM
        movwf   CONTROL_ANALOG
        movlw   0x0F                ; Mask OFF unipolar/bipolar bit
        andwf   CONTROL_ANALOG, F
        call    Process_Analog
        decf    STREAM_ANALOG_CNT, F    ; Decr ANALOG query count
        incf    STREAM_CH, F            ; Next channel
        goto    Main_Check_Cont3


Main_Loop_Parse
;
; Parse incoming message
;
        clrf    RCV_LAST_CHAR       ; Clear last char rcvd.

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
        movlw   'I'
        call    Transmit_Char
        movlw   '0'                 ; PORT 1 = UNUSED send 00's
        call    Transmit_Char
        movlw   '0'                 ; PORT 1 = UNUSED send 00's
        call    Transmit_Char
        movfw   PORTB               ; PORT 2 = PORT B - MSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   PORTB               ; PORT 2 = PORT B - LSB
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
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
        goto    Main_Loop_Check_U
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        ;
        ; Ignore PORT 1 data characters 5 and 6
        ;

        movfw   RCV_BUFFER+7        ; Get the PORT 2 (PORT B) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+8        ; Get the PORT 2 (PORT B) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        xorwf   EXPANDER, W         ; EXPANDER Opto-22 inversion
        movwf   PORTB

        movlw   'O'
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_U
        movlw   'U'                 ; UNIPOLAR SAMPLE ANALOG command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_L
        movlw   RCV_BUFFER+0x07     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the ANALOG CONTROL nibble
        call    Hex_To_Nibble
        movwf   CONTROL_ANALOG
        call    Process_Analog
        goto    Main_Loop_Rcv_End

Process_Analog
        call    Acquire_Sample_1861 ; Acquire the sample from ADC
        movlw   'U'                 ; UNIPOLAR
        call    Transmit_Char
        movfw   CONTROL_ANALOG      ; Return the Analog Control nibble
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   ADC_NIBBLE1
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

Main_Loop_Check_L
        movlw   'L'                 ; ANALOG OUTPUT command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_N
        movlw   RCV_BUFFER+0x0A     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet length OK

        movlw   '0'                 ; Check if CH0
        xorwf   RCV_BUFFER+5, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_L1
                                    ; We're working on Channel 0
        movfw   RCV_BUFFER+6        ; Get 1st nibble of 12 bit DAC value
        call    Hex_To_Nibble
        movwf   DAC0_MSB
        movfw   RCV_BUFFER+7        ; Get 2nd nibble of 12 bit DAC value
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap nibble in MSB location
        movfw   RCV_BUFFER+8        ; Get 3rd nibble of 12 bit DAC value
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   DAC0_LSB
        goto    Main_Loop_Check_Lx

Main_Loop_Check_L1
        movlw   '1'                 ; Check if CH1
        xorwf   RCV_BUFFER+5, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; We're working on Channel 1
        movfw   RCV_BUFFER+6        ; Get 1st nibble of 12 bit DAC value
        call    Hex_To_Nibble
        movwf   DAC1_MSB
        movfw   RCV_BUFFER+7        ; Get 2nd nibble of 12 bit DAC value
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap nibble in MSB location
        movfw   RCV_BUFFER+8        ; Get 3rd nibble of 12 bit DAC value
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   DAC1_LSB

Main_Loop_Check_Lx
        call    Update_DAC          ; Update the DAC with the new value
        movlw   'L'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End


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
        movlw   'N'
        call    Transmit_Char
        bcf     INTCON, GIE         ; Disable interupts
        movfw   TMR0                ; Get TMR0 count
        movwf   OUR_TMR0L           ; save local copy so count does during output
        movfw   TMR0H
        movwf   OUR_TMR0H
        movfw   TMR0H_MSB
        movwf   OUR_TMR0H_MSB
        movfw   TMR0L_MSB
        movwf   OUR_TMR0L_MSB
        bsf     INTCON, GIE         ; Enable interrupts

        movfw   OUR_TMR0H_MSB       ; BYTE 4
        andlw   0xF0                ; Most significant nibble
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0H_MSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L_MSB       ; BYTE 3
        andlw   0xF0                ; Most significant nibble
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L_MSB
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0H           ; BYTE 2
        andlw   0xF0                ; Most significant nibble
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0H
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L           ; BYTE 1
        andlw   0xF0                ; Most significant nibble
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TMR0L
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

        bcf     INTCON, GIE         ; Disable interupts
        clrf    TMR0                ; Clear the Timer0 counts
        clrf    TMR0H
        clrf    TMR0H_MSB
        clrf    TMR0L_MSB
        bcf     INTCON, T0IF        ; Clear TMR0 interrupt flag
        bsf     INTCON, GIE         ; Enable interrupts
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
        ;
        ; Ignore PORT 1 characters 5 and 6
        ;

        movfw   RCV_BUFFER+7        ; Get the PORT 2 (PORT B) - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+8        ; Get the PORT 2 (PORT B) - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   OUR_TRISB
        bsf     STATUS, RP0         ; select BANK 1
        movwf   TRISB               ; set direction PORT B
        bcf     STATUS, RP0         ; select BANK 0

        movlw   EE_PORT2_DIR        ; save PORT2 TRIS registers in EEPROM
        movwf   EEPROM_ADDRESS
        movfw   OUR_TRISB
        call    Write_EEPROM

        movlw   'T'                 ; EEPROM Write delayed enough already
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_G
        movlw   'G'                 ; GET DATA DIRECTION command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_P
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movlw   'G'
        call    Transmit_Char
        movlw   '0'                 ; PORT 1 - UNUSED send 00's
        call    Transmit_Char
        movlw   '0'                 ; PORT 1 - UNUSED send 00's
        call    Transmit_Char
        movfw   OUR_TRISB           ; PORT 2 = PORT B data direction
        andlw   0xF0                ; TMR0H - MSB
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   OUR_TRISB           ; PORT 2 = PORTB data direction
        andlw   0x0F
        call    Nibble_To_Hex
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Main_Loop_Rcv_End

Main_Loop_Check_P
        movlw   'P'                 ; PWM command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_W
        movlw   RCV_BUFFER+0x0B     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+5        ; Get the PWM_DIVISOR
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+6        ; Get the PWM_DIVISOR
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   PWM_DIVISOR
        bsf     STATUS, RP0         ; select BANK 1
        movwf   PR2                 ; set PWM_DIVISOR PR2 value
        bcf     STATUS, RP0         ; select BANK 0

        movfw   RCV_BUFFER+7        ; Get the PWM_DUTY (MSB)
        call    Hex_To_Nibble
        andlw   0x03                ; Mask off everything but bits 0-1
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+8        ; Get the PWM_DUTY (MSB)
        call    Hex_To_Nibble
        iorwf   SCRATCH, F
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F
        rlf     SCRATCH, W          ; last rotate keep in W
        movwf   PWM_MSB             ; save
        movfw   RCV_BUFFER+9
        call    Hex_To_Nibble
        andlw   0x0C
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     SCRATCH, F
        rrf     SCRATCH, W          ; last rotate keep in W
        iorwf   PWM_MSB, W
        movwf   CCPR1L              ; Make it so
        movwf   PWM_MSB             ; save off

        movfw   RCV_BUFFER+9        ; Get the PWM_DUTY (LSB)
        call    Hex_To_Nibble
        andlw   0x03                ; Mask off everything but bits 0-1
        movwf   PWM_LSB
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap the nibble to low order
        iorlw   0x0C                ; OR in the PWM mode code
        movwf   CCP1CON             ; make it so

        movlw   'P'                 ; EEPROM Write delayed enough already
        call    Transmit_Char
        movlw   0x0D                ; CR
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
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+6        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   EEPROM_ADDRESS

        movfw   RCV_BUFFER+7        ; Get the EEPROM VALUE - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+8        ; Get the EEPROM VALUE - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Write_EEPROM        ; Write value to EEPROM

        movlw   'W'                 ; EEPROM Write delayed enough already
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char

;        call    Read_EEPROM_Config  ; Read new EEPROM configuration values
; Don't read EEPROM Config every time since the Async Timed mode may start a ridiculous stream of data
;
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
        swapf   SCRATCH, F          ; swap the nibble to low order
        movfw   RCV_BUFFER+6        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Read_EEPROM
        movwf   SCRATCH             ; Save the data
        movwf   EEPROM_ADDRESS      ; Save off in EEPROM address also

        movlw   'R'
        call    Transmit_Char
        movfw   SCRATCH             ; Get the saved Data
        andlw   0xF0                ; Send out the MSB
        movwf   SCRATCH
        swapf   SCRATCH, W          ; swap the nibble to low order
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   EEPROM_ADDRESS      ; Get the saved Data
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
        movlw   0x01                ; Set STREAM FLAG
        movwf   STREAM
        movlw   EE_STREAM_DIG       ; Get stream DIGITAL EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_DIG
        movlw   EE_STREAM_COUNT     ; Get stream COUNT EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_COUNT
        movlw   EE_STREAM_ANALOG    ; Get stream ANALOG EEPROM setting
        call    Read_EEPROM
        movwf   STREAM_ANALOG

        movlw   'S'                 ; send response
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
        clrf    STREAM              ; Clear Stream Flag
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
        goto    Main_Loop_Check_Question
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        movlw   'Z'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
Main_Loop_Reset_Cpu                 ; Force a CPU reset by not kicking the DOG
        goto    Main_Loop_Reset_Cpu

Main_Loop_Check_Question
        movlw   '?'                 ; ? command
        xorwf   RCV_BUFFER+4, W
        btfss   STATUS, Z
        goto    Main_Loop_Check_K
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Main_Loop_Error_Response
                                    ; Command/Packet OK
        clrf    STRING_INDEX
Printf_Startup_Q                    ; Display the startup string
        movfw   STRING_INDEX
        call    Startup_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Startup_end_Q
        call    Transmit_Char
        incf    STRING_INDEX, F
        goto    Printf_Startup_Q
Printf_Startup_end_Q
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
        movlw   'K'
        call    Transmit_Char
        movfw   RCV_ERROR_CNT       ; ERROR count - MSB
        andlw   0xF0
        movwf   SCRATCH             ; Rotate to LSB
        swapf   SCRATCH, W          ; swap the nibble to low order
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
        movlw   'X'                 ; ERROR Response
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char

Main_Loop_Rcv_End
        movfw   RCV_BUFFER_ST       ; Reset the RCV buffer pointer
        movwf   RCV_BUFFER_PTR
        goto    Main_Loop           ; Ready for new message


;----------------------------------------------------------------------------
; Reads EEPROM configuration values and updates RAM variables
;
Read_EEPROM_Config
        movlw   EE_ASYNC_CONFIG_LSB ; ASYNC update configuration
        call    Read_EEPROM
        movwf   ASYNC_TIME_L        ; Save off 16 bit value
        movlw   EE_ASYNC_CONFIG_MSB
        call    Read_EEPROM
        movwf   ASYNC_TIME_H

        xorlw   0x00                ; See if High order byte == 0
        btfsc   STATUS, Z
        goto    Read_EEPROM_Config_A
        movlw   0x02                ; ASYNC config mode 2 == TIMED UPDATE
        movwf   ASYNC_CONFIG
        goto    Read_EEPROM_Config_E

Read_EEPROM_Config_A
        movfw   ASYNC_TIME_L
        xorlw   0x00                ; See if Low order byte == 0
        btfss   STATUS, Z
        goto    Read_EEPROM_Config_B
        movlw   0x00                ; ASYNC config mode 0 == NONE
        movwf   ASYNC_CONFIG
        goto    Read_EEPROM_Config_E

Read_EEPROM_Config_B
        movfw   ASYNC_TIME_L
        xorlw   0x01                ; See if Low order byte == 1
        btfss   STATUS, Z
        goto    Read_EEPROM_Config_C
        movlw   0x01                ; ASYNC config mode 1 == UPDATE on CHANGE (DELTA)
        movwf   ASYNC_CONFIG
        goto    Read_EEPROM_Config_E

Read_EEPROM_Config_C
        movlw   0x02                ; ASYNC config mode 2 == TIMED UPDATE
        movwf   ASYNC_CONFIG

Read_EEPROM_Config_E
        movlw   EE_EXPANDER         ; EXPANDER flag
        call    Read_EEPROM
        movwf   EXPANDER

        movlw   EE_SLOW_ADC         ; Load the Slow ADC clock setting
        call    Read_EEPROM
        andlw   0xFF                ; Check for non-zero number
        btfss   STATUS, Z
        goto    Read_EEPROM_Config_F
        movlw   0x30                    ; SPI Master mode, CKP=1, Fosc/4 = 3,686,400 Hz
        goto    Read_EEPROM_Config_G
Read_EEPROM_Config_F
        movlw   0x32                    ; SPI Master mode, CKP=1, Fosc/64 = 230,400 Hz
Read_EEPROM_Config_G
        movwf   ADC_CLOCK

        return

;----------------------------------------------------------------------------
; Checks the DIP switch and changes BAUD RATE
;
Check_DIP_Switch
        movfw   PORTC           ; Read DIP switch PORT C
        andlw   0x03            ; Mask everything but bits 0,1
        xorwf   DIP_SWITCH, W
        btfsc   STATUS, Z
        goto    Check_DIP_SW_NC

        movfw   PORTC           ; Read DIP switch PORT C
        andlw   0x03            ; Mask everything but bits 1,2
        movwf   DIP_SWITCH      ; save off for compare operations

        xorlw   0x00            ; Val=0, Pos1=OFF Pos2= OFF, Baud=9600
        btfss   STATUS, Z
        goto    Check_DIP_SW1
        movlw   SPBRG_9600      ; 9600 BAUD
        movwf   OUR_SPBRG
        goto    Check_DIP_SW_End

Check_DIP_SW1
        movfw   DIP_SWITCH
        xorlw   0x01            ; Val=1, Pos1=ON Pos2=OFF, Baud=19200
        btfss   STATUS, Z
        goto    Check_DIP_SW2
        movlw   SPBRG_19200     ; 19200 BAUD
        movwf   OUR_SPBRG
        goto    Check_DIP_SW_End

Check_DIP_SW2
        movfw   DIP_SWITCH
        xorlw   0x02            ; Val=2, Pos1=OFF Pos2=ON, Baud=57600
        btfss   STATUS, Z
        goto    Check_DIP_SW3
        movlw   SPBRG_57600     ; 57600 BAUD
        movwf   OUR_SPBRG
        goto    Check_DIP_SW_End

Check_DIP_SW3                   ; Val=3, Pos1=ON Pos2=ON, BAUD=115200
        movlw   SPBRG_115200    ; 115200 BAUD
        movwf   OUR_SPBRG

Check_DIP_SW_End
        movfw   OUR_SPBRG       ; Load Baud Rate Generator
; ------------- BANK 1 ------------------------------------------------------
        bsf     STATUS, RP0     ; select BANK 1
        movwf   SPBRG           ; Load Baud Rate Generator
; ------------- BANK 0 ------------------------------------------------------
        bcf     STATUS, RP0         ; select BANK 0

Check_DIP_SW_NC
        return


;----------------------------------------------------------------------------
; Transmits a single character
;
Transmit_Char
        movwf   TXREG
        bsf     PORTA, LED_BIT  ; LED ON
        bsf     STATUS, RP0     ; select BANK 1
Transmit_Pending
        clrwdt                  ; Kick the dog
        btfss   TXSTA, TRMT
        goto    Transmit_Pending
        bcf     STATUS, RP0     ; select BANK 0
        return



;----------------------------------------------------------------------------
; Acquires an ADC sample from the Linear Tech LTC1861
;
; ADC config in CONTROL_ANALOG
;
; Data return in ADC_NIBBLE1/2/3
;
Acquire_Sample_1861
        movfw   ADC_CLOCK
        movwf   SSPCON
        bcf     PORTA, ADC_CS           ; Select the ADC

        movfw   CONTROL_ANALOG          ; Only bits 0,1 are used to control the LTC1861
        movwf   SCRATCH
        swapf   SCRATCH, F              ; Move into high order nibble location
        bcf     STATUS, C
        rlf     SCRATCH, F
        rlf     SCRATCH, W
        call    Spi_Send                ; Send out control bits for next conversion - ignore returned data
        call    Spi_Send
        call    Spi_Send

        bsf     PORTA, ADC_CS           ; De-select the ADC and start conversion - 3 us typical
        nop                             ; 11 NOP's at 14.7456 Mhz
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop

        movfw   CONTROL_ANALOG          ; Only bits 0,1 are used to control the LTC1861
        movwf   SCRATCH
        swapf   SCRATCH, F              ; Move into high order nibble location
        bcf     STATUS, C
        rlf     SCRATCH, F
        rlf     SCRATCH, W

        bcf     PORTA, ADC_CS           ; Select the ADC
        call    Spi_Send                ; Send crap - get first 2 nibbles back ftom LTC1861
        movwf   SCRATCH
        andlw   0x0F
        movwf   ADC_NIBBLE2
        movlw   0xF0
        andwf   SCRATCH, F
        swapf   SCRATCH, W
        movwf   ADC_NIBBLE1

        call    Spi_Send                ; Send crap = get last nibble back from LTC1861
        andlw   0xF0
        movwf   SCRATCH
        swapf   SCRATCH, W
        movwf   ADC_NIBBLE3
        bsf     PORTA, ADC_CS           ; De-select ADC
        return

;----------------------------------------------------------------------------
; Output and updated DAC value in LTC1448
;
; DAC values in DAC0_MSB, DAC0_LSB, DAC1_MSB and DAC1_LSB
; These need to be put together into 3 bytes (24 bits) for the DAC
;
Update_DAC
        movlw   0x31                    ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        bcf     PORTA, DAC_CS           ; Select the DAC

        movfw   DAC0_LSB
        andlw   0xF0                    ; mask LSB nibble (2)
        iorwf   DAC0_MSB, W             ; OR in the MSB nibble (1)
        movwf   SCRATCH
        swapf   SCRATCH, W
        call    Spi_Send                ; Send byte 1 of 3

        movfw   DAC0_LSB
        andlw   0x0F                    ; mask LSB nibble (3)
        movwf   SCRATCH
        swapf   SCRATCH, W
        iorwf   DAC1_MSB, W
        call    Spi_Send                ; Send byte 2 of 3

        movfw   DAC1_LSB
        call    Spi_Send                ; Send byte 3 of 3

        bsf     PORTA, DAC_CS           ; De-Select the DAC
        return

;----------------------------------------------------------------------------
; Reads the EEPROM
;
; Address to read in W
; Value returned in W
;
Read_EEPROM
        movwf   SCRATCH                 ; Save off address
        movlw   0x31                    ; SPI Master mode, CKP=1, Fosc/16
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
        movlw   0x31                    ; SPI Master mode, CKP=1, Fosc/16
        movwf   SSPCON
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x06                    ; WRITE ENABLE command
        call    Spi_Send
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM to set WRITE ENABLE
        nop
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
        nop
Write_EEPROM_Done
        bcf     PORTA, EEPROM_CS        ; Select the EEPROM
        movlw   0x05                    ; READ STATUS REGISTER
        call    Spi_Send
        movlw   0xFF                    ; Send DUMMY just to receive
        call    Spi_Send
        bsf     PORTA, EEPROM_CS        ; De-Select the EEPROM
        nop
        nop
        movwf   SCRATCH
        btfsc   SCRATCH, 0
        goto    Write_EEPROM_Done
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
        movlw   TMR1_RELOAD_L
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
        bcf     PORTA, LED_BIT          ; Turn LED OFF
        clrf    LED_ON_FLAG             ; clear the ON flag
        goto    Timer1_Isr_End

Timer1_Led_Off
        incf    LED_CNT_L, F
        movfw   LED_CNT_L
        xorlw   0x07
        btfss   STATUS,Z
        goto    Timer1_Isr_End
        bsf     PORTA, LED_BIT          ; Turn LED ON
        movlw   TRUE                    ; set the ON flag
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
        incf    RCV_ERROR_CNT, F
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

        bsf     PORTA, LED_BIT      ; LED ON
        clrf    LED_CNT_L
        movlw   TRUE                ; Set LED on flag
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
        incfsz  TMR0H, F            ; Bump the High order byte count (LSB)
        goto    Tmr0_Isr_End
        incfsz  TMR0L_MSB, F        ; Bump the Low order byte count (MSB)
        goto    Tmr0_Isr_End
        incfsz  TMR0H_MSB, F        ; Bump the High order byte count (MSB)

Tmr0_Isr_End
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
        movfw   STATUS          ; save STATUS into W
        bcf     STATUS, RP0     ; select BANK 0
        movwf   STATUS_TEMP     ; save STATUS in temp

        btfsc   PIR1, TMR1IF    ; Is TMR1IF Set?
        call    Timer1_Isr

        btfsc   PIR1, RCIF      ; is bit 5 (RCIF) Set?
        call    Rcv_Isr

        btfsc   INTCON, T0IF    ; Is Timer0 Set?
        call    Tmr0_Isr

        movfw   STATUS_TEMP     ; move old STATUS into W
        movwf   STATUS          ; restore status
        swapf   W_TEMP, F       ; swap it so it will be right in the next instruction.
        swapf   W_TEMP, W       ; move old W into W
        retfie
;
;----------------------------------------------------------------------------
;

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;
; Strings to print follow
; NOTE!!!!!
; The PCLATH bits must be set for the correct page location of the code.
;
        org     0x500
; Index is assumed to be in W
Startup_String
        bsf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        addwf   PCL, F
        RETLW   0D
        RETLW   'I'
        RETLW   'n'
        RETLW   't'
        RETLW   'e'
        RETLW   'g'
        RETLW   'r'
        RETLW   'i'
        RETLW   't'
        RETLW   'y'
        RETLW   ' '
        RETLW   'I'
        RETLW   'n'
        RETLW   's'
        RETLW   't'
        RETLW   'r'
        RETLW   'u'
        RETLW   'm'
        RETLW   'e'
        RETLW   'n'
        RETLW   't'
        RETLW   's'
        RETLW   ' '
        RETLW   '2'
        RETLW   '3'
        RETLW   '2'
        RETLW   'M'
        RETLW   '2'
        RETLW   '0'
        RETLW   '0'
        RETLW   ' '
        RETLW   'M'
        RETLW   'o'
        RETLW   'd'
        RETLW   'u'
        RETLW   'l'
        RETLW   'e'
        RETLW   ' '
        RETLW   'v'
        RETLW   VERSION_MAJOR
        RETLW   '.'
        RETLW   VERSION_MINOR
        RETLW   0D
        RETLW   00

;----------------------------------------------------------------------------
; Converts a binary nibble in W to an ASCII Hexadecimal value
; returned in W
;
Nibble_To_Hex
        bsf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
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

;
; End of Code
;
        end

