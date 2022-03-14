

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Sep 16 09:01:13 2005
 */
/* Compiler settings for .\IntegrityBarGraph.idl:
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


#ifndef __IntegrityBarGraphidl_h__
#define __IntegrityBarGraphidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DIntegrityBarGraph_FWD_DEFINED__
#define ___DIntegrityBarGraph_FWD_DEFINED__
typedef interface _DIntegrityBarGraph _DIntegrityBarGraph;
#endif 	/* ___DIntegrityBarGraph_FWD_DEFINED__ */


#ifndef ___DIntegrityBarGraphEvents_FWD_DEFINED__
#define ___DIntegrityBarGraphEvents_FWD_DEFINED__
typedef interface _DIntegrityBarGraphEvents _DIntegrityBarGraphEvents;
#endif 	/* ___DIntegrityBarGraphEvents_FWD_DEFINED__ */


#ifndef __IntegrityBarGraph_FWD_DEFINED__
#define __IntegrityBarGraph_FWD_DEFINED__

#ifdef __cplusplus
typedef class IntegrityBarGraph IntegrityBarGraph;
#else
typedef struct IntegrityBarGraph IntegrityBarGraph;
#endif /* __cplusplus */

#endif 	/* __IntegrityBarGraph_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __IntegrityBarGraphLib_LIBRARY_DEFINED__
#define __IntegrityBarGraphLib_LIBRARY_DEFINED__

/* library IntegrityBarGraphLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_IntegrityBarGraphLib;

#ifndef ___DIntegrityBarGraph_DISPINTERFACE_DEFINED__
#define ___DIntegrityBarGraph_DISPINTERFACE_DEFINED__

/* dispinterface _DIntegrityBarGraph */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIntegrityBarGraph;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("43F8802F-69F3-428C-875A-9C1EBBBE0C85")
    _DIntegrityBarGraph : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIntegrityBarGraphVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIntegrityBarGraph * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIntegrityBarGraph * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIntegrityBarGraph * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIntegrityBarGraph * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIntegrityBarGraph * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIntegrityBarGraph * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIntegrityBarGraph * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIntegrityBarGraphVtbl;

    interface _DIntegrityBarGraph
    {
        CONST_VTBL struct _DIntegrityBarGraphVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIntegrityBarGraph_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DIntegrityBarGraph_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DIntegrityBarGraph_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DIntegrityBarGraph_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DIntegrityBarGraph_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DIntegrityBarGraph_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DIntegrityBarGraph_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIntegrityBarGraph_DISPINTERFACE_DEFINED__ */


#ifndef ___DIntegrityBarGraphEvents_DISPINTERFACE_DEFINED__
#define ___DIntegrityBarGraphEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DIntegrityBarGraphEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIntegrityBarGraphEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("45D064D8-9DE5-4A47-8472-F5FBC4FD0639")
    _DIntegrityBarGraphEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIntegrityBarGraphEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIntegrityBarGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIntegrityBarGraphEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIntegrityBarGraphEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIntegrityBarGraphEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIntegrityBarGraphEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIntegrityBarGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIntegrityBarGraphEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIntegrityBarGraphEventsVtbl;

    interface _DIntegrityBarGraphEvents
    {
        CONST_VTBL struct _DIntegrityBarGraphEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIntegrityBarGraphEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DIntegrityBarGraphEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DIntegrityBarGraphEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DIntegrityBarGraphEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DIntegrityBarGraphEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DIntegrityBarGraphEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DIntegrityBarGraphEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIntegrityBarGraphEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IntegrityBarGraph;

#ifdef __cplusplus

class DECLSPEC_UUID("6C8AF639-E23A-4041-B6B2-3303D7D0E066")
IntegrityBarGraph;
#endif
#endif /* __IntegrityBarGraphLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


