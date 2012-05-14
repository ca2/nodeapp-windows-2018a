// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// special header for _ApplicationFrameworkDLL variant.

// default __XXX_DATA and __XXX_DATADEF macros for using MFC DLLs

#ifdef _C__DLL
	#define __CORE_DATA       CLASS_DECL_ca
	#define __CORE_DATADEF
#elif __CORE_DATA
	#define __CORE_DATA       __DATA_IMPORT
	#define __CORE_DATADEF
#endif

#ifndef __OLE_DATA
	#define __OLE_DATA        __DATA_IMPORT
	#define __OLE_DATADEF
#endif

#ifndef __DB_DATA
	#define __DB_DATA         __DATA_IMPORT
	#define __DB_DATADEF
#endif

#ifndef __NET_DATA
	#define __NET_DATA        __DATA_IMPORT
	#define __NET_DATADEF
#endif

// default __EXT_DATA and __EXT_DATADEF macros for using or
//  creating MFC extension DLLs, depending on ___EXT_IMPL
// __EXT_CLASS can be used to import or export entire classes
//  in an extension DLL without the hassle of creating a .DEF file
//  with decorated names.

#ifndef __EXT_DATA
	#ifdef _AFXEXT
		#define __EXT_CLASS       __CLASS_EXPORT
		#define __EXT_API         __API_EXPORT
		#define __EXT_DATA        __DATA_EXPORT
		#define __EXT_DATADEF
	#else
		#define __EXT_CLASS       __CLASS_IMPORT
		#define __EXT_API         __API_IMPORT
		#define __EXT_DATA        __DATA_IMPORT
		#define __EXT_DATADEF
	#endif
#endif
