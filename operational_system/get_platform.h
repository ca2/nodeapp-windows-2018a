#pragma once


#define WINDOWS
#define DECL_C __cdecl

#define _WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE


#define _WIDEN(x) L#x

#define WIN32


#if defined(_M_X64)  // X64

#define _AMD64_

#endif


#include "inttypes.h"


#define IMPLEMENT_VISTA_TOOLS
#define DRAWDIB_INCLUDE_STRETCHDIB


#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif


#include "win32_system.h"



#define _ApplicationFrameworkDLL




#if defined(_WIN64) && defined(_M_X64)


#define _AMD64_

#else


#define _X86_


#endif



#ifndef _DEBUG
#define _DEBUG // ca2 is in debug - bugs come and go, always
#endif



#include "bare_operational_system.h"        // Target version control

