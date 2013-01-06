#include "framework.h"


namespace win
{


   namespace file
   {


   system::system(::ca::application * papp) :
      ca(papp)
   {

   }


   system::~system()
   {

   }


   bool system::FullPath(string &str, const char * lpszFileIn)
   {

      //if(::ex1::system::FullPath(str, lpszFileIn))
        // return true;

      if(gen::str::begins_ci(lpszFileIn, "http://"))
      {

         str = lpszFileIn;

         return true;

      }
      else if(gen::str::begins_ci(lpszFileIn, "https://"))
      {

         str = lpszFileIn;

         return true;

      }
      wstring wstrFileIn;
      wstrFileIn = gen::international::utf8_to_unicode(lpszFileIn);
      wstring wstrFileOut;
      bool b = vfxFullPath(wstrFileOut.alloc(MAX_PATH * 8), wstrFileIn) != FALSE;
      if(b)
      {
         gen::international::unicode_to_utf8(str, wstrFileOut);
      }
      return b;
   }

   bool system::FullPath(wstring & wstrFullPath, const wstring & wstrPath)
   {

/*      if(::ex1::system::FullPath(wstrFullPath, wstrPath))
         return true;*/

      if(gen::str::begins_ci(wstrPath, L"http://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }
      else if(gen::str::begins_ci(wstrPath, L"https://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }

      return vfxFullPath(wstrFullPath, wstrPath) != FALSE;

   }


   UINT system::GetFileName(const char * lpszPathName, string & str)
   {
      int32_t nMax = MAX_PATH * 8;
      wstring wstrPathName;
      wstrPathName = gen::international::utf8_to_unicode(lpszPathName);
      wstring wstrTitle;
      UINT user = vfxGetFileName(wstrPathName, wstrTitle.alloc(nMax), nMax);
      str = gen::international::unicode_to_utf8(wstrTitle);
      return user;
   }

   void system::GetModuleShortFileName(HINSTANCE hInst, string & strShortName)
   {
      vfxGetModuleShortFileName(hInst, strShortName);
   }

      var system::length(const char * pszPath)
      {

         var varRet;

#ifdef WINDOWS

         WIN32_FILE_ATTRIBUTE_DATA data;

         if(!GetFileAttributesExW(gen::international::utf8_to_unicode(pszPath), GetFileExInfoStandard, &data))
         {
            varRet.set_type(var::type_null);
         }
         else
         {
            varRet = (uint32_t) data.nFileSizeLow;
         }

#else

         struct stat stat;

         if(::stat(pszPath, &stat)  == -1)
         {
            varRet.set_type(var::type_null);
         }
         else
         {
            varRet = stat.st_size;
         }

#endif

         return varRet;

      }

   }

} // namespace win



