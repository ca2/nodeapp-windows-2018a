#pragma once

//#define __VLD

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#ifdef __VLD
#include "vld.h"
#define memory_alloc malloc
#define memory_realloc realloc
#define memory_free free
#endif

