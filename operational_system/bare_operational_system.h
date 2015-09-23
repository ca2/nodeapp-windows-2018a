#pragma once


#define WINDOWS


#define DECL_SPEC_ANY __declspec(selectany)


#include "version_windows.h"

// Include any non-Intel platform specific items
#ifndef X86
	#include "version_cpu.h"
#endif

#ifdef X86
	#define ___MINREBUILD
#endif

#ifdef _CUSTOM
// Put any custom configuration items in afxv_cfg.h
	#include <afxv_cfg.h>
#endif

// setup default packing value
#ifndef ___PACKING
	#define ___PACKING    4   // default packs structs at 4 bytes
#endif

#include "version_dll.h"

// Define this virtual key for use by status bar
#ifndef VK_KANA
#define VK_KANA             0x15
#endif

#define debug_break() __debugbreak()


/////////////////////////////////////////////////////////////////////////////
// Standard preprocessor symbols if not already defined
/////////////////////////////////////////////////////////////////////////////

// SIZE_T_MAX is used by the collection classes
#ifndef SIZE_T_MAX
	#define SIZE_T_MAX  UINT_MAX
#endif

// FASTCALL is used for static member functions with little or no params
#ifndef FASTCALL
	#define FASTCALL __fastcall
#endif

// CDECL and EXPORT are defined in case WINDOWS.H doesn't
#ifndef CDECL
	#define CDECL __cdecl
#endif

#ifndef EXPORT
	#define EXPORT
#endif

// UNALIGNED is used for unaligned data access (in CArchive mostly)
#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

// __DEPRECATED is used for functions that should no longer be used
#ifndef __DEPRECATED
#ifdef ___DISABLE_DEPRECATED
	#define __DEPRECATED(_Message)
#else
	#define __DEPRECATED(_Message) __declspec(deprecated(_Message))
#endif
#endif

// ___INSECURE_DEPRECATE is used for deprecated, insecure functions.
#ifndef ___INSECURE_DEPRECATE
#ifdef ___SECURE_NO_DEPRECATE
#define ___INSECURE_DEPRECATE(_Message)
#else
#define ___INSECURE_DEPRECATE(_Message) __declspec(deprecated(_Message))
#endif // ___SECURE_NO_DEPRECATE
#endif // ___INSECURE_DEPRECATE

// AFXAPI is used on global public functions
#ifndef AFXAPI
	#define AFXAPI __stdcall
#endif

// AFXOLEAPI is used for some special OLE functions
#ifndef AFXOLEAPI
	#define AFXOLEAPI __stdcall
#endif

// c_cdecl is used for rare functions taking variable arguments
#ifndef c_cdecl
	#define c_cdecl __cdecl
#endif

// __EXPORT is used for functions which need to be exported
#ifndef __EXPORT
	#define __EXPORT EXPORT
#endif

#ifndef __STATIC
	#define __STATIC extern
	#define __STATIC_DATA extern __declspec(selectany)
#endif

// The following macros are used to enable export/import

// for data
#ifndef __DATA_EXPORT
	#define __DATA_EXPORT __declspec(dllexport)
#endif
#ifndef __DATA_IMPORT
	#define __DATA_IMPORT __declspec(dllimport)
#endif

// for classes
#ifndef __CLASS_EXPORT
	#define __CLASS_EXPORT __declspec(dllexport)
#endif
#ifndef __CLASS_IMPORT
	#define __CLASS_IMPORT __declspec(dllimport)
#endif

// for global APIs
#ifndef __API_EXPORT
	#define __API_EXPORT __declspec(dllexport)
#endif
#ifndef __API_IMPORT
	#define __API_IMPORT __declspec(dllimport)
#endif

// This macro is used to reduce size requirements of some classes
#ifndef __ALWAYS_VTABLE
#ifndef __NOVTABLE
#if _MSC_VER >= 1100 && !defined(DEBUG)
#define __NOVTABLE __declspec(novtable)
#else
#define __NOVTABLE
#endif
#endif
#endif

// for global data that should be in COMDATs (packaged data)
#ifndef __COMDAT
#define __COMDAT __declspec(selectany)
#endif

// The following macros are used on data declarations/definitions
//  (they are redefined for extension DLLs and the shared MFC DLL)
#define __DATADEF
#define __API CLASS_DECL_BASE

// used when building the "core.dll" MFC80.DLL
#ifndef __CORE_DATA
	#define __CORE_DATA CLASS_DECL_BASE
	#define __CORE_DATADEF
#endif

// used when building the MFC/OLE support MFCO80.DLL
#ifndef __OLE_DATA
	#define __OLE_DATA
	#define __OLE_DATADEF
#endif

// used when building the MFC/DB support MFCD80.DLL
#ifndef __DB_DATA
	#define __DB_DATA
	#define __DB_DATADEF
#endif

// used when building the MFC/NET support MFCN80.DLL
#ifndef __NET_DATA
	#define __NET_DATA
	#define __NET_DATADEF
#endif

// used when building extension DLLs
#ifndef __EXT_DATA
	#define __EXT_DATA
	#define __EXT_DATADEF
	#define __EXT_CLASS
	#define __EXT_API
#endif

// BASED_XXXX macros are provided for backward compatibility
#ifndef BASED_CODE
	#define BASED_CODE
#endif

#ifndef BASED_DEBUG
	#define BASED_DEBUG
#endif

#ifndef BASED_STACK
	#define BASED_STACK
#endif





#include "version_cpu.h"






// core API inline constructors (including compiler generated) can get deep
#pragma inline_depth(16)

#ifdef ___DEVBUILD
	#define __IMPL_DATA __DATA_EXPORT
#else
	#define __IMPL_DATA CLASS_DECL_BASE 
#endif


// #define ___NOFORCE_LIBS
#define ___FULLTYPEINFO
#define VC_EXTRALEAN
#define NO_ANSIUNI_ONLY
#define _MFC_OVERRIDES_NEW

#define __COMDAT __declspec(selectany)

#define MAX_DWORD_PTR ((uint_ptr)(-1))



#define DECLSPEC_NO_RETURN __declspec(noreturn)



#include "cross/cross.h"







