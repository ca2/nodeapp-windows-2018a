#pragma once

#define SECURITY_WIN32


#define WINDOWS
#define WINDOWSEX
#ifndef WIN32
#define WIN32
#endif
#define DECL_C __cdecl

#define NOCRYPT

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
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

#if defined(_M_X64)  // X64


#define AMD64
#define OS64BIT
#define OSBIT 64


#elif defined(_M_IX86)


#define X86
#define OS32BIT
#define OSBIT 32


#endif


#pragma warning(disable:4250) // disable warning if inherited via dominance



#define HAVE_UINT64_T 1
#define HAVE_WINSOCK2_H 1



#ifdef cplus

#ifdef inline
#undef inline
#endif


#define inline static

#endif