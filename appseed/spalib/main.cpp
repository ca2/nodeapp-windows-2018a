#include "StdAfx.h"



Gdiplus::GdiplusStartupInput *   g_pgdiplusStartupInput     = NULL;
Gdiplus::GdiplusStartupOutput *  g_pgdiplusStartupOutput    = NULL;
DWORD_PTR                        g_gdiplusToken             = NULL;
DWORD_PTR                        g_gdiplusHookToken         = NULL;

#define CLASS_DECL_AURA
#define CLASS_DECL_AXIS
#define WINDOWS
#define WINDOWSEX
typedef HWND oswindow;
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MSGFLT_ADD 1
#define MSGFLT_REMOVE 2

#include "aura/aura/aura_launcher.h"
#include "aura/aura/aura_small_ipc_channel.h"
#include "axis/install_launcher.h"

typedef int
(WINAPI * LPFN_ChangeWindowMessageFilter)(
UINT message,
DWORD dwFlag);


CLASS_DECL_AURA LPFN_ChangeWindowMessageFilter g_pfnChangeWindowMessageFilter = NULL;

bool app_install_send_short_message(const char * psz,bool bLaunch,const char * pszBuild);
void app_install_call_sync(const char * szParameters,const char * pszBuild);
int register_spa_file_type();

#if defined(LINUX) || defined(WINDOWS)
//#include <omp.h>
#else
int omp_get_thread_num()
{
   return 0;
}
#endif

int call_async(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int iShow);


#ifdef WINDOWSEX


string get_module_path(HMODULE hmodule)
{
   
   wstring wstrPath;

   wchar_t * pwz = (wchar_t *) malloc(4906);

   DWORD dwSize = ::GetModuleFileNameW(hmodule, pwz, 4096);

   wstrPath = pwz;

   free(pwz);

   return ::u8(wstrPath.c_str());

}

#endif


bool app_install_send_short_message(const char * psz,bool bLaunch,const char * pszBuild);

using namespace std;

std::string get_app_id(std::wstring wstr);

DWORD g_dwMain2;
DWORD g_iRet;
string g_strVersion;
//string g_strBuild;

int starter_start(const char * pszId);
int run_file(const char * pszFile,int nCmdShow);
int run_uninstall_run(const char * lpCmdLine,int nCmdShow);
int ca2_app_install_run(const char * psz,const char * pszParam1,const char * pszParam2,DWORD & dwStartError,bool bSynch);


int check_soon_launch();
int check_spa_installation();
int show_spa_window(bool bShow = true);
int spa_main_fork();


int spa_admin();
int spa_main();

string get_latest_build_number(const char * pszVersion);


SPALIB_API int spalib_main(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{

   g_pgdiplusStartupInput     = new Gdiplus::GdiplusStartupInput();
   g_pgdiplusStartupOutput    = new Gdiplus::GdiplusStartupOutput();
   g_gdiplusToken             = NULL;
   g_gdiplusHookToken         = NULL;

   g_pgdiplusStartupInput->SuppressBackgroundThread = TRUE;

   Gdiplus::Status statusStartup = GdiplusStartup(&g_gdiplusToken,g_pgdiplusStartupInput,g_pgdiplusStartupOutput);

   if(statusStartup != Gdiplus::Ok)
   {


      return 0;

   }




   statusStartup = g_pgdiplusStartupOutput->NotificationHook(&g_gdiplusHookToken);


   if(statusStartup != Gdiplus::Ok)
   {


      return 0;

   }


#if CA2_PLATFORM_VERSION == CA2_BASIS

   g_strVersion = "basis";

#else

   g_strVersion = "stage";

#endif

   g_hinstance = hInstance;

   if(spa_get_admin())
   {

      g_iRet = spa_admin();

   }
   else
   {

      g_iRet = spa_main();

   }


   g_pgdiplusStartupOutput->NotificationUnhook(g_gdiplusHookToken);


   ::Gdiplus::GdiplusShutdown(g_gdiplusToken);

   return g_iRet;

}

SPALIB_API int spa_main()
{
   if(check_soon_launch())
      return 0;

   if(!show_spa_window())
      return -1;

   if(!spa_main_fork())
      return -2;

   while(GetMessage(&g_msg,NULL,0,0))
   {

      TranslateMessage(&g_msg);

      DispatchMessage(&g_msg);

   }

   return g_iRet;

}


void start_app_install_in_context();


SPALIB_API int spa_admin()
{

   trace("--\r\n");
   trace(":::::Installing spa and installer\r\n");
   trace("***Installing spa\r\n");
   trace("Registering spa file handler\r\n");
   trace(0.0);

   register_spa_file_type();

   trace(0.05);

   int iTry = 5;

   while(!check_spa_installation())
   {

      iTry--;

      if(iTry < 0)
      {

         return -3;

      }

   }

   trace("***Preparing app.install\r\n");
   trace("Starting app.install\r\n");
   trace(0.84);


   start_app_install_in_context();


   trace(":::::spa Installed Successfully\r\n");
   trace("***spa and installer Installed Successfully\r\n");
   trace("Thank you\r\n");
   trace(1.0);


   return 0;

}


DWORD WINAPI spa_fork_proc(LPVOID);

int spa_main_fork()
{

   if(!::CreateThread(NULL,0,spa_fork_proc,NULL,0,&g_dwMain2))
   {
      
      return 0;

   }

   return 1;

}

int spalib_main2();

DWORD WINAPI spa_fork_proc(LPVOID)
{
   
   g_iRet = spalib_main2();

   ::PostMessage(g_hwnd, WM_QUIT, 0, 0);

   return g_iRet;

}


int spalib_main2()
{

   int iTry = 1440;
   
   while(!check_spa_installation())
   {
   
      iTry--;

      if(iTry < 0)
      {

         return 0;

      }

      Sleep(84 + 77);

   }


   std::wstring strId;

   std::wstring wstr = ::GetCommandLineW();

   int iFind1 = 0;

   if(wstr[0] == L'\"')
   {

      iFind1= wstr.find('\"',1);

   }

   int iFind = wstr.find(L" : ",iFind1 + 1);

   if(iFind < 0)
   {

      string strAppId = get_app_id(wstr.substr(iFind1 + 1));

      if(strAppId.length() <= 0)
      {
         return 1;
      }

      HMODULE hmoduleUser32 = ::LoadLibrary("User32");
      g_pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter) ::GetProcAddress(hmoduleUser32,"ChangeWindowMessageFilter");


         string strCommandLine;


      strCommandLine = " : app=" + get_app_id(wstr.substr(iFind1 + 1));

      strCommandLine += " install";
      strCommandLine += " locale=_std";
      strCommandLine += " schema=_std";
      strCommandLine += " version=stage";

      strCommandLine += " ";


      string strCommand;

      strCommand = "synch_spaadmin:";

      strCommand += "starter_start:";

      strCommand += strCommandLine;

      bool bBackground = true;

      if(bBackground)
      {

         strCommand += " background";

      }

      app_install_call_sync(strCommand.c_str(),"");

   }

   return 1;

}

int check_soon_file_launch(std::wstring wstr);

int check_soon_app_id(std::wstring wstr);

int check_soon_launch()
{

   std::wstring strId;
   
   std::wstring wstr = ::GetCommandLineW();

   int iFind1 = 0;

   if(wstr[0] == L'\"')
   {

      iFind1= wstr.find('\"',1);

   }

   int iFind = wstr.find(L" : ", iFind1 + 1);

   if(iFind < 0)
   {
      
      if(check_soon_file_launch(wstr.substr(iFind1 + 1)))
      {

         return 1;

      }

   }
   else
   {
      iFind = wstr.find(L"app=",iFind);
      if(iFind >= 0)
      {
         int iEnd = wstr.find(L" ",iFind);
         if(iEnd < 0)
         {
            strId = wstr.substr(iFind + 4);
         }
         else
         {
            strId = wstr.substr(iFind + 4,iEnd - iFind - 4);
         }

         // trim initial quote
         if(strId[0] == '\"')
            strId = strId.substr(1);

         // trim final quote
         if(strId[strId.length() - 1] == '\"')
            strId = strId.substr(0,strId.length() - 1);

      }

   }

   if(strId.empty())
      return FALSE;

   return check_soon_app_id(strId);

}


// trim initial quote
int trim(::std::wstring & wstr,wchar_t wch)
{

   if(wstr.length() <= 0)
   {

      return 0;

   }

   int c = 0;

   while(wstr[0] == wch)
   {

      wstr = wstr.substr(1);

      if(wstr.length() <= 0)
         return c;

      c++;

   }

   // trim final quote
   while(wstr[wstr.length() - 1] == wch)
   {

      wstr = wstr.substr(0,wstr.length() - 1);

      if(wstr.length() <= 0)
         return c;

      c++;

   }

   return c;

}


int trim(::std::string & wstr,char wch)
{

   if(wstr.length() <= 0)
   {

      return 0;

   }

   int c = 0;

   while(wstr[0] == wch)
   {

      wstr = wstr.substr(1);

      if(wstr.length() <= 0)
         return c;

      c++;

   }

   // trim final quote
   while(wstr[wstr.length() - 1] == wch)
   {

      wstr = wstr.substr(0,wstr.length() - 1);

      if(wstr.length() <= 0)
         return c;

      c++;

   }

   return c;

}

void trim(::std::string & wstr)
{
   int c = 1;

   while(c > 0)
   {
      c = 0;
      c += trim(wstr,' ');
      c += trim(wstr,'\t');
      c += trim(wstr,'\n');
      c += trim(wstr,'\r');

   }

}

std::string get_app_id(std::wstring wstr)
{

   if(wstr.length() <= 0)
   {

      return "";

   }

   int c = 1;

   while(c > 0)
   {
      c = 0;
      c += trim(wstr,'\"');
      c += trim(wstr,' ');

   }

   if(wstr.length() <= 0)
   {

      return "";

   }

   XDoc doc;

   ::std::string strPath = u8(wstr.c_str());

   ::std::string strContents = file::get_contents(strPath.c_str());

   if(!doc.Load(strContents.c_str()))
   {

      return "";

   }

   XNode * pnode = doc.GetRoot();

   if(pnode == NULL)
   {

      return "";

   }

   /*if(pnode->GetChildCount() <= 0)
   {
      if(pnode->name == "meta")
      {
         return file::name(strPath.c_str());
      }

      return "";
   }*/

   const char * psz = pnode->GetChildAttrValue("launch","app");

   if(psz == NULL || *psz == '\0')
   {

      return "";

   }

   return psz;

}

int check_soon_file_launch(std::wstring wstr)
{


   return check_soon_app_id(u16(get_app_id(wstr.c_str()).c_str()));

}


std::wstring spa_app_id_to_app_name(std::wstring strId)
{
   std::wstring strName;
   for(std::wstring::iterator it = strId.begin(); it != strId.end(); it++)
   {
      if(!iswalpha(*it))
      {
         strName += L"_";
      }
      else
      {
         strName += *it;
      }
   }
   return strName;
}

void get_program_files_x86(std::wstring &wstr)
{

   wchar_t * lpszModuleFolder = (wchar_t *)malloc(MAX_PATH * sizeof(wchar_t) * 8);

   wchar_t * lpszModuleFilePath = (wchar_t *)malloc(MAX_PATH * sizeof(wchar_t) * 8);

   SHGetSpecialFolderPathW(
      NULL,
      lpszModuleFilePath,
      CSIDL_PROGRAM_FILES,
      FALSE);
   if(lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '\\'
      || lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '/')
   {
      lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] = '\0';
   }

   wcscpy(lpszModuleFolder,lpszModuleFilePath);

   wstr = lpszModuleFolder + wstr;

}

int check_soon_app_id(std::wstring strId)
{

   if(strId.length() <= 0)
      return 0;

   std::wstring strName = spa_app_id_to_app_name(strId);



   //{

   //   wchar_t * lpszCurFolder = (wchar_t *)malloc(MAX_PATH * sizeof(wchar_t) * 8);

   //   _wgetcwd(lpszCurFolder,MAX_PATH * 8 - 1);

   //   if(lpszCurFolder[wcslen(lpszCurFolder) - 1] == '\\'
   //      || lpszCurFolder[wcslen(lpszCurFolder) - 1] == '/')
   //   {
   //      lpszCurFolder[wcslen(lpszCurFolder) - 1] = '\0';
   //   }
   //   wcscat(lpszCurFolder,L"\\");

   //   std::wstring wstr;

   //   wstr = lpszCurFolder;

   //   wstr += strName;

   //   wstr += L".exe";

   //   STARTUPINFOW si;
   //   memset(&si,0,sizeof(si));
   //   si.cb = sizeof(si);
   //   si.dwFlags = STARTF_USESHOWWINDOW;
   //   si.wShowWindow = SW_SHOWNORMAL;
   //   PROCESS_INFORMATION pi;
   //   memset(&pi,0,sizeof(pi));

   //   if(::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
   //      NULL,NULL,FALSE,0,NULL,NULL,
   //      &si,&pi))
   //      return TRUE;


   //}


   //{

   //   int iCount = MAX_PATH * 8;

   //   wchar_t * lpszModuleFolder = (wchar_t *)malloc(iCount * sizeof(wchar_t));

   //   wchar_t * lpszModuleFilePath = (wchar_t *)malloc(iCount * sizeof(wchar_t));

   //   HMODULE hmodule = ::GetModuleHandleA(NULL);

   //   if(hmodule != NULL)
   //   {


   //      if(GetModuleFileNameW(hmodule,lpszModuleFilePath,iCount))
   //      {

   //         LPWSTR lpszModuleFileName;

   //         if(GetFullPathNameW(lpszModuleFilePath,iCount,lpszModuleFolder,&lpszModuleFileName))
   //         {

   //            lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   //            if(wcslen(lpszModuleFolder) > 0)
   //            {

   //               if(lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '/')
   //               {

   //                  lpszModuleFolder[wcslen(lpszModuleFolder) - 1] = '\0';

   //               }

   //            }




   //            wcscat(lpszModuleFolder,L"\\");

   //            std::wstring wstr;

   //            wstr = lpszModuleFolder;

   //            wstr += strName;

   //            wstr += L".exe";

   //            STARTUPINFOW si;
   //            memset(&si,0,sizeof(si));
   //            si.cb = sizeof(si);
   //            si.dwFlags = STARTF_USESHOWWINDOW;
   //            si.wShowWindow = SW_SHOWNORMAL;
   //            PROCESS_INFORMATION pi;
   //            memset(&pi,0,sizeof(pi));

   //            if(::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
   //               NULL,NULL,FALSE,0,NULL,NULL,
   //               &si,&pi))
   //               return TRUE;
   //         }

   //      }

   //   }

   //}


//   {
//
//      std::wstring wstr;
//
//      wstr = L"\\ca2\\";
//#ifdef _M_X64
//      wstr += L"stage\\x64\\";
//#else
//      wstr += L"stage\\x86\\";
//#endif
//
//      wstr += strName + L".exe";
//
//      get_program_files_x86(wstr);
//
//      STARTUPINFOW si;
//      memset(&si,0,sizeof(si));
//      si.cb = sizeof(si);
//      si.dwFlags = STARTF_USESHOWWINDOW;
//      si.wShowWindow = SW_SHOWNORMAL;
//      PROCESS_INFORMATION pi;
//      memset(&pi,0,sizeof(pi));
//
//      if(::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
//         NULL,NULL,FALSE,0,NULL,NULL,
//         &si,&pi))
//         return TRUE;
//
//   }

   {

      std::wstring wstr;

      wstr = L"\\ca2\\";
#ifdef _M_X64
      wstr += L"stage\\x64\\";
#else
      wstr += L"stage\\x86\\";
#endif

      wstr += strName + L".dll";

      get_program_files_x86(wstr);

      std::wstring wstrApp;

      wstrApp = L"\\ca2\\";
#ifdef _M_X64
      wstrApp += L"stage\\x64\\";
#else
      wstrApp += L"stage\\x86\\";
#endif

      wstrApp += L"app.exe";

      get_program_files_x86(wstrApp);

      if(file::exists(::u8(wstr).c_str()))
      {

         STARTUPINFOW si;
         memset(&si,0,sizeof(si));
         si.cb = sizeof(si);
         si.dwFlags = STARTF_USESHOWWINDOW;
         si.wShowWindow = SW_SHOWNORMAL;
         PROCESS_INFORMATION pi;
         memset(&pi,0,sizeof(pi));

         wstring wstrCmdLine = (L"\"" + wstrApp + L"\" : app=" + strId + L" build_number=installed").c_str();

         if(::CreateProcessW((wchar_t *)wstrApp.c_str(),(wchar_t *)wstrCmdLine.c_str(),
            NULL,NULL,FALSE,0,NULL,NULL,
            &si,&pi))
            return TRUE;

      }

   }

   return FALSE;

}


int check_spa_bin();
int check_spaadmin_bin();
int check_install_bin_set();

int check_spa_installation()
{

   if(spa_get_admin())
   {
      trace("Downloading spaadmin\r\n");
   }


   if(!check_spaadmin_bin())
      return 0;

   if(spa_get_admin())
   {
      trace("Downloading spa\r\n");
      trace(0.15);
   }

   if(!check_spa_bin())
      return 0;

   if(spa_get_admin())
   {
      trace("***Installing installer\r\n");
      trace("Checking installer\r\n");
      trace(0.25);
   }

   if(!check_install_bin_set())
      return 0;

   return 1;

}

int download_spa_bin();

int check_spa_bin()
{

   std::wstring wstr;

   wstr = L"\\ca2\\spa\\spa.exe";

   get_program_files_x86(wstr);

   if(!file::exists(u8(wstr.c_str())))
   {
      
      if(!spa_get_admin())
         return 0;
      
      if(!download_spa_bin())
      {
         return 0;
      }
      if(!file::exists(u8(wstr.c_str())))
      {
         return 0;
      }
   }

   return 1;

}

int download_spaadmin_bin();


int check_spaadmin_bin()
{

   std::wstring wstr;

   wstr = L"\\ca2\\spa\\spaadmin.exe";

   get_program_files_x86(wstr);

   if(!file::exists(u8(wstr.c_str())))
   {
      if(!download_spaadmin_bin())
      {
         return 0;
      }
      if(!file::exists(u8(wstr.c_str())))
      {
         return 0;
      }
   }

   return true;

}

std::string download_tmp_spa_bin();

int download_spa_bin()
{
   std::string strTempSpa = download_tmp_spa_bin();

   if(!file::exists(strTempSpa.c_str()))
   {

      return 0;

   }

   if(spa_get_admin())
   {

      std::wstring wstr;

      wstr = L"\\ca2\\spa\\spa.exe";

      get_program_files_x86(wstr);

      if(!::CopyFileW(u16(strTempSpa.c_str()).c_str(),wstr.c_str(),FALSE))
         return 0;

      if(!file::exists(u8(wstr.c_str())))
         return 0;

   }
   else
   {

      return 0;

   }


   return 1;

   

}

std::string download_tmp_spaadmin_bin();



int download_spaadmin_bin()
{

   std::string strTempSpa = download_tmp_spaadmin_bin();

   if(!file::exists(strTempSpa.c_str()))
   {

      return 0;

   }

   if(spa_get_admin())
   {

      std::wstring wstr;

      wstr = L"\\ca2\\spa\\spaadmin.exe";

      get_program_files_x86(wstr);

      if(!dir::mk(dir::name(u8(wstr.c_str())).c_str()))
         return 0;

      if(!::CopyFileW(u16(strTempSpa.c_str()).c_str(),wstr.c_str(),FALSE))
         return 0;

      if(!file::exists(u8(wstr.c_str())))
         return 0;

   }
   else
   {

      SHELLEXECUTEINFOW sei ={};

      wstring wstrFile = u16(strTempSpa.c_str());

      sei.cbSize =sizeof(SHELLEXECUTEINFOW);
      sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
      sei.lpVerb = L"RunAs";
      sei.lpFile = wstrFile.c_str();
      ::ShellExecuteExW(&sei);
      DWORD dwGetLastError = GetLastError();

      std::wstring wstr;

      wstr = L"\\ca2\\spa\\spaadmin.exe";

      get_program_files_x86(wstr);

      for(int i = 0; i < (19840 + 19770); i++)
      {

         if(file::exists(u8(wstr.c_str())))
            break;

         Sleep(84 + 77);

      }

      // Wait for the process to complete.
//      ::WaitForSingleObject(sei.hProcess,INFINITE);
  //    DWORD code;
    //  if(::GetExitCodeProcess(sei.hProcess,&code) == 0)
      //   return 0;

   }


   return 1;
}



std::string download_tmp_spaadmin_bin()
{
   
   std::string strTempSpa = get_temp_file_name("spaadmin", "exe");

   int iTry = 0;

   while(iTry <= 3)
   {

      if(ms_download("http://server.ca2.cc/spaadmin.exe", strTempSpa.c_str())
         && file::exists(strTempSpa.c_str())
         && file::length(strTempSpa.c_str()) > 0)
      {

         return strTempSpa;

      }

      iTry++;

   }


   return "";
   
}



std::string download_tmp_spa_bin()
{

   std::string strTempSpa = get_temp_file_name("spa","exe");

   int iTry = 0;

   while(iTry <= 3)
   {

      if(ms_download("http://server.ca2.cc/spa.exe",strTempSpa.c_str())
         && file::exists(strTempSpa.c_str())
         && file::length(strTempSpa.c_str()) > 0)
      {

         return strTempSpa;

      }

      iTry++;

   }


   return "";

}







bool is_file_ok(const char * path1,const char * pszTemplate,const char * pszFormatBuild)
{

   string strFormatBuild(pszFormatBuild);

   string strUrl;

   strUrl = "http://" + g_strVersion + "-server.ca2.cc/api/spaignition/md5?authnone&version=" + g_strVersion + "&stage=";
   strUrl += pszTemplate;
   strUrl += "&build=";
   strUrl += strFormatBuild;

   return file::exists(path1) && !_stricmp(file::md5(path1).c_str(),ms_get(strUrl.c_str()).c_str());

}

bool is_file_ok(const stringa & straPath,const stringa & straTemplate,stringa & straMd5,const string & strFormatBuild,int iMd5Retry)
{

   bool bOk = true;

   if(straPath.size() != straTemplate.size())
      return false;

   if(bOk)
   {

      for(int i = 0; i < straPath.size(); i++)
      {

         if(!file::exists(straPath[i].c_str()))
         {

            bOk = false;

            break;

         }

      }

   }

   if(iMd5Retry > 0 || straMd5.size() != straPath.size())
   {

      string strUrl;

      strUrl = "http://" + g_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + g_strVersion + "&stage=";
      strUrl += straTemplate.implode(",");
      strUrl += "&build=";
      strUrl += strFormatBuild;

      string strMd5List = ms_get(strUrl.c_str());

      straMd5.add_tokens(strMd5List.c_str(),",");

      if(straMd5.size() != straPath.size())
         return false;

      if(!bOk)
         return false;

   }

   for(int i = 0; i < straMd5.size(); i++)
   {

      if(_stricmp(file::md5(straPath[i].c_str()).c_str(), straMd5[i].c_str()) != 0)
         return false;

   }

   return true;

}


int check_install_bin_set()
{

//   trace().rich_trace("***Verifying installer");

   string strPath;

#ifdef WIN32

   string strPlatform = "x86";

   wstring wstrPath = u16((string("\\ca2\\install\\stage\\") + strPlatform + "\\app.install.exe").c_str());

   get_program_files_x86(wstrPath);

   strPath = u8(wstrPath.c_str());


#else

   throw "TODO";

#endif


   stringa straFile = install_get_plugin_base_library_list(g_strVersion);

   if(!::dir::exists(dir::name(strPath.c_str()).c_str()))
   {

      if(!::dir::mk(dir::name(strPath.c_str()).c_str()))
         return 0;

   }

   stringa straDownload;

   for(int iFile = 0; iFile < straFile.size(); iFile++)
   {

      string strFile = straFile[iFile];

      string strDownload = dir::path(dir::name(strPath.c_str()).c_str(),strFile.c_str());

      straDownload.push_back(strDownload);

   }

   stringa straMd5;


   string strFormatBuild = get_latest_build_number(g_strVersion.c_str());

   int iMd5Retry = 0;

   g_strBuild = strFormatBuild;

   g_strBuild = str_replace(g_strBuild.c_str()," ","_");

md5retry:

   if(!is_file_ok(straDownload,straFile,straMd5,strFormatBuild,iMd5Retry))
   {

      if(!spa_get_admin())
      {
         return 0;
      }


      if(straMd5.size() != straFile.size())
      {

         iMd5Retry++;

         if(iMd5Retry < 8)
            goto md5retry;

         return 0;

      }

#ifdef WINDOWSEX

      // first try to copy from current path (may be there is a version of app.install at the same folder).

      HMODULE hmodule = ::GetModuleHandle("aura.dll");

      if(hmodule != NULL)
      {

         string str = get_module_path(hmodule);

         if(str.length() > 0)
         {

            string strAuraDir = ::dir::name(str.c_str());

            for(int iFile = 0; iFile < straFile.size(); iFile++)
            {

               string strFile = ::dir::path(strAuraDir.c_str(),straFile[iFile].c_str());

               if(!file::exists(straDownload[iFile].c_str()) && file::exists(strFile.c_str()) && file::md5(strFile.c_str()) == straMd5[iFile].c_str())
               {

                  ::CopyFile(strFile.c_str(),straDownload[iFile].c_str(),false);

               }

            }


         }


      }

#endif

//      single_lock sl(pinstaller != NULL ? &pinstaller->m_mutexOmp : NULL);


      //{

      //   if(pinstaller != NULL)
      //   {

      //      pinstaller->m_daProgress.remove_all();
      //      pinstaller->m_daProgress.add(0.0);
      //      pinstaller->m_dAppInstallFileCount = straFile.get_size();
      //      pinstaller->m_dAppInstallProgressBase = 0.0;

      //   }
      //}

      trace(0.3);

//#pragma omp parallel for
      for(int iFile = 0; iFile < straFile.size(); iFile++)
      {

         string strFile = straFile[iFile];


         if(spa_get_admin())
         {
            trace("Downloading ");
            trace(::file::title(strFile.c_str()));
            trace("\r\n");

         }

         string strDownload = dir::path(dir::name(strPath.c_str()).c_str(),strFile.c_str());

         //if(pinstaller != NULL)
         //{
         //   sl.lock();
         //   pinstaller->m_daProgress.element_at_grow(omp_get_thread_num() + 1)  = 0.0;
         //   sl.unlock();
         //}

         if(!file::exists(strDownload.c_str()) || _stricmp(file::md5(strDownload.c_str()).c_str(), straMd5[iFile].c_str()) != 0)
         {

            //trace().rich_trace("***Downloading installer");

            string strUrlPrefix = "http://server.ca2.cc/ccvotagus/" + g_strVersion + "/" + g_strBuild + "/install/x86/";

            string strUrl;


            //if(pinstaller != NULL)
            //{

            //   set["int_scalar_source_listener"] = pinstaller;

            //}

            int iRetry;

            bool bFileNice;

            iRetry = 0;

            strUrl = strUrlPrefix + strFile + ".bz";

            bFileNice = false;


            //sl.lock();
            //::sockets::http_session * & psession = m_httpsessionptra.element_at_grow(omp_get_thread_num() + 1);
            //sl.unlock();


            while(iRetry < 8 && !bFileNice)
            {




               if(ms_download(strUrl.c_str(),(strDownload + ".bz").c_str()))
               {

                  bzuncompress((strDownload).c_str(),(strDownload + ".bz").c_str());

                  if(file::exists(strDownload.c_str()) && _stricmp(file::md5(strDownload.c_str()).c_str(), straMd5[iFile].c_str()) == 0)
                  {

                     bFileNice = true;

                  }


               }

               iRetry++;

            }

            if(!bFileNice)
            {

               // failed by too much retry in any number of the files already downloaded :
               // so, return failure (no eligible app.install.exe file).
               //return "";

            }
            else
            {
               //if(pinstaller != NULL)
               //{
               //   sl.lock();
               //   pinstaller->m_daProgress.element_at_grow(omp_get_thread_num() + 1)  = 0.0;
               //   pinstaller->m_dAppInstallProgressBase += 1.0;
               //   sl.unlock();
               //}

            }


         }
         else
         {
            //if(pinstaller != NULL)
            //{
            //   sl.lock();
            //   pinstaller->m_daProgress.element_at_grow(omp_get_thread_num() + 1)  = 0.0;
            //   pinstaller->m_dAppInstallProgressBase += 1.0;
            //   sl.unlock();
            //}
         }


         if(spa_get_admin())
         {
            trace(0.3 + ((((double)iFile + 1.0) * (0.84 - 0.3)) / ((double)straFile.size())));
         }

      }

   }


   return 1;

}






string get_latest_build_number(const char * pszVersion)
{

   if(g_strBuild.length() > 0)
   {
      return g_strBuild;
   }

   string strBuildNumber;

   string strSpaIgnitionBaseUrl;

   string strVersion(pszVersion);

   if(file::get_contents("C:\\ca2\\config\\system\\ignition_server.txt").length() > 0)
   {

      strSpaIgnitionBaseUrl = "https://" + file::get_contents("C:\\ca2\\config\\system\\ignition_server.txt") + "/api/spaignition";

   }
   else if(pszVersion != NULL && !strcmp(pszVersion,"basis"))
   {

      strSpaIgnitionBaseUrl = "http://basis-server.ca2.cc/api/spaignition";

   }
   else if(pszVersion != NULL && !strcmp(pszVersion,"stage"))
   {

      strSpaIgnitionBaseUrl = "http://stage-server.ca2.cc/api/spaignition";

   }
   else
   {

      if(g_strVersion == "basis")
      {

         strVersion = "basis";

         strSpaIgnitionBaseUrl = "http://basis-server.ca2.cc/api/spaignition";

      }
      else
      {

         strVersion = "stage";

         strSpaIgnitionBaseUrl = "http://stage-server.ca2.cc/api/spaignition";

      }

   }

   int iRetry = 0;

RetryBuildNumber:

   if(iRetry > 10)
   {

      return "";

   }

   iRetry++;

   strBuildNumber = ms_get((strSpaIgnitionBaseUrl + "/query?node=build&version=" + strVersion).c_str());
   //strBuildNumber = ms_get((strSpaIgnitionBaseUrl + "/ca2_build_number?authnon").c_str());

   trim(strBuildNumber);

   if(strBuildNumber.length() != 19)
   {

      Sleep(184 * iRetry);

      goto RetryBuildNumber;

   }

   return strBuildNumber;

}




bool app_install_send_short_message(const char * psz,bool bLaunch,const char * pszBuild)
{

#ifdef METROWIN

   throw "todo";

#else

   small_ipc_tx_channel txchannel;

   install_launcher launcher("", "");

   if(!txchannel.open("core/spaboot_install",bLaunch ? &launcher : NULL))
      return false;

   txchannel.send(psz,false);

#endif

   return true;

}




//#include "aura/aura/aura_launcher.cpp"
#include "axis/install_launcher.cpp"
#include "aura/aura/aura_small_ipc_channel.cpp"
//#include "aura/aura/aura_small_ipc_channel.cpp"
#include "aura/os/windows/windows_small_ipc_channel.cpp"



bool launcher::start()
{

   if(!ensure_executable())
      return false;

   string strPath(get_executable_path());

   string strDir(dir::name(strPath.c_str()));

   string strParams = get_params();

   call_async(strPath.c_str(),strParams.c_str(),strDir.c_str(),SW_HIDE);

   return true;

}


int call_async(
   const char * pszPath,
   const char * pszParam,
   const char * pszDir,
   int iShow)
{

   SHELLEXECUTEINFOA infoa;

   memset(&infoa,0,sizeof(infoa));

   infoa.cbSize         = sizeof(infoa);
   infoa.lpFile         = pszPath;
   infoa.lpParameters   = pszParam;
   infoa.lpDirectory    = pszDir;
   infoa.nShow          = iShow;

   int iOk = ::ShellExecuteExA(&infoa);

   return iOk;

}



void app_install_call_sync(const char * szParameters,const char * pszBuild)
{
   bool bLaunch;

   if(_stricmp(szParameters,"exit") == 0
      || _stricmp(szParameters,"quit") == 0)
   {
      bLaunch = false;
   }
   else
   {
      bLaunch = true;
   }

   app_install_send_short_message(szParameters,bLaunch,pszBuild);

}



void start_app_install_in_context()
{

   install_launcher launcher("","");

   launcher.start_in_context();

}



void install_launcher::start_in_context()
{

   if(!ensure_executable())
      return;

   string strDir = dir::name(m_strPath.c_str());

   wstring wstrDir = u16(strDir.c_str());

   wstring wstr = u16(m_strPath.c_str());

   STARTUPINFOW si;
   memset(&si,0,sizeof(si));
   si.cb = sizeof(si);
   si.dwFlags = 0;
   si.wShowWindow = SW_HIDE;
   PROCESS_INFORMATION pi;
   memset(&pi,0,sizeof(pi));

   ::CreateProcessW(NULL,(wchar_t *)wstr.c_str(), NULL,NULL,FALSE,0,NULL,wstrDir.c_str(), &si,&pi);

   Sleep(1984);

}


int register_spa_file_type()
{
   HKEY hkey;

   wstring extension=L".spa";                     // file extension
   wstring desc=L"spafile";          // file type description
   wstring content_type = L"application/x-spa";

   std::wstring app;

   app = L"\\ca2\\spa\\spa.exe";

   get_program_files_x86(app);

   std::wstring icon(app);

   app = L"\"" + app + L"\"" + L" %1";
   icon = L"\"" + icon + L"\",107";

   wstring action=L"Open";

   wstring sub=L"\\shell\\";

   wstring path=L"spafile\\shell\\open\\command";


   // 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.002
   if(RegCreateKeyExW(HKEY_CLASSES_ROOT,extension.c_str(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey,L"",0,REG_SZ,(BYTE*)desc.c_str(),desc.length()*sizeof(wchar_t)); // default vlaue is description of file extension
   RegSetValueExW(hkey,L"ContentType",0,REG_SZ,(BYTE*)content_type.c_str(),content_type.length()*sizeof(wchar_t)); // default vlaue is description of file extension
   RegCloseKey(hkey);



   // 2: Create Subkeys for action ( "Open with my program" )
   // HKEY_CLASSES_ROOT\.002\Shell\\open with my program\\command
   if(RegCreateKeyExW(HKEY_CLASSES_ROOT,path.c_str(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey,L"",0,REG_SZ,(BYTE*)app.c_str(),app.length()*sizeof(wchar_t));
   RegCloseKey(hkey);


   path=L"spafile\\DefaultIcon";

   if(RegCreateKeyExW(HKEY_CLASSES_ROOT,path.c_str(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0) != ERROR_SUCCESS)
   {
      printf("Could not create or open a registrty key\n");
      return 0;
   }
   RegSetValueExW(hkey,L"",0,REG_SZ,(BYTE*)icon.c_str(),icon.length()*sizeof(wchar_t));
   RegCloseKey(hkey);

   std::wstring wstr;

   wstr = L"\\ca2\\spa\\spa_register.txt";

   get_program_files_x86(wstr);

   int iRetry = 9;

   while(!file::exists(u8(wstr.c_str())) && iRetry > 0)
   {

      dir::mk(dir::name(u8(wstr.c_str())).c_str());

      file::put_contents(u8(wstr.c_str()).c_str(),"");

      iRetry--;

      Sleep(84);

   }

   return 1;

}



//#include "framework.h"




