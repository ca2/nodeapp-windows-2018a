#pragma once


#define WINDOWS
#define WINDOWSEX
#define WIN32
#define DECL_C __cdecl


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_RAND_S


#define CLASS_DECL_EXPORT _declspec(dllexport)
#define CLASS_DECL_IMPORT _declspec(dllimport)
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





