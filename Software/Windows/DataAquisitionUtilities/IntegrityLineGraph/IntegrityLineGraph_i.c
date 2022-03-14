

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Sep 16 09:01:19 2005
 */
/* Compiler settings for .\IntegrityLineGraph.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_IntegrityLineGraphLib,0x9CDEC8B9,0x5777,0x4029,0x9F,0x02,0xFA,0x47,0x13,0xA4,0x88,0xC8);


MIDL_DEFINE_GUID(IID, DIID__DIntegrityLineGraph,0x97EFD0FF,0x4A44,0x43C6,0xB2,0xBC,0x96,0x5B,0x3B,0xEA,0xBA,0xCA);


MIDL_DEFINE_GUID(IID, DIID__DIntegrityLineGraphEvents,0x7F5C1214,0x301A,0x4244,0x81,0xC2,0x30,0xDB,0x79,0x6A,0x38,0x6E);


MIDL_DEFINE_GUID(CLSID, CLSID_IntegrityLineGraph,0xB68EE2FB,0xA4D5,0x4580,0x93,0x1C,0x3D,0x43,0xE8,0x5D,0x72,0x4D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

