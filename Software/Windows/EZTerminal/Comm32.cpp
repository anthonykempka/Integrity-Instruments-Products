// Comm32.cpp: implementation of the CComm32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EZTerm.h"

#include "Comm32.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComm32::CComm32()
{
    bComport_Open = FALSE;
}

CComm32::~CComm32()
{
    //
    // If there's an open comport, close the darn thing
    //
    if (bComport_Open) {
        Close_Comport ();
    }
}

BOOL CComm32::Open_Comport(DWORD iComport, DWORD iBaudRate, DWORD iDataBits, DWORD iStopBits, DWORD iParity)
{
    HANDLE  DriverHandle;
    
    
    DCB     Our_DCB;

    //
    // Make a COMPORT name from a number
    //

    if ((iComport < 0) || (iComport > 255)) {
        iComport = 1;
    }

    Com_Name.Format ("\\\\.\\COM%i", iComport);
    
    //
    // Convert the Baud Rate to the Com library define for the DCB
    //
    switch (iBaudRate) {
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
        case 4800:
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
            return FALSE;
    }

    //
    // Open a channel to the Comport - This example DOES NOT use overlapped I/O
    // Since the benefits of overlapped I/O are few, we suggest not using it.
    //
    DriverHandle = ::CreateFile (Com_Name, GENERIC_READ | GENERIC_WRITE,
                                 0, NULL, OPEN_EXISTING, 0, NULL);

    //
    // Do we have a valid handle? (If not, the driver probably isn't loaded)
    //

    if (DriverHandle == INVALID_HANDLE_VALUE) {
        return (FALSE);
    } else {

        hDriver_Handle = DriverHandle;
        bComport_Open = TRUE;
        //
        // The SetupComm() function establishes the Transmit and Receive 
        // buffer sizes
        //
        ::SetupComm (DriverHandle, 1024, 1024);

        //
        // Obtain the current DCB structure. this can be saved away for restore after
        // the application is done using the Comport
        //
        ::GetCommState (DriverHandle, &Our_DCB);

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
        ::SetCommState (DriverHandle, &Our_DCB); 

        //
        // Setup a mask that allows us to tell if the port is done transmitting 
        // the current buffer of data
        //
        ::SetCommMask (DriverHandle, EV_TXEMPTY);
    }

    return TRUE;
}



void CComm32::Close_Comport()
{
    //
    // Close the communications port
    //
    if (bComport_Open) {
        ::CloseHandle (hDriver_Handle);
        bComport_Open = FALSE;
		TRACE("Closing Commport\n");
    }
}

BOOL CComm32::Write_Comport(DWORD Length, char *Buffer)
{
//    DWORD EvtMask;    
    DWORD BytesWritten;
    BOOL status;

    if (!bComport_Open) {
        return FALSE;
    }

    //
    // Some devices may require setting RTS or DTR to specific states prior to 
    // tranmission of data 
    //
//    EscapeCommFunction (DriverHandle, SETRTS);

    //
    // Output the data to the Comport
    //
    status = ::WriteFile (hDriver_Handle, Buffer, Length, &BytesWritten, 0);

    //
    // If you want, use the WaitCommEvent() function to wait for the 
    // data Transmission to complete
    //
//    WaitCommEvent(DriverHandle, &EvtMask, NULL);

    //
    // Some devices may require clearing RTS or DTR to after transmission of data 
    //
//    EscapeCommFunction (DriverHandle, CLRRTS);
 
    return TRUE;
}

BOOL CComm32::Read_Comport(DWORD *BytesRead, DWORD Length, char *Buffer)
{
    COMSTAT     Our_Comstat;
    DWORD       Com_Errors, BytesToRead;

    //
    // Is the comport open?
    //
    if (!bComport_Open) {
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
    //       not enough data has been receive. Although this will work, it is inefficient
    //       and generally works like crap. For everyone's sake, use the technique above 
    //       and handle your own timeout in applications code.
    //

    //
    // Check the Input buffer
    //
    ::ClearCommError (hDriver_Handle, &Com_Errors, &Our_Comstat);

    //
    // ReadFile() only if there is data.
    //
    if (Our_Comstat.cbInQue > 0) {

        //
        // Make sure we do not overrun the Buffer
        //
        if (Our_Comstat.cbInQue > Length) {
            BytesToRead = Length;
        } else {
            BytesToRead = Our_Comstat.cbInQue;
        }

        //
        // Read the data, no (NULL) Overlapped I/O
        //
        ::ReadFile (hDriver_Handle, Buffer, BytesToRead, BytesRead, NULL);
        return TRUE;
    } else {
        *BytesRead = 0;
        return FALSE;
    }
}

BOOL CComm32::Set_Comport_Signals(BOOL RTS_On, BOOL DTR_On)
{
    //
    // Is the Comport open?
    //
    if (!bComport_Open) {
        return FALSE;
    }
    
    //
    // Handle the RTS signal
    //
    if (RTS_On) {
        ::EscapeCommFunction (hDriver_Handle, SETRTS);
    } else {
        ::EscapeCommFunction (hDriver_Handle, CLRRTS);
    }

    //
    // Handle the DTR signal
    //
    if (DTR_On) {
        ::EscapeCommFunction (hDriver_Handle, SETDTR);
    } else {
        ::EscapeCommFunction (hDriver_Handle, CLRDTR);
    }

    return TRUE;
}


BOOL CComm32::Get_Comport_Signals(DWORD *CTS_On, DWORD *DSR_On, DWORD *CD_On, DWORD *RI_On)
{
    DWORD ModemStat;

    //
    // Is the Comport open?
    //
    if (!bComport_Open) {
        return FALSE;
    }
    
    if (::GetCommModemStatus(hDriver_Handle, &ModemStat)) {

        if (ModemStat & MS_CTS_ON)
            *CTS_On = TRUE;
        else
            *CTS_On = FALSE;

        if (ModemStat & MS_DSR_ON)
            *DSR_On = TRUE;
        else
            *DSR_On = FALSE;

        if (ModemStat & MS_RLSD_ON)
            *CD_On = TRUE;
        else 
            *CD_On = FALSE;

        if (ModemStat & MS_RING_ON)
            *RI_On = TRUE;
        else 
            *RI_On = FALSE;
    } else {
        return FALSE;
    }

    return FALSE;
}

HANDLE CComm32::getCommHandle()
{
	return hDriver_Handle;
}

BOOL CComm32::getOpenStatus()
{
	return bComport_Open;
}
