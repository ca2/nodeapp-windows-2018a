#pragma once

#if core_level_3
#define IMPLEMENT_VISTA_TOOLS
#endif

#include "os.h"
#include "win_internal.h"

CLASS_DECL_win void vfxThrowFileException(::ca::application * papp, int cause, LONG lOsError, const char * lpszFileName = NULL);
