#include "framework.h"
#include "aura/os/windows/windows_app.inl"
#include <Shlobj.h>
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <string.h>

#include <psapi.h>

#define WINDOWS
#include <tlhelp32.h>

class removal :
    public ::aura::app
{
public:

   
   HANDLE                     m_hmutex_app_removal;
   
   char *                     m_modpath;
   char *                     m_pszDllEnds;
   UINT *                     m_dwaProcess;
   INT                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   INT                        m_iSizeModule;
   bool                       m_bInstallerInstalling;
   HINSTANCE                  m_hinstance;

   removal();

   virtual ~removal();
   
   bool is_user_using(const char * pszDll);

   INT cube_run(const char * id);

   bool are_there_user_files_in_use();

   bool is_user_using(UINT processid, const char * pszDll);

   virtual int32_t run();

   virtual bool finalize();


   virtual void system(const char * pszCmd);

   virtual ::file::path get_known_folder_dir(const KNOWNFOLDERID & rfid,const char * lpcsz);


   virtual void rmdir(const char * pszDir);

   virtual void rmdir_n_v(const char * pszDir);

   virtual void g_n_rmdir_n_v(const KNOWNFOLDERID & rfid,const char * pszDir);

};


//// if MSVC CRT is used
//extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int32_t nCmdShow)
//{
//
//   UNREFERENCED_PARAMETER(lpCmdLine);
//
//   if(!defer_aura_init())
//   {
//
//      return -1;
//
//   }
//
//   removal * premoval = new removal();
//
//   int iRet = ::app_main(premoval, hInstance,hPrevInstance,lpCmdLine,nCmdShow);
//
//   defer_aura_term();
//
//   return iRet;
//
//
//
//}


::aura::application * get_acid_app(::aura::application * pappParent)
{
   
   return new removal();

}


removal::removal() 
{
   m_hinstance             = ::GetModuleHandleA(NULL);
   m_hmutex_app_removal  = NULL;
   m_modpath               = NULL;
   m_pszDllEnds            = NULL;
   m_dwaProcess            = NULL;
   m_iSizeProcess          = 0;
   m_hmodulea              = NULL;
   m_iSizeModule           = 0;
   m_bInstallerInstalling  = false;
   //g_pstraTrace            = NULL;
}

removal::~removal()
{
}


// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszPathLink - Address of a buffer that contains the path where the 
//                Shell link is to be stored, including the file name.
// lpszDesc     - Address of a buffer that contains a description of the 
//                Shell link, stored in the Comment field of the link
//                properties.


HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszIconPath = NULL, INT iIcon = 0) 
{ 
    HRESULT hres; 
    IShellLinkW* psl; 
 
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // set the path to the shortcut target and add the description. 
        psl->SetPath(lpszPathObj); 
        psl->SetDescription(lpszDesc); 
        if(lpszIconPath!= NULL)
        {
        psl->SetIconLocation(lpszIconPath, iIcon);
        }
 
        // Query IShellLink for the IPersistFile interface, used for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
        if (SUCCEEDED(hres)) 
        { 
            //WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
//            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
            // Add code here to check return value from MultiByteWideChar 
            // for success.
 
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(lpszPathLink, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 
    return hres; 

}



::file::path removal::get_known_folder_dir(const KNOWNFOLDERID & rfid,const char * lpcsz)
{

   wchar_t * buf = NULL;
   
   SHGetKnownFolderPath(rfid, 0, NULL, &buf);

   ::file::path str(buf);

   CoTaskMemFree(buf);

   return str /lpcsz;

}




void removal::system(const char * pszCmd)
{

   System.process().synch(pszCmd);


/*STARTUPINFO si;
PROCESS_INFORMATION pi;

ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));

si.dwFlags |= STARTF_USESHOWWINDOW;
si.wShowWindow = SW_HIDE;

if (CreateProcess(NULL, (char *) pszCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
{
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}*/

}


void removal::rmdir(const char * pszDir)
{

   system(("cmd.exe /C rmdir /S /Q \"" + string(pszDir) + "\"").c_str());

}


void removal::rmdir_n_v(const char * pszDir)
{

   string str(pszDir);
   rmdir(str.c_str());

   str.replace(":", "");

   string str2 = get_known_folder_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Temporary Internet Files\\Virtualized")/  str;
   rmdir(str2.c_str());

  
   str2 = get_known_folder_dir(FOLDERID_LocalAppData,"Microsoft\\Windows\\INetCache\\Virtualized") / str;
   rmdir(str2.c_str());

}


void removal::g_n_rmdir_n_v(const KNOWNFOLDERID & rfid, const char * pszDir)
{

   string strDir = get_known_folder_dir(rfid,pszDir);

   rmdir_n_v(strDir.c_str());

}




int32_t removal::run()
{


   m_hmutex_app_removal = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::ccca2_spa_app_removal::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      ::MessageBox(NULL, "ca2 app.removal.exe is already running.\n\nPlease wait for app.removal to finish or close it - using Task Manager - Ctrl+Shift+ESC - to continue.", "app_app_admin.exe is running!", MB_ICONEXCLAMATION);
      return false;
   }

   char szFile[MAX_PATH * 8];

   ::GetModuleFileName(NULL, szFile, sizeof(szFile));

   ::file::path strTargetDir = get_known_folder_dir(FOLDERID_ProgramFilesX86,"ca2.app.removal");

   Application.dir().mk(strTargetDir.c_str());

   string strTarget = strTargetDir / "app.removal.exe";

   if(::CopyFile(szFile,strTarget.c_str(),TRUE))
   {
      INT i = ::MessageBox(NULL,"Do you want to place a shortcut to ca2 app.removal in Desktop?\n\nProgram has already been copied to Program Files Folder.","app.removal installation",MB_ICONQUESTION | MB_YESNOCANCEL);

      if(i == IDCANCEL)
         return false;

      if(i == IDYES)
      {
         string strLink= get_known_folder_dir(FOLDERID_Desktop,"ca2 app.removal Tool.lnk");
         wstring wstrTarget(strTarget);
         wstring wstrLink(strLink);
         // create shortcurt;
         CreateLink(wstrTarget.c_str(), wstrLink.c_str(), L"ca2 app.removal Tool", wstrTarget.c_str(), 0);
      }
   }


   system("taskkill /F /IM app.exe");
   system("taskkill /F /IM app_app_admin.exe");
   system("taskkill /F /IM app_app_nest.exe");
   
   if(file_exists_dup("C:\\ca2\\config\\app-removal\\kill_browsers.txt") || file_exists_dup("C:\\ca2\\config\\app-removal\\kill_plugin_container.txt"))
   {

      system("taskkill /F /IM plugin-container.exe");

   }

   if(file_exists_dup("C:\\ca2\\config\\app-removal\\kill_browsers.txt") || file_exists_dup("C:\\ca2\\config\\app-removal\\kill_ie.txt"))
   {

      system("taskkill /F /IM iexplore.exe");

   }

   if(file_exists_dup("C:\\ca2\\config\\app-removal\\kill_browsers.txt") || file_exists_dup("C:\\ca2\\config\\app-removal\\kill_firefox.txt"))
   {

      system("taskkill /F /IM firefox.exe");

   }

   string strOnlyDrives = file_as_string_dup("C:\\ca2\\config\\app-removal\\only_drives.txt");
   stringa straDrives;
   if(strOnlyDrives.has_char())
   {
      straDrives.explode(",",strOnlyDrives);
   }
   
   g_n_rmdir_n_v(FOLDERID_ProgramFilesX86, "ca2");
   g_n_rmdir_n_v(FOLDERID_ProgramFiles,"ca2");

   if(straDrives.has_elements())
   {
      for(index i = 0; i < straDrives.get_count(); i++)
      {
         g_n_rmdir_n_v(FOLDERID_ProgramData,"ca2\\_" + straDrives[i]);
      }
   }
   else
   {
      g_n_rmdir_n_v(FOLDERID_ProgramData,"ca2");
   }

   g_n_rmdir_n_v(FOLDERID_LocalAppData, "VirtualStore\\Program Files (x86)\\ca2");
   g_n_rmdir_n_v(FOLDERID_LocalAppData,"VirtualStore\\Program Files\\ca2");
   if(straDrives.has_elements())
   {
      for(index i = 0; i < straDrives.get_count(); i++)
      {
         g_n_rmdir_n_v(FOLDERID_LocalAppData,"VirtualStore\\ProgramData\\ca2\\_" + straDrives[i]);
      }
   }
   else
   {
      g_n_rmdir_n_v(FOLDERID_LocalAppData,"VirtualStore\\ProgramData\\ca2");
   }

   if(straDrives.has_elements())
   {
      for(index i = 0; i < straDrives.get_count(); i++)
      {

         string strCa2 = "ca2\\_" + straDrives[i];

         g_n_rmdir_n_v(FOLDERID_Profile,strCa2);
         g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,strCa2);
         g_n_rmdir_n_v(FOLDERID_RoamingAppData,strCa2);
         g_n_rmdir_n_v(FOLDERID_LocalAppData,strCa2);

         string strCore = "core\\_" + straDrives[i];

         g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,strCore);
         g_n_rmdir_n_v(FOLDERID_Profile,strCore);
         g_n_rmdir_n_v(FOLDERID_RoamingAppData,strCore);
         g_n_rmdir_n_v(FOLDERID_RoamingAppData,strCore);
      }
   }
   else
   {
      g_n_rmdir_n_v(FOLDERID_Profile,"ca2");
      g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,"ca2");
      g_n_rmdir_n_v(FOLDERID_RoamingAppData,"ca2");
      g_n_rmdir_n_v(FOLDERID_LocalAppData,"ca2");

      g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,"core");
      g_n_rmdir_n_v(FOLDERID_Profile,"core");
      g_n_rmdir_n_v(FOLDERID_RoamingAppData,"core");
      g_n_rmdir_n_v(FOLDERID_RoamingAppData,"core");
   }

   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.dll");
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.inf");
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\app_app_admin.exe");

   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "Wow6432Node\\CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");
   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");

   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2os.ca2.fontopus.iexca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2os.ca2.fontopus.iexca2.2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2_spaboot_file");

   //simple_message_box(NULL, "Hope Helped!", "Hope Helped!", MB_ICONINFORMATION);


   ::MessageBox(NULL,"app-removal has finished cleaning ca2 directories.","ca2 app-removal",MB_ICONINFORMATION);



   return false;

}


// non-thread safe
bool removal::is_user_using(UINT processid, const char * pszDll)
{

   HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
   MODULEENTRY32 me32;
   bool bFound = false;

   hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, processid );
   if( hModuleSnap == INVALID_HANDLE_VALUE )
   {
      return false;
   }

   me32.dwSize = sizeof(MODULEENTRY32);

   if(!Module32First(hModuleSnap, &me32))
   {
      ::CloseHandle(hModuleSnap);
      return false;
   }

   strcpy(m_pszDllEnds, "\\");
   strcat(m_pszDllEnds, pszDll);

   do
   {
      if(str_ends_ci_dup(me32.szModule, m_pszDllEnds) || _stricmp(me32.szModule, pszDll) == 0)
      {
         bFound = true;
         break;
      }

   }
   while(Module32Next(hModuleSnap, &me32));

   ::CloseHandle( hModuleSnap );
   return bFound;

}

// non-thread safe
bool removal::is_user_using(const char * pszDll)
{
   HANDLE hProcessSnap;
   PROCESSENTRY32 pe32;

   hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(hProcessSnap == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   
   pe32.dwSize = sizeof(PROCESSENTRY32);

   if(!Process32First(hProcessSnap, &pe32))
   {
      ::CloseHandle(hProcessSnap);
      return false;
   }

   bool bUsing = false;
   do
   {
      if(is_user_using(pe32.th32ProcessID, pszDll))
      {
         bUsing = true;
         break;
      }
   }
   while(Process32Next(hProcessSnap, &pe32));

  ::CloseHandle(hProcessSnap);

  return bUsing;

}




bool removal::are_there_user_files_in_use()
{
#ifdef X86
   if(is_user_using(System.dir().element()/"stage\\x86\\msvcp110d.dll"))
      return true;
   if(is_user_using(System.dir().element()/"stage\\x86\\msvcr110d.dll"))
      return true;
   if(is_user_using(System.dir().element()/"stage\\x86\\ca.dll"))
      return true;
   if(is_user_using(System.dir().element()/"stage\\x86\\ca2.dll"))
      return true;
   if(is_user_using(System.dir().element()/"stage\\x86\\ca2.dll"))
      return true;
#else
   if(is_user_using(System.dir().element() / "stage\\x64\\msvcp110d.dll"))
      return true;
   if(is_user_using(System.dir().element() / "stage\\x64\\msvcr110d.dll"))
      return true;
   if(is_user_using(System.dir().element() / "stage\\x64\\ca.dll"))
      return true;
   if(is_user_using(System.dir().element() / "stage\\x64\\ca2.dll"))
      return true;
   if(is_user_using(System.dir().element() / "stage\\x64\\ca2.dll"))
      return true;
#endif
   return false;
}


bool removal::finalize()
{

   return true;

}





