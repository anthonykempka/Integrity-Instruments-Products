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
; XP Converter Code
;
; Version: 1.01
; ---------------
;
; Revision History
; -------------------------------------------------------
; A. Kempka     Original        01/19/2002      v1.0
; A. Kempka                     12/16/2002      v1.01
;               Added "Run Mode" message when transitioning from Program Mode
;               to Run Mode when powering up Silent.
;   A. Kempka   RTM             12/16/2002
;
;


;
;----------------------------------------------------------------------------
; Specify processor type and output format.
;
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

VERSION_MAJOR       EQU     '1'
VERSION_MINOR1      EQU     '0'
VERSION_MINOR2      EQU     '1'

TRUE                EQU     0x01
FALSE               EQU     0x00

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


;
; Timer reload count values
; Note: Assumes a clock of 14.7456 Mhz, prescale 1:16
;
; TMR2 = 1 millisecond
;
; TMR2_PERIOD         EQU     0x73  7.3728 Mhz
TMR2_PERIOD         EQU     0xE6


;----------------------------------------------------------------------------
; Port defines
;----------------------------------------------------------------------------

; PORT A BITS
ENABLE_HOLD     EQU     0x00
EEPROM_CS       EQU     0x01
LED             EQU     0x04

; PORT B BITS
RS232_TXD       EQU     0x00
ADDR_TXD        EQU     0x01
ADDR_RXD        EQU     0x02

; PORTA (OUT: 0-7)
PORTA_DIR       EQU     0x00

; PORTB (IN: 0,2,4 | OUT: 1, 3-7)
PORTB_DIR       EQU     0x15

; PORTC (IN: 4,7,6 | OUT: 0-3, 5)
PORTC_DIR       EQU     0xD0


;--------------------------------------------------------------------------
; Variables
; Set Ram Bank 0 and 1 (Variable memory definitions)
;--------------------------------------------------------------------------
RamBank0        EQU     0x20
W_TEMP          EQU     RamBank0+0x00
STATUS_TEMP     EQU     RamBank0+0x01
MODULE_ADDRESS  EQU     RamBank0+0x02
TMR1H_SAVE      EQU     RamBank0+0x03
TMR1L_SAVE      EQU     RamBank0+0x04
TMR1H_MIN       EQU     RamBank0+0x05
TMR1L_MIN       EQU     RamBank0+0x06

ONE_MS_TICK      EQU    RamBank0+0x07
QUARTER_SEC_TICK EQU    RamBank0+0x08
STRING_INDEX     EQU    RamBank0+0x09

CUR_SPBRG       EQU     RamBank0+0x0A

HEX_TEMP        EQU     RamBank0+0x0C
CUR_MODE        EQU     RamBank0+0x0D
OUR_FLAGS       EQU     RamBank0+0x0E
CUR_BAUD        EQU     RamBank0+0x0F
NEW_BAUD        EQU     RamBank0+0x10

SCRATCH         EQU     RamBank0+0x11

TIMEOUTH        EQU     RamBank0+0x12
TIMEOUTL        EQU     RamBank0+0x13

EEPROM_ADDRESS  EQU     RamBank0+0x16

CURRENT_PTR     EQU     RamBank0+0x1E
RCV_BUFFER_PTR  EQU     RamBank0+0x1F
RCV_BUFFER      EQU     RamBank0+0x20
RCV_LAST_CHAR   EQU     RamBank0+0x30


;
; Bit defines for OUR_FLAGS
;
TMR1_ROLLOVER   EQU     0
INT_EVENT       EQU     1

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
        bcf     STATUS, RP0     ; **** select BANK 0
        clrf    PORTA           ; clear PORT A
        clrf    PORTB           ; clear PORT B
        clrf    PORTC           ; clear PORT C

        bsf     STATUS, RP0     ; **** select BANK 1
        movlw   PORTA_DIR
        movwf   TRISA           ; set direction PORT A
        movlw   PORTB_DIR
        movwf   TRISB           ; set direction PORT B
        movlw   PORTC_DIR
        movwf   TRISC           ; set direction PORT C
        movlw   TMR2_PERIOD     ; set TIMER2 period
        movwf   PR2

        bcf     STATUS, RP0     ; **** select BANK 0
        clrf    PORTA           ; clear PORT A
        clrf    PORTB           ; clear PORT B
        clrf    PORTC           ; clear PORT C

        clrf    CUR_SPBRG       ; clear registers
        clrf    RCV_LAST_CHAR
        clrf    TMR1_ROLLOVER
        clrf    ONE_MS_TICK
        clrf    QUARTER_SEC_TICK
        clrf    OUR_FLAGS
        clrf    CUR_BAUD
        clrf    NEW_BAUD

        bsf     PORTA, EEPROM_CS    ; Idle - not enabled
        bsf     PORTA, ENABLE_HOLD  ; Idle - ON
        bsf     PORTB, ADDR_TXD     ; Idle - ON

        movlw   SPBRG_9600      ; Starting baud rate - for programming mode
        movwf   CUR_SPBRG       ; Save SPBRG

        clrf    TMR1H           ; clear TMR1 counters
        clrf    TMR1L
        movlw   0x01            ; Timer1 prescale 1:1, enable Timer1
        movwf   T1CON

        movwf   TMR2            ; Load Timer2 counter
        movlw   0x06            ; Timer 2 prescale 1:16, enable Timer 2
        movwf   T2CON

        movfw   CUR_SPBRG       ; Load Baud Rate Generator
        bsf     STATUS, RP0     ; **** select BANK 1
        movwf   SPBRG           ; Load Baud Rate Generator
        bsf     PIE1, TMR1IE    ; Enable Timer1 Interrupt
        bsf     PIE1, TMR2IE    ; Enable Timer2 Interrupt
        bsf     PIE1, RCIE      ; Enable USART rcv Interrupt
        movlw   0x20            ; TXEN=1, SYNC=0, BRGH=0
        movwf   TXSTA
        movlw   0xBF            ; Interrupt - falling edge on INT pin
        movwf   OPTION_REG

        bcf     STATUS, RP0     ; **** select BANK 0
        movlw   0x90            ; UART: SPEN=1, CREN=1
        movwf   RCSTA           ; RS-485/232 setup

        movlw   0x30            ; SPI Master mode, CKP=1, Fosc/4
        movwf   SSPCON

        movlw   RCV_BUFFER      ; Set the RCV buffer pointer
        movwf   RCV_BUFFER_PTR

        movfw   RCREG           ; Clear UART interrupts before enabling
        bcf     PIR1, TMR1IF    ; clear Timer 1 Irq
        bcf     PIR1, TMR2IF    ; clear Timer 2 Irq
        ;movlw   0xD0            ; Enable interrupts
        movlw   0xC0
        movwf   INTCON

        movlw   0x04                ; Is this a silent power up?
        call    Read_EEPROM
        xorlw   0x00
        btfss   STATUS, Z
        goto    Wait_For_Key

        clrf    STRING_INDEX
Printf_Startup                      ; Display the startup string
        movfw   STRING_INDEX
        call    Startup_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Startup_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Startup
Printf_Startup_end

;
; Wait a few seconds for the user to hit a key and go into programming mode
;
Wait_For_Key
        clrwdt                  ; Kick the dog while waiting for a key
        movfw   RCV_LAST_CHAR
        xorlw   ' '             ; Look for the Space key
        btfss   STATUS, Z
        goto    Wait_For_Key1

        clrf    RCV_LAST_CHAR   ; Go into programming mode
        call    Programming_Mode
        goto    Printf_Run_Start

Wait_For_Key1
        movfw   QUARTER_SEC_TICK    ; Check if a few seconds has elapsed
        sublw   0x0C                ; 12 quarter seconds = 3 seconds
        btfsc   STATUS, C
        goto    Wait_For_Key

Lets_Run
        movlw   0x04                ; Is this a silent power up?
        call    Read_EEPROM
        xorlw   0x00
        btfss   STATUS, Z
        goto    Main_Loop_Start

Printf_Run_Start
        clrf    STRING_INDEX
Printf_Run
        movfw   STRING_INDEX
        call    Run_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Run_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Run
Printf_Run_end


;----------------------------------------------------------------------------
; Main processing loop.
;
Main_Loop_Start
        bsf     STATUS, RP0     ; **** select BANK 1
        bcf     PIE1, TMR2IE    ; Disable Timer2 Interrupt
        bcf     STATUS, RP0     ; **** select BANK 0

        movlw   0x00            ; Read the mode out of EEPROM
        call    Read_EEPROM
        movwf   CUR_MODE

        movlw   0x01            ; Read the baud rate code
        call    Read_EEPROM
        movwf   CUR_BAUD        ; Save off current BAUD code
        movwf   NEW_BAUD
        call    Baud_To_SPBRG   ; Convert to the baud rate generator value
        movwf   CUR_SPBRG

        movfw   CUR_SPBRG       ; Load Baud Rate Generator
        bsf     STATUS, RP0     ; **** select BANK 1
        movwf   SPBRG           ; Load Baud Rate Generator
        bcf     STATUS, RP0     ; **** select BANK 0

        movfw   CUR_MODE
        xorlw   0x00
        btfsc   STATUS, Z
;        goto    Main_Loop_Mode_0  -- NOTE!!! Not used in v1.01
        goto    Main_Loop_Mode_1
        movfw   CUR_MODE
        xorlw   0x01
        btfsc   STATUS, Z
        goto    Main_Loop_Mode_1
        movfw   CUR_MODE
        xorlw   0x02
        btfsc   STATUS, Z
        goto    Main_Loop_Mode_2

        ; Default is Mode 1

        goto    Main_Loop_Mode_1

;----------------------------------------------------------------------------
; Mode 0 - Automatic Baud Rate match
;
; INT trigger sets RS-485 enable hold
; UART interrupt clears RS-485 enable hold (primary)
; TIMER1 interrupt Rollover clears RS-485 enable hold (fail safe) 35.5 ms
;
; Auto Mode:
; Main loop
Main_Loop_Mode_0
        bsf     STATUS, RP0     ; **** select BANK 1
        bsf     PIE1, TMR2IE    ; Enable Timer2 Interrupt
        bcf     STATUS, RP0     ; **** select BANK 0

Main_Loop_Mode_0H
        clrwdt                          ; Kick the dog
        btfsc   PORTB, RS232_TXD        ; Wait for H-L transition of TXD
        goto    Main_Loop_Mode_0H

        bcf     INTCON, GIE
        bcf     PORTA, ENABLE_HOLD      ; Hold the RS-485 enable
        clrf    TMR1L                   ; clear TMR1 counters
        clrf    TMR1H
        bcf     OUR_FLAGS, TMR1_ROLLOVER

Main_Loop_Mode_0L
        clrwdt                          ; Kick the dog
        btfss   PORTB, RS232_TXD        ; Wait for L-H transition of TXD
        goto    Main_Loop_Mode_0L

        movfw   TMR1H                   ; TXD is now HIGH
        movwf   TMR1H_SAVE              ; Save TIMER1 count
        movfw   TMR1L
        movwf   TMR1L_SAVE
        clrf    ONE_MS_TICK             ; Clear tick count to stop  256 ms rollover

        bsf     INTCON, GIE

        ;
        ; Check if new minimum count
        ;
        btfsc   OUR_FLAGS, TMR1_ROLLOVER    ; Ignore counts when rollover
        goto    Main_Loop_Mode_0WAIT

        movfw   TMR1H_SAVE                  ; MSB
        subwf   TMR1H_MIN, W
        btfss   STATUS, C
        goto    Main_Loop_Mode_0WAIT
        movfw   TMR1L_SAVE                  ; LSB
        subwf   TMR1L_MIN, W
        btfss   STATUS, C
        goto    Main_Loop_Mode_0WAIT

        movfw   TMR1H_SAVE                  ; **** New minimum values
        movwf   TMR1H_MIN
        movfw   TMR1L_SAVE
        movwf   TMR1L_MIN

        call    Get_Baud_From_Count         ; Save off new baud code
        movwf   NEW_BAUD

        ;
        ; Check if new BAUD and SPBRG (set in TIMER2 ISR)
        ; Save in EEPROM

Main_Loop_Mode_0WAIT
        clrwdt                          ; Kick the dog
        btfss   PORTB, RS232_TXD        ;
        goto    Main_Loop_Mode_0WAIT

        goto    Main_Loop_Mode_0H

;----------------------------------------------------------------------------
; Mode 1 - Automatic Bit Time Calculated
;
; INT trigger sets RS-485 enable hold
; Main loop clears RS-485 enable hold (primary)
; TIMER1 interrupt Rollover clears RS-485 enable hold (fail safe)  35.5 ms
; Note: UART interrupt disabled since it will continuously trigger with errors
;
; Auto Mode:
; Main loop
Main_Loop_Mode_1
        bsf     STATUS, RP0     ; **** select BANK 1
        bcf     PIE1, RCIE      ; Disable USART rcv Interrupt
        bcf     PIE1, TMR2IE    ; Disable Timer2 Interrupt
        bcf     PIE1, TMR1IE    ; Disable Timer1 Interrupt
        bcf     STATUS, RP0     ; **** select BANK 0

Main_Loop_Mode_1START                   ; Wait for the start bit
        clrwdt                          ; Kick the dog
        btfsc   PORTB, RS232_TXD        ; Wait for H-L transition of TXD
        goto    Main_Loop_Mode_1START

        bcf     PORTA, ENABLE_HOLD      ; Hold the RS-485 enable
        clrf    TMR1L                   ; clear TMR1 counters
        clrf    TMR1H
        movlw   0xFF                    ; Reset TMR1 minimum time
        movwf   TMR1H_MIN
        movwf   TMR1L_MIN

Main_Loop_Mode_1L
        clrwdt                          ; Kick the dog
        btfss   PORTB, RS232_TXD        ; Wait for L-H transition of TXD
        goto    Main_Loop_Mode_1L
        call    Check_New_TMR1_Min

Main_Loop_Mode_1H
        btfsc   PORTB, RS232_TXD        ; Check for H-L transition of TXD
        goto    Main_Loop_Mode_1H_A     ; Still H
        call    Check_New_TMR1_Min
        goto    Main_Loop_Mode_1L

Main_Loop_Mode_1H_A
        clrwdt                          ; Kick the dog
        movfw   TIMEOUTH                ; Check for TIMEOUT - looking at MSB
        subwf   TMR1H, W
        btfss   STATUS, C
        goto    Main_Loop_Mode_1H
        movfw   TIMEOUTL                ; Check for TIMEOUT - looking at LSB
        subwf   TMR1L, W
        btfss   STATUS, C
        goto    Main_Loop_Mode_1H

Main_Loop_Mode_1TIMEOUT
        bsf     PORTA, ENABLE_HOLD      ; Release the RS-485 enable
        goto    Main_Loop_Mode_1START


;----------------------------------------------------------------------------
; Checks new minimum times
;
Check_New_TMR1_Min
        movfw   TMR1L                   ; TXD is now HIGH
        movwf   TMR1L_SAVE              ; Save TIMER1 counts
        clrf    TMR1L                   ; clear TMR1 counters
        movfw   TMR1H
        movwf   TMR1H_SAVE
        clrf    TMR1H                   ; clear TMR1 counters

        movfw   TMR1H_MIN                   ; MSB
        subwf   TMR1H_SAVE, W
        btfss   STATUS, C
        goto    Check_New_TMR1_Min_New      ; Saved < Min
        btfss   STATUS, Z
        goto    Check_New_TMR1_Min_x        ; Saved > Min
                                            ; Saved = Min
        movfw   TMR1L_MIN                   ; LSB
        subwf   TMR1L_SAVE, W
        btfsc   STATUS, C
        goto    Check_New_TMR1_Min_x        ; Saved > Min

Check_New_TMR1_Min_New
        movfw   TMR1H_SAVE                  ; **** New minimum values
        movwf   TMR1H_MIN
        movwf   TIMEOUTH
        movfw   TMR1L_SAVE
        movwf   TMR1L_MIN
        movwf   TIMEOUTL

        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     TIMEOUTL, F
        rlf     TIMEOUTH, F
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     TIMEOUTL, F
        rlf     TIMEOUTH, F
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     TIMEOUTL, F
        rlf     TIMEOUTH, F

Check_New_TMR1_Min_x
        return

;----------------------------------------------------------------------------
; Mode 2 - Manual Baud Rate
;
; INT trigger sets RS-485 enable hold
; UART interrupt clears RS-485 enable hold (primary)
; TIMER1 interrupt Rollover clears RS-485 enable hold (fail safe)  35.5 ms
; Note: TIMER2 interrupt disabled since we're not auto detecting
Main_Loop_Mode_2
        bsf     STATUS, RP0     ; **** select BANK 1
        bcf     PIE1, TMR2IE    ; Disable Timer2 Interrupt
        bcf     STATUS, RP0     ; **** select BANK 0

Main_Loop_Mode_2H
        clrwdt                          ; Kick the dog
        btfsc   PORTB, RS232_TXD        ; Wait for H-L transition of TXD
        goto    Main_Loop_Mode_2H

        bcf     PORTA, ENABLE_HOLD      ; Hold the RS-485 enable
        clrf    TMR1L                   ; clear TMR1 counters
        clrf    TMR1H
        bcf     OUR_FLAGS, TMR1_ROLLOVER

Main_Loop_Mode_2L
        clrwdt                          ; Kick the dog
        btfss   PORTB, RS232_TXD        ; Wait for L-H transition of TXD
        goto    Main_Loop_Mode_2L

        goto    Main_Loop_Mode_2H



Get_Baud_From_Count
        movf    TMR1H_SAVE, 0    ; Check if MSB == 0
        btfss   STATUS, Z
        goto    Get_Baud_MSB
        movfw   TMR1L_SAVE
        sublw   0x18            ; Check 115200
        btfsc   STATUS, C
        retlw   8
        movfw   TMR1L_SAVE
        sublw   0x24            ; Check 57600
        btfsc   STATUS, C
        retlw   7
        movfw   TMR1L_SAVE
        sublw   0x48            ; Check 38400
        btfsc   STATUS, C
        retlw   6
        movfw   TMR1L_SAVE
        sublw   0x90            ; Check 19200
        btfsc   STATUS, C
        retlw   5
Get_Baud_MSB
;        movfw   TMR1H_SAVE       ; Check 9600 MSB
        retlw   4

;----------------------------------------------------------------------------
; Transmits a single character out the USART
; W = Character to xmit
;
Transmit_Char
        clrwdt                  ; Kick the dog
        movwf   TXREG
        bsf     STATUS, RP0     ; select BANK 1
Transmit_Pending
        btfss   TXSTA, TRMT
        goto    Transmit_Pending
        bcf     STATUS, RP0     ; select BANK 0
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
; Writes the EEPROM
;
; Address in EEPROM_ADDRESS
; Value in W
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
; Unit programming mode
;
Programming_Mode
        clrf    STRING_INDEX
Printf_Programming
        movfw   STRING_INDEX
        call    Programming_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Programming_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Programming
Printf_Programming_end

        movlw   RCV_BUFFER      ; Set the RCV buffer pointer to start of buffer
        movwf   RCV_BUFFER_PTR

Programming_Main
        clrwdt                  ; Kick the dog while we're waiting for a character
        movfw   RCV_LAST_CHAR
        xorlw   0x0D            ; Is this a CR?
        btfss   STATUS, Z
        goto    Programming_Main

        clrf    RCV_LAST_CHAR   ; delete the CR
        movfw   RCV_BUFFER_PTR  ; Save off Current Buffer ptr (before it moves)
        movwf   CURRENT_PTR

Programming_Check_?
        movlw   '?'                 ; Display Current Settinga
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_W
        movlw   RCV_BUFFER+0x02     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        clrf    STRING_INDEX
Printf_Startup2                     ; Print the startup string (Firmware Version)
        movfw   STRING_INDEX
        call    Startup_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Startup2_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Startup2
Printf_Startup2_end

        movlw   'S'                 ; Serial Number
        call    Transmit_Char
        movlw   '/'
        call    Transmit_Char
        movlw   'N'
        call    Transmit_Char
        movlw   ' '
        call    Transmit_Char
        movlw   0x10                ; Get S/N byte 1 EEPROM location 0x10
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x11                ; Get S/N byte 2 EEPROM location 0x11
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x12                ; Get S/N byte 3 EEPROM location 0x12
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x13                ; Get S/N byte 4 EEPROM location 0x13
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x0D                ; CR
        call    Transmit_Char

        movlw   'M'                 ; Current mode
        call    Transmit_Char
        movlw   'o'
        call    Transmit_Char
        movlw   'd'
        call    Transmit_Char
        movlw   'e'
        call    Transmit_Char
        movlw   ' '
        call    Transmit_Char
        movlw   0x00                ; Get MODE setting EEPROM location 0x00
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x0D                ; CR
        call    Transmit_Char

        clrf    STRING_INDEX
Printf_Default_Baud                 ; Print the Default Baud Rate
        movfw   STRING_INDEX
        call    Default_Baud_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Default_Baud_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Default_Baud
Printf_Default_Baud_end
        movlw   0x01                ; Get Default Baud Rate EEPROM location 0x01
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x0D                ; CR
        call    Transmit_Char

        clrf    STRING_INDEX
Printf_Silent                       ; Print the Silent Power up mode
        movfw   STRING_INDEX
        call    Silent_String
        xorlw   0x00
        btfsc   STATUS, Z
        goto    Printf_Silent_end
        call    Transmit_Char
        incf    STRING_INDEX
        goto    Printf_Silent
Printf_Silent_end
        movlw   0x04                ; Get Silent Mode flag EEPROM location 0x04
        call    Read_EEPROM
        call    Print_Hex_Byte
        movlw   0x0D                ; CR
        call    Transmit_Char

        goto    Programming_Finish_Command

Programming_Check_W
        movlw   'W'                 ; WRITE EEPROM command
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_M
        movlw   RCV_BUFFER+0x06     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+1        ; Get the EEPROM ADDRESS - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+2        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        movwf   EEPROM_ADDRESS

        movfw   RCV_BUFFER+3        ; Get the EEPROM VALUE - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+4        ; Get the EEPROM VALUE - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Write_EEPROM        ; Write value to EEPROM

        movlw   'W'                 ; EEPROM Write delayed enough already
        call    Transmit_Char
        movlw   0x0D                ; CR
        call    Transmit_Char
        goto    Programming_Finish_Command

Programming_Check_M
        movlw   'M'                 ; MODE change command
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_B
        movlw   RCV_BUFFER+0x03     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+1        ; Get the MODE value
        call    Hex_To_Nibble
        movwf   SCRATCH
        movlw   0x00                ; Mode EEPROM address 0x00
        movwf   EEPROM_ADDRESS
        movfw   SCRATCH
        call    Write_EEPROM

        movlw   'M'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Programming_Finish_Command

Programming_Check_B
        movlw   'B'                 ; BAUD change command
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_S
        movlw   RCV_BUFFER+0x03     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+1        ; Get the MODE value
        call    Hex_To_Nibble
        movwf   SCRATCH
        movlw   0x01                ; BAUD EEPROM address 0x01
        movwf   EEPROM_ADDRESS
        movfw   SCRATCH
        call    Write_EEPROM

        movlw   'B'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Programming_Finish_Command

Programming_Check_S
        movlw   'S'                 ; SILENT POWER UP change command
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_R
        movlw   RCV_BUFFER+0x03     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+1        ; Get the MODE value
        call    Hex_To_Nibble
        movwf   SCRATCH
        movlw   0x04                ; SILENT POWER UP FLAG EEPROM address 0x04
        movwf   EEPROM_ADDRESS
        movfw   SCRATCH
        call    Write_EEPROM

        movlw   'S'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
        goto    Programming_Finish_Command

Programming_Check_R
        movlw   'R'                 ; READ EEPROM command
        xorwf   RCV_BUFFER, W
        btfss   STATUS, Z
        goto    Programming_Check_Q
        movlw   RCV_BUFFER+0x04     ; Check for correct command/packet length
        xorwf   CURRENT_PTR, W
        btfss   STATUS, Z
        goto    Programming_Error_Response
                                    ; Command/Packet OK
        movfw   RCV_BUFFER+1        ; Get the EEPROM ADDRESS - MSB
        call    Hex_To_Nibble
        movwf   SCRATCH
        bcf     STATUS, C           ; Clear the carry before rotate
        rlf     SCRATCH, F          ; Rotate to High order nibble
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        rlf     SCRATCH, F
        movfw   RCV_BUFFER+2        ; Get the EEPROM ADDRESS - LSB
        call    Hex_To_Nibble
        iorwf   SCRATCH, W
        call    Read_EEPROM
        movwf   SCRATCH             ; Save the data

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
        goto    Programming_Finish_Command

Programming_Check_Q
        movlw   'Q'                 ; QUIT
        xorwf   RCV_BUFFER, W
        btfsc   STATUS, Z
        goto    Programming_End

Programming_Error_Response
        movlw   'E'
        call    Transmit_Char
        movlw   'r'
        call    Transmit_Char
        movlw   'r'
        call    Transmit_Char
        movlw   '!'
        call    Transmit_Char
        movlw   0x0D
        call    Transmit_Char
Programming_Finish_Command
        movlw   RCV_BUFFER      ; Set the RCV buffer pointer to start of buffer
        movwf   RCV_BUFFER_PTR
        goto    Programming_Main

Programming_End
        movlw   RCV_BUFFER      ; Set the RCV buffer pointer to start of buffer
        movwf   RCV_BUFFER_PTR
        movfw   RCREG           ; Read receive register to clear errors and any data
        return


;----------------------------------------------------------------------------
; Prints the hexidecimal byte in W
;
Print_Hex_Byte
        movwf   SCRATCH             ; Save the data
        andlw   0xF0                ; Send out the MSB
        movwf   HEX_TEMP
        bcf     STATUS, C           ; Clear the carry before rotate
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, F
        rrf     HEX_TEMP, W
        call    Nibble_To_Hex
        call    Transmit_Char
        movfw   SCRATCH             ; Get the saved Data
        andlw   0x0F                ; Send out the LSB
        call    Nibble_To_Hex
        call    Transmit_Char
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
;
; Interrupt Service Routines Follow.
;
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; Timer1_Isr
;
; Bump the running time variables
;----------------------------------------------------------------------------
;
Timer1_Isr
        bsf     PORTA, ENABLE_HOLD  ; Release the RS-485 enable
        bsf     OUR_FLAGS, TMR1_ROLLOVER
        bcf     PIR1, TMR1IF    ; clear the Timer 1 Irq
        return

;----------------------------------------------------------------------------
; Timer2_Isr
;
; Bump the running time variables
;----------------------------------------------------------------------------
;
Timer2_Isr
        incf    ONE_MS_TICK         ; Increments every 1 ms
        btfss   STATUS, Z
        goto    Timer2_Isr_End

        incf    QUARTER_SEC_TICK    ; Increments every 256 ms

        ;
        ;
        movlw   0xFF
        movwf   TMR1H_MIN
        movwf   TMR1L_MIN

        ;
        ; Check if new Baud code to save in EEPROM and SPBRG
        ;
        movfw   CUR_BAUD
        xorwf   NEW_BAUD, W
        btfsc   STATUS, Z
        goto    Timer2_Isr_End

        movfw   NEW_BAUD            ; Save new baud code
        movwf   CUR_BAUD
        call    Baud_To_SPBRG       ; Make it into a new SPBRG
        movwf   CUR_SPBRG
        movwf   SPBRG

Timer2_Isr_End
        bcf     PIR1, TMR2IF        ; clear the Timer 2 Irq
        return


PortB_Isr

        movfw   PORTB               ; Read to clear interrupt
        bcf     INTCON, RBIF        ; clear the INT flag
        return

;----------------------------------------------------------------------------
; External Interrupt (INT)
;
; Holds the RS-485 enable
;----------------------------------------------------------------------------
;
Int_Isr
        bcf     PORTA, ENABLE_HOLD  ; Hold the RS-485 enable
        bsf     OUR_FLAGS, INT_EVENT
        clrf    TMR1H               ; clear TMR1 counters
        clrf    TMR1L
        bcf     OUR_FLAGS, TMR1_ROLLOVER
        bcf     INTCON, INTF        ; clear the INT flag
        return

;----------------------------------------------------------------------------
; Rcv_Isr
;
; Asynchronous data reception ISR
; Releases RS-485 enable hold any data receive as well as all errors
;----------------------------------------------------------------------------
;
Rcv_Isr
        bsf     PORTA, ENABLE_HOLD  ; Release the RS-485 enable
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
;        incf    RCV_ERROR_CNT
        movfw   RCREG               ; read data byte into W and DUMP
        clrf    RCV_LAST_CHAR
        return

Rcv_Isr_Read
        movfw   RCREG               ; read data byte into W
        movwf   RCV_LAST_CHAR

        movfw   RCV_BUFFER_PTR      ; Get RCV buffer pointer
        movwf   FSR
        movfw   RCV_LAST_CHAR
        movwf   INDF                ; Save off in RCV buffer

        movfw   RCV_BUFFER_PTR      ; Get RCV buffer pointer
        incf    RCV_BUFFER_PTR, W   ; Bump RCV buffer pointer for next read
        andlw   0x4F                ; Prevent Buffer overrun
        movwf   RCV_BUFFER_PTR
        return


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

        btfsc   PIR1, TMR2IF    ; Is TMR2IF Set?
        call    Timer2_Isr

        btfsc   PIR1, RCIF      ; is bit 5 (RCIF) Set?
        call    Rcv_Isr

        btfsc   INTCON, RBIF
        call    PortB_Isr

        movfw   STATUS_TEMP     ; move old STATUS into W
        movwf   STATUS          ; restore status
        swapf   W_TEMP, F       ; swap it so it will be right in the next instruction.
        swapf   W_TEMP, W       ; move old W into W
        retfie
;
;----------------------------------------------------------------------------
;

        ORG     0x400

;----------------------------------------------------------------------------
; Converts a binary nibble in W to an ASCII Hexadecimal value
; returned in W
;
Nibble_To_Hex
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
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


;----------------------------------------------------------------------------
; Converts a Baud rate code in W to baud rate generator value
; Note: Assumes clock rate 7.3728 Mhz
;
Baud_To_SPBRG
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        andlw   0x0F
        addwf   PCL, F
        retlw   SPBRG_1200      ; 0 = 1200
        retlw   SPBRG_1200      ; 1 = 1200
        retlw   SPBRG_2400      ; 2 = 2400
        retlw   SPBRG_4800      ; 3 = 4800
        retlw   SPBRG_9600      ; 4 = 9600
        retlw   SPBRG_19200     ; 5 = 19200
        retlw   SPBRG_38400     ; 6 = 38400
        retlw   SPBRG_57600     ; 7 = 57600
        retlw   SPBRG_115200    ; 8 = 115200
        retlw   SPBRG_230400    ; 9 = 230400
        retlw   SPBRG_115200    ; A = Reserved
        retlw   SPBRG_115200    ; B = Reserved
        retlw   SPBRG_115200    ; C = Reserved
        retlw   SPBRG_115200    ; D = Reserved
        retlw   SPBRG_115200    ; E = Reserved
        retlw   SPBRG_115200    ; F = Reserved

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;
; Strings to print follow

; Index is assumed to be in W
Startup_String
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
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
        RETLW   'X'
        RETLW   'P'
        RETLW   ' '
        RETLW   'C'
        RETLW   'o'
        RETLW   'n'
        RETLW   'v'
        RETLW   'e'
        RETLW   'r'
        RETLW   't'
        RETLW   'e'
        RETLW   'r'
        RETLW   ' '
        RETLW   'v'
        RETLW   VERSION_MAJOR
        RETLW   '.'
        RETLW   VERSION_MINOR1
        RETLW   VERSION_MINOR2
        RETLW   0D
        RETLW   00

; Index is assumed to be in W
Programming_String
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        addwf   PCL, F
        RETLW   'P'
        RETLW   'r'
        RETLW   'o'
        RETLW   'g'
        RETLW   'r'
        RETLW   'a'
        RETLW   'm'
        RETLW   'm'
        RETLW   'i'
        RETLW   'n'
        RETLW   'g'
        RETLW   ' '
        RETLW   'm'
        RETLW   'o'
        RETLW   'd'
        RETLW   'e'
        RETLW   0D
        RETLW   00

; Index is assumed to be in W
Run_String
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        addwf   PCL, F
        RETLW   'R'
        RETLW   'u'
        RETLW   'n'
        RETLW   ' '
        RETLW   'm'
        RETLW   'o'
        RETLW   'd'
        RETLW   'e'
        RETLW   0D
        RETLW   00

; Index is assumed to be in W
Silent_String
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        addwf   PCL, F
        RETLW   'S'
        RETLW   'i'
        RETLW   'l'
        RETLW   'e'
        RETLW   'n'
        RETLW   't'
        RETLW   ' '
        RETLW   'P'
        RETLW   'o'
        RETLW   'w'
        RETLW   'e'
        RETLW   'r'
        RETLW   ' '
        RETLW   'U'
        RETLW   'p'
        RETLW   ' '
        RETLW   00

; Index is assumed to be in W
Default_Baud_String
        bcf     PCLATH, 0   ; Adjust PCLATH so we're in the same page when
        bcf     PCLATH, 1   ; the add to PCL is done
        bsf     PCLATH, 2
        addwf   PCL, F
        RETLW   'D'
        RETLW   'e'
        RETLW   'f'
        RETLW   'a'
        RETLW   'u'
        RETLW   'l'
        RETLW   't'
        RETLW   ' '
        RETLW   'B'
        RETLW   'a'
        RETLW   'u'
        RETLW   'd'
        RETLW   ' '
        RETLW   00

;
; End of Code
;
        end



