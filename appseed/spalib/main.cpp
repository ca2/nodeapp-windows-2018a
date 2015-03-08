#include "StdAfx.h"

#if defined(LINUX) || defined(WINDOWS)
//#include <omp.h>
#else
int omp_get_thread_num()
{
   return 0;
}
#endif

using namespace std;

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


SPALIB_API int spa_admin()
{

   int iTry = 0;

retry_check_spa_installation:

   if(!check_spa_installation())
   {

      iTry++;

      if(iTry <= 3)
      {

         goto retry_check_spa_installation;

      }

      return -3;

   }

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

   
   int iTry = 0;
   
retry_check_spa_installation:

   if(!check_spa_installation())
   {
   
      iTry++;

      if(iTry <= 3)
      {

         goto retry_check_spa_installation;

      }

      return 0;

   }

   ::srand(::GetTickCount());
   try
   {
      dir::mk(dir::ca2().c_str());
      g_ftrace = fopen(dir::ca2("install.log").c_str(),"ab");
   }
   catch(...)
   {
   }

   g_hmutexTrace =  ::CreateMutex(NULL,FALSE,NULL);

   if(!g_machineevent.initialize())
   {
      return 5;
   }

   g_iStyle = 0;

   g_bShowPercentage = false;

   g_iStart = 0;

   

   std::string strCmdLine = utf16_to_8(::GetCommandLineW());

   const char * lpCmdLine = strCmdLine.c_str();

   std::string str =strCmdLine;

   std::string strExe = file::name(file::module_path().c_str());



   if(str.find(" background ") != std::string::npos
      || str_ends(str.c_str()," background") || strExe.find(" in background ") != std::string::npos
      || str_ends(strExe.c_str()," in background"))
   {
      g_bShow = false;
   }


   int nCmdShow = SW_SHOW;
   

   g_bLoginStartup = false;
   size_t i = str.find("install_filter=");
   if(i != std::string::npos)
   {
      int iStart = i + 15; // 15 = strlen("install_filter=")
      i = str.find(" ",i);
      if(i != std::string::npos)
      {
         g_strInstallFilter = str.substr(iStart,i - iStart);
      }
      else
      {
         g_strInstallFilter = str.substr(iStart);
      }
   }
   //MessageBox(NULL, "xxx", "yyy", MB_OK);
   i = str.find("install=");
   if(i != std::string::npos)
   {
      int iStart = i + 8; // 8 = strlen("install=")
      i = str.find(" ",i);
      if(i != std::string::npos)
      {
         g_strStart = str.substr(iStart,i - iStart);
      }
      else
      {
         g_strStart = str.substr(iStart);
      }
      g_iStart = 4;
      return run_install(lpCmdLine,nCmdShow);

   }
   if(str_begins_ci(strExe.c_str(),"Install "))
   {
      int iStart = strlen("Install ");
      i = strExe.find(" ",iStart);
      std::string strIdStart;
      if(i != std::string::npos)
      {
         strIdStart = strExe.substr(iStart,i - iStart);
      }
      else
      {
         strIdStart = strExe.substr(iStart);
      }
      g_iStart = 4;
      return starter_start(strIdStart.c_str());
   }
   std::string strFind("starter_start=");
   i = str.find(strFind);
   if(i != std::string::npos)
   {
      int iStart = i + strFind.length(); // 8 = strlen("install=")
      i = str.find(" ",iStart);
      std::string strIdStart;
      if(i != std::string::npos)
      {
         strIdStart = str.substr(iStart,i - iStart);
      }
      else
      {
         strIdStart = str.substr(iStart);
      }
      g_iStart = 4;
      return starter_start(strIdStart.c_str());

   }


   /*wchar_t * lpwstr = ::GetCommandLineW();
   int numargs;
   int numchars;
   wparse_cmdline(
      lpwstr,
      NULL,
      NULL,
      &numargs,
      &numchars);

   wchar_t * pszCmdLine = new WCHAR[numchars];
   wchar_t ** argv = new WCHAR*[numargs];

   wparse_cmdline(
      lpwstr,
      argv,
      pszCmdLine,
      &numargs,
      &numchars);

   if(numargs >= 2)
   {
      std::string str(utf16_to_8(argv[1]));
      return run_file(str.c_str(),nCmdShow);
   }
*/

   int iFind;
   if((iFind = str.find("uninstall")) != std::string::npos)
   {
      return run_uninstall_run(&str[iFind + 10],nCmdShow);
   }
   else if(str.find("uninstall") != std::string::npos)
   {
      return run_uninstall(lpCmdLine,nCmdShow);
   }
   else
   {
      //g_strStart = "_set_windesk";  g_iStart = 4; return run_install(lpCmdLine, nCmdShow);
      return run_install(lpCmdLine,nCmdShow);
   }
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

int check_soon_file_launch(std::wstring wstr)
{

   if(wstr.length() <= 0)
   {

      return 0;

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

      return 0;

   }

   XDoc doc;

   ::std::string strPath = utf16_to_8(wstr.c_str());

   ::std::string strContents = file::get_contents(strPath.c_str());

   if(!doc.Load(strContents.c_str()))
   {
      
      return 0;

   }

   XNode * pnode = doc.GetRoot();

   if(pnode == NULL)
   {
    
      return 0;

   }

   if(pnode->GetChildCount() <= 0)
   {
      if(pnode->name == "meta")
      {
         return check_soon_app_id(utf8_to_16(file::name(strPath.c_str()).c_str()));
      }

      return 0;
   }

   const char * psz = pnode->GetChildAttrValue("launch","app");

   if(psz == NULL || *psz == '\0')
   {

      return 0;

   }

   return check_soon_app_id(utf8_to_16(psz));

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

   std::wstring strName = spa_app_id_to_app_name(strId);



   {

      wchar_t * lpszCurFolder = (wchar_t *)malloc(MAX_PATH * sizeof(wchar_t) * 8);

      _wgetcwd(lpszCurFolder,MAX_PATH * 8 - 1);

      if(lpszCurFolder[wcslen(lpszCurFolder) - 1] == '\\'
         || lpszCurFolder[wcslen(lpszCurFolder) - 1] == '/')
      {
         lpszCurFolder[wcslen(lpszCurFolder) - 1] = '\0';
      }
      wcscat(lpszCurFolder,L"\\");

      std::wstring wstr;

      wstr = lpszCurFolder;

      wstr += strName;

      wstr += L".exe";

      STARTUPINFOW si;
      memset(&si,0,sizeof(si));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_SHOWNORMAL;
      PROCESS_INFORMATION pi;
      memset(&pi,0,sizeof(pi));

      if(::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
         NULL,NULL,FALSE,0,NULL,NULL,
         &si,&pi))
         return TRUE;


   }


   {

      int iCount = MAX_PATH * 8;

      wchar_t * lpszModuleFolder = (wchar_t *)malloc(iCount * sizeof(wchar_t));

      wchar_t * lpszModuleFilePath = (wchar_t *)malloc(iCount * sizeof(wchar_t));

      HMODULE hmodule = ::GetModuleHandleA(NULL);

      if(hmodule != NULL)
      {


         if(GetModuleFileNameW(hmodule,lpszModuleFilePath,iCount))
         {

            LPWSTR lpszModuleFileName;

            if(GetFullPathNameW(lpszModuleFilePath,iCount,lpszModuleFolder,&lpszModuleFileName))
            {

               lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

               if(wcslen(lpszModuleFolder) > 0)
               {

                  if(lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '/')
                  {

                     lpszModuleFolder[wcslen(lpszModuleFolder) - 1] = '\0';

                  }

               }




               wcscat(lpszModuleFolder,L"\\");

               std::wstring wstr;

               wstr = lpszModuleFolder;

               wstr += strName;

               wstr += L".exe";

               STARTUPINFOW si;
               memset(&si,0,sizeof(si));
               si.cb = sizeof(si);
               si.dwFlags = STARTF_USESHOWWINDOW;
               si.wShowWindow = SW_SHOWNORMAL;
               PROCESS_INFORMATION pi;
               memset(&pi,0,sizeof(pi));

               if(::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
                  NULL,NULL,FALSE,0,NULL,NULL,
                  &si,&pi))
                  return TRUE;
            }

         }

      }

   }


   {

      std::wstring wstr;

      wstr = L"\\ca2\\";
#ifdef _M_X64
      wstr += L"stage\\x64\\";
#else
      wstr += L"stage\\x86\\";
#endif

      wstr += strName + L".exe";

      get_program_files_x86(wstr);

      STARTUPINFOW si;
      memset(&si,0,sizeof(si));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_SHOWNORMAL;
      PROCESS_INFORMATION pi;
      memset(&pi,0,sizeof(pi));

      if(!::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
         NULL,NULL,FALSE,0,NULL,NULL,
         &si,&pi))
         return FALSE;

   }

   return TRUE;

}

int check_spa_bin();
int check_spaadmin_bin();
int check_install_bin_set();

int check_spa_installation()
{

   if(!check_spaadmin_bin())
      return 0;

   if(!check_spa_bin())
      return 0;
   
   if(!check_install_bin_set())
      return 0;

   return 0;

}

int download_spa_bin();

int check_spa_bin()
{

   std::wstring wstr;

   wstr = L"\\ca2.spa\\spa.exe";

   get_program_files_x86(wstr);

   if(!file::exists(utf16_to_8(wstr.c_str())))
   {
      
      if(!spa_get_admin())
         return 0;
      
      if(!download_spa_bin())
      {
         return 0;
      }
      if(!file::exists(utf16_to_8(wstr.c_str())))
      {
         return 0;
      }
   }

   return true;

}

int download_spaadmin_bin();


int check_spaadmin_bin()
{

   std::wstring wstr;

   wstr = L"\\ca2.spa\\spaadmin.exe";

   get_program_files_x86(wstr);

   if(!file::exists(utf16_to_8(wstr.c_str())))
   {
      if(!download_spaadmin_bin())
      {
         return 0;
      }
      if(!file::exists(utf16_to_8(wstr.c_str())))
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

      wstr = L"\\ca2.spa\\spa.exe";

      get_program_files_x86(wstr);

      if(!::CopyFileW(utf8_to_16(strTempSpa.c_str()),wstr.c_str(),FALSE))
         return 0;

      if(!file::exists(utf16_to_8(wstr.c_str())))
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

      wstr = L"\\ca2.spa\\spaadmin.exe";

      get_program_files_x86(wstr);

      if(!dir::mk(dir::name(utf16_to_8(wstr.c_str())).c_str()))
         return 0;

      if(!::CopyFileW(utf8_to_16(strTempSpa.c_str()),wstr.c_str(),FALSE))
         return 0;

      if(!file::exists(utf16_to_8(wstr.c_str())))
         return 0;

   }
   else
   {

      SHELLEXECUTEINFOW s ={};

      

      s.cbSize =sizeof(SHELLEXECUTEINFOW);
      s.lpFile = utf8_to_16(strTempSpa.c_str());
      ::ShellExecuteExW(&s);
      DWORD dwExit;
      int iTry = 8;
      while(::GetExitCodeProcess(s.hProcess,&dwExit) && dwExit == STILL_ACTIVE && iTry > 0)
      {
         Sleep(1984);
         iTry--;
      }

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



void get_plugin_base_library_list(stra & straFile,const string & strVersion);




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

bool is_file_ok(const stra & straPath,const stra & straTemplate,stra & straMd5,const string & strFormatBuild,int iMd5Retry)
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

   wstring wstrPath = utf8_to_16((string("\\ca2\\install\\stage\\") + strPlatform + "\\app.install.exe").c_str());

   get_program_files_x86(wstrPath);

   strPath = utf16_to_8(wstrPath.c_str());


#else

   throw "TODO";

#endif


   stra straFile;

   get_plugin_base_library_list(straFile,g_strVersion);

   if(!::dir::exists(dir::name(strPath.c_str()).c_str()))
   {

      if(!::dir::mk(dir::name(strPath.c_str()).c_str()))
         return 0;

   }

   stra straDownload;

   for(int iFile = 0; iFile < straFile.size(); iFile++)
   {

      string strFile = straFile[iFile];

      string strDownload = dir::path(dir::name(strPath.c_str()).c_str(),strFile.c_str());

      straDownload.push_back(strDownload);

   }

   stra straMd5;


   string strFormatBuild = get_latest_build_number(g_strVersion.c_str());

   int iMd5Retry = 0;

md5retry:

   if(!is_file_ok(straDownload,straFile,straMd5,strFormatBuild,iMd5Retry))
   {


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

         if(str.has_char())
         {

            string strAuraDir = ::dir::name(str);

            for(int iFile = 0; iFile < straFile.get_size(); iFile++)
            {

               string strFile = System.dir().path(strAuraDir,straFile[iFile]);

               if(!file::exists(straDownload[iFile]) && file::exists(strFile) && System.file().md5(strFile) == straMd5[iFile])
               {

                  ::file_copy_dup(straDownload[iFile],strFile,false);

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

#pragma omp parallel for
      for(int iFile = 0; iFile < straFile.size(); iFile++)
      {

         string strFile = straFile[iFile];

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

      }

   }


   return 1;

}





void get_plugin_base_library_list(stra & straFile,const string & strVersion)
{


   straFile.push_back("app.install.exe");
   straFile.push_back("aqua.dll");
   straFile.push_back("aura.dll");
   straFile.push_back("aurasqlite.dll");
   straFile.push_back("axis.dll");
   straFile.push_back("axisbzip2.dll");
   straFile.push_back("axiscrypto.dll");
   //straFile.push_back("axisfreeimage.dll");
   //straFile.push_back("axisfreetype.dll");
   straFile.push_back("axisidn.dll");
   straFile.push_back("axisopenssl.dll");
   straFile.push_back("axiszlib.dll");
   straFile.push_back("axis.dll");
   straFile.push_back("base.dll");
   straFile.push_back("draw2d_gdiplus.dll");

   if(strVersion == "stage")
   {

      straFile.push_back("msvcp120.dll");
      straFile.push_back("msvcr120.dll");
      straFile.push_back("vcomp120.dll");

   }
   else
   {

      straFile.push_back("msvcp120d.dll");
      straFile.push_back("msvcr120d.dll");
      straFile.push_back("vcomp120d.dll");

   }


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

   trim(strBuildNumber);

   if(strBuildNumber.length() != 19)
   {

      Sleep(184 * iRetry);

      goto RetryBuildNumber;

   }

   return strBuildNumber;

}
