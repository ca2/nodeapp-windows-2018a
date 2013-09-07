#pragma once


#undef new
#include <GdiPlus.h>
#include <Gdipluseffects.h>

#define new DEBUG_NEW

#include <wincrypt.h>


#include "win_dir.h"
#include "win_file_find.h"
#include "win_file_system.h"
#include "win_file.h"
#include "win_shell.h"
//#include "core/gen_resource.h"
#include "win_resource.h"
#include "win_stdio_file.h"
#include "win_file_set.h"

#include "win_copydesk.h"
#include "win_crypt.h"
#include "win_ip_enum.h"
#include "win_registry.h"

#include "win_os.h"


// Sanity checks for ATOMs
CLASS_DECL_win bool __is_valid_atom(ATOM nAtom);
CLASS_DECL_win bool __is_valid_atom(const char * psz);


