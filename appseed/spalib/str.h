#pragma once


#define _unitext(text) (std_gen_international_unicode_to_utf8(L##text).c_str())


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
SPALIB_API WCHAR * utf8_to_16(const char * psz);
SPALIB_API std::string std_gen_international_unicode_to_utf8(const wchar_t * psz);
SPALIB_API CHAR * utf16_to_8(const wchar_t * psz);
SPALIB_API std::string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
