

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __IntegrityLineGraphidl_h__
#define __IntegrityLineGraphidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DIntegrityLineGraph_FWD_DEFINED__
#define ___DIntegrityLineGraph_FWD_DEFINED__
typedef interface _DIntegrityLineGraph _DIntegrityLineGraph;
#endif 	/* ___DIntegrityLineGraph_FWD_DEFINED__ */


#ifndef ___DIntegrityLineGraphEvents_FWD_DEFINED__
#define ___DIntegrityLineGraphEvents_FWD_DEFINED__
typedef interface _DIntegrityLineGraphEvents _DIntegrityLineGraphEvents;
#endif 	/* ___DIntegrityLineGraphEvents_FWD_DEFINED__ */


#ifndef __IntegrityLineGraph_FWD_DEFINED__
#define __IntegrityLineGraph_FWD_DEFINED__

#ifdef __cplusplus
typedef class IntegrityLineGraph IntegrityLineGraph;
#else
typedef struct IntegrityLineGraph IntegrityLineGraph;
#endif /* __cplusplus */

#endif 	/* __IntegrityLineGraph_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __IntegrityLineGraphLib_LIBRARY_DEFINED__
#define __IntegrityLineGraphLib_LIBRARY_DEFINED__

/* library IntegrityLineGraphLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_IntegrityLineGraphLib;

#ifndef ___DIntegrityLineGraph_DISPINTERFACE_DEFINED__
#define ___DIntegrityLineGraph_DISPINTERFACE_DEFINED__

/* dispinterface _DIntegrityLineGraph */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIntegrityLineGraph;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("97EFD0FF-4A44-43C6-B2BC-965B3BEABACA")
    _DIntegrityLineGraph : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIntegrityLineGraphVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIntegrityLineGraph * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIntegrityLineGraph * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIntegrityLineGraph * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIntegrityLineGraph * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIntegrityLineGraph * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIntegrityLineGraph * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIntegrityLineGraph * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIntegrityLineGraphVtbl;

    interface _DIntegrityLineGraph
    {
        CONST_VTBL struct _DIntegrityLineGraphVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIntegrityLineGraph_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DIntegrityLineGraph_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DIntegrityLineGraph_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DIntegrityLineGraph_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DIntegrityLineGraph_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DIntegrityLineGraph_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DIntegrityLineGraph_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIntegrityLineGraph_DISPINTERFACE_DEFINED__ */


#ifndef ___DIntegrityLineGraphEvents_DISPINTERFACE_DEFINED__
#define ___DIntegrityLineGraphEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DIntegrityLineGraphEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIntegrityLineGraphEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7F5C1214-301A-4244-81C2-30DB796A386E")
    _DIntegrityLineGraphEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIntegrityLineGraphEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIntegrityLineGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIntegrityLineGraphEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIntegrityLineGraphEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIntegrityLineGraphEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIntegrityLineGraphEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIntegrityLineGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIntegrityLineGraphEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIntegrityLineGraphEventsVtbl;

    interface _DIntegrityLineGraphEvents
    {
        CONST_VTBL struct _DIntegrityLineGraphEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIntegrityLineGraphEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DIntegrityLineGraphEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DIntegrityLineGraphEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DIntegrityLineGraphEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DIntegrityLineGraphEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DIntegrityLineGraphEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DIntegrityLineGraphEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIntegrityLineGraphEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IntegrityLineGraph;

#ifdef __cplusplus

class DECLSPEC_UUID("B68EE2FB-A4D5-4580-931C-3D43E85D724D")
IntegrityLineGraph;
#endif
#endif /* __IntegrityLineGraphLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


