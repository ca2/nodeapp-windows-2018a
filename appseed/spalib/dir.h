#pragma once

#define PATH_SEPARATOR "\\"

class dir
{
public:
   static SPALIB_API std::string beforeca2();
   static SPALIB_API std::string ca2(const char * path = NULL);
   static SPALIB_API std::string afterca2();
   static SPALIB_API std::string module_folder(const char * path = NULL);
   static SPALIB_API std::string path(const char * path1, const char * path2);
   static SPALIB_API std::string name(const char * path1);
   static SPALIB_API std::string appdata(const char * lpcsz);
   static SPALIB_API bool exists(const char * path1);
   static SPALIB_API bool mk(LPCTSTR lpcsz);
};
