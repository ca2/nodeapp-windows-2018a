#include "StdAfx.h"


int starter_start(const char * pszId);
int run_file(const char * pszFile,int nCmdShow);
int run_uninstall_run(const char * lpCmdLine,int nCmdShow);
int ca2_app_install_run(const char * psz,const char * pszParam1,const char * pszParam2,DWORD & dwStartError,bool bSynch);


int check_soon_launch();

SPALIB_API int spalib_main(HINSTANCE hInstance,
   HINSTANCE hPrevInstance,
   LPTSTR    lpCmdLine,
   int       nCmdShow)
{

   if(check_soon_launch())
      return 0;


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

   g_hinstance = hInstance;

   if(!g_machineevent.initialize())
   {
      return 5;
   }

   g_iStyle = 0;

   g_bShowPercentage = false;

   UNREFERENCED_PARAMETER(hPrevInstance);
   //UNREFERENCED_PARAMETER(lpCmdLine);
   g_iStart = 0;
   //g_iStyle = 0;

   std::string str = lpCmdLine;

   std::string strExe = file::name(file::module_path().c_str());



   if(str.find(" background ") != std::string::npos
      || str_ends(str.c_str()," background") || strExe.find(" in background ") != std::string::npos
      || str_ends(strExe.c_str()," in background"))
   {
      g_bShow = false;
   }




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


   wchar_t * lpwstr = ::GetCommandLineW();
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
      wcscat(lpszModuleFilePath,L"\\ca2\\");
#ifdef _M_X64
      wcscat_dup(lpszModuleFilePath,L"stage\\x64\\");
#else
      wcscat(lpszModuleFilePath,L"stage\\x86\\");
#endif

      wcscpy(lpszModuleFolder,lpszModuleFilePath);

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

      if(!::CreateProcessW(NULL,(wchar_t *)wstr.c_str(),
         NULL,NULL,FALSE,0,NULL,NULL,
         &si,&pi))
         return FALSE;

   }

   return TRUE;

}