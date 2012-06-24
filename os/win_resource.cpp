#include "framework.h"


WinResource::WinResource(::ca::application * papp) :
   ca(papp)
{
}

WinResource::~WinResource()
{

}


/*bool WinResource::ReadResource(
   ex1::filesp & file, 
   UINT nID, 
   const char * lpcszType)
{

   HINSTANCE hinst = ::gen::FindResourceHandle(MAKEINTRESOURCE(nID), lpcszType);
   if(hinst == NULL)
      return false;
   return ReadResource(hinst, file, nID, lpcszType);
}*/

/*bool WinResource::ReadResource(
   HINSTANCE hinst,
   ex1::filesp & file, 
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
//           ex1::filesp f(lpcszFilePath, ::ex1::file::mode_create | ::ex1::file::mode_write );

           // write the ::fontopus::user-defined resource to the .mdb file
           spfile->write(lpnRes, dwResSize);
           spfile->Flush();
        }
        catch(ex1::file_exception_sp * pe)
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
   gen::memory_file file(get_app());
   if(!ReadResource(file, hinst, nID, lpcszType))
      return false;
   file.allocate_add_up(1);
   ((char * )file.get_data())[file.get_length() -1 ] = '\0';
   str = ((char * )file.get_data());
   return true;
}  



bool WinResource::ReadResource(ex1::file & spfile, HINSTANCE hinst, UINT nID,  const char * lpcszType)
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
           spfile.Flush();
        }
        catch(base_exception &)
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

//} //namespace gen


