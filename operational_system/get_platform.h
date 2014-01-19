#pragma once




#ifdef _DEBUG
#define DEBUG
#endif

#define NO_VARIADIC_TEMPLATE



#define _WIDEN(x) L#x






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


__declspec(selectany)  int32_t _forceCRTManifest;


END_EXTERN_C


#define NOTHROW throw()
#define THROWS throw(...)

#ifdef DEBUG
#define RELEASENOTHROW throw(...)
#else
#define RELEASENOTHROW throw()
#endif



#define _WIN32_WINNT    0x0601 // _WIN32_WINNT_WIN7
#define _WIN32_IE       0x0800 // _WIN32_IE_IE80
#define WINVER          _WIN32_WINNT












#include "bare_operational_system.h"        // Target version control



