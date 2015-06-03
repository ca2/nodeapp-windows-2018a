#pragma once

#define PATH_SEPARATOR "\\"

class dir
{
public:
   static SPALIB_API string beforeca2();
   static SPALIB_API string ca2(const char * path = NULL);
   static SPALIB_API string afterca2();
   static SPALIB_API string module_folder(const char * path = NULL);
   static SPALIB_API string path(const char * path1, const char * path2);
   static SPALIB_API string name(const char * path1);
   inline static string name(const wstring & wstr){ return name(u8(wstr).c_str()); }
   inline static string name(const wchar_t * pwsz) { return name(u8(pwsz).c_str()); }
   inline static string name(const string & str){ return name(str.c_str()); }

   static SPALIB_API string appdata(const char * lpcsz);
   static SPALIB_API bool exists(const char * path1);
   static SPALIB_API bool mk(LPCTSTR lpcsz);
};
