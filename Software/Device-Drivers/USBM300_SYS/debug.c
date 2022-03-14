//---------------------------------------------------------------------------
//
// DEBUG.C
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
//      A. A. Kempka    04/09/2001      Original. Moved from RAIDCTRL.C
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

//---------------------------------------------------------------------------
//
// Include files 
//
#include "stdio.h"
#include "wdm.h"

//
// Enable/Disable debug print functions
//
#if DBG

#include "stdarg.h"

#define DBG_ERROR   0
#define DBG_WARN    1
#define DBG_INFO    2
#define DBG_VERBOSE 3

#define OUR_BUFFER_LENGTH 256
UCHAR OurPrintBuffer[OUR_BUFFER_LENGTH + 1] = {0};

//
// Current Debug Level
//
ULONG       cdl = DBG_WARN;
KSPIN_LOCK  DbgPrintLock;
BOOLEAN     DbgLockInitialized = FALSE;

VOID
ThisDebugPrint(
    ULONG DebugPrintLevel,
    PCCHAR DebugMessage,
    ...
    )

{
    KIRQL   OldIrql;
    va_list ap;
    va_start(ap, DebugMessage);

    if (!DbgLockInitialized) {
        DbgLockInitialized = TRUE;
        KeInitializeSpinLock(&DbgPrintLock);
    }

    KeAcquireSpinLock(&DbgPrintLock, &OldIrql);

    if (DebugPrintLevel <= cdl) {
        _vsnprintf(OurPrintBuffer, OUR_BUFFER_LENGTH, DebugMessage, ap);
        DbgPrint("USBM300: ");
        DbgPrint(OurPrintBuffer);
    }

    KeReleaseSpinLock(&DbgPrintLock, OldIrql);

    va_end(ap);
    return;
} 

#define OurDebugPrint(a) ThisDebugPrint a


PCHAR MajorIrpString (UCHAR MajorFunction)
{
    switch (MajorFunction)
    {
        case IRP_MJ_CREATE:
            return "IRP_MJ_CREATE";
        case IRP_MJ_CREATE_NAMED_PIPE:        
            return "IRP_MJ_CREATE_NAMED_PIPE";
        case IRP_MJ_CLOSE:                    
            return "IRP_MJ_CLOSE";
        case IRP_MJ_READ:                     
            return "IRP_MJ_READ";
        case IRP_MJ_WRITE:                    
            return "IRP_MJ_WRITE";
        case IRP_MJ_QUERY_INFORMATION:        
            return "IRP_MJ_QUERY_INFORMATION";
        case IRP_MJ_SET_INFORMATION:          
            return "IRP_MJ_SET_INFORMATION";
        case IRP_MJ_QUERY_EA:                 
            return "IRP_MJ_QUERY_EA";
        case IRP_MJ_SET_EA:                   
            return "XXIRP_MJ_SET_EAXXX";
        case IRP_MJ_FLUSH_BUFFERS:            
            return "IRP_MJ_FLUSH_BUFFERS";
        case IRP_MJ_QUERY_VOLUME_INFORMATION: 
            return "IRP_MJ_QUERY_VOLUME_INFORMATION";
        case IRP_MJ_SET_VOLUME_INFORMATION:   
            return "IRP_MJ_SET_VOLUME_INFORMATION";
        case IRP_MJ_DIRECTORY_CONTROL:        
            return "IRP_MJ_DIRECTORY_CONTROL";
        case IRP_MJ_FILE_SYSTEM_CONTROL:      
            return "IRP_MJ_FILE_SYSTEM_CONTROL";
        case IRP_MJ_DEVICE_CONTROL:           
            return "IRP_MJ_DEVICE_CONTROL";
        case IRP_MJ_INTERNAL_DEVICE_CONTROL:  
            return "IRP_MJ_INTERNAL_DEVICE_CONTROL";
        case IRP_MJ_SHUTDOWN:                 
            return "IRP_MJ_SHUTDOWN";
        case IRP_MJ_LOCK_CONTROL:             
            return "IRP_MJ_LOCK_CONTROL";
        case IRP_MJ_CLEANUP:                  
            return "IRP_MJ_LOCK_CONTROL";
        case IRP_MJ_CREATE_MAILSLOT:          
            return "IRP_MJ_LOCK_CONTROL";
        case IRP_MJ_QUERY_SECURITY:           
            return "IRP_MJ_QUERY_SECURITY";
        case IRP_MJ_SET_SECURITY:             
            return "IRP_MJ_SET_SECURITY";
        case IRP_MJ_POWER:                    
            return "IRP_MJ_POWER";
        case IRP_MJ_SYSTEM_CONTROL:           
            return "IRP_MJ_SYSTEM_CONTROL";
        case IRP_MJ_DEVICE_CHANGE:            
            return "IRP_MJ_DEVICE_CHANGE";
        case IRP_MJ_QUERY_QUOTA:              
            return "IRP_MJ_QUERY_QUOTA";
        case IRP_MJ_SET_QUOTA:                
            return "IRP_MJ_SET_QUOTA";
        case IRP_MJ_PNP:                      
            return "IRP_MJ_PNP";

        default:
            return "IRP_MJ_?????";
    }
}


PCHAR PnPMinorFunctionString (UCHAR MinorFunction)
{
    switch (MinorFunction)
    {
        case IRP_MN_START_DEVICE:
            return "IRP_MN_START_DEVICE";
        case IRP_MN_QUERY_REMOVE_DEVICE:
            return "IRP_MN_QUERY_REMOVE_DEVICE";
        case IRP_MN_REMOVE_DEVICE:
            return "IRP_MN_REMOVE_DEVICE";
        case IRP_MN_CANCEL_REMOVE_DEVICE:
            return "IRP_MN_CANCEL_REMOVE_DEVICE";
        case IRP_MN_STOP_DEVICE:
            return "IRP_MN_STOP_DEVICE";
        case IRP_MN_QUERY_STOP_DEVICE:
            return "IRP_MN_QUERY_STOP_DEVICE";
        case IRP_MN_CANCEL_STOP_DEVICE:
            return "IRP_MN_CANCEL_STOP_DEVICE";
        case IRP_MN_QUERY_DEVICE_RELATIONS:
            return "IRP_MN_QUERY_DEVICE_RELATIONS";
        case IRP_MN_QUERY_INTERFACE:
            return "IRP_MN_QUERY_INTERFACE";
        case IRP_MN_QUERY_CAPABILITIES:
            return "IRP_MN_QUERY_CAPABILITIES";
        case IRP_MN_QUERY_RESOURCES:
            return "IRP_MN_QUERY_RESOURCES";
        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:
            return "IRP_MN_QUERY_RESOURCE_REQUIREMENTS";
        case IRP_MN_QUERY_DEVICE_TEXT:
            return "IRP_MN_QUERY_DEVICE_TEXT";
        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS:
            return "IRP_MN_FILTER_RESOURCE_REQUIREMENTS";
        case IRP_MN_READ_CONFIG:
            return "IRP_MN_READ_CONFIG";
        case IRP_MN_WRITE_CONFIG:
            return "IRP_MN_WRITE_CONFIG";
        case IRP_MN_EJECT:
            return "IRP_MN_EJECT";
        case IRP_MN_SET_LOCK:
            return "IRP_MN_SET_LOCK";
        case IRP_MN_QUERY_ID:
            return "IRP_MN_QUERY_ID";
        case IRP_MN_QUERY_PNP_DEVICE_STATE:
            return "IRP_MN_QUERY_PNP_DEVICE_STATE";
        case IRP_MN_QUERY_BUS_INFORMATION:
            return "IRP_MN_QUERY_BUS_INFORMATION";
        case IRP_MN_DEVICE_USAGE_NOTIFICATION:
            return "IRP_MN_DEVICE_USAGE_NOTIFICATION";
        case IRP_MN_SURPRISE_REMOVAL:
            return "IRP_MN_SURPRISE_REMOVAL";
#ifndef _WDMDDK_
        case IRP_MN_QUERY_LEGACY_BUS_INFORMATION:
           return "IRP_MN_QUERY_LEGACY_BUS_INFORMATION";
#endif
        default:
            return "IRP_MN_?????";
    }
}


PCHAR PowerMinorFunctionString (UCHAR MinorFunction)
{
    switch (MinorFunction)
    {
        case IRP_MN_WAIT_WAKE:
            return "IRP_MN_WAIT_WAKE";
        case IRP_MN_POWER_SEQUENCE:
            return "IRP_MN_POWER_SEQUENCE";
        case IRP_MN_SET_POWER:
            return "IRP_MN_SET_POWER";
        case IRP_MN_QUERY_POWER:
            return "IRP_MN_QUERY_POWER";
        default:
            return "IRP_MN_?????";
    }
}

PCHAR DeviceRelationString (ULONG DeviceRelationType)
{
    switch (DeviceRelationType)
    {
        case BusRelations:
            return "BusRelations";
        case EjectionRelations:
            return "EjectionRelations";
        case PowerRelations:
            return "PowerRelations";
        case RemovalRelations:
            return "RemovalRelations";
        case TargetDeviceRelation:
            return "TargetDeviceRelation";
        default:
            return "UNKNOWN Device Relation Type";
    }
}

#else
#define OurDebugPrint(a)
#endif


