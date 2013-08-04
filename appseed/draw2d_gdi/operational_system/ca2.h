#pragma once

#ifndef _DEBUG
#define _DEBUG
#endif

#define IMPLEMENT_VISTA_TOOLS
#define WIN32
#define DRAWDIB_INCLUDE_STRETCHDIB

#pragma pointers_to_members( full_generality, virtual_inheritance )
#include "win32_system.h"
#define _ApplicationFrameworkDLL

#include <typeinfo.h>

typedef std::type_info std_type_info;

#if defined(_M_X64)

   #define _AMD64_

#else 

   #define _X86_

#endif


#include "inttypes.h"
