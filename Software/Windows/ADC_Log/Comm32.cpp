//
// COM32.CPP
// -----------
// WIN32 communications library interface functions and code examples.
//
//
// This code should compile under Visual C/C++ as a WIN32 console application.
//
//
// Written By:
// ------------
// Anthony A. Kempka
// Integrity Instruments
// 
// Tel: 8000-450-2001
// Web: www.rs-485.com
//
// Note:
// -----
// Please read the following license
//


/*  LICENSE AGREEMENT FOR INTEGRITY INSTRUMENTS SOFTWARE

    This legal agreement between you, the customer or end user, and 
    INTEGRITY INSTRUMENTS gives you certain rights, benefits, and 
    obligations.  By breaking any sealed package or by using the product, 
    you agree to be bound by the terms of this agreement and indicate that 
    you have read, understood, and will comply with these terms.

1.  Product - The term product refers to the software supplied 
    regardless of the media or means by which it is supplied.

2.  License - The license agreement that INTEGRITY INSTRUMENTS grants 
    is to install, use, and modify the product in whatever manner you choose.

3.  You may not sell, lease, distribute, or release the product, in 
    whole or in part, without explicit written permission from 
    INTEGRITY INSTRUMENTS.  

4.  INTEGRITY INSTRUMENTS makes no warranties to the product's contents, 
    disclaims any implied warranties of any kind, except that the 
    product will perform functionally in accordance with the written 
    documentation supplied.

5.  INTEGRITY INSTRUMENTS shall not be liable to any person or entity for 
    any direct, indirect, special, incidental or consequential damages 
    resulting from the use or misuse of its product, including but not 
    limited to loss of business, business profits, or business credibility.  */


#include "stdafx.h"
#include <windows.h>            // Standard Windows include for system defines
#include <stdlib.h>             // Standard library include
#include <winbase.h>            // Has WIN32 Comm library and structure DEFS


#include <stdio.h>              // I/O include for printf() function
#include <conio.h>              // Include for the getch() function


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// Open_Comport
// -------------
// Opens a communications port for use.
//
// Returns:   Handle number if successful, else 0
//
// Comport  = Communications port to open (ie. 1, 2, 3, 4)
// BaudRate = Desired baud rate (ie, 110, 300, 600, 1200, 2400, 4800, 9600, 19200, ...)
//
HANDLE Open_Comport (DWORD Comport, DWORD BaudRate)
{
    HANDLE  DriverHandle;
    char    Com_Name[10];
    DWORD   DCB_Baud_Rate;
    DCB     Our_DCB;

    //
    // Make a COMPORT name from a number
    //
    switch (Comport) {
        case 1:
            strcpy (Com_Name, "COM1");
            break;
        case 2:
            strcpy (Com_Name, "COM2");
            break;
        case 3:
            strcpy (Com_Name, "COM3");
            break;
        case 4:
            strcpy (Com_Name, "COM4");
            break;
        default:
            return INVALID_HANDLE_VALUE;
    }

    //
    // Convert the Baud Rate to the Com library define for the DCB
    //
    switch (BaudRate) {
        case 110:
            DCB_Baud_Rate = 110;
            break;
        case 300:
            DCB_Baud_Rate = CBR_300;
            break;        
        case 600:
            DCB_Baud_Rate = CBR_600;
            break;
        case 1200:
            DCB_Baud_Rate = CBR_1200;
            break;
        case 2400:
            DCB_Baud_Rate = CBR_2400;
            break;
        case 4700:
            DCB_Baud_Rate = CBR_4800;
            break;
        case 9600:
            DCB_Baud_Rate = CBR_9600;
            break;            
        case 19200:
            DCB_Baud_Rate = CBR_19200;
            break;
        case 38400:
            DCB_Baud_Rate = CBR_38400;
            break;        
        case 57600:
            DCB_Baud_Rate = CBR_57600;
            break;
        case 115200:
            DCB_Baud_Rate = CBR_115200;
            break;
        default:
            return INVALID_HANDLE_VALUE;
    }

    //
    // Open a channel to the Comport - This example DOES NOT use overlapped I/O
    // Since the benefits of overlapped I/O are few, we suggest not using it.
    //
    DriverHandle = CreateFile (Com_Name, GENERIC_READ | GENERIC_WRITE,
                               0, NULL, OPEN_EXISTING, 0, NULL);

    //
    // Do we have a valid handle? (If not, the driver probably isn't loaded)
    //

    if (DriverHandle == INVALID_HANDLE_VALUE) {
        return (DriverHandle);
    } else {


        //
        // The SetupComm() function establishes the Transmit and Receive 
        // buffer sizes
        //
		SetupComm (DriverHandle, 1024, 1024);

        //
        // Obtain the current DCB structure. this can be saved away for restore after
        // the application is done using the Comport
        //
        GetCommState (DriverHandle, &Our_DCB);

        //
        // Fill in the DCB structure with our own settings.
        //
        Our_DCB.BaudRate = DCB_Baud_Rate;
        Our_DCB.fParity = 0;
        Our_DCB.fOutxCtsFlow = 0;
        Our_DCB.fOutxDsrFlow = 0;
        Our_DCB.fDtrControl = DTR_CONTROL_ENABLE;
        Our_DCB.fDsrSensitivity = FALSE;
        Our_DCB.fTXContinueOnXoff = 0;
        Our_DCB.fOutX = 0;
        Our_DCB.fInX = 0;
        Our_DCB.fErrorChar = 0;
        Our_DCB.fNull = 0;
        Our_DCB.fRtsControl = RTS_CONTROL_DISABLE;
        Our_DCB.fAbortOnError = 0;
        Our_DCB.ByteSize = 8;
        Our_DCB.Parity = NOPARITY;
        Our_DCB.StopBits = ONESTOPBIT;

        //
        // Configure the comport with our new DCB
        //
        SetCommState (DriverHandle, &Our_DCB); 

        //
        // Setup a mask that allows us to tell if the port is done transmitting 
        // the current buffer of data
        //
        SetCommMask (DriverHandle, EV_TXEMPTY);
    }

    return DriverHandle;
}


//
// Close_Comport
// -------------
// Closes a communications port previously opened
//
// Returns:   TRUE on success, FALSE on failure
//
// DriverHandle = Handle returned on Open_Comport() call
//
int Close_Comport (HANDLE DriverHandle)
{
    //
    // Sanity check on the Handle
    //
    if ((DriverHandle == 0) || (DriverHandle == INVALID_HANDLE_VALUE)) {
        return FALSE;
    }

    //
    // Close the communications port
    //
    CloseHandle (DriverHandle);

    return TRUE;
}


//
// Write_Comport
// -------------
// Writes a buffer of data out the RS232 commmunications port
//
// Returns:   TRUE on success, FALSE on failure
//
// DriverHandle = Handle returned on Open_RS232() call
// NumBytes     = Number of bytes to send
// Buffer       = Buffer of data to send
//
int Write_Comport (HANDLE DriverHandle, DWORD NumBytes, void *Buffer)
{
    DWORD EvtMask, BytesWritten;
    BOOL status;

    //
    // Sanity check on the Handle
    //
    if ((DriverHandle == 0) || (DriverHandle == INVALID_HANDLE_VALUE)) {
        return FALSE;
    } 

    //
    // Some devices may require setting RTS or DTR to specific states prior to 
    // tranmission of data 
    //
    EscapeCommFunction (DriverHandle, SETRTS);

    //
    // Output the data to the Comport
    //
    status = WriteFile (DriverHandle, Buffer, NumBytes, &BytesWritten, 0);

    //
    // If you want, use the WaitCommEvent() function to wait for the 
    // data Transmission to complete
    //
    WaitCommEvent(DriverHandle, &EvtMask, NULL);

    //
    // Some devices may require clearing RTS or DTR to after transmission of data 
    //
    EscapeCommFunction (DriverHandle, CLRRTS);
 
    return TRUE;
}



//
// Read_Comport
// -------------
// Reads data received on the RS232 communications port
//
// Returns:   TRUE on success, FALSE on failure
//
// DriverHandle = Handle returned on Open_RS232() call
// BytesRead    = Number of bytes read by the ReadFile() function
// BufferSize   = Size of Buffer in bytes
// Buffer       = Data buffer that holds received data
//
int Read_Comport (HANDLE DriverHandle, DWORD *BytesRead, DWORD BufferSize, void *Buffer)
{
    COMSTAT     Our_Comstat;
    DWORD       Com_Errors, BytesToRead;

    //
    // Sanity check on the Handle
    //
    if ((DriverHandle == 0) || (DriverHandle == INVALID_HANDLE_VALUE)) {
        return FALSE;
    } 

    //
    // By far the most efficient way to input Comport data is the following:
    // ---------------------------------------------------------------------
    // A) See how much data is present
    // B) Read exactly that amount of data
    //
    // Note: It is possible to ask for more data than is present, but the ReadFile() 
    //       function call will not return until that amount of data has been received.
    //       Some programs will request more data and establish a timeout using the 
    //       SetCommTimeouts() function to return from the ReadFile() function when
    //       not enough data has been receive. Although this may work, it is inefficient
    //       and generally works like crap. For everyone's sake, use the technique above 
    //       and handle your own timeout in applications code.
    //

    //
    // Check the Input buffer
    //
    ClearCommError (DriverHandle, &Com_Errors, &Our_Comstat);

    //
    // ReadFile() only if there is data.
    //
    if (Our_Comstat.cbInQue > 0) {

        //
        // Make sure we do not overrun the Buffer
        //
        if (Our_Comstat.cbInQue > BufferSize) {
            BytesToRead = BufferSize;
        } else {
            BytesToRead = Our_Comstat.cbInQue;
        }

        //
        // Read the data, no (NULL) Overlapped I/O
        //
        ReadFile (DriverHandle, Buffer, BytesToRead, BytesRead, NULL);

    } else {
        *BytesRead = 0;
    }

    return TRUE;
}
