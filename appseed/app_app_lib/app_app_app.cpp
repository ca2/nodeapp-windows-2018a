#include "framework.h"


namespace app_app
{


   app::app() :
      ::aura::system(NULL, this)
   {

      m_bRunMainLoop = true;
      
      m_pwindow = NULL;

      m_bAdmin = false;

      m_dProgress = -1.0;
      
      m_dProgress1 = -1.0;
      
      m_dProgress2 = -1.0;

      m_pthreadSsl = NULL;

   }


   app::~app()
   {


   }





   void app::defer_show_debug_box()
   {

      if (::file_exists_dup(::dir::system() / "config/spa/beg_debug_box.txt"))
      {

         string str;

         if (get_admin())
         {

            str = "zzzAPPzzz app_appadmin : ";

         }
         else
         {

            str = "zzzAPPzzz app_app : ";

         }

         str += string(::GetCommandLineW());

         ::MessageBoxA(NULL, str.c_str(), "zzzAPPzzz app_app", MB_ICONINFORMATION);

      }


   }


   int32_t app::run()
   {

      ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

      if (get_admin())
      {

         m_strTraceLabel = "app-admin-" + get_system_configuration() + "-" + ::str::from(OSBIT);

         start_web_server();

         manual_reset_event ev86(this);

         ev86.ResetEvent();

         manual_reset_event ev64(this);

         ev64.ResetEvent();

         ::fork(this, [&]()
         {

            try
            {

               ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

               m_bootstrap["admin:x86"] = new bootstrap(this);

               m_bootstrap["admin:x86"]->m_strTraceLabel = "bootstrap-admin-" + get_system_configuration() + "-x86";

               m_bootstrap["admin:x86"]->admin_main("x86");

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

               m_bootstrap["admin:x64"] = new bootstrap(this);

               m_bootstrap["admin:x64"]->m_strTraceLabel = "bootstrap-admin-" + get_system_configuration() + "-x64";

               m_bootstrap["admin:x64"]->admin_main("x64");

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

         m_strTraceLabel = "app_app-" + get_system_configuration() + "-" + ::str::from(OSBIT);

         m_iReturnCode = app_app_main();

      }


      return m_iReturnCode;

   }


   void app::add_command_line(string str)
   {

      {

         ::mutex mutexCommandFile(get_thread_app(), "Local\\ca2_spa_command:" + process_platform_dir_name2());

         ::file::path path = ::dir::system() / process_platform_dir_name2() / "spa_command.txt";

         stringa stra;
         
         if(file_load_stra(path, stra, false))
         {

            stra.add_unique_ci(str);

            file_save_stra(path, stra);

         }

      }

   }


   string app::pick_command_line()
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

      stra.remove_ci(str);

      file_save_stra(path, stra);

      return str;

   }


   bool app::show_window()
   {

      if (m_pwindow == NULL)
      {

         m_pwindow = new window(this);

      }

      if (!::IsWindow(m_pwindow->m_hwnd))
      {

         if (!m_pwindow->create())
         {

            return false;

         }

      }

      if (!m_pwindow->show())
      {

         return false;

      }

      return true;

   }


   bool app::hide_window()
   {

      if (m_pwindow == NULL)
      {

         return true;

      }

      if (!::IsWindow(m_pwindow->m_hwnd))
      {

         return true;

      }

      if (!m_pwindow->hide())
      {

         return false;

      }

      return true;

   }


   int app::app_app_main()
   {

      ::app_app::mutex mutex(process_platform_dir_name2());

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
         {

            return 0;

         }

      }

      add_command_line(str);

      if (!show_window())
      {

         return -1;

      }

      start_web_server();

      if (!start_app_app(process_platform_dir_name2()))
      {

         return -2;

      }

      m_pwindow->main();

      return m_iReturnCode;

   }

   
   string  app::get_build()
   {

      return m_strBuild;

   }



   int app::start_app_app(string strPlatform)
   {

      m_bFinished = false;

      m_bootstrap[strPlatform] = new bootstrap(this);

      m_bootstrap[strPlatform]->m_pthreadInstall = ::fork(this, [=]()
      {

         ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

         m_bootstrap[strPlatform]->m_strPlatform = strPlatform;

         m_bootstrap[strPlatform]->m_strTraceLabel = "bootstrap-" + get_system_configuration() + "-" + strPlatform;

         try
         {

            m_iReturnCode = m_bootstrap[strPlatform]->app_app();

         }
         catch (...)
         {

         }

         end_app_app();

         m_bFinished = true;

         return m_iReturnCode;

      });

      return 1;

   }



   int app::check_soon_launch(string strCommandLine, bool bLaunch)
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



   string app::get_app_id(string wstr)
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

      const char * psz = pnode->GetChildAttrValue("launch", "app");

      if (psz == NULL || *psz == '\0')
      {

         return "";

      }

      return psz;

   }


   int app::check_soon_file_launch(string wstr, bool bLaunch)
   {

      return check_soon_app_id(u16(get_app_id(wstr.c_str()).c_str()), bLaunch);

   }




   int app::check_soon_app_id(string strId, bool bLaunch)
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


   int app::check_soon_app_id1(string strId, bool bLaunch)
   {

      if (strId.length() <= 0)
      {

         return 0;

      }

      string strName = ::process::app_id_to_app_name(strId);

      string strApp = dir::stage(process_platform_dir_name()) / strName + ".exe";

      bool bOk;

      if (bLaunch)
      {

         bOk = System.is_application_installed(strId);

      }
      else
      {

         bOk = System.is_application_updated(strId);

      }

      if (file_exists_dup(strApp) && bOk)
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


   int app::check_soon_app_id2(string strId, bool bLaunch)
   {

      if (strId.length() <= 0)
      {

         return 0;

      }

      string strName = ::process::app_id_to_app_name(strId);

      {

         string strDll = dir::stage(process_platform_dir_name()) / strName + ".dll";

         string strApp = dir::stage(process_platform_dir_name()) / "app.exe";

         bool bOk;

         if (bLaunch)
         {

            bOk = System.is_application_installed(strId);

         }
         else
         {

            bOk = System.is_application_updated(strId);

         }

         if (file_exists_dup(strDll) && file_exists_dup(strApp) && bOk)
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












   void app::trace(const string & str)
   {

      synch_lock sl(m_pmutex);

      trace_file(this, m_strTraceLabel).print(str);

   }


   void app::trace(double dRate)
   {

      synch_lock sl(m_pmutex);

      trace_file(this, m_strTraceLabel).print(dRate);

   }

   void app::start_web_server()
   {

      if (m_pthreadSsl == NULL)
      {

         m_pthreadSsl = new socket_thread(this);

         m_pthreadSsl->m_iSsl = 1;

         m_pthreadSsl->m_strIp = "127.0.0.1";

         m_pthreadSsl->m_strCat = "cat://" + read_resource_as_string_dup(NULL, IDR_CAT, "CAT");

         if (get_admin())
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


   bool app::get_admin()
   {
      
      return m_bAdmin;

   }


   void app::set_admin(bool bSet)
   {

      m_bAdmin = bSet;

   }



   bool app::is_user_service_running()
   {

      ::mutex mutex(get_thread_app(), "Local\\ca2_application_local_mutex:app-core/user_service");

      return mutex.already_exists();

   }









   //string app::get_version(string strVersion)
   //{
   //   static string  s_strVersion;

   //   if (strVersion.has_char())
   //   {
   //      s_strVersion = strVersion;
   //   }

   //   if (s_strVersion.is_empty())
   //   {
   //      if (_ca_is_basis())
   //      {
   //         s_strVersion = "basis";
   //      }
   //      else
   //      {
   //         s_strVersion = "stage";
   //      }
   //   }

   //   return s_strVersion;
   //}


   string app::get_title(string strTitle)
   {
      static string  s_strTitle;

      if (strTitle.has_char())
      {
         s_strTitle = strTitle;
      }

      return s_strTitle;
   }


   bool app::register_spa_file_type()
   {

      HKEY hkey;

      wstring extension = L".spa";                     // file extension
      wstring desc = L"spafile";          // file type description
      wstring content_type = L"application/x-spa";

      wstring app(::path::app_app("x86"));

      wstring icon(app);

      app = L"\"" + app + L"\"" + L" \"%1\"";
      icon = L"\"" + icon + L"\",107";

      wstring action = L"Open";

      wstring sub = L"\\shell\\";

      wstring path = L"spafile\\shell\\open\\command";


      // 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.002
      if (RegCreateKeyExW(HKEY_CLASSES_ROOT, extension.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
      {
         output_debug_string("Could not create or open a registrty key\n");
         return 0;
      }
      RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)desc.c_str(), desc.length() * sizeof(wchar_t)); // default vlaue is description of file extension
      RegSetValueExW(hkey, L"ContentType", 0, REG_SZ, (BYTE*)content_type.c_str(), content_type.length() * sizeof(wchar_t)); // default vlaue is description of file extension
      RegCloseKey(hkey);



      // 2: Create Subkeys for action ( "Open with my program" )
      // HKEY_CLASSES_ROOT\.002\Shell\\open with my program\\command
      if (RegCreateKeyExW(HKEY_CLASSES_ROOT, path.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
      {
         output_debug_string("Could not create or open a registrty key\n");
         return 0;
      }
      RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)app.c_str(), app.length() * sizeof(wchar_t));
      RegCloseKey(hkey);


      path = L"spafile\\DefaultIcon";

      if (RegCreateKeyExW(HKEY_CLASSES_ROOT, path.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
      {
         output_debug_string("Could not create or open a registrty key\n");
         return 0;
      }
      RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)icon.c_str(), icon.length() * sizeof(wchar_t));
      RegCloseKey(hkey);

      wstring wstr(dir::app_app("x86") / "spa_register.txt");

      int iRetry = 9;

      while (!file_exists_dup(u8(wstr.c_str())) && iRetry > 0)
      {

         dir::mk(dir::name(u8(wstr.c_str())).c_str());

         file_put_contents_dup(u8(wstr.c_str()).c_str(), "");

         iRetry--;

         Sleep(84);

      }

      return true;

   }



   void app::start_program_files_app_app_admin(string strPlatform)
   {

      SHELLEXECUTEINFOW sei = {};

      string str = ::path::app_app_admin(strPlatform);

      if (!::file_exists_dup(str))
      {

         return;

      }

      ::app_app::admin_mutex mutexStartup("-startup");

      wstring wstr(str);

      sei.cbSize = sizeof(SHELLEXECUTEINFOW);
      sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
      sei.lpVerb = L"RunAs";
      sei.lpFile = wstr.c_str();
      ::ShellExecuteExW(&sei);
      DWORD dwGetLastError = GetLastError();

   }


   void app::defer_start_program_files_app_app_admin(string strPlatform)
   {

      if (low_is_app_app_admin_running(strPlatform))
      {

         return;

      }

      start_program_files_app_app_admin(strPlatform);

   }


   bool app::low_is_app_app_admin_running(string strPlatform)
   {

      ::app_app::admin_mutex smutex(strPlatform);

      return smutex.already_exists();

   }



   void app::get_system_locale_schema(string & strLocale, string & strSchema)
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



   void app::end_app_app()
   {

      m_bRunMainLoop = false;

      System.post_quit();


   }


   string app::get_id()
   {

      return m_strId;

   }

   
   void app::set_id(const char * psz)
   {

      m_strId = psz;

   }




   //string app::get_latest_build_number(const char * pszVersion)
   //{

   //   if (m_strBuild.length() > 0)
   //   {

   //      return m_strBuild;

   //   }

   //   string strBuildNumber;

   //   string strSpaIgnitionBaseUrl;

   //   string strVersion(pszVersion);

   //   if (file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt").length() > 0)
   //   {

   //      strSpaIgnitionBaseUrl = "https://" + file_as_string_dup("C:\\ca2\\config\\system\\ignition_server.txt") + "/api/spaignition";

   //   }
   //   else if (pszVersion != NULL && !strcmp(pszVersion, "basis"))
   //   {

   //      strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

   //   }
   //   else if (pszVersion != NULL && !strcmp(pszVersion, "stage"))
   //   {

   //      strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

   //   }
   //   else
   //   {

   //      if (m_strVersion == "basis")
   //      {

   //         strVersion = "basis";

   //         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

   //      }
   //      else
   //      {

   //         strVersion = "stage";

   //         strSpaIgnitionBaseUrl = "https://server.ca2.cc/api/spaignition";

   //      }

   //   }

   //   int iRetry = 0;

   //RetryBuildNumber:

   //   if (iRetry > 10)
   //   {

   //      return "";

   //   }

   //   iRetry++;

   //   strBuildNumber = http_get(strSpaIgnitionBaseUrl + "/query?node=build&configuration=" + strVersion);

   //   ::str::_008Trim(strBuildNumber);

   //   if (strBuildNumber.length() != 19)
   //   {

   //      Sleep(100 * iRetry);

   //      goto RetryBuildNumber;

   //   }

   //   return strBuildNumber;

   //}


   bool app::is_application_updated(string strAppId)
   {

      status & status = m_mapStatus[strAppId];

      if (status.m_iCheck >= 1)
      {

         if (status.m_bSlopeOk)
         {

            status.m_iCheck++;

            return true;

         }

         if (status.m_bOk)
         {

            status.m_iCheck++;

            if (get_tick_count() - status.m_dwLastOk < 5000)
            {

               return false; // still return not-updated status for 5s

            }
            else
            {

               return true;


            }


         }

      }

      bool bApplicationUpdated = false;

      string strName = ::process::app_id_to_app_name(strAppId);

      string strApplication = dir::stage(process_platform_dir_name()) / strName + ".exe";

      string strDll = dir::stage(process_platform_dir_name()) / strName + ".dll";

      string strApp = dir::stage(process_platform_dir_name()) / "app.exe";

      if (file_exists_dup(strApplication) || (file_exists_dup(strDll) && file_exists_dup(strApp)))
      {

         string strConfiguration = System.get_system_configuration();

         string strLatestBuildNumber = System.get_latest_build_number(strConfiguration);

         bApplicationUpdated = System.is_application_installed(strAppId, strLatestBuildNumber);

      }

      status.m_iCheck++;

      if (!status.m_bOk && bApplicationUpdated)
      {

         status.m_dwLastOk = get_tick_count();

      }

      status.m_bOk = bApplicationUpdated;

      if (status.m_iCheck <= 1)
      {

         status.m_bSlopeOk = bApplicationUpdated;

      }

      return bApplicationUpdated;

   }


} // namespace app_app




