#include "StdAfx.h"


void get_ca2_module_folder_dup(char * lpszModuleFolder)
{
   char lpszModuleFilePath[MAX_PATH * 8];
   HMODULE hmodule = ::GetModuleHandleA("ca2.dll");
   if(hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");
   GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));
   LPTSTR lpszModuleFileName;
   GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);
   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';
   if(strlen(lpszModuleFolder) > 0)
   {
      if(lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '\\'
      || lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '/')
      {
         lpszModuleFolder[strlen(lpszModuleFolder) - 1] = '\0';
      }
   }
}

void get_program_files_x86(std::wstring &wstr);

std::string dir::beforeca2()
{
   std::wstring wstr;
   get_program_files_x86(wstr);
   return u8(wstr.c_str());
   /*char lpszModuleFolder[MAX_PATH * 8];
   get_ca2_module_folder_dup(lpszModuleFolder);
   char * psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip x86 or x64...
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip stage
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip ca2
   // now, lpszModuleFolder should be beforeca2
   return lpszModuleFolder;*/
}

std::string dir::ca2(const char * path)
{
   if(path == NULL)
      return dir::path(beforeca2().c_str(), "ca2");
   else
      return dir::path(ca2().c_str(), path);
}

bool dir::mk(LPCTSTR lpcsz)
{
   std::string url(lpcsz);
   std::string tmp;
   std::string dir;
   int oldpos = -1;
   int pos = url.find("\\");
	while (pos >= 0)
	{
      tmp = url.substr(oldpos + 1, pos - oldpos -1 );
		dir += tmp + "\\";
      DWORD dw = GetFileAttributes(dir.c_str());
      if(dw == INVALID_FILE_ATTRIBUTES)
      {
         ::CreateDirectory(dir.c_str(), NULL);
      }
      oldpos = pos;
      pos = url.find("\\", oldpos + 1);

	}
   tmp = url.substr(oldpos + 1);
	dir += tmp + "\\";
   if(GetFileAttributes(dir.c_str()) == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory(dir.c_str(), NULL);
   }
   return true;
}

std::string dir::module_folder(const char * path1)
{
   char path[MAX_PATH * 4];
   if(!GetModuleFileName(NULL,
      path,
      sizeof(path)))
      return path1;
   else 
   {
      return dir::path(name(path).c_str(), path1);
   }
}

std::string dir::path(const char * path1, const char * path2)
{
   std::string str1(path1);
   if(str1.substr(str1.length() - 1, 1) != PATH_SEPARATOR)
   {
      str1 = str1 + PATH_SEPARATOR;
   }
   if(path2 == NULL)
   {
      return str1;
   }
   else
   {
      return str1 + path2;
   }
}

bool dir::exists(const char * path1)
{
   DWORD dwFileAttributes = ::GetFileAttributes(path1);
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return true;
   else
      return false;
}

std::string dir::name(const char * path1)
{
   std::string str;

   str = path1;
   int iPos = str.rfind('\\');
   return str.substr(0, iPos + 1);
}

