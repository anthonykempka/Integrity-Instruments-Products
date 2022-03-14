

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Sep 23 09:08:05 2005
 */
/* Compiler settings for .\DAMUtils.idl:
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


#ifndef __DAMUtils_h_h__
#define __DAMUtils_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDAMUtils_FWD_DEFINED__
#define __IDAMUtils_FWD_DEFINED__
typedef interface IDAMUtils IDAMUtils;
#endif 	/* __IDAMUtils_FWD_DEFINED__ */


#ifndef __CDAMUtilsDoc_FWD_DEFINED__
#define __CDAMUtilsDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CDAMUtilsDoc CDAMUtilsDoc;
#else
typedef struct CDAMUtilsDoc CDAMUtilsDoc;
#endif /* __cplusplus */

#endif 	/* __CDAMUtilsDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __DAMUtils_LIBRARY_DEFINED__
#define __DAMUtils_LIBRARY_DEFINED__

/* library DAMUtils */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DAMUtils;

#ifndef __IDAMUtils_DISPINTERFACE_DEFINED__
#define __IDAMUtils_DISPINTERFACE_DEFINED__

/* dispinterface IDAMUtils */
/* [uuid] */ 


EXTERN_C const IID DIID_IDAMUtils;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5722A921-D07C-4805-A2E2-1AA6FC4F8ACF")
    IDAMUtils : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDAMUtilsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDAMUtils * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDAMUtils * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDAMUtils * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDAMUtils * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDAMUtils * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDAMUtils * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDAMUtils * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDAMUtilsVtbl;

    interface IDAMUtils
    {
        CONST_VTBL struct IDAMUtilsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDAMUtils_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDAMUtils_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDAMUtils_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDAMUtils_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDAMUtils_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDAMUtils_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDAMUtils_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDAMUtils_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CDAMUtilsDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("85BA914E-1A63-435B-A61A-34FE6BBFB474")
CDAMUtilsDoc;
#endif
#endif /* __DAMUtils_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


