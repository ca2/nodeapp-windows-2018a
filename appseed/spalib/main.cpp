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
void start_program_files_spa_admin();
void defer_start_program_files_spa_admin();
bool low_is_spaadmin_running();

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

   wchar_t * pwz = (wchar_t *)malloc(4906);

   DWORD dwSize = ::GetModuleFileNameW(hmodule,pwz,4096);

   wstrPath = pwz;

   free(pwz);

   return ::u8(wstrPath.c_str());

}

#endif

#define INSTALL_BIN_SPA -1977
#define INSTALL_BIN_SPAADMIN -1984
#define OP_INSTALL_SPA 1
#define OP_INSTALL_SET 2

class install_bin_item
{
public:


   string         m_strUrlPrefix;
   string         m_strPath;
   string         m_strFile;
   LONG *         m_plong;
   LONG *         m_plongOk;
   DWORD          m_dwThreadId;
   string         m_strMd5;
   string         m_strPlatform;
   LONG           m_lTotal;
   int            m_iOp;

   install_bin_item(string strFile,LONG * plong,LONG lTotal,LONG * plongOk):
      m_strFile(strFile),
      m_plong(plong),
      m_lTotal(lTotal),
      m_plongOk(plongOk)
   {
      m_iOp = OP_INSTALL_SPA;
      ::CreateThread(NULL,0,&install_bin_item::proc,this,0,&m_dwThreadId);
   }


   install_bin_item(string strUrlPrefix, string strPath, string strFile,LONG * plong, string strMd5, string strPlatform, LONG lTotal) :
      m_strUrlPrefix(strUrlPrefix),
      m_strPath(strPath),
      m_strFile(strFile),
      m_plong(plong),
      m_strMd5(strMd5),
      m_strPlatform(strPlatform),
      m_lTotal(lTotal)
   {
      m_iOp = OP_INSTALL_SET;
      ::CreateThread(NULL,0,&install_bin_item::proc,this,0,&m_dwThreadId);

   }


   static DWORD CALLBACK proc(LPVOID lp)
   {
      install_bin_item * pitem = (install_bin_item *)lp;
      try
      {
         pitem->run();
      }
      catch(...)
      {
         (*pitem->m_plong)--;
      }
      delete pitem;
      return 0;
   }


   void run();

   void progress(double dRate = 1.0)
   {

      if(spa_get_admin())
      {
         if(m_iOp == OP_INSTALL_SPA)
         {
            trace(0.05 + ((((double)m_lTotal - (double)(*m_plong)) * (0.25 - 0.05)) / ((double)m_lTotal)));
         }
         else if(m_iOp == OP_INSTALL_SET)
         {
            trace(0.3 + ((((double)m_lTotal - (double)(*m_plong)) * (0.84 - 0.3)) / ((double)m_lTotal)));
         }
      }

   }

   void op_spa();

   void op_set();
   
};


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

   if(::file::exists("C:\\ca2\\config\\spa\\beg_debug_box.txt"))
   {

      string str;

      if(spa_get_admin())
      {

         str = "zzzAPPzzz spaadmin : ";

      }
      else
      {

         str = "zzzAPPzzz spa : ";

      }

      str += lpCmdLine;

      ::MessageBoxA(NULL,str.c_str(),"zzzAPPzzz spa",MB_ICONINFORMATION);

   }


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


   spaadmin_mutex smutex;

   if(smutex.already_exists())
   {

      return 0;

   }

   if(!smutex.is_ok())
   {

      return 0;

   }

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


   trace(":::::Thank you\r\n");
   trace("***Thank you\r\n");
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

   ::PostMessage(g_hwnd,WM_QUIT,0,0);

   return g_iRet;

}


int spalib_main2()
{

   int iTry = 1440;

   while(!check_spa_installation())
   {

      defer_start_program_files_spa_admin();

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

   if(iFind >= 0)
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
   else
   {

      strId = u16(get_app_id(wstr.substr(iFind1 + 1)));

      if(strId.length() <= 0)
      {
         return 1;
      }

      HMODULE hmoduleUser32 = ::LoadLibrary("User32");
      g_pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter) ::GetProcAddress(hmoduleUser32,"ChangeWindowMessageFilter");


   }

   string strCommandLine;


   strCommandLine = " : app=" + u8(strId);

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

   strCommand += " enable_desktop_launch";

   app_install_call_sync(strCommand.c_str(),"");

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

   int iFind = wstr.find(L" : ",iFind1 + 1);

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
      if(*it == L'-' || *it == L'/' || *it == L'\\')
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

   wcscpy(lpszModuleFilePath,_wgetenv(L"PROGRAMFILES(X86)"));

   if(wcslen(lpszModuleFilePath) == 0)
   {

      SHGetSpecialFolderPathW(
         NULL,
         lpszModuleFilePath,
         CSIDL_PROGRAM_FILES,
         FALSE);

   }
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
#if defined(_M_IX86)
      wstr += L"stage\\x86\\";
#else
      wstr += L"stage\\x64\\";
#endif

      wstr += strName + L".dll";

      get_program_files_x86(wstr);

      std::wstring wstrApp;

      wstrApp = L"\\ca2\\";
#if defined(_M_IX86)
      wstrApp += L"stage\\x86\\";
#else
      wstrApp += L"stage\\x64\\";
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

      trace("Downloading\r\n");

   }

   stringa straFile;

   straFile.add("spaadmin");
   straFile.add("spa");
   straFile.add("install_bin_set");

   LONG lTotal = straFile.size();
   LONG lCount = lTotal;
   LONG lOk = 0;

   for(int iFile = 0; iFile < straFile.size(); iFile++)
   {

      new install_bin_item(straFile[iFile],&lCount,lTotal, &lOk);

   }

   int iRetry = 0;

   while(lCount > 0 && iRetry < ((84 + 77) * 4))
   {
      Sleep(84);
      iRetry++;
   }

   Sleep(84);


   if(lOk != lTotal)
      return 0;

   return 1;

}

int download_spa_bin();

int check_spa_bin()
{

   std::wstring wstr;

#if defined(_M_IX86)

   wstr = L"\\ca2\\spa\\x86\\spa.exe";

#else

   wstr = L"\\ca2\\spa\\x64\\spa.exe";

#endif

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

#if defined(_M_IX86)

   wstr = L"\\ca2\\spa\\x86\\spaadmin.exe";

#else

   wstr = L"\\ca2\\spa\\x64\\spaadmin.exe";

#endif

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

#if defined(_M_IX86)

      wstr = L"\\ca2\\spa\\x86\\spa.exe";

#else

      wstr = L"\\ca2\\spa\\x64\\spa.exe";

#endif

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

#if defined(_M_IX86)

      wstr = L"\\ca2\\spa\\x86\\spaadmin.exe";

#else

      wstr = L"\\ca2\\spa\\x64\\spaadmin.exe";

#endif

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

      if(!low_is_spaadmin_running())
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

#if defined(_M_IX86)

         wstr = L"\\ca2\\spa\\x86\\spaadmin.exe";

#else

         wstr = L"\\ca2\\spa\\x64\\spaadmin.exe";

#endif

         get_program_files_x86(wstr);

         DWORD dwExitCode = 0;

         for(int i = 0; i < (19840 + 19770); i++)
         {

            if(::GetExitCodeProcess(sei.hProcess,&dwExitCode))
            {

               if(dwExitCode != STILL_ACTIVE)
               {

                  break;

               }

            }
            else
            {

               Sleep(1984 + 1977);

               break;

            }

            if(file::exists(u8(wstr.c_str())))
               break;

            Sleep(84 + 77);

         }

         ::CloseHandle(sei.hProcess);

         // Wait for the process to complete.
         //      ::WaitForSingleObject(sei.hProcess,INFINITE);
         //    DWORD code;
         //  if(::GetExitCodeProcess(sei.hProcess,&code) == 0)
         //   return 0;

      }

   }


   return 1;
}



std::string download_tmp_spaadmin_bin()
{

   std::string strTempSpa = get_temp_file_name("spaadmin","exe");

   int iTry = 0;

   while(iTry <= 3)
   {

#if defined(_M_IX86)
      if(ms_download("http://server.ca2.cc/x86/spaadmin.exe", strTempSpa.c_str())
#else
      if(ms_download("http://server.ca2.cc/x64/spaadmin.exe",strTempSpa.c_str())
#endif
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

#if defined(_M_IX86)

      if(ms_download("http://server.ca2.cc/x86/spa.exe",strTempSpa.c_str())

#else

      if(ms_download("http://server.ca2.cc/x64/spa.exe",strTempSpa.c_str())

#endif

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
      strUrl += "&platform=";
#if defined(_M_IX86)
      strUrl += "win32";
#else
      strUrl += "win64";
#endif

      string strMd5List = ms_get(strUrl.c_str());

      straMd5.add_tokens(strMd5List.c_str(),",");

      if(straMd5.size() != straPath.size())
      {

         return false;

      }

      if(!bOk)
      {

         return false;

      }

   }

   for(int i = 0; i < straMd5.size(); i++)
   {

      if(_stricmp(file::md5(straPath[i].c_str()).c_str(),straMd5[i].c_str()) != 0)
      {

         return false;

      }

   }

   return true;

}


void install_bin_item::op_spa()
{
   
   if(m_strFile == "spaadmin")
   {

      if(check_spaadmin_bin())
      {

         InterlockedIncrement(m_plongOk);

      }

   }
   else if(m_strFile == "spa")
   {

      if(check_spa_bin())
      {

         InterlockedIncrement(m_plongOk);

      }

   }
   else if(m_strFile == "install_bin_set")
   {

      if(check_install_bin_set())
      {

         InterlockedIncrement(m_plongOk);

      }

   }



}


void install_bin_item::op_set()
{

   string strUrlPrefix = m_strUrlPrefix;

   string strPath = m_strPath;

   string strFile = m_strFile;

   string strMd5 = m_strMd5;

   string strPlatform = m_strPlatform;

   string strDownload = dir::path(dir::name(strPath.c_str()).c_str(),strFile.c_str());

   if(!file::exists(strDownload.c_str()) || _stricmp(file::md5(strDownload.c_str()).c_str(),strMd5.c_str()) != 0)
   {

      string strUrl;

      int iRetry;

      bool bFileNice;

      iRetry = 0;

      strUrl = strUrlPrefix + strFile + ".bz";

      bFileNice = false;

      while(iRetry < 8 && !bFileNice)
      {

         if(ms_download(strUrl.c_str(),(strDownload + ".bz").c_str()))
         {

            bzuncompress((strDownload).c_str(),(strDownload + ".bz").c_str());

            if(file::exists(strDownload.c_str()) && _stricmp(file::md5(strDownload.c_str()).c_str(),strMd5.c_str()) == 0)
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

   }

}

void install_bin_item::run()
{
   
   if(m_iOp == OP_INSTALL_SPA)
   {
      
      op_spa();

   }
   else if(m_iOp == OP_INSTALL_SET)
   {

      op_set();

   }


   InterlockedDecrement(m_plong);

   progress();

}





int check_install_bin_set()
{

   string strPath;

#if defined(_M_IX86)

   string strPlatform = "x86";

#else

   string strPlatform = "x64";

#endif

   wstring wstrPath = u16((string("\\ca2\\install\\stage\\") + strPlatform + "\\app.install.exe").c_str());

   get_program_files_x86(wstrPath);

   strPath = u8(wstrPath.c_str());

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

      LONG lTotal = straFile.size();
      LONG lCount = lTotal;

      trace("Downloading install bin set\r\n");

      string strUrlPrefix = "http://server.ca2.cc/ccvotagus/" + g_strVersion + "/" + g_strBuild + "/install/" + strPlatform + "/";

      //#pragma omp parallel for
      for(int iFile = 0; iFile < straFile.size(); iFile++)
      {

         new install_bin_item(strUrlPrefix,strPath,straFile[iFile],&lCount,straMd5[iFile],strPlatform,lTotal);

      }

      int iRetry = 0;

      while(lCount > 0 && iRetry < ((84 + 77) * 10))
      {
         Sleep(84);
         iRetry++;
      }

      Sleep(84);

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

   install_launcher launcher("","");

   const char * pszChannel;

   // "core/spaboot_install"

#if defined(_M_IX86)

   pszChannel = "::ca2::fontopus::ca2_spaboot_install_x86::7807e510-5579-11dd-ae16-0800200c7784";

#else

   pszChannel = "::ca2::fontopus::ca2_spaboot_install_x64::7807e510-5579-11dd-ae16-0800200c7784";

#endif

   if(!txchannel.open(pszChannel,bLaunch ? &launcher : NULL))
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

   ::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),NULL,NULL,FALSE,0,NULL,wstrDir.c_str(),&si,&pi);

   Sleep(1984);

}


int register_spa_file_type()
{
   HKEY hkey;

   wstring extension=L".spa";                     // file extension
   wstring desc=L"spafile";          // file type description
   wstring content_type = L"application/x-spa";

   std::wstring app;

#if defined(_M_IX86)

   app = L"\\ca2\\spa\\x86\\spa.exe";

#else

   app = L"\\ca2\\spa\\x64\\spa.exe";

#endif

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





void start_program_files_spa_admin()
{

   SHELLEXECUTEINFOW sei ={};

   std::wstring wstr;

#if defined(_M_IX86)

   wstr = L"\\ca2\\spa\\x86\\spaadmin.exe";

#else

   wstr = L"\\ca2\\spa\\x64\\spaadmin.exe";

#endif

   get_program_files_x86(wstr);

   if(!::file::exists(u8(wstr)))
   {

      return;

   }

   sei.cbSize =sizeof(SHELLEXECUTEINFOW);
   sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
   sei.lpVerb = L"RunAs";
   sei.lpFile = wstr.c_str();
   ::ShellExecuteExW(&sei);
   DWORD dwGetLastError = GetLastError();

}




void defer_start_program_files_spa_admin()
{

   if(low_is_spaadmin_running())
   {

      return;

   }

   start_program_files_spa_admin();

}


bool low_is_spaadmin_running()
{

   spaadmin_mutex smutex;

   return smutex.already_exists();

}


