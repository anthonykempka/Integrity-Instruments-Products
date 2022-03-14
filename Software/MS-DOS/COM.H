//***************************************************************************
//
//
// COM.H
//
//                      INTEGRITY DESIGNS, LLC
//                      ---------------------------
//                      209 Grand Avenue, Suite 403
//                      Laramie, WY  82070
//                      (307)-721-4881
//
//
// Last Modified:       
//      A. A. Kempka    11/10/95        Original.
//  
//
//***************************************************************************
//
//

#ifndef _COMINC
#define _COMINC

//
// Macro definitions
//

#define COM1    1                       // COM1 PC serial port
#define COM2    2                       // COM2 PC serial port

#define COM1_BASE_ADDRESS 0x3F8         // Base address of the COM1 PC
#define COM2_BASE_ADDRESS 0x2F8         // Base address of the COM2 PC

#define RX_REGISTER_8250        0       // 8250 Receiver Buffer Register
#define TX_REGISTER_8250        0       // 8250 Transmit Buffer Register
#define IER_8250                1       // 8250 Interrupt Enable Register
#define IIR_8250                2       // 8250 Interrupt Id Register
#define LCR_8250                3       // 8250 Line Control Register
#define MCR_8250                4       // 8250 Modem Control Register
#define LSR_8250                5       // 8250 Line Status Register
#define MSR_8250                6       // 8250 Modem Status Register
#define DIVISOR_REGISTER_8250   0       // 8250 16-bit Baud Rate Divisor


//***************************************************************************
//
// Interrupt Enable Register -- IER defines
//

#define IER_ENABLE_RX_DATA_READY_IRQ 0x01   // Enables the Receiver Data
                                            // Ready interrupt when Or'd
                                            // with the IER

#define IER_DISABLE_RX_DATA_READY_IRQ 0xFE  // Disables the Receiver Data
                                            // Ready interrupt when
                                            // And'd with the IER

#define IER_ENABLE_TX_BE_IRQ 0x02           // Enables the Transmitter
                                            // Buffer Empty interrupt
                                            // when Or'd with the IER

#define IER_DISABLE_TX_BE_IRQ 0xFD          // Disables the Transmitter
                                            // Buffer Empty interrupt
                                            // when And'd with the IER

#define IER_ENABLE_RX_ERROR_IRQ 0x04        // Enables the Receiver Line
                                            // Status interrupt when
                                            // Or'd with the IER

#define IER_DISABLE_RX_ERROR_IRQ 0xFB       // Disables the Receiver Line
                                            // Status interrupt when
                                            // And'd with the IER

#define IER_ENABLE_MODEM_STATUS_IRQ 0x08    // Enables the MODEM Status
                                            // interrupt when Or'd with
                                            // the IER

#define IER_DISABLE_MODEM_STATUS_IRQ 0xF7   // Disables the MODEM Status
                                            // interrupt when And'd with
                                            // the IER


//***************************************************************************
//
// Interrupt ID Register -- IIR defines
//

#define IIR_INTERRUPT_MASK 0x07             // Masks off 3 interrupt pending
                                            // bits when And'd with the
                                            // IIR

#define IIR_NO_INTERRUPT_PENDING 0x01       // This defines the Interrupt
                                            // Identification register
                                            // pattern when no UART
                                            // interrupts are pending.

#define IIR_MODEM_STATUS_IRQ_PENDING 0x00   // Interrupt pending pattern for
                                            // MODEM Status interrupt.

#define IIR_TX_HBE_IRQ_PENDING 0x02         // Interrupt pending pattern for
                                            // Transmitter Buffer Empty

#define IIR_RX_DATA_READY_IRQ_PENDING 0x04  // Interrupt pending pattern for
                                            // Receiver Buffer Full

#define IIR_RX_ERROR_IRQ_PENDING 0x06       // Interrupt pending pattern for
                                            // Receiver Line Status


//***************************************************************************
//
// Line Control Register -- LCR defines
//

#define LCR_FIVE_BITS_PER_WORD 0x00         // Sets serial word length to 5
                                            // bits when Or'd with the LCR

#define LCR_SIX_BITS_PER_WORD 0x01          // Sets serial word length to 6
                                            // bits when Or'd with the LCR 

#define LCR_SEVEN_BITS_PER_WORD 0x02        // Sets serial word length to 7
                                            // bits when Or'd with the LCR 

#define LCR_EIGHT_BITS_PER_WORD 0x03        // Sets serial word length to 8
                                            // bits when Or'd with the LCR

#define LCR_ONE_STOP_BIT 0x00               // Sets the number of stop bits
                                            // to 1 when Or'd with the LCR

#define LCR_TWO_STOP_BITS 0x04              // Sets the number of stop bits
                                            // to 2 when Or'd with the LCR

#define LCR_NO_PARITY 0x00                  // Disables parity when Or'd
                                            // with the LCR

#define LCR_ODD_PARITY 0x08                 // Enables odd parity when Or'd
                                            // with the LCR

#define LCR_EVEN_PARITY 0x18                // Enables even parity when Or'd
                                            // with the LCR

#define LCR_MARK_PARITY 0x28                // Enables mark parity when Or'd
                                            // with the LCR

#define LCR_SPACE_PARITY 0x38               // Enables space parity when
                                            // Or'd with the LCR

#define LCR_DISABLE_BREAK 0xBF              // Disables the break condition
                                            // when And'd with the LCR

#define LCR_ENABLE_BREAK 0x40               // Enables the break condition
                                            // when Or'd with the LCR

#define LCR_DISABLE_DIVISOR_LATCH 0x7F      // Disables the divisor latch
                                            // when And'd with the LCR

#define LCR_ENABLE_DIVISOR_LATCH 0x80       // Enables the divisor latch
                                            // when Or'd with the LCR


//***************************************************************************
//
// Modem Control Register -- MCR defines
//

#define MCR_DEACTIVATE_ALL 0x00             // Deactivates all MCR outputs

#define MCR_ACTIVATE_DTR 0x01               // Activates the RS-232 DTR line
                                            // when Or'd with the MCR

#define MCR_DEACTIVATE_DTR 0xFE             // Deactivates the RS-232 DTR
                                            // line when And'd with the MCR

#define MCR_ACTIVATE_RTS 0x02               // Activates the RS-232 RTS line
                                            // when Or'd with the MCR

#define MCR_DEACTIVATE_RTS 0xFD             // Deactivates the RS-232 RTS
                                            // line when And'd with the MCR

#define MCR_ACTIVATE_GP01 0x04              // Activates the general purpose
                                            // output #1 when Or'd with MCR

#define MCR_DEACTIVATE_GP01 0xFB            // Deativates the general
                                            // purpose output #1 when
                                            // And'd with the MCR

#define MCR_ACTIVATE_GP02 0x08              // Activates the general purpose
                                            // output #2 when Or'd with MCR

#define MCR_DEACTIVATE_GP02 0xF7            // Deativates the general
                                            // purpose output #2 when
                                            // And'd with the MCR

#define MCR_ACTIVATE_LOOPBACK 0x10          // Activates the local loopback
                                            // test when Or'd with the MCR

#define MCR_DEACTIVATE_LOOPBACK 0xEF        // Deactivates the local
                                            // loopback test when And'd
                                            // with the MCR


//***************************************************************************
//
// Line Status Register -- LSR defines
//

#define LSR_RX_DATA_READY 0x01              // This bit signals a
                                            // character is ready

#define LSR_RX_OVERRUN_ERROR 0x02           // This bit signals a
                                            // receiver overrun error 

#define LSR_RX_PARITY_ERROR 0x04            // This bit signals a
                                            // parity error

#define LSR_RX_FRAMING_ERROR 0x08           // This bit signals a
                                            // framing error

#define LSR_RX_BREAK_DETECTED 0x10          // This bit signals a break char

#define LSR_TX_BUFFER_EMPTY 0x20            // This bit signals the transmit
                                            // buffer is empty

#define LSR_TX_BOTH_EMPTY 0x40              // This bit signals both the
                                            // transmit buffer and the shift
                                            // register are empty


//***************************************************************************
//
// Modem Status Register -- MSR defines
//

#define MSR_DELTA_CTS 0x01                  // Signals CTS has changed

#define MSR_DELTA_DSR 0x02                  // Signals DSR has changed

#define MSR_DELTA_RI 0x04                   // Signals RI has changed

#define MSR_DELTA_DCD 0x08                  // Signals CDC has changed

#define MSR_CURRENT_CTS 0x10                // Reads current CTS

#define MSR_CURRENT_DSR 0x20                // Reads current DSR

#define MSR_CURRENT_RI 0x40                 // Reads current RI

#define MSR_CURRENT_DCD 0x80                // Reads current DCD


//
// Communication system BAUD rate defines.
//

#define BAUD_RATE_DIVISOR_300       0x0180  // 300 baud

#define BAUD_RATE_DIVISOR_600       0x00C0  // 600 baud

#define BAUD_RATE_DIVISOR_1200      0x0060  // 1200 baud

#define BAUD_RATE_DIVISOR_2400      0x0030  // 2400 baud

#define BAUD_RATE_DIVISOR_4800      0x0018  // 4800 baud 

#define BAUD_RATE_DIVISOR_9600      0x000C  // 9600 baud 

#define BAUD_RATE_DIVISOR_19200     0x0006  // 19200 baud

#define BAUD_RATE_DIVISOR_38400     0x0003  // 38400 baud

#define BAUD_RATE_DIVISOR_57600     0x0002  // 57600 baud

#define BAUD_RATE_DIVISOR_115200    0x0001  // 152000 baud

//
// Communication port type definition
//

typedef struct _COMPORT {
    short RBR;                          // Receiver Buffer reg.
    short TBR;                          // Transmitter Buffer reg.
    short IER;                          // Interrupt Enable reg.
    short IIR;                          // Interrupt ID reg.
    short LCR;                          // Line Control register.
    short MCR;                          // Modem Control register.
    short LSR;                          // Line Status register.
    short MSR;                          // Modem Status register.
    short BAUD;                         // 16-bit Baud Rate Divisor.
    short SPEM;                         // Mask to enable the serial port.
    short SPDM;                         // Mask to disable the serial port.
    short InterruptVector;              // Used with setvect ()
} COMPORT, *PCOM_PORT;

#endif // _COMINC

