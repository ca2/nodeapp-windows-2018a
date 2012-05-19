#pragma once


#undef new
#include <GdiPlus.h>
#include <Gdipluseffects.h>

#define new DEBUG_NEW

#include <wincrypt.h>


#include "dir.h"
#include "WFileFind.h"
#include "WinFileSystem.h"
#include "WinFile.h"
#include "WindowsShell.h"
//#include "ca2/ex1_resource.h"
#include "WinResource.h"
#include "application.h"
#include "WinStdioFile.h"
#include "WinFileSet.h"

#include "copydesk.h"
#include "crypt.h"
#include "ip_enum.h"
#include "registry.h"

#include "os_os.h"


// Sanity checks for ATOMs
CLASS_DECL_win bool __is_valid_atom(ATOM nAtom);
CLASS_DECL_win bool __is_valid_atom(const char * psz);
