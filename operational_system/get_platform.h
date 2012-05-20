#pragma once


#ifdef _DEBUG
#define DEBUG
#endif


#define WINDOWS
#define WIN32
#define DECL_C __cdecl

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_RAND_S


#define _WIDEN(x) L#x



#if defined(_M_X64)  // X64

#define AMD64

#elif defined(_M_IX86)

#define X86

#endif


#include "inttypes.h"


#define IMPLEMENT_VISTA_TOOLS
#define DRAWDIB_INCLUDE_STRETCHDIB


#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif


#define _BIND_TO_CURRENT_CRT_VERSION 1
#define _CRT_NOFORCE_MANIFEST
#define _STL_NOFORCE_MANIFEST


BEGIN_EXTERN_C


__declspec(selectany)  int _forceCRTManifest;


END_EXTERN_C


#define NOTHROW throw()
#define THROWS throw(...)

#ifdef DEBUG
#define RELEASENOTHROW throw(...)
#else
#define RELEASENOTHROW throw()
#endif


#define WINVER 0x0600
#define _WIN32_WINNT   0x0600
#define _WIN32_IE 0x0600












#include "bare_operational_system.h"        // Target version control

