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


/////////////////////////////////////////////////////////////////////////////
// locale-invariant comparison helpers till CRT gets that support
inline int32_t __invariant_stricmp(const char *pszLeft, const char *pszRight)
{
#ifdef WINDOWSEX
    return ::CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                            NORM_IGNORECASE,
                            pszLeft,
                            -1,
                            pszRight,
                            -1)-CSTR_EQUAL;
#else
   return stricmp_dup(pszLeft, pszRight);
#endif
}

inline int32_t __invariant_stricmp(const wchar_t *pwszLeft, const wchar_t *pwszRight)
{
#ifdef WINDOWSEX
    return ::CompareStringW(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                            NORM_IGNORECASE,
                            pwszLeft,
                            -1,
                            pwszRight,
                            -1)-CSTR_EQUAL;
#else
   return wcsicmp_dup(pwszLeft, pwszRight);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Portability abstractions

#define __set_dialog_control_id(oswindow, nID)     SetWindowLong(oswindow, GWL_ID, nID)
#define __set_dialog_control_id_(oswindow, nID)     oswindow->SetWindowLong(GWL_ID, nID)
#define __get_dialog_control_id(oswindow)         ((UINT)(WORD)::GetDlgCtrlID(oswindow))
#define __get_dialog_control_id_(oswindow)         ((UINT)(WORD)oswindow->GetDlgCtrlId())
