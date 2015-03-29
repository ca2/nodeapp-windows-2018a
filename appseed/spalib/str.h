#pragma once

#include <string>

using namespace std;

#define _unitext(text) (u8(L##text).c_str())


SPALIB_API void str_trim_left(std::string & str);
SPALIB_API void str_trim_right(std::string & str);
SPALIB_API void str_trim(std::string & str);
SPALIB_API bool str_ends(const char * psz, const char * pszSuffix);
SPALIB_API bool str_ends_ci(const char * psz, const char * pszSuffix);
SPALIB_API bool str_begins_ci(const char * psz, const char * pszSuffix);
SPALIB_API std::string str_replace(const char * psz, const char * pszFind, const char * pszReplace);
SPALIB_API BSTR BSTRFromCStr(UINT codePage, LPCSTR s);



#define NULWCHAR     L'\0'
#define TABWCHAR     L'\t'
#define SLASHWCHAR   L'\\'
#define SPACEWCHAR   L' '
#define DQUOTEWCHAR  L'\"'



SPALIB_API void __cdecl wparse_cmdline (WCHAR *cmdstart, WCHAR **argv, WCHAR *args, int *numargs, int *numchars);
SPALIB_API wstring u16(const char * psz);
inline wstring u16(const string & str) { return u16(str.c_str()); }
SPALIB_API string u8(const wchar_t * psz);
inline string u8(const wstring & wstr) { return u8(wstr.c_str()); }
SPALIB_API std::string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
