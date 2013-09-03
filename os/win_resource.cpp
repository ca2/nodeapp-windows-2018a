#include "framework.h"


WinResource::WinResource(sp(base_application) papp) :
   element(papp)
{
}

WinResource::~WinResource()
{

}


/*bool WinResource::ReadResource(
   ::ca2::filesp & file, 
   UINT nID, 
   const char * lpcszType)
{

   HINSTANCE hinst = ::ca2::FindResourceHandle(MAKEINTRESOURCE(nID), lpcszType);
   if(hinst == NULL)
      return false;
   return ReadResource(hinst, file, nID, lpcszType);
}*/

/*bool WinResource::ReadResource(
   HINSTANCE hinst,
   ::ca2::filesp & file, 
   UINT nID, 
   const char * lpcszType)
{

   HRSRC hrsrc = ::FindResource(
      hinst,
      MAKEINTRESOURCE(nID), 
      lpcszType);
   if(hrsrc == NULL)
      return false;
    HGLOBAL hres = ::LoadResource(hinst, hrsrc);
   if(hres == NULL)
      return false;
    DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

   if(hres != NULL)
    {
      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
        try
        {
           // create the .mdb file
//           ::ca2::filesp f(lpcszFilePath, ::file::file::mode_create | ::file::file::mode_write );

           // write the ::fontopus::user-defined resource to the .mdb file
           spfile->write(lpnRes, dwResSize);
           spfile->Flush();
        }
        catch(::ca2::file_exception_sp * pe)
        {
      #ifdef DEBUG
         g_dumpcontext << "File could not be opened " << e->m_cause << "\n";
      #endif
        }
        

      #ifndef WIN32 //unlock WinResource is obsolete in the Win32 API
         ::UnlockResource(hres);
      #endif
            ::FreeResource(hres);
   }
   return true;

}
*/


bool WinResource::ReadResource(string & str, HINSTANCE hinst,  UINT nID, const char * lpcszType)
{
   ::primitive::memory_file file(get_app());
   if(!ReadResource(file, hinst, nID, lpcszType))
      return false;
   file.allocate_add_up(1);
   ((char * )file.get_data())[file.get_length() -1 ] = '\0';
   str = ((char * )file.get_data());
   return true;
}  



bool WinResource::ReadResource(::file::file & spfile, HINSTANCE hinst, UINT nID,  const char * lpcszType)
{

   HRSRC hrsrc = ::FindResource(
      hinst,
      MAKEINTRESOURCE(nID), 
      lpcszType);
   if(hrsrc == NULL)
      return false;
    HGLOBAL hres = ::LoadResource(hinst, hrsrc);
   if(hres == NULL)
      return false;
    DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

   if(hres != NULL)
    {
      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
        try
        {
           // create the .mdb file
//           File f(lpcszFilePath, File::mode_create | File::mode_write );

           // write the ::fontopus::user-defined resource to the .mdb file
           spfile.write(lpnRes, dwResSize);
           spfile.flush();
        }
        catch(::exception::base &)
        {
      #ifdef DEBUG
//         g_dumpcontext << "File could not be opened " << pe->m_cause << "\n";
      #endif
        }
        

      #ifndef WIN32 //unlock WinResource is obsolete in the Win32 API
         ::UnlockResource(hres);
      #endif
            ::FreeResource(hres);
   }
   return true;

}

//} //namespace ca2


