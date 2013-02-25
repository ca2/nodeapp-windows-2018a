#include "framework.h"
#include <Shlobj.h>
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"


#include <psapi.h>


#include <tlhelp32.h>




class removal :
   public simple_app,
   public small_ipc_rx_channel::receiver
{
public:

   
   enum e_message
   {
      message_none,
      message_ok,
      message_failed,
      message_unknown,
   };


   e_message                  m_emessage;
   HANDLE                     m_hmutex_app_removal;
   
   char *                     m_modpath;
   char *                     m_pszDllEnds;
   uint32_t *                    m_dwaProcess;
   int32_t                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   int32_t                        m_iSizeModule;
   bool                       m_bInstallerInstalling;

   removal();
   virtual ~removal();
   
   bool is_user_using(const char * pszDll);

   ATOM spaboot_message_register_class(HINSTANCE hInstance);

   int32_t cube_run(const char * id);

   void call_self_privileged_sync(const char * param);

   int32_t spaboot_registry_register(const char * lpszFile);

   void call_sync(const char * path, const char * param);

   void installer_call_sync(const char * path, const char * param);

   bool are_there_user_files_in_use();

   bool is_user_using(uint32_t processid, const char * pszDll);

   virtual void on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage);

   virtual bool initialize();

   virtual bool finalize();

};

// if MSVC CRT is used
extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow)
{
   // call shared/exported WinMain
   return simple_app::s_main < removal > ();
}

// if MSVC CRT is stripped
/*extern "C" int32_t WinMainCRTStartup() \
{ 

   ExitProcess(simple_app::s_main < removal > ());

}*/

extern bool g_bInstalling;
extern stra_dup * g_pstraTrace;

removal::removal()
{
   xxdebug_box("app-install", "app", 0);
   m_hinstance             = ::GetModuleHandleA(NULL);
   m_hmutex_app_removal  = NULL;
   e_message m_emessage    = message_none;
   m_modpath               = NULL;
   m_pszDllEnds            = NULL;
   m_dwaProcess            = NULL;
   m_iSizeProcess          = 0;
   m_hmodulea              = NULL;
   m_iSizeModule           = 0;
   m_bInstallerInstalling  = false;
   g_pstraTrace            = NULL;
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


HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszIconPath = NULL, int32_t iIcon = 0) 
{ 
    HRESULT hres; 
    IShellLinkW* psl; 
 
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // Set the path to the shortcut target and add the description. 
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

vsstring get_dir(const KNOWNFOLDERID & rfid, const char * lpcsz)
{

   vsstring str;

   wchar_t * buf = NULL;
   
   SHGetKnownFolderPath(rfid, 0, NULL, &buf);

   char * psz = utf16_to_8(buf);

   str = dir::path(psz, lpcsz);

   ca2_free(psz);
   
   CoTaskMemFree(buf);

   return str;

}

void my_system(const char * pszCmd)
{
STARTUPINFO si;
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
}

}


void rmdir(const char * pszDir)
{
   my_system("cmd.exe /C rmdir /S /Q \"" + vsstring(pszDir) + "\"");
}

void rmdir_n_v(const char * pszDir)
{
   vsstring str(pszDir);
   rmdir(str);

   str.replace(":", "");
   str = dir::path(get_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Temporary Internet Files\\Virtualized"), str);
   rmdir(str);
}


void g_n_rmdir_n_v(const KNOWNFOLDERID & rfid, const char * pszDir)
{

   vsstring strDir = get_dir(rfid, pszDir);

   rmdir_n_v(strDir);

}




bool removal::initialize()
{


   m_hmutex_app_removal = ::CreateMutex(NULL, FALSE, "Global\\::ca::fontopus::ccvotagus_ca2_spa_app_removal::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      ::MessageBox(NULL, "ca app-removal.exe is already running.\n\nPlease wait for app-removal to finish or close it - using Task Manager - Ctrl+Shift+ESC - to continue.", "app-install.exe is running!", MB_ICONEXCLAMATION);
      m_iError = -202;
      return false;
   }

   char szFile[MAX_PATH * 8];

   ::GetModuleFileName(NULL, szFile, sizeof(szFile));

   vsstring strTargetDir = get_dir(FOLDERID_ProgramFilesX86, "ca-app-removal");

   dir::mk(strTargetDir);

   vsstring strTarget = dir::path(strTargetDir, "app-removal.exe");

   if(::CopyFile(szFile, strTarget, TRUE))
   {
      int32_t i = MessageBox(NULL, "Do you want to place a shortcut to ca app-removal in Desktop?\n\nProgram has already been copied to Program Files Folder.", "app-removal installation", MB_ICONQUESTION | MB_YESNOCANCEL);

      if(i == IDCANCEL)
         return false;

      if(i == IDYES)
      {
         vsstring strLink= get_dir(FOLDERID_Desktop, "ca app-removal Tool.lnk");
         wstring wstrTarget(strTarget);
         wstring wstrLink(strLink);
         // create shortcurt;
         CreateLink(wstrTarget, wstrLink, L"ca app-removal Tool", wstrTarget, 0);
      }
   }


   my_system("taskkill /F /IM app.exe");
   my_system("taskkill /F /IM app-install.exe");
   my_system("taskkill /F /IM ca2plugin-container.exe");
   my_system("taskkill /F /IM plugin-container.exe");
   my_system("taskkill /F /IM iexplore.exe");
   my_system("taskkill /F /IM firefox.exe");
   g_n_rmdir_n_v(FOLDERID_ProgramFilesX86, "ca");
   g_n_rmdir_n_v(FOLDERID_ProgramData, "ca");

   vsstring str;

   g_n_rmdir_n_v(FOLDERID_Profile, "ca");
//rmdir /S /Q "C:\Users\votagus\ca"
   g_n_rmdir_n_v(FOLDERID_LocalAppData, "ca");
//rmdir /S /Q "C:\Users\votagus\AppData\Local\ca"
//rmdir /S /Q "C:\Users\votagus\AppData\Local\VirtualStore\Program Files (x86)\ca"
   g_n_rmdir_n_v(FOLDERID_LocalAppData, "VirtualStore\\Program Files (x86)\\ca");
//rmdir /S /Q "C:\Users\votagus\AppData\Local\VirtualStore\ProgramData\ca"
   g_n_rmdir_n_v(FOLDERID_LocalAppData, "VirtualStore\\ProgramData\\ca");
//rmdir /S /Q "C:\Users\votagus\AppData\LocalLow\ca"
   g_n_rmdir_n_v(FOLDERID_LocalAppDataLow, "ca");
//rmdir /S /Q "C:\Users\votagus\AppData\Roaming\ca"
   g_n_rmdir_n_v(FOLDERID_RoamingAppData, "ca");
//del "C:\Windows\Downloaded Program Files\iexca2.dll"
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.dll");
//del "C:\Windows\Downloaded Program Files\iexca2.inf"
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.inf");
//del "C:\Windows\Downloaded Program Files\app-install.exe"
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\app-install.exe");
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\ProgramData\ca"
   //str = get_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Tempoarary Internet Files\\Virtualized\\VirtualStore\\ProgramsData\\ca");
   //rmdir(str);
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Program Files (x86)\ca"
   //str = get_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Tempoarary Internet Files\\Virtualized\\VirtualStore\\Programs Files(x86)\\ca");
   //rmdir(str);
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Users\votagus\ca"
   //str = get_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Tempoarary Internet Files\\Virtualized\\VirtualStore\\ProgramsData\\ca");
   //rmdir(str);
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Users\votagus\AppData\Local\ca"
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Users\votagus\AppData\Local\VirtualStore\Program Files (x86)\ca"
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Users\votagus\AppData\Local\VirtualStore\ProgramData\ca"
//rmdir /S /Q "C:\Users\votagus\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\C\Users\votagus\AppData\Roaming\ca"
//call ".\windows_registry\delete_global_npca2.bat"
//call ".\windows_registry\delete_activex_iexca2.bat"

   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "Wow6432Node\\CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");
   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");

   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ccvotagus.ca.fontopus.iexca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ccvotagus.ca.fontopus.iexca2.2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ccvotagus_ca2_spaboot_file");

   //MessageBox(NULL, "Hope Helped!", "Hope Helped!", MB_ICONINFORMATION);

   return false;

}


// non-thread safe
bool removal::is_user_using(uint32_t processid, const char * pszDll)
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

   strcpy_dup(m_pszDllEnds, "\\");
   strcat_dup(m_pszDllEnds, pszDll);

   do
   {
      if(str_ends_ci_dup(me32.szModule, m_pszDllEnds) || stricmp_dup(me32.szModule, pszDll) == 0)
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

void removal::on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage)
{
   vsstring strMessage(pszMessage);
   int32_t iRet = 0;
   const char * pszSuffix;
   if((pszSuffix = str_begins_inc_dup(strMessage, "synch_spaadmin:")) != NULL)
   {
      if(g_bInstalling)
      {
         iRet = 1;
         return;
      }
      if(m_bInstallerInstalling)
      {
         iRet = 1;
         return;
      }
      m_bInstallerInstalling = true;
      synch_spaadmin(pszSuffix);
      m_bInstallerInstalling = false;
   }
   else if((pszSuffix = str_begins_inc_dup(strMessage, "spaadmin:")) != NULL)
   {
      if(g_bInstalling)
      {
         iRet = 1;
         return;
      }
      //if(m_bInstallerInstalling)
      //{
        // iRet = 1;
         //return;
      //}
      //m_bInstallerInstalling = true;
      start_spaadmin(pszSuffix);
   }
   else if(stricmp_dup(strMessage, "ok") == 0)
   {
      m_emessage = message_ok;
   }
   else if(stricmp_dup(strMessage, "failed") == 0)
   {
      m_emessage = message_failed;
   }
   else if(stricmp_dup(strMessage, "exit") == 0)
   {
      PostQuitMessage(0);
   }
   else if(stricmp_dup(strMessage, "quit") == 0)
   {
      PostQuitMessage(0);
   }
   else
   {
      m_emessage = message_unknown;
   }
}



bool removal::are_there_user_files_in_use()
{
#ifdef X86
   if(is_user_using(dir::ca("stage\\x86\\msvcp110d.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x86\\msvcr110d.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x86\\c.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x86\\ca.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x86\\ca.dll")))
      return true;
#else
   if(is_user_using(dir::ca("stage\\x64\\msvcp110d.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x64\\msvcr110d.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x64\\c.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x64\\ca.dll")))
      return true;
   if(is_user_using(dir::ca("stage\\x64\\ca.dll")))
      return true;
#endif
   return false;
}


bool removal::finalize()
{

   simple_app::finalize();

   //ca2_free(m_hmodulea);
   //ca2_free(m_dwaProcess);
   //ca2_free(m_pszDllEnds);
   //ca2_free(m_modpath);

   return true;

}








