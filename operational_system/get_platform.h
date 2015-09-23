#pragma once


#ifdef _DEBUG
#define DEBUG
#endif



#define _WIDEN(x) L#x



#include "_inttypes.h"
#include "_meminttypes.h"



#include <wchar.h>



#ifdef __cplusplus
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif



#define NOTHROW throw()
#define THROWS throw(...)


#ifdef DEBUG
#define RELEASENOTHROW throw(...)
#else
#define RELEASENOTHROW throw()
#endif




#include "bare_operational_system.h" 










