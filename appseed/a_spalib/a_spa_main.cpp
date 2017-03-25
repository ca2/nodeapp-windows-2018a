#include "framework.h"
#include <shellapi.h>
#include <shlobj.h>

bool g_bRunMainLoop = true;

void end_spa(::aura::application * papp);

void kill_process(string strImageName);
int is_downloading_spaadmin();

void gdiplus_start();
void gdiplus_end();

void run_vcredist(string strPlatform);

#define CLASS_DECL_AURA
#define CLASS_DECL_AXIS
typedef HWND oswindow;

//#include "aura/aura/aura_launcher.h"
//#include "aura/aura/aura_small_ipc_channel.h"
#include "axis/app_install_launcher.h"


bool app_install_send_short_message(const char * pszPlatform, const char * psz, bool bLaunch, const char * pszBuild);
void app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild);
int register_spa_file_type();
void start_program_files_spa_admin(string strPlatform);
void defer_start_program_files_spa_admin(string strPlatform);
bool low_is_spaadmin_running(string strPlatform);
void get_system_locale_schema(string & strLocale, string & strSchema);


int call_async(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int iShow);


#ifdef WINDOWSEX


string get_module_path(HMODULE hmodule)
{

   string wstrPath;

   wchar_t * pwz = (wchar_t *)malloc(4906);

   DWORD dwSize = ::GetModuleFileNameW(hmodule, pwz, 4096);

   wstrPath = pwz;

   free(pwz);

   return wstrPath.c_str();

}


#endif


#define INSTALL_BIN_SPA -1977
#define INSTALL_BIN_SPAADMIN -1984


bool app_install_send_short_message(const char * psz, bool bLaunch, const char * pszBuild);

//using namespace std;



IDTHREAD g_dwMain2;

#include "axis/app_install_launcher.cpp"


void app_install_launcher::start_in_context()
{

   if (!ensure_executable())
      return;

   string strDir = dir::name(m_strPath.c_str());

   wstring wstrDir = u16(strDir.c_str());

   wstring wstr = u16(m_strPath.c_str());

   STARTUPINFOW si;
   memset(&si, 0, sizeof(si));
   si.cb = sizeof(si);
   si.dwFlags = 0;
   si.wShowWindow = SW_HIDE;
   PROCESS_INFORMATION pi;
   memset(&pi, 0, sizeof(pi));

   if (!::CreateProcessW(NULL, (wchar_t *)wstr.c_str(), NULL, NULL, FALSE, 0, NULL, wstrDir.c_str(), &si, &pi))
   {

      output_debug_string("Could not create process \"" + m_strPath + "\"");

   }

   Sleep(2000);

}


int register_spa_file_type()
{

   HKEY hkey;

   wstring extension = L".spa";                     // file extension
   wstring desc = L"spafile";          // file type description
   wstring content_type = L"application/x-spa";

   wstring app(::path::a_spa("x86"));

   wstring icon(app);

   app = L"\"" + app + L"\"" + L" \"%1\"";
   icon = L"\"" + icon + L"\",107";

   wstring action = L"Open";

   wstring sub = L"\\shell\\";

   wstring path = L"spafile\\shell\\open\\command";


   // 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.002
   if (RegCreateKeyExW(HKEY_CLASSES_ROOT, extension.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)desc.c_str(), desc.length() * sizeof(wchar_t)); // default vlaue is description of file extension
   RegSetValueExW(hkey, L"ContentType", 0, REG_SZ, (BYTE*)content_type.c_str(), content_type.length() * sizeof(wchar_t)); // default vlaue is description of file extension
   RegCloseKey(hkey);



   // 2: Create Subkeys for action ( "Open with my program" )
   // HKEY_CLASSES_ROOT\.002\Shell\\open with my program\\command
   if (RegCreateKeyExW(HKEY_CLASSES_ROOT, path.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)app.c_str(), app.length() * sizeof(wchar_t));
   RegCloseKey(hkey);


   path = L"spafile\\DefaultIcon";

   if (RegCreateKeyExW(HKEY_CLASSES_ROOT, path.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)icon.c_str(), icon.length() * sizeof(wchar_t));
   RegCloseKey(hkey);

   wstring wstr(dir::a_spa("x86") / "spa_register.txt");

   int iRetry = 9;

   while (!file_exists_dup(u8(wstr.c_str())) && iRetry > 0)
   {

      dir::mk(dir::name(u8(wstr.c_str())).c_str());

      file_put_contents_dup(u8(wstr.c_str()).c_str(), "");

      iRetry--;

      Sleep(84);

   }

   return 1;

}



void start_program_files_spa_admin(string strPlatform)
{

   SHELLEXECUTEINFOW sei = {};

   string str = ::path::a_spaadmin(strPlatform);

   if (!::file_exists_dup(str))
   {

      return;

   }

   spaadmin_mutex mutexStartup("-startup");

   wstring wstr(str);

   sei.cbSize = sizeof(SHELLEXECUTEINFOW);
   sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
   sei.lpVerb = L"RunAs";
   sei.lpFile = wstr.c_str();
   ::ShellExecuteExW(&sei);
   DWORD dwGetLastError = GetLastError();

}


void run_vcredist(string strPlatform)
{

   SHELLEXECUTEINFOW sei = {};

   string str = ::path::vcredist(strPlatform);

   if (!::file_exists_dup(str))
   {

      return;

   }

   wstring wstr(str);

   sei.cbSize = sizeof(SHELLEXECUTEINFOW);
   sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
   // sei.lpVerb = L"RunAs";
   sei.lpFile = wstr.c_str();
   sei.lpParameters = L"/install /passive /norestart /q";
   ::ShellExecuteExW(&sei);
   DWORD dwGetLastError = GetLastError();

   DWORD dwExitCode = 0;

   for (int i = 0; i < (19840 + 19770); i++)
   {

      if (::GetExitCodeProcess(sei.hProcess, &dwExitCode))
      {

         if (dwExitCode != STILL_ACTIVE)
         {

            break;

         }

      }
      else
      {

         Sleep(1984 + 1977);

         break;

      }

      Sleep(84 + 77);

   }

   ::CloseHandle(sei.hProcess);

}



void defer_start_program_files_spa_admin(string strPlatform)
{

   if (low_is_spaadmin_running(strPlatform))
   {

      return;

   }

   start_program_files_spa_admin(strPlatform);

}


bool low_is_spaadmin_running(string strPlatform)
{

   spaadmin_mutex smutex(strPlatform);

   return smutex.already_exists();

}



void get_system_locale_schema(string & strLocale, string & strSchema)
{

#if defined(WINDOWS)

   LANGID langid = ::GetUserDefaultLangID();

#define SPR_DEUTSCH LANG_GERMAN

   if (langid == LANG_SWEDISH)
   {
      strLocale = "se";
      strSchema = "se";
   }
   else if (langid == MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN))
   {
      strLocale = "pt-br";
      strSchema = "pt-br";
   }
   else if (PRIMARYLANGID(langid) == SPR_DEUTSCH)
   {
      strLocale = "de";
      strSchema = "de";
   }
   else if (PRIMARYLANGID(langid) == LANG_ENGLISH)
   {
      strLocale = "en";
      strSchema = "en";
   }
   else if (PRIMARYLANGID(langid) == LANG_JAPANESE)
   {
      strLocale = "jp";
      strSchema = "jp";
   }
   else if (PRIMARYLANGID(langid) == LANG_POLISH)
   {
      strLocale = "pl";
      strSchema = "pl";
   }

#endif

   if (strLocale.length() <= 0)
      strLocale = "_std";

   if (strSchema.length() <= 0)
      strSchema = "_std";

}



// http://stackoverflow.com/questions/7956519/how-to-kill-processes-by-name-win32-api
// http://stackoverflow.com/users/1055018/jeremy-whitcher
void kill_process(string strImageName)
{

   const int maxProcIds = 1024;
   DWORD procList[maxProcIds];
   DWORD procCount;
   char* exeName = "ExeName.exe";
   char processName[MAX_PATH];

   // get the process by name
   if (!EnumProcesses(procList, sizeof(procList), &procCount))
      return;

   // convert from bytes to processes
   procCount = procCount / sizeof(DWORD);

   // loop through all processes
   for (DWORD procIdx = 0; procIdx < procCount; procIdx++)
   {
      // get a handle to the process
      HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procList[procIdx]);
      // get the process name
      GetProcessImageFileName(procHandle, processName, sizeof(processName));

      // terminate all pocesses that contain the name

      if (strImageName.CompareNoCase(processName) == 0)
      {

         TerminateProcess(procHandle, 0);

      }

      CloseHandle(procHandle);

   }

}

void end_spa(::aura::application * papp)
{

   g_bRunMainLoop = false;

   Sys(papp).post_quit();


}







int g_iDownloadingSpaadmin;

int is_downloading_spaadmin()
{

   return g_iDownloadingSpaadmin;

}


void app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild)
{
   bool bLaunch;

   if (_stricmp(szParameters, "exit") == 0
      || _stricmp(szParameters, "quit") == 0)
   {
      bLaunch = false;
   }
   else
   {
      bLaunch = true;
   }

   app_install_send_short_message(pszPlatform, szParameters, bLaunch, pszBuild);

}







//#include "aura/aura/aura_launcher.cpp"
//#include "aura/aura/aura_small_ipc_channel.cpp"
//#include "aura/aura/aura_small_ipc_channel.cpp"
//#include "aura/os/windows/windows_small_ipc_channel.cpp"



//bool launcher::start()
//{
//
//   if(!ensure_executable())
//      return false;
//
//   string strPath(get_executable_path());
//
//   string strDir(dir::name(strPath.c_str()));
//
//   string strParams = get_params();
//
//   call_async(strPath.c_str(),strParams.c_str(),strDir.c_str(),SW_HIDE);
//
//   return true;
//
//}


//int call_async(
//   const char * pszPath,
//   const char * pszParam,
//   const char * pszDir,
//   int iShow)
//{
//
//   SHELLEXECUTEINFOA infoa;
//
//   memset(&infoa,0,sizeof(infoa));
//
//   infoa.cbSize         = sizeof(infoa);
//   infoa.lpFile         = pszPath;
//   infoa.lpParameters   = pszParam;
//   infoa.lpDirectory    = pszDir;
//   infoa.nShow          = iShow;
//
//   int iOk = ::ShellExecuteExA(&infoa);
//
//   return iOk;
//
//}
//
//





bool app_install_send_short_message(const char * pszPlatform, const char * psz, bool bLaunch, const char * pszBuild)
{

#ifdef METROWIN

   throw "todo";

#else

   ::aura::ipc::tx txchannel(::get_thread_app());

   app_install_launcher launcher(pszPlatform, "", "");

   string strChannel;

   // "core/spaboot_install"

   strChannel = "::ca2::fontopus::ca2_spaboot_install_" + string(pszPlatform) + "::7807e510-5579-11dd-ae16-0800200c7784";

   if (!txchannel.open(strChannel, bLaunch ? &launcher : NULL))
      return false;

   txchannel.send(psz, INFINITE);

#endif

   return true;

}


namespace a_spa
{


   void simple_app::start_app_install_in_context(string strPlatform)
   {

      app_install_launcher launcher(strPlatform, "", "");

      launcher.start_in_context();

   }


} // namespace a_spa






string read_resource_as_string(
   HINSTANCE hinst,
   UINT nID,
   LPCTSTR lpcszType)
{
   string str;
   HRSRC hrsrc = ::FindResource(
      hinst,
      MAKEINTRESOURCE(nID),
      lpcszType);
   if (hrsrc == NULL)
      return str;
   HGLOBAL hres = ::LoadResource(hinst, hrsrc);
   if (hres == NULL)
      return str;
   DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

   if (hres != NULL)
   {
      UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      std::string strRes((char *)lpnRes, dwResSize);
      str = strRes;
#ifndef WIN32 //Unlock Resource is obsolete in the Win32 API
      ::UnlockResource(hres);
#endif
      ::FreeResource(hres);
   }
   return str;

}