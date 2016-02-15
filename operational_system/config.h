#pragma once

#define __VLD

#ifdef __VLD
#define _CRT_SECURE_NO_WARNINGS
#include "vld.h"
#define memory_alloc malloc
#define memory_realloc realloc
#define memory_free free
#endif

#if !defined(_DLL)
#define CUBE
#endif




#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#define NOMINMAX


#ifndef _WINDOWS
#define _WINDOWS
#endif



#define SECURITY_WIN32


#define WINDOWS
#define WINDOWSEX
#ifndef WIN32
#define WIN32
#endif
#define DECL_C __cdecl

#ifndef NOCRYPT
#define NOCRYPT
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#ifndef _CRT_RAND_S
#define _CRT_RAND_S
#endif

#ifdef CUBE
#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#else
#define CLASS_DECL_EXPORT _declspec(dllexport)
#define CLASS_DECL_IMPORT _declspec(dllimport)
#endif
#define CLASS_DECL_THREAD _declspec(thread)

#define SECURITY_WIN32


#include "bit.h"


#pragma warning(disable:4250) // disable warning if inherited via dominance



#define HAVE_UINT64_T 1
#define HAVE_WINSOCK2_H 1



#ifdef cplus

#ifdef inline
#undef inline
#endif


#define inline static

#endif


#define static_inline __forceinline


#ifndef _DEBUG
#pragma warning(disable: 4101)  // unreferenced local function has been removed
#endif


/////////////////////////////////////////////////////////////////////////////
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after the _ include files

#pragma warning(disable: 4505)  // unreferenced local function has been removed
#pragma warning(disable: 4511)  // private copy constructors are good to have
#pragma warning(disable: 4512)  // private operator= are good to have
#pragma warning(disable: 4514)  // unreferenced inlines are common
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4660)  // template is already instantiated
#pragma warning(disable: 4127)  // constant expression used in macros do while(0);

#pragma warning(disable: 4275)  // deriving exported class from non-exported
#pragma warning(disable: 4251)  // using non-exported as public in exported


#ifdef ___ALL_WARNINGS
#pragma warning( push )
#endif

// warnings caerated with common core API/Windows code
#pragma warning(disable: 4134)  // message map member fxn casts
#pragma warning(disable: 4201)  // nameless unions are part of C++
#pragma warning(disable: 4320)  // structs with uuid handled as interfaces
#pragma warning(disable: 4705)  // statement has no effect in optimized code
#pragma warning(disable: 4191)  // pointer-to-function casting
// warnings caused by normal optimizations
#ifndef DEBUG
#pragma warning(disable: 4701)  // local var *may* be used without init
#pragma warning(disable: 4702)  // unreachable code caused by optimizations
#pragma warning(disable: 4791)  // loss of debugging info in release version
#pragma warning(disable: 4189)  // initialized but unused var
#pragma warning(disable: 4390)  // is_empty controlled statement
#endif
#pragma warning(disable: 4204)  // non-constant aggregate initializer
#pragma warning(disable: 4263 4264)  // base class method is hidden


#define FONT_SANS "Lucida Sans Unicode"
#define FONT_SERIF "Times New Roman"
#define FONT_MONO "Courier New"
#define FONT_SANS_EX "Geneva"
#define FONT_SERIF_EX "Palatino Linotype"
#define FONT_SANS_FX "Tahoma"
#define FONT_SERIF_FX "Georgia"

