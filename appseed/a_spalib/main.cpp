#include "a_spalib.h"
#include <shellapi.h>
#include <shlobj.h>


void kill_process(string strImageName);

void gdiplus_start();
void gdiplus_end();

void run_vcredist();

#define CLASS_DECL_AURA
#define CLASS_DECL_AXIS
typedef HWND oswindow;

//#include "aura/aura/aura_launcher.h"
//#include "aura/aura/aura_small_ipc_channel.h"
#include "axis/app_install_launcher.h"


bool app_install_send_short_message(const char * psz,bool bLaunch,const char * pszBuild);
void app_install_call_sync(const char * szParameters,const char * pszBuild);
int register_spa_file_type();
void start_program_files_spa_admin();
void defer_start_program_files_spa_admin();
bool low_is_spaadmin_running();
void get_system_locale_schema(string & strLocale,string & strSchema);


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

   DWORD dwSize = ::GetModuleFileNameW(hmodule,pwz,4096);

   wstrPath = pwz;

   free(pwz);

   return wstrPath.c_str();

}

#endif

#define INSTALL_BIN_SPA -1977
#define INSTALL_BIN_SPAADMIN -1984
#define OP_INSTALL_SPA 1
#define OP_INSTALL_SET 2

class install_bin_item :
   virtual public object
{
public:

   a_spa *        m_paspa;
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

   install_bin_item(a_spa * paspa, string strFile,LONG * plong,LONG lTotal,LONG * plongOk):
      object(paspa),
      m_paspa(paspa),
      m_strFile(strFile),
      m_plong(plong),
      m_lTotal(lTotal),
      m_plongOk(plongOk)
   {
      m_iOp = OP_INSTALL_SPA;
      ::CreateThread(NULL,0,&install_bin_item::proc,this,0,&m_dwThreadId);
   }


   install_bin_item(a_spa * paspa,string strUrlPrefix,string strPath,string strFile,LONG * plong,string strMd5,string strPlatform,LONG lTotal):
      object(paspa),
      m_paspa(paspa),
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
            m_paspa->trace(0.05 + ((((double)m_lTotal - (double)(*m_plong)) * (0.25 - 0.05)) / ((double)m_lTotal)));
         }
         else if(m_iOp == OP_INSTALL_SET)
         {
            m_paspa->trace(0.3 + ((((double)m_lTotal - (double)(*m_plong)) * (0.84 - 0.3)) / ((double)m_lTotal)));
         }
      }

   }

   void op_spa();

   void op_set();
   
};


bool app_install_send_short_message(const char * psz,bool bLaunch,const char * pszBuild);

//using namespace std;



DWORD g_dwMain2;
DWORD g_iRet;
//string g_strBuild;

//int starter_start(const char * pszId);
//int run_file(const char * pszFile,int nCmdShow);
//int run_uninstall_run(const char * lpCmdLine,int nCmdShow);
//int ca2_app_install_run(const char * psz,const char * pszParam1,const char * pszParam2,DWORD & dwStartError,bool bSynch);





int32_t a_spa::run()
{


   if(::file_exists_dup("C:\\ca2\\config\\spa\\beg_debug_box.txt"))
   {

      string str;

      if(spa_get_admin())
      {

         str = "zzzAPPzzz a_spaadmin : ";

      }
      else
      {

         str = "zzzAPPzzz a_spa : ";

      }

      str += string(::GetCommandLineW());

      ::MessageBoxA(NULL,str.c_str(),"zzzAPPzzz a_spa",MB_ICONINFORMATION);

   }


   //g_pgdiplusStartupInput     = new Gdiplus::GdiplusStartupInput();
   //g_pgdiplusStartupOutput    = new Gdiplus::GdiplusStartupOutput();
   //g_gdiplusToken             = NULL;
   //g_gdiplusHookToken         = NULL;

   //g_pgdiplusStartupInput->SuppressBackgroundThread = TRUE;

   //Gdiplus::Status statusStartup = GdiplusStartup(&g_gdiplusToken,g_pgdiplusStartupInput,g_pgdiplusStartupOutput);

   //if(statusStartup != Gdiplus::Ok)
   //{


   //   return 0;

   //}




   //statusStartup = g_pgdiplusStartupOutput->NotificationHook(&g_gdiplusHookToken);


   //if(statusStartup != Gdiplus::Ok)
   //{


   //   return 0;

   //}


#if CA2_PLATFORM_VERSION == CA2_BASIS

   m_strVersion = "basis";

#else

   m_strVersion = "stage";

#endif

//   g_hinstance = hInstance;

   if(spa_get_admin())
   {

      g_iRet = spaadmin_main();

   }
   else
   {

      g_iRet = spa_main();

   }


   return g_iRet;

}

int a_spa::spa_main()
{

   spa_mutex mutex;

   if (mutex.already_exists())
   {

      return -34;

   }

   string str(::GetCommandLineW());

   if(str.find(" install ") < 0)
   {

      if(check_soon_launch())
         return 0;

   }

   if(!show_spa_window())
   {

      return -1;

   }

   if(!spa_main_start())
   {

      return -2;

   }

   while(GetMessage(&g_msg,NULL,0,0))
   {

      TranslateMessage(&g_msg);

      DispatchMessage(&g_msg);

   }

   {

      HWND hwnd = m_hwnd;

      ::ShowWindow(hwnd,SW_HIDE);

      ::DestroyWindow(hwnd);

   }

   return g_iRet;

}





int a_spa::spaadmin_main()
{


   spaadmin_mutex smutex;

   if(smutex.already_exists())
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
   //trace(0.84);


   start_app_install_in_context();


   trace(":::::Thank you\r\n");
   trace("***Thank you\r\n");
   trace("Thank you\r\n");
   trace(1.0);

   return 0;

}




int a_spa::spa_main_start()
{

   if(!::CreateThread(NULL,0,&::a_spa::spa_main_proc,this,0,&g_dwMain2))
   {

      return 0;

   }

   return 1;

}



DWORD WINAPI a_spa::spa_main_proc(LPVOID lpvoid)
{

   a_spa * paspa = (a_spa *) lpvoid;

   g_iRet = paspa->spalib_main2();

   ::PostMessage(paspa->m_hwnd,WM_QUIT,0,0);

   return g_iRet;

}


int a_spa::spalib_main2()
{

   int iFullInstallationMaxTryCount = 3;

   int iFullInstallationTryCount = 0;

   while(iFullInstallationTryCount < iFullInstallationMaxTryCount)
   {

      iFullInstallationTryCount++;

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


      while (!check_berdge())
      {
         Sleep(1984 + 1977 + 1951);
      }

      string strId;

      string wstr(::GetCommandLineW());

      string strParams;

      int iFind1 = 0;

      if(wstr[0] == '\"')
      {

         iFind1= wstr.find('\"',1);

      }

      int iFind = wstr.find(" : ",iFind1 + 1);

      if(iFind >= 0)
      {
         strParams = wstr.substr(iFind);
         iFind = wstr.find("app=",iFind);
         if(iFind >= 0)
         {
            int iEnd = wstr.find(" ",iFind);
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

         strId = get_app_id(directrix()->m_varTopicFile);

         if(strId.length() <= 0)
         {
            return 1;
         }

         HMODULE hmoduleUser32 = ::LoadLibrary("User32");
//         g_pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter) ::GetProcAddress(hmoduleUser32,"ChangeWindowMessageFilter");


      }

      do_spa(strId, strParams);


   }

   return 1;

}

int a_spa::do_spa(const char * pszId, const char * pszParams)
{

   string strId(pszId);

   string strParams(pszParams);

   string strCommandLine;

   if (strParams.length() > 0)
   {

      strCommandLine = strParams;

   }
   else
   {

      string strLocale;
      string strSchema;

      get_system_locale_schema(strLocale, strSchema);

      strCommandLine = " : app=" + strId;

      strCommandLine += " install";
      strCommandLine += " locale=" + strLocale;
      strCommandLine += " schema=" + strSchema;
      strCommandLine += " version=" + m_strVersion;

   }

   strCommandLine += " ";

   string strCommand;

   strCommand = "synch_spaadmin:";

   strCommand += "starter_start:";

   strCommand += strCommandLine;

   bool bBackground = true;

   if (bBackground)
   {

      strCommand += " background";

   }

   app_install_call_sync(strCommand.c_str(), "");

   if (!check_soon_app_id(strId))
      return 0;

   return 1;

}


int a_spa::check_soon_launch()
{

   string strId;

   string wstr = ::GetCommandLineW();

   int iFind1 = 0;

   if(wstr[0] == '\"')
   {

      iFind1= wstr.find('\"',1);

   }

   int iFind = wstr.find(" : ",iFind1 + 1);

   if(iFind < 0)
   {
      
      string strFile = wstr.substr(iFind1 + 1);

      strFile.trim();

      if(check_soon_file_launch(strFile))
      {

         return 1;

      }

   }
   else
   {
      
      string wstrRequest = wstr.substr(iFind + 3);

      string wstrValue;

      if(get_command_line_param(wstrValue,wstrRequest,"install"))
      {

         if(get_command_line_param(strId,wstrRequest,"enable_desktop_launch") && strId.length() > 0)
         {

            return check_soon_app_id(strId);

         }

         if(get_command_line_param(strId,wstrRequest,"app") && strId.length() > 0)
         {

            return check_soon_app_id(strId);

         }

      }
      else
      {

         if(get_command_line_param(strId,wstrRequest,"app") && strId.length() > 0)
         {

            return check_soon_app_id(strId);

         }
      }

   }

   if(strId.empty())
      return FALSE;

   return check_soon_app_id(strId);

}



string a_spa::get_app_id(string wstr)
{

   if(wstr.length() <= 0)
   {

      return "";

   }

   wstr.trim();
  
   ::str::trim_any_quotes(wstr);

   wstr.trim();

   if(wstr.length() <= 0)
   {

      return "";

   }

   ::xml::document doc(get_app());

   ::string strPath = wstr.c_str();

   ::string strContents = file_as_string_dup(strPath.c_str());

   if(!doc.load(strContents.c_str()))
   {

      return "";

   }

   sp(xml::node) pnode = doc.get_root();

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

int a_spa::check_soon_file_launch(string wstr)
{


   return check_soon_app_id(u16(get_app_id(wstr.c_str()).c_str()));

}


string a_spa::spa_app_id_to_app_name(string strId)
{
   string strName;
   for(index i = 0; i < strId.length(); i++)
   {
      if(strId[i] == L'-' || strId[i] == L'/' || strId[i] == L'\\')
      {
         strName += L"_";
      }
      else
      {
         strName += strId[i];
      }
   }
   return strName;
}


int a_spa::check_soon_app_id(string strId)
{

   if(strId.length() <= 0)
      return 0;

   string strName = spa_app_id_to_app_name(strId);

   {

      string strDll = dir::stage() / strName + ".dll";

      string strApp = dir::stage() / "app.exe";

      if(file_exists_dup(strDll))
      {

         STARTUPINFOW si;
         memset(&si,0,sizeof(si));
         si.cb = sizeof(si);
         si.dwFlags = STARTF_USESHOWWINDOW;
         si.wShowWindow = SW_SHOWNORMAL;
         PROCESS_INFORMATION pi;
         memset(&pi,0,sizeof(pi));

         wstring wstrCmdLine = u16("\"" + string(strApp) + "\" : app=" + strId + " build_number=installed enable_desktop_launch=" + strId);

         wstring wstrApp(strApp);

         if(::CreateProcessW((wchar_t *)wstrApp.c_str(),(wchar_t *)wstrCmdLine.c_str(),
            NULL,NULL,FALSE,0,NULL,NULL,
            &si,&pi))
            return TRUE;

      }

   }

   return FALSE;

}


int a_spa::check_spa_installation()
{

   if(spa_get_admin())
   {

      trace("Downloading\r\n");

   }

   stringa straFile;

   straFile.add("spaadmin");
   straFile.add("spa");
   straFile.add("vcredist");

   if(!file_exists_dup(dir::a_spa() / "no_install_bin_set.txt"))
   {

      straFile.add("install_bin_set");

   }

   LONG lTotal = straFile.size();
   LONG lCount = lTotal;
   LONG lOk = 0;

   for(int iFile = 0; iFile < straFile.size(); iFile++)
   {

      new install_bin_item(this,straFile[iFile],&lCount,lTotal, &lOk);

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

int a_spa::check_berdge()
{

   string strDll = dir::stage() / "app_core_berdge.dll";

   string strId = "app-core/berdge";

   if (file_exists_dup(strDll))
   {

      string strApp = dir::stage() / "app.exe";

      STARTUPINFOW si;
      memset(&si, 0, sizeof(si));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_SHOWNORMAL;
      PROCESS_INFORMATION pi;
      memset(&pi, 0, sizeof(pi));

      wstring wstrCmdLine = u16("\"" + string(strApp) + "\" : app=" + strId + " build_number=installed enable_desktop_launch=" + strId);

      wstring wstrApp(strApp);

      if (::CreateProcessW((wchar_t *)wstrApp.c_str(), (wchar_t *)wstrCmdLine.c_str(),
         NULL, NULL, FALSE, 0, NULL, NULL,
         &si, &pi))
         return TRUE;

   }
   else
   {

      do_spa(strId);

   }

   return 1;

}



int a_spa::check_vcredist()
{

   string str = ::path::vcredist();

   if (!file_exists_dup(str))
   {

      if (!spa_get_admin())
      {

         return 0;

      }

      if (!download_vcredist())
      {

         return 0;

      }

      if (!file_exists_dup(str))
      {

         return 0;

      }

      run_vcredist();

   }

   return 1;

}
int a_spa::download_vcredist()
{

   string strTempSpa = download_tmp_vcredist();

   if (!file_exists_dup(strTempSpa.c_str()))
   {

      return 0;

   }

   if (spa_get_admin())
   {

      string str = ::path::vcredist();

      if (!::CopyFileW(u16(strTempSpa.c_str()).c_str(), u16(str), FALSE))
      {

         return 0;

      }

      if (!file_exists_dup(str))
      {

         return 0;

      }

   }
   else
   {

      return 0;

   }


   return 1;



}

string a_spa::download_tmp_vcredist()
{

   string strTempSpa = get_temp_file_name_dup(::path::vcredist().title(), ::path::vcredist().extension());

   int iTry = 0;

   while (iTry <= 3)
   {

      if (ms_download("http://server.ca2.cc/" + process_platform_dir_name() + "/" + ::path::vcredist().name(), strTempSpa.c_str())
         && file_exists_dup(strTempSpa.c_str())
         && file_length_dup(strTempSpa.c_str()) > 0)
      {

         return strTempSpa;

      }

      iTry++;

   }


   return "";

}

int a_spa::check_spa_bin()
{

   string str = ::path::a_spa();

   if(!file_exists_dup(str))
   {

      if(!spa_get_admin())
      {

         return 0;

      }

      if(!download_spa_bin())
      {

         return 0;

      }

      if(!file_exists_dup(str))
      {

         return 0;

      }

   }

   return 1;

}

int a_spa::check_spaadmin_bin()
{

   string str = ::path::a_spaadmin();

   if(!file_exists_dup(str))
   {
      
      if(!download_spaadmin_bin())
      {

         return 0;

      }

      if(!file_exists_dup(str))
      {

         return 0;

      }

   }

   return true;

}

int a_spa::download_spa_bin()
{

   string strTempSpa = download_tmp_spa_bin();

   if(!file_exists_dup(strTempSpa.c_str()))
   {

      return 0;

   }

   if(spa_get_admin())
   {

      string str = ::path::a_spa();

      if(!::CopyFileW(u16(strTempSpa.c_str()).c_str(),u16(str),FALSE))
      {

         return 0;

      }

      if(!file_exists_dup(str))
      {

         return 0;

      }

   }
   else
   {

      return 0;

   }


   return 1;



}

int a_spa::download_spaadmin_bin()
{

   string strTempSpa = download_tmp_spaadmin_bin();

   if(!file_exists_dup(strTempSpa.c_str()))
   {

      return 0;

   }

   if(spa_get_admin())
   {

      string str = ::path::a_spaadmin();

      if(!dir::mk(dir::name(str)))
      {
       
         return 0;

      }

      if(!::CopyFileW(u16(strTempSpa).c_str(),u16(str),FALSE))
      {

         return 0;

      }

      if(!file_exists_dup(str))
      {

         return 0;

      }

   }
   else
   {

      if(!low_is_spaadmin_running())
      {

         SHELLEXECUTEINFOW sei ={};

         wstring wstrFile = u16(strTempSpa.c_str());

         {

            spaadmin_mutex mutexStartup("-startup");

            sei.cbSize = sizeof(SHELLEXECUTEINFOW);
            sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
            sei.lpVerb = L"RunAs";
            sei.lpFile = wstrFile.c_str();
            ::ShellExecuteExW(&sei);

         }

         DWORD dwGetLastError = GetLastError();

         string str = ::path::a_spaadmin();

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

            if(file_exists_dup(str))
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



string a_spa::download_tmp_spaadmin_bin()
{

   string strTempSpa = get_temp_file_name_dup(::path::a_spaadmin().title(),::path::a_spaadmin().extension());

   int iTry = 0;

   while(iTry <= 3)
   {

      if(ms_download("http://server.ca2.cc/"+process_platform_dir_name() + "/" + m_strVersion + "/" + ::path::a_spaadmin().name(), strTempSpa.c_str())
         && file_exists_dup(strTempSpa.c_str())
         && file_length_dup(strTempSpa.c_str()) > 0)
      {

         return strTempSpa;

      }

      iTry++;

   }


   return "";

}



string a_spa::download_tmp_spa_bin()
{

   string strTempSpa = get_temp_file_name_dup(::path::a_spa().title(),::path::a_spa().extension());

   int iTry = 0;

   while(iTry <= 3)
   {

      if(ms_download("http://server.ca2.cc/" + process_platform_dir_name() + "/" + m_strVersion + "/" + ::path::a_spa().name(),strTempSpa.c_str())
         && file_exists_dup(strTempSpa.c_str())
         && file_length_dup(strTempSpa.c_str()) > 0)
      {

         return strTempSpa;

      }

      iTry++;

   }


   return "";

}







bool a_spa::is_file_ok(const char * path1,const char * pszTemplate,const char * pszFormatBuild)
{

   string strFormatBuild(pszFormatBuild);

   string strUrl;

   strUrl = "http://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5?authnone&version=" + m_strVersion + "&stage=";
   strUrl += pszTemplate;
   strUrl += "&build=";
   strUrl += strFormatBuild;

   return file_exists_dup(path1) && !_stricmp(file_md5_dup(path1).c_str(),ms_get(strUrl.c_str()).c_str());

}

bool a_spa::is_file_ok(const stringa & straPath,const stringa & straTemplate,stringa & straMd5,const string & strFormatBuild,int iMd5Retry)
{

   bool bOk = true;

   if(straPath.size() != straTemplate.size())
      return false;

   if(bOk)
   {

      for(int i = 0; i < straPath.size(); i++)
      {

         string strPath = straPath[i];

         if(!file_exists_dup(strPath.c_str()))
         {

            bOk = false;

            break;

         }

      }

   }

   if(iMd5Retry > 0 || straMd5.size() != straPath.size())
   {

      string strUrl;

      strUrl = "http://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
      strUrl += straTemplate.implode(",");
      strUrl += "&build=";
      strUrl += strFormatBuild;
      strUrl += "&platform=";
      strUrl += process_platform_dir_name();

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

      string strPath = straPath[i];

      string strFileMd5 = file_md5_dup(strPath);

      string strMd5 = straMd5[i];

      if(strFileMd5.CompareNoCase(strMd5) != 0)
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

      if(m_paspa->check_spaadmin_bin())
      {

         InterlockedIncrement(m_plongOk);

      }

   }
   else if(m_strFile == "spa")
   {

      if(m_paspa->check_spa_bin())
      {

         InterlockedIncrement(m_plongOk);

      }

   }
   else if (m_strFile == "vcredist")
   {

      if (m_paspa->check_vcredist())
      {

         InterlockedIncrement(m_plongOk);

      }

   }
   else if(m_strFile == "install_bin_set")
   {

      if(m_paspa->check_install_bin_set())
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

   string strDownload =dir::name(strPath) / strFile;

   if(!file_exists_dup(strDownload) || _stricmp(file_md5_dup(strDownload).c_str(),strMd5) != 0)
   {

      if (strFile.CompareNoCase("app.install.exe") == 0)
      {

         kill_process(strFile);

      }

      string strUrl;

      int iRetry;

      bool bFileNice;

      iRetry = 0;

      strUrl = strUrlPrefix + strFile + ".bz";

      bFileNice = false;

      while(iRetry < 8 && !bFileNice)
      {

         if(m_paspa->ms_download(strUrl.c_str(),(strDownload + ".bz").c_str()))
         {

            bzuncompress((strDownload).c_str(),(strDownload + ".bz").c_str());

            if(file_exists_dup(strDownload.c_str()) && _stricmp(file_md5_dup(strDownload.c_str()).c_str(),strMd5.c_str()) == 0)
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





int a_spa::check_install_bin_set()
{

   string strPath = path::app_install();

   stringa straFile = install_get_plugin_base_library_list(m_strVersion);

   if(!::dir::is(dir::name(strPath)))
   {

      if(!::dir::mk(dir::name(strPath)))
         return 0;

   }

   stringa straDownload;

   for(int iFile = 0; iFile < straFile.size(); iFile++)
   {

      string strFile = straFile[iFile];

      string strDownload = dir::name(strPath.c_str()) / strFile.c_str();

      straDownload.push_back(strDownload);

   }

   stringa straMd5;


   string strFormatBuild = get_latest_build_number(m_strVersion);

   int iMd5Retry = 0;

   string strBuild = strFormatBuild;

   strBuild = str_replace(strBuild.c_str()," ","_");

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

               string strFile = strAuraDir / straFile[iFile];

               if(!file_exists_dup(straDownload[iFile].c_str()) && file_exists_dup(strFile.c_str()) && file_md5_dup(strFile.c_str()) == straMd5[iFile].c_str())
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

      string strUrlPrefix = "http://server.ca2.cc/ccvotagus/" + m_strVersion + "/" + strBuild + "/install/" + process_platform_dir_name() + "/";

      //#pragma omp parallel for
      for(int iFile = 0; iFile < straFile.size(); iFile++)
      {

         string strDownload = dir::name(strPath) / straFile[iFile];

         if (!file_exists_dup(strDownload) || _stricmp(file_md5_dup(strDownload).c_str(), straMd5[iFile]) != 0)
         {

            if (straFile[iFile].CompareNoCase("app.install.exe") == 0)
            {

               kill_process("app.install.exe");

            }

            new install_bin_item(this, strUrlPrefix, strPath, straFile[iFile], &lCount, straMd5[iFile], process_platform_dir_name(), lTotal);

         }

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



string a_spa::get_latest_build_number(const char * pszVersion)
{

   if(m_strBuild.length() > 0)
   {
      
      return m_strBuild;

   }

   string strBuildNumber;

   string strSpaIgnitionBaseUrl;

   string strVersion(pszVersion);

   if(file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt").length() > 0)
   {

      strSpaIgnitionBaseUrl = "https://" + file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt") + "/api/spaignition";

   }
   else if(pszVersion != NULL && !strcmp(pszVersion,"basis"))
   {

      strSpaIgnitionBaseUrl = "http://server.ca2.cc/api/spaignition";

   }
   else if(pszVersion != NULL && !strcmp(pszVersion,"stage"))
   {

      strSpaIgnitionBaseUrl = "http://server.ca2.cc/api/spaignition";

   }
   else
   {

      if(m_strVersion == "basis")
      {

         strVersion = "basis";

         strSpaIgnitionBaseUrl = "http://server.ca2.cc/api/spaignition";

      }
      else
      {

         strVersion = "stage";

         strSpaIgnitionBaseUrl = "http://server.ca2.cc/api/spaignition";

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

   ::str::_008Trim(strBuildNumber);

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

   ::aura::ipc::tx txchannel(::get_thread_app());

   app_install_launcher launcher("","");

   string strChannel;

   // "core/spaboot_install"

   strChannel = "::ca2::fontopus::ca2_spaboot_install_"+process_platform_dir_name()+"::7807e510-5579-11dd-ae16-0800200c7784";

   if(!txchannel.open(strChannel,bLaunch ? &launcher : NULL))
      return false;

   txchannel.send(psz,INFINITE);

#endif

   return true;

}




//#include "aura/aura/aura_launcher.cpp"
#include "axis/app_install_launcher.cpp"
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



void a_spa::start_app_install_in_context()
{

   app_install_launcher launcher("","");

   launcher.start_in_context();

}



void app_install_launcher::start_in_context()
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

   if (!::CreateProcessW(NULL, (wchar_t *)wstr.c_str(), NULL, NULL, FALSE, 0, NULL, wstrDir.c_str(), &si, &pi))
   {

      output_debug_string("Could not create process \"" + m_strPath + "\"");

   }

   Sleep(1984);

}


int register_spa_file_type()
{
   HKEY hkey;

   wstring extension=L".spa";                     // file extension
   wstring desc=L"spafile";          // file type description
   wstring content_type = L"application/x-spa";

   wstring app(::path::a_spa());

   wstring icon(app);

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

   wstring wstr(dir::a_spa() / "spa_register.txt");

   int iRetry = 9;

   while(!file_exists_dup(u8(wstr.c_str())) && iRetry > 0)
   {

      dir::mk(dir::name(u8(wstr.c_str())).c_str());

      file_put_contents_dup(u8(wstr.c_str()).c_str(),"");

      iRetry--;

      Sleep(84);

   }

   return 1;

}



void start_program_files_spa_admin()
{

   SHELLEXECUTEINFOW sei ={};

   string str = ::path::a_spaadmin();

   if(!::file_exists_dup(str))
   {

      return;

   }

   spaadmin_mutex mutexStartup("-startup");

   wstring wstr(str);

   sei.cbSize =sizeof(SHELLEXECUTEINFOW);
   sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
   sei.lpVerb = L"RunAs";
   sei.lpFile = wstr.c_str();
   ::ShellExecuteExW(&sei);
   DWORD dwGetLastError = GetLastError();

}


void run_vcredist()
{

   SHELLEXECUTEINFOW sei = {};

   string str = ::path::vcredist();

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



void get_system_locale_schema(string & strLocale,string & strSchema)
{

#if defined(WINDOWS)

   LANGID langid = ::GetUserDefaultLangID();

#define SPR_DEUTSCH LANG_GERMAN

   if(langid == LANG_SWEDISH)
   {
      strLocale = "se";
      strSchema = "se";
   }
   else if(langid == MAKELANGID(LANG_PORTUGUESE,SUBLANG_PORTUGUESE_BRAZILIAN))
   {
      strLocale = "pt-br";
      strSchema = "pt-br";
   }
   else if(PRIMARYLANGID(langid) == SPR_DEUTSCH)
   {
      strLocale = "de";
      strSchema = "de";
   }
   else if(PRIMARYLANGID(langid) == LANG_ENGLISH)
   {
      strLocale = "en";
      strSchema = "en";
   }
   else if(PRIMARYLANGID(langid) == LANG_JAPANESE)
   {
      strLocale = "jp";
      strSchema = "jp";
   }
   else if(PRIMARYLANGID(langid) == LANG_POLISH)
   {
      strLocale = "pl";
      strSchema = "pl";
   }

#endif

   if(strLocale.length() <= 0)
      strLocale = "_std";

   if(strSchema.length() <= 0)
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
	for (DWORD procIdx = 0; procIdx<procCount; procIdx++)
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