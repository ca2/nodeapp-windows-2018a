#include "framework.h"
#include <stdio.h>


extern MSG g_msg;
extern bool g_bRunMainLoop;
extern IDTHREAD g_dwMain2;
extern int g_iDownloadingSpaadmin;


bool is_user_service_running();


namespace a_spa
{


   simple_app * simple_app::s_papp = NULL;


   simple_app::simple_app() :
      ::aura::system(NULL, this)
   {

      m_pthreadSsl = NULL;
      m_bAcid = true;
      m_hwnd = NULL;
      m_pcanvas = NULL;
      s_papp = this;

      m_hinstance = ::GetModuleHandleA(NULL);

   }


   simple_app::~simple_app()
   {


   }


   bool simple_app::http_download(const char * pszUrl, const char * pszFile, bool bUrlEncode, int * piStatus)
   {

      string strUrl = pszUrl;

      if (bUrlEncode)
      {

         strUrl = url_encode(strUrl);
         strUrl = str::replace("%5C", "\\", strUrl);
         strUrl = str::replace("\\", "/", strUrl);
         strUrl = str::replace("%3A", ":", strUrl);
         strUrl = str::replace("%2F", "/", strUrl);

      }

      property_set set;

      set["raw_http"] = true;

      set["disable_common_name_cert_check"] = true;

      return http().download(strUrl, pszFile, set);

   }


   string simple_app::http_get(const char * pszUrl, bool bCache)
   {

      property_set set;

      set["get_response"] = "";

      set["raw_http"] = true;

      set["disable_common_name_cert_check"] = true;

      if (!http().get(pszUrl, set))
      {

         return "";

      }

      string strResponse = set["get_response"].get_string();

      return strResponse;

   }



   void simple_app::defer_show_debug_box()
   {

      if (::file_exists_dup(::dir::system() / "config/spa/beg_debug_box.txt"))
      {

         string str;

         if (spa_get_admin())
         {

            str = "zzzAPPzzz a_spaadmin : ";

         }
         else
         {

            str = "zzzAPPzzz a_spa : ";

         }

         str += string(::GetCommandLineW());

         ::MessageBoxA(NULL, str.c_str(), "zzzAPPzzz a_spa", MB_ICONINFORMATION);

      }


   }


   int32_t simple_app::run()
   {


      ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

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

      if (spa_get_admin())
      {

         start_a_spa_web_server();

         manual_reset_event ev86(this);

         ev86.ResetEvent();

         manual_reset_event ev64(this);

         ev64.ResetEvent();


         ::fork(this, [&]()
         {

            try
            {

               ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

               spaadmin_main("x86");

            }
            catch (...)
            {

            }

            ev86.SetEvent();

         });

         ::fork(this, [&]()
         {

            try
            {

               ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

               spaadmin_main("x64");

            }
            catch (...)
            {

            }

            ev64.SetEvent();

         });

         ev86.wait();

         ev64.wait();

      }
      else
      {

         m_iReturnCode = spa_main();

      }


      return m_iReturnCode;

   }

   void simple_app::add_command_line(string str)
   {

      {

         ::mutex mutexCommandFile(get_thread_app(), "Local\\ca2_spa_command:" + process_platform_dir_name2());

         ::file::path path = ::dir::system() / process_platform_dir_name2() / "spa_command.txt";

         stringa stra;
         
         if(file_load_stra(path, stra, false))
         {

            stra.add(str);

            file_save_stra(path, stra);

         }

      }

   }


   string simple_app::pick_command_line()
   {

      ::mutex mutexCommandFile(get_thread_app(), "Local\\ca2_spa_command:" + process_platform_dir_name2());

      ::file::path path = ::dir::system() / process_platform_dir_name2() / "spa_command.txt";

      stringa stra;

      if (!file_load_stra(path, stra, false))
      {

         return "";

      }

      if (stra.is_empty())
      {

         return "";

      }

      string str;

      str = stra[0];

      stra.remove_at(0);

      file_save_stra(path, stra);

      return str;

   }


   int simple_app::spa_main()
   {

      spa_mutex mutex(process_platform_dir_name2());

      if (mutex.already_exists())
      {

         add_command_line(::GetCommandLineW());

         return -34;

      }

      defer_show_debug_box();

      string str(::GetCommandLineW());

      if (str.find(" install ") < 0)
      {

         if (check_soon_launch(str, true))
            return 0;

      }

      add_command_line(str);

      if (!show_spa_window())
      {

         return -1;

      }

      if (!spa_main_start(process_platform_dir_name2()))
      {

         return -2;

      }

      while (GetMessage(&g_msg, NULL, 0, 0) && g_bRunMainLoop && !m_bFinished)
      {

         TranslateMessage(&g_msg);

         DispatchMessage(&g_msg);

      }

      {

         HWND hwnd = m_hwnd;

         ::ShowWindow(hwnd, SW_HIDE);

         ::DestroyWindow(hwnd);

      }

      return m_iReturnCode;

   }





   int simple_app::spaadmin_main(string strPlatform)
   {

      //::MessageBoxA(NULL, "Test1", "Test1", MB_OK);

      //if (process_platform_dir_name2() == "x86")
      //{

      // spa_main_start("x64");

      //}
      //else
      //{

      // m_bFinished = true;

      //}

      spaadmin_mutex smutex(strPlatform);

      if (smutex.already_exists())
      {

         System.post_quit();

         return 0;

      }

      defer_show_debug_box();

      trace("--\r\n");
      trace(":::::Installing spa and installer\r\n");
      trace("***Installing spa\r\n");
      trace("Registering spa file handler\r\n");
      trace(0.0);

      if (!check_spaadmin_bin(strPlatform))
      {

         return 0;

      }

      if (strPlatform == "x86")
      {

         register_spa_file_type();

      }

      trace(0.05);

      int iTry = 1440;

      while (true)
      {

         try
         {


            if (check_spa_installation(strPlatform))
            {

               break;

            }


            if (!check_spaadmin_bin(strPlatform))
            {

               return 0;

            }

         }
         catch (...)
         {

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(500);

      }

      trace("***Preparing app.install\r\n");

      trace("Starting app.install\r\n");

      start_app_install_in_context(strPlatform, true);

      if (strPlatform == "x86")
      {

         iTry = 8;

         while (true)
         {

            try
            {

               {

                  int iUserServiceTry = 300;

                  while (!check_user_service("Win32", false))
                  {

                     Sleep(500);

                     if (iUserServiceTry <= 0)
                     {

                        break;

                     }

                     iUserServiceTry--;

                  }

               }

               if (!check_spaadmin_bin(strPlatform))
               {

                  return 0;

               }

            }
            catch (...)
            {

            }

            iTry--;

            if (iTry < 0)
            {

               return 0;

            }

            Sleep(500);

         }

      }

      trace(":::::Thank you\r\n");
      trace("***Thank you\r\n");
      trace("Thank you\r\n");
      trace(1.0);

      //end_spa(this);

      return 0;

   }




   int simple_app::spa_main_start(string strPlatform)
   {

      m_bFinished = false;

      m_strPlatform = strPlatform;

      if (!__begin_thread(this, &simple_app::spa_main_proc, this, 50, 0, 0, NULL, &g_dwMain2))
      {

         return 0;

      }

      return 1;

   }



   UINT c_cdecl simple_app::spa_main_proc(LPVOID lpvoid)
   {

      ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

      simple_app * papp = (simple_app *)lpvoid;

      try
      {

         papp->m_iReturnCode = papp->spalib_main2();

      }
      catch (...)
      {

      }

      end_spa(papp);

      papp->m_bFinished = true;

      return papp->m_iReturnCode;

   }



   int simple_app::spalib_main2()
   {

      if (m_strPlatform == "x86" || m_strPlatform.is_empty())
      {

         return spalib_main32();

      }
      else
      {

         return spalib_main_plat();

      }

   }


   int simple_app::spalib_main_plat()
   {

      bool bSomeSortOfInstall = false;

      int iTry;

      iTry = 1440;

      string strPlatform = m_strPlatform;

      int iFullInstallationMaxTryCount;

      int iFullInstallationTryCount;

   spa_admin:

      while (!is_file_ok(::path::a_spaadmin(strPlatform), ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform))
      {

         bSomeSortOfInstall = true;

         if (!is_downloading_spaadmin())
         {

            check_spaadmin_bin(strPlatform);

         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(500);

      }

      bSomeSortOfInstall = false;

      iFullInstallationMaxTryCount = 3;

      iFullInstallationTryCount = 0;

      while (iFullInstallationTryCount < iFullInstallationMaxTryCount)
      {

         iFullInstallationTryCount++;

         int iTry;


         iTry = 1440;

         while (!check_spa_installation(strPlatform))
         {

            bSomeSortOfInstall = true;

            if (!is_downloading_spaadmin()
               && is_file_ok(::path::a_spaadmin(strPlatform), ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform)
               && !low_is_spaadmin_running(strPlatform))
            {

               if (!is_file_ok(::path::a_spaadmin(strPlatform), ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform))
               {

                  goto spa_admin;

               }

               defer_start_program_files_spa_admin(strPlatform);

            }

            iTry--;

            if (iTry < 0)
            {

               return 0;

            }

            Sleep(500);

         }

         int iUserServiceTry = 300;

         while (!check_user_service("Win32", false))
         {

            Sleep(500);

            if (iUserServiceTry <= 0)
            {

               break;

            }

            iUserServiceTry--;

         }

         string strId;

         while (true)
         {

            string wstr = pick_command_line();

            if (wstr.is_empty())
            {

               break;

            }

            if (check_soon_launch(wstr, false))
            {

               m_straCommand.add(wstr);

               continue;

            }

            string strParams;

            int iFind1 = 0;

            if (wstr[0] == '\"')
            {

               iFind1 = wstr.find('\"', 1);

            }

            int iFind = wstr.find(" : ", iFind1 + 1);

            if (iFind >= 0)
            {
               strParams = wstr.substr(iFind);
               iFind = wstr.find("app=", iFind);
               if (iFind >= 0)
               {
                  int iEnd = wstr.find(" ", iFind);
                  if (iEnd < 0)
                  {
                     strId = wstr.substr(iFind + 4);
                  }
                  else
                  {
                     strId = wstr.substr(iFind + 4, iEnd - iFind - 4);
                  }

                  // trim initial quote
                  if (strId[0] == '\"')
                     strId = strId.substr(1);

                  // trim final quote
                  if (strId[strId.length() - 1] == '\"')
                     strId = strId.substr(0, strId.length() - 1);

               }

            }
            else
            {

               strId = get_app_id(directrix()->m_varTopicFile);

               if (strId.length() <= 0)
               {

                  continue;

               }

            }

            m_straCommand.add(wstr);

            if (do_spa(strId, strParams))
            {

            }

         }

         for (auto & wstr : m_straCommand)
         {

            check_soon_launch(wstr, true);

         }

         iUserServiceTry = 300;

         while (!check_user_service("Win32", true))
         {

            Sleep(500);

            if (iUserServiceTry <= 0)
            {

               break;

            }

            iUserServiceTry--;

         }


      }

      return 1;

   }


   int simple_app::spalib_main32()
   {

      int iTry;

      iTry = 1440;

      string strPlatform = m_strPlatform;

      int iFullInstallationMaxTryCount;

      int iFullInstallationTryCount;

   spa_admin:

      while (iTry > 0)
      {

         try
         {

            if (check_spaadmin_bin("x86"))
            {

               break;

            }

         }
         catch (...)
         {


         }

         iTry--;

         if (iTry < 0)
         {

            return 0;

         }

         Sleep(500);

      }

      ::file::path pathSpaAdmin = ::path::a_spaadmin("x86");

      ::file::path pathSpaAdminName = pathSpaAdmin.name();

      iFullInstallationMaxTryCount = 3;

      iFullInstallationTryCount = 0;

      while (iFullInstallationTryCount < iFullInstallationMaxTryCount)
      {

         iFullInstallationTryCount++;

         int iTry;

         iTry = 1440;

         while (true)
         {

            try
            {

               if (check_spa_installation("x86") && check_spa_installation("x64"))
               {

                  break;

               }

               if (!is_file_ok(pathSpaAdmin, pathSpaAdminName, NULL, "x86"))
               {

                  goto spa_admin;

               }
               else if (!is_downloading_spaadmin() && !low_is_spaadmin_running("x86"))
               {

                  defer_start_program_files_spa_admin("x86");

                  Sleep(500);

               }

            }
            catch (...)
            {

            }

            iTry--;

            if (iTry < 0)
            {

               return 0;

            }

            Sleep(500);

         }

         int iUserServiceTry = 300;

         while (!check_user_service("Win32", false))
         {

            Sleep(500);

            if (iUserServiceTry <= 0)
            {

               break;

            }

            iUserServiceTry--;

         }


         while (true)
         {

            string strId;

            string wstr(pick_command_line());

            if (wstr.is_empty())
            {

               break;

            }

            if (check_soon_launch(wstr, false))
            {

               m_straCommand.add(wstr);

               continue;

            }

            string strParams;

            int iFind1 = 0;

            if (wstr[0] == '\"')
            {

               iFind1 = wstr.find('\"', 1);

            }

            int iFind = wstr.find(" : ", iFind1 + 1);

            if (iFind >= 0)
            {
               strParams = wstr.substr(iFind);
               iFind = wstr.find("app=", iFind);
               if (iFind >= 0)
               {
                  int iEnd = wstr.find(" ", iFind);
                  if (iEnd < 0)
                  {
                     strId = wstr.substr(iFind + 4);
                  }
                  else
                  {
                     strId = wstr.substr(iFind + 4, iEnd - iFind - 4);
                  }

                  // trim initial quote
                  if (strId[0] == '\"')
                     strId = strId.substr(1);

                  // trim final quote
                  if (strId[strId.length() - 1] == '\"')
                     strId = strId.substr(0, strId.length() - 1);

               }
            }
            else
            {

               strId = get_app_id(directrix()->m_varTopicFile);

               if (strId.length() <= 0)
               {

                  continue;

               }

            }

            m_straCommand.add(wstr);

            if (do_spa(strId, strParams))
            {

            }

         }

         for (auto & wstr : m_straCommand)
         {

            check_soon_launch(wstr, true);

         }

         iUserServiceTry = 300;

         while (!check_user_service("Win32", true))
         {

            Sleep(500);

            if (iUserServiceTry <= 0)
            {

               break;

            }

            iUserServiceTry--;

         }

      }

      return 1;

   }

   //
   // do_spa
   // installs the application synchronously
   // it just installs the application, it doesn't launch it
   int simple_app::do_spa(const char * pszId, const char * pszParams)
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

         strCommandLine = " : app=" + strId;

         strCommandLine += " install";

      }

      strCommandLine += " ";

      string strCommand;

      strCommand = "appinstall:";

      strCommand += strCommandLine;

      string strPlatform;

      if (strId == "app-core/user_service")
      {

         strPlatform = "x86";

      }
      else
      {

         strPlatform = process_platform_dir_name2();

      }

      app_install_call_sync(strPlatform, strCommand.c_str(), "");

      return true;

   }


   int simple_app::check_soon_launch(string strCommandLine, bool bLaunch)
   {

      string strId;

      string wstr = strCommandLine;

      int iFind1 = 0;

      if (wstr[0] == '\"')
      {

         iFind1 = wstr.find('\"', 1);

      }

      int iFind = wstr.find(" : ", iFind1 + 1);

      if (iFind < 0)
      {

         string strFile = wstr.substr(iFind1 + 1);

         strFile.trim();

         if (check_soon_file_launch(strFile, bLaunch))
         {

            return 1;

         }

      }
      else
      {

         string wstrRequest = wstr.substr(iFind + 3);

         string wstrValue;

         if (get_command_line_param(wstrValue, wstrRequest, "install"))
         {

            if (get_command_line_param(strId, wstrRequest, "enable_desktop_launch") && strId.length() > 0)
            {

               return check_soon_app_id(strId, bLaunch);

            }

            if (get_command_line_param(strId, wstrRequest, "app") && strId.length() > 0)
            {

               return check_soon_app_id(strId, bLaunch);

            }

         }
         else
         {

            if (get_command_line_param(strId, wstrRequest, "app") && strId.length() > 0)
            {

               return check_soon_app_id(strId, bLaunch);

            }
         }

      }

      if (strId.empty())
         return FALSE;

      return check_soon_app_id(strId, bLaunch);

   }



   string simple_app::get_app_id(string wstr)
   {

      if (wstr.length() <= 0)
      {

         return "";

      }

      wstr.trim();

      ::str::trim_any_quotes(wstr);

      wstr.trim();

      if (wstr.length() <= 0)
      {

         return "";

      }

      ::xml::document doc(get_app());

      ::string strPath = wstr.c_str();

      ::string strContents = file_as_string_dup(strPath.c_str());

      if (!doc.load(strContents.c_str()))
      {

         return "";

      }

      sp(xml::node) pnode = doc.get_root();

      if (pnode == NULL)
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

      const char * psz = pnode->GetChildAttrValue("launch", "app");

      if (psz == NULL || *psz == '\0')
      {

         return "";

      }

      return psz;

   }


   int simple_app::check_soon_file_launch(string wstr, bool bLaunch)
   {

      return check_soon_app_id(u16(get_app_id(wstr.c_str()).c_str()), bLaunch);

   }


   string simple_app::spa_app_id_to_app_name(string strId)
   {
      string strName;
      for (index i = 0; i < strId.length(); i++)
      {
         if (strId[i] == L'-' || strId[i] == L'/' || strId[i] == L'\\')
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


   int simple_app::check_soon_app_id(string strId, bool bLaunch)
   {

      if (check_soon_app_id1(strId, bLaunch))
      {

         return TRUE;

      }

      if (check_soon_app_id2(strId, bLaunch))
      {

         return TRUE;

      }

      return FALSE;

   }


   int simple_app::check_soon_app_id1(string strId, bool bLaunch)
   {

      if (strId.length() <= 0)
      {

         return 0;

      }

      string strName = spa_app_id_to_app_name(strId);

      string strApp = dir::stage(process_platform_dir_name()) / strName + ".exe";

      if (file_exists_dup(strApp))
      {

         if (!bLaunch)
         {

            // if dll loads consider good state

            string strDll = dir::stage(process_platform_dir_name()) / strName + ".dll";

            HMODULE hmodule = ::LoadLibraryW(wstring(strDll));

            bool bOk = hmodule != NULL;

            if (bOk)
            {

               ::FreeLibrary(hmodule);

            }

            return bOk;

         }

         SHELLEXECUTEINFOW sei = {};

         wstring wstrFile(strApp.c_str());

         sei.cbSize = sizeof(SHELLEXECUTEINFOW);

         sei.lpFile = wstrFile.c_str();

         if (::ShellExecuteExW(&sei))
         {

            return TRUE;

         }

      }

      return FALSE;

   }


   int simple_app::check_soon_app_id2(string strId, bool bLaunch)
   {

      if (strId.length() <= 0)
      {

         return 0;

      }

      string strName = spa_app_id_to_app_name(strId);

      {

         string strDll = dir::stage(process_platform_dir_name()) / strName + ".dll";

         string strApp = dir::stage(process_platform_dir_name()) / "app.exe";

         if (file_exists_dup(strDll) && file_exists_dup(strApp))
         {

            if (!bLaunch)
            {

               // if dll loads consider good state

               HMODULE hmodule = ::LoadLibraryW(wstring(strDll));

               bool bOk = hmodule != NULL;

               if (bOk)
               {

                  ::FreeLibrary(hmodule);

               }

               return bOk;

            }

            wstring wstrParams(": app=" + strId );

            wstring wstrApp(strApp);

            SHELLEXECUTEINFOW sei = {};

            sei.cbSize = sizeof(SHELLEXECUTEINFOW);

            sei.lpFile = wstrApp.c_str();
            
            sei.lpParameters = wstrParams.c_str();
            
            if (::ShellExecuteExW(&sei))
            {

               return TRUE;

            }

         }

      }

      return FALSE;

   }


   int simple_app::check_spa_installation(string strPlatform)
   {

      install & install = m_mapInstall[strPlatform];

      if (spa_get_admin())
      {

         trace("Downloading\r\n");

      }

      if (install.m_lProcessing > 0)
      {

         return 0;

      }

      stringa straFile;

      straFile.add("spaadmin");
      straFile.add("spa");
      straFile.add("vcredist");

      if (!file_exists_dup(dir::a_spa(strPlatform) / "no_install_bin_set.txt"))
      {

         straFile.add("install_bin_set");

      }

      install.m_lTotal = straFile.size();
      install.m_lProcessing = install.m_lTotal;
      install.m_lOk = 0;
      install.m_lBad = 0;


      for (int iFile = 0; iFile < straFile.size(); iFile++)
      {

         new install_item(this, straFile[iFile], strPlatform, &install);

      }

      int iRetry = 0;

      while (iRetry < 360 && install.m_lProcessing > 0 && ::get_thread_run() && (install.m_lBad <= 0 || spa_get_admin()))
      {

         Sleep(500);

         iRetry++;

      }

      Sleep(100);

      if (install.m_lProcessing > 0 || install.m_lBad > 0)
      {

         return 0;

      }

      return 1;

   }

   
   int simple_app::check_user_service(string strPlatform, bool bLaunch)
   {

      string strApp = dir::stage(strPlatform) / "app_core_user_service.exe";

      if (file_exists_dup(strApp))
      {

         if (is_user_service_running())
         {

            return TRUE;

         }

         if (!bLaunch || spa_get_admin())
         {

            // if dll loads consider good state

            string strDll = dir::stage(strPlatform) / "app_core_user_service.dll";

            HMODULE hmodule = ::LoadLibraryW(wstring(strDll));

            bool bOk = hmodule != NULL;

            if (bOk)
            {

               ::FreeLibrary(hmodule);

            }

            return bOk;

         }

         SHELLEXECUTEINFOW sei = {};

         wstring wstrFile(strApp);

         sei.cbSize = sizeof(SHELLEXECUTEINFOW);

         sei.lpFile = wstrFile.c_str();

         if (::ShellExecuteExW(&sei))
         {

            return TRUE;

         }

      }

      if (!do_spa("app-core/user_service"))
      {

         return FALSE;

      }

      return 1;

   }



   int simple_app::check_vcredist(string strPlatform)
   {

      string str = ::path::vcredist(strPlatform);

      if (!file_exists_dup(str) || !file_exists_dup(str + ".installed.txt"))
      {

         if (!spa_get_admin())
         {

            return 0;

         }

         if (!download_vcredist(strPlatform))
         {

            return 0;

         }

         if (!file_exists_dup(str))
         {

            return 0;

         }

         run_vcredist(strPlatform);

         file_put_contents_dup(str + ".installed.txt", "");

      }

      return 1;

   }
   int simple_app::download_vcredist(string strPlatform)
   {

      string strTempSpa = download_tmp_vcredist(strPlatform);

      if (!file_exists_dup(strTempSpa.c_str()))
      {

         return 0;

      }

      if (spa_get_admin())
      {

         string str = ::path::vcredist(strPlatform);

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

   string simple_app::download_tmp_vcredist(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::vcredist(strPlatform).title(), ::path::vcredist(strPlatform).extension());

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = http_download("https://server.ca2.cc/" + strPlatform + "/" + ::path::vcredist(strPlatform).name(), strTempSpa);

         if (bOk)
         {

            bOk = file_exists_dup(strTempSpa);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }

      return "";

   }


   int simple_app::check_spa_bin(string strPlatform)
   {

      string str = ::path::a_spa(strPlatform);

      if (!is_file_ok(::path::a_spa(strPlatform), ::path::a_spa(strPlatform).name(), NULL, strPlatform))
      {

         if (!spa_get_admin())
         {

            return 0;

         }

         if (!download_spa_bin(strPlatform))
         {

            return 0;

         }

         if (!is_file_ok(::path::a_spa(strPlatform), ::path::a_spa(strPlatform).name(), NULL, strPlatform))
         {

            return 0;

         }

      }

      return 1;

   }


   int simple_app::check_spaadmin_bin(string strPlatform, bool bStartNok)
   {

      ::file::path path = ::path::a_spaadmin(strPlatform);

      if (!bStartNok && is_file_ok(path, path.name(), NULL, strPlatform))
      {

         return true;

      }

      if (is_downloading_spaadmin())
      {

         return false;

      }

      keep < int > keepDownloadingSpaadmin(&g_iDownloadingSpaadmin, 1, 0, true);

      if (!download_spaadmin_bin(strPlatform))
      {

         return false;

      }

      if (!is_file_ok(path, path.name(), NULL, strPlatform))
      {

         return false;

      }

      return true;

   }


   int simple_app::download_spa_bin(string strPlatform)
   {

      string strTempSpa = download_tmp_spa_bin(strPlatform);

      if (!is_file_ok(strTempSpa, ::path::a_spa(strPlatform).name(), NULL, strPlatform))
      {

         return 0;

      }

      if (spa_get_admin())
      {

         string str = ::path::a_spa(strPlatform);

         if (!::CopyFileW(u16(strTempSpa.c_str()).c_str(), u16(str), FALSE))
         {

            return 0;

         }

         if (!is_file_ok(str, ::path::a_spa(strPlatform).name(), NULL, strPlatform))
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

   int simple_app::download_spaadmin_bin(string strPlatform)
   {

      string strTempSpa = download_tmp_spaadmin_bin(strPlatform);

      if (!is_file_ok(strTempSpa, ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform))
      {

         return 0;

      }

      if (spa_get_admin())
      {

         string str = ::path::a_spaadmin(strPlatform);

         if (!dir::mk(dir::name(str)))
         {

            return 0;

         }

         if (!::CopyFileW(u16(strTempSpa).c_str(), u16(str), FALSE))
         {

            return 0;

         }

         if (!is_file_ok(str, ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform))
         {

            return 0;

         }


         SHELLEXECUTEINFOW sei = {};

         wstring wstrFile(str);

         {

            spaadmin_mutex mutexStartup("-startup");

            sei.cbSize = sizeof(SHELLEXECUTEINFOW);

            sei.lpFile = wstrFile.c_str();

            ::ShellExecuteExW(&sei);

         }

      }
      else
      {

         if (strPlatform == "x86" && !low_is_spaadmin_running(strPlatform))
         {

            SHELLEXECUTEINFOW sei = {};

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

            string str = ::path::a_spaadmin(strPlatform);

            DWORD dwExitCode = 0;

            for (int i = 0; i < 1440; i++)
            {

               if (::GetExitCodeProcess(sei.hProcess, &dwExitCode))
               {

                  if (dwExitCode != STILL_ACTIVE && !low_is_spaadmin_running(strPlatform))
                  {

                     break;

                  }

               }
               else
               {

                  break;

               }

               if (is_file_ok(str, ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform))
                  break;

               ::WaitForSingleObject(sei.hProcess, 500);

            }

            ::CloseHandle(sei.hProcess);

         }

      }


      return 1;
   }



   string simple_app::download_tmp_spaadmin_bin(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::a_spaadmin(strPlatform).title(), ::path::a_spaadmin(strPlatform).extension());

      string strUrl;

      strUrl = "https://server.ca2.cc/" + strPlatform + "/" + m_strVersion + "/" + ::path::a_spaadmin(strPlatform).name();

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = http_download(strUrl, strTempSpa);

         if(bOk)
         {

            bOk = is_file_ok(strTempSpa, ::path::a_spaadmin(strPlatform).name(), NULL, strPlatform);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }


      return "";

   }



   string simple_app::download_tmp_spa_bin(string strPlatform)
   {

      string strTempSpa = get_temp_file_name_dup(::path::a_spa(strPlatform).title() + "-" + strPlatform, ::path::a_spa(strPlatform).extension());

      int iTry = 0;

      bool bOk;

      while (iTry <= 3)
      {

         bOk = http_download("https://server.ca2.cc/" + strPlatform + "/" + m_strVersion + "/" + ::path::a_spa(strPlatform).name(), strTempSpa);

         if (bOk)
         {

            bOk = is_file_ok(strTempSpa, ::path::a_spa(strPlatform).name(), NULL, strPlatform);

            if (bOk)
            {

               bOk = file_length_dup(strTempSpa) > 0;

               if (bOk)
               {

                  return strTempSpa;

               }

            }

         }

         iTry++;

      }


      return "";

   }


   void simple_app::http_get_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform)
   {

      if (straTemplate.get_size() <= 0)
      {

         return;

      }

      string strFormatBuild(pszFormatBuild);

      //string strUrl;

      //strUrl = "https://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
      //strUrl += System.url().url_encode(string(pszTemplate));
      //strUrl += "&build=";
      //strUrl += System.url().url_encode(string(pszFormatBuild));
      //strUrl += "&platform=";
      //strUrl += System.url().url_encode(strPlatform);

      //return http_get(strUrl);

      string strUrl;

      strUrl = "https://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
      strUrl += System.url().url_encode(straTemplate.implode(","));
      strUrl += "&build=";
      strUrl += System.url().url_encode(strFormatBuild);
      strUrl += "&platform=";
      strUrl += System.url().url_encode(strPlatform);

      string strMd5List = http_get(strUrl);

      stringa straMd5;

      straMd5.add_tokens(strMd5List.c_str(), ",");

      if (straMd5.get_size() != straTemplate.get_size())
      {

         return;

      }

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         // if (item.m_strMd5.is_empty() || (get_tick_count() - item.m_dwStart) > 10000)
      //{

         item.m_strMd5 = straMd5[i];

         item.m_dwStart = get_tick_count();

      }


   }


   stringa simple_app::get_reference_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform)
   {

      string strFormatBuild(pszFormatBuild);

      stringa straRequest;

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         if (item.m_strMd5.is_empty() || (get_tick_count() - item.m_dwStart) > 30000)
         {

            straRequest.add(straTemplate[i]);

         }

      }

      if (straRequest.has_elements())
      {

         http_get_md5(straRequest, strFormatBuild, strPlatform);

      }

      stringa straMd5;

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         straMd5.add(item.m_strMd5);

      }

      return straMd5;

   }


   bool simple_app::is_file_ok(const char * path1, const char * pszTemplate, const char * pszFormatBuild, string strPlatform)
   {

      if (!file_exists_dup(path1))
      {

         return false;

      }

      stringa straReferenceMd5 = get_reference_md5({ pszTemplate }, pszFormatBuild, strPlatform);

      if (straReferenceMd5.get_size() <= 0)
      {

         return false;

      }

      if (straReferenceMd5[0].compare_ci(file_md5_dup(path1)) != 0)
      {

         return false;

      }

      return true;

   }


   bool simple_app::is_file_ok(const stringa & straPath, const stringa & straTemplate, stringa & straMd5, const string & strFormatBuild, int iMd5Retry, string strPlatform)
   {

      bool bOk = true;

      if (straPath.size() != straTemplate.size())
      {
       
         return false;

      }

      if (bOk)
      {

         for (int i = 0; i < straPath.size(); i++)
         {

            string strPath = straPath[i];

            if (!file_exists_dup(strPath))
            {

               bOk = false;

               break;

            }

         }

      }

      if (iMd5Retry > 0 || straMd5.size() != straPath.size())
      {

         /*string strUrl;

         strUrl = "https://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
         strUrl += System.url().url_encode(straTemplate.implode(","));
         strUrl += "&build=";
         strUrl += System.url().url_encode(strFormatBuild);
         strUrl += "&platform=";
         strUrl += System.url().url_encode(strPlatform);*/

         straMd5 = get_reference_md5(straTemplate, strFormatBuild, strPlatform);

         //string strMd5List = ms_get(strUrl.c_str());

         //straMd5.add_tokens(strMd5List.c_str(), ",");

         if (straMd5.size() != straPath.size())
         {

            OutputDebugString("Error: Fetched " + str::from(straMd5.size()));
            OutputDebugString(" from " + str::from(straPath.size()));
            OutputDebugString("\r\n");

            int iMax = MAX(straMd5.size(), straPath.size());
            for (index i = 0; i < iMax; i++)
            {

               OutputDebugString("File: ");
               if (i < straPath.size())
               {
                  OutputDebugString(straPath[i]);
               }
               else
               {
                  OutputDebugString("(missing)");
               }
               OutputDebugString("\r\n");
               OutputDebugString("MD5: ");
               if (i < straPath.size())
               {
                  OutputDebugString(straMd5[i]);
               }
               else
               {
                  OutputDebugString("(missing)");
               }
               OutputDebugString("\r\n");

            }

            return false;

         }

         if (!bOk)
         {

            return false;

         }

      }

      for (int i = 0; i < straMd5.size(); i++)
      {

         string strPath = straPath[i];

         string strFileMd5 = file_md5_dup(strPath);

         string strMd5 = straMd5[i];

         if (strFileMd5.compare_ci(strMd5) != 0)
         {

            OutputDebugString("MD5 failed: " + strPath + " file MD5=" + strFileMd5 + " ref MD5=" + strMd5);
            OutputDebugString("\r\n");

            return false;

         }

      }

      return true;

   }





   int simple_app::check_install_bin_set(string strPlatform)
   {

      string strPath = path::app_install(strPlatform);

      stringa straFile = install_get_plugin_base_library_list(strPlatform, m_strVersion);

      if (!::dir::is(dir::name(strPath)))
      {

         if (!::dir::mk(dir::name(strPath)))
            return 0;

      }

      stringa straDownload;

      for (int iFile = 0; iFile < straFile.size(); iFile++)
      {

         string strFile = straFile[iFile];

         string strDownload = dir::name(strPath.c_str()) / strFile.c_str();

         straDownload.push_back(strDownload);

      }

      stringa straMd5;


      string strFormatBuild = get_latest_build_number(m_strVersion);

      int iMd5Retry = 0;

      string strBuild = strFormatBuild;

      strBuild = str::replace(" ", "_", strBuild);

   md5retry:

      if (!is_file_ok(straDownload, straFile, straMd5, strFormatBuild, iMd5Retry, strPlatform))
      {

         if (!spa_get_admin())
         {

            return 0;

         }


         if (straMd5.size() != straFile.size())
         {

            iMd5Retry++;

            if (iMd5Retry < 8)
               goto md5retry;

            return 0;

         }

#ifdef WINDOWSEX

         // first try to copy from current path (may be there is a version of app.install at the same folder).

         HMODULE hmodule = ::GetModuleHandle("aura.dll");

         if (hmodule != NULL)
         {

            string str = get_module_path(hmodule);

            if (str.length() > 0)
            {

               string strAuraDir = ::dir::name(str.c_str());

               for (int iFile = 0; iFile < straFile.size(); iFile++)
               {

                  string strFile = strAuraDir / straFile[iFile];

                  if (!file_exists_dup(straDownload[iFile].c_str()) && file_exists_dup(strFile.c_str()) && file_md5_dup(strFile.c_str()) == straMd5[iFile].c_str())
                  {

                     ::CopyFile(strFile.c_str(), straDownload[iFile].c_str(), false);

                  }

               }


            }


         }

#endif
         install install;
         install.m_lTotal = straFile.size();
         install.m_lProcessing = install.m_lTotal;

         trace("Downloading install bin set\r\n");

         string strUrlPrefix = "https://server.ca2.cc/ccvotagus/" + m_strVersion + "/" + strBuild + "/install/" + strPlatform + "/";
         

         //#pragma omp parallel for
         for (int iFile = 0; iFile < straFile.size(); iFile++)
         {

            string strDownload = dir::name(strPath) / straFile[iFile];

            if (!file_exists_dup(strDownload) || _stricmp(file_md5_dup(strDownload).c_str(), straMd5[iFile]) != 0)
            {

               if (straFile[iFile].compare_ci("app.install.exe") == 0)
               {

                  kill_process("app.install.exe");

               }

               new install_item(this, strUrlPrefix, strPath, straFile[iFile], &install, straMd5[iFile], strPlatform);

            }

         }

         int iRetry = 0;

         while (install.m_lProcessing > 0 && iRetry < 2000)
         {
            
            Sleep(100);

            iRetry++;

         }

         Sleep(100);

      }

      return 1;

   }



   string simple_app::get_latest_build_number(const char * pszVersion)
   {

      if (m_strBuild.length() > 0)
      {

         return m_strBuild;

      }

      string strBuildNumber;

      string strSpaIgnitionBaseUrl;

      string strVersion(pszVersion);

      if (file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt").length() > 0)
      {

         strSpaIgnitionBaseUrl = "https://" + file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt") + "/api/spaignition";

      }
      else if (pszVersion != NULL && !strcmp(pszVersion, "basis"))
      {

         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

      }
      else if (pszVersion != NULL && !strcmp(pszVersion, "stage"))
      {

         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

      }
      else
      {

         if (m_strVersion == "basis")
         {

            strVersion = "basis";

            strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

         }
         else
         {

            strVersion = "stage";

            strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

         }

      }

      int iRetry = 0;

   RetryBuildNumber:

      if (iRetry > 10)
      {

         return "";

      }

      iRetry++;

      strBuildNumber = http_get(strSpaIgnitionBaseUrl + "/query?node=build&version=" + strVersion);

      ::str::_008Trim(strBuildNumber);

      if (strBuildNumber.length() != 19)
      {

         Sleep(100 * iRetry);

         goto RetryBuildNumber;

      }

      return strBuildNumber;

   }





   void simple_app::trace(const string & str)
   {

      synch_lock sl(m_pmutex);

      trace_file(this).print(str);

      m_strHtmlLog += str + "<br>";

   }


   void simple_app::trace(double dRate)
   {

      synch_lock sl(m_pmutex);

      trace_file(this).print(dRate);

      char sz[200];

      sprintf(sz, "%0.1f", dRate * 100.0);

      m_strHtmlLog += string(sz) + "<br>";

   }

   void simple_app::start_a_spa_web_server()
   {

      if (m_pthreadSsl == NULL)
      {

         m_pthreadSsl = new ::a_spa::socket_thread(this);

         m_pthreadSsl->m_iSsl = 1;

         m_pthreadSsl->m_strIp = "127.0.0.1";

         m_pthreadSsl->m_strCat = "cat://" + read_resource_as_string(::GetModuleHandleA("spalib.dll"), IDR_CA2SP1, "CA2SP");

         if (spa_get_admin())
         {
#if OSBIT == 32
            if (m_pthreadSsl->m_iSsl > 0)
            {
               m_pthreadSsl->m_iPort = (port_t)57332;
            }
            else
            {
               m_pthreadSsl->m_iPort = (port_t)37332;
            }
#else
            if (m_pthreadSsl->m_iSsl > 0)
            {
               m_pthreadSsl->m_iPort = (port_t)57364;
            }
            else
            {
               m_pthreadSsl->m_iPort = (port_t)37364;
            }
#endif
         }
         else
         {
#if OSBIT == 32
            if (m_pthreadSsl->m_iSsl > 0)
            {
               m_pthreadSsl->m_iPort = (port_t)57232;
            }
            else
            {
               m_pthreadSsl->m_iPort = (port_t)37232;
            }
#else
            if (m_pthreadSsl->m_iSsl > 0)
            {
               m_pthreadSsl->m_iPort = (port_t)57264;
            }
            else
            {
               m_pthreadSsl->m_iPort = (port_t)37264;
            }
#endif
         }

         m_pthreadSsl->begin();

      }

   }


} // namespace a_spa



bool is_user_service_running()
{
   
   mutex mutex(get_thread_app(), "Local\\ca2_application_local_mutex:app-core/user_service");

   return mutex.already_exists();

}



