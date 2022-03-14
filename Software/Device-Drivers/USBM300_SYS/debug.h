
//---------------------------------------------------------------------------
//
// DEBUG.H
//
// Author:  Anthony A. Kempka
//          Device Drivers International, LLC
//
//          Tel: 208-762-4746
//          Tel: 513-984-4491
//          Web: www.ddiusa.com
//
//---------------------------------------------------------------------------
// 
//
// Last Modified:       
//      A. A. Kempka    04/09/2001      Original. 
//  
//---------------------------------------------------------------------------
//
// Description:
// ------------
//
//
// Distribution Notice:
// --------------------
// Internal use only!!!!
//
//
//---------------------------------------------------------------------------


#define DBG_ERROR   0
#define DBG_WARN    1
#define DBG_INFO    2
#define DBG_VERBOSE 3

#define OUR_BUFFER_LENGTH 128

#if DBG

VOID
ThisDebugPrint(
    ULONG DebugPrintLevel,
    PCCHAR DebugMessage,
    ...
    );

#define OurDebugPrint(a) ThisDebugPrint a

PCHAR MajorIrpString (UCHAR MajorFunction);
PCHAR PnPMinorFunctionString (UCHAR MinorFunction);
PCHAR PowerMinorFunctionString (UCHAR MinorFunction);
PCHAR DeviceRelationString (ULONG DeviceRelationType);

#else // DBG

#define OurDebugPrint(a)

#endif // DBG



