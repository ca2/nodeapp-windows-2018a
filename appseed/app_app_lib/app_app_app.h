#pragma once


namespace app_app
{


   class SPALIB_API app :
      virtual public ::aura::app
   {
   public:

      class md5_item
      {
      public:

         DWORD       m_dwStart;
         string      m_strMd5;

      };

      TCHAR szTitle[2048];					// The title bar text
      TCHAR szWindowClass[2048];			// the main window class name





      ::app_app::socket_thread *   m_pthreadSsl;

      string                     m_strId;
      string                     m_strVersion;
      string                     m_strBuild;
      canvas *                   m_pcanvas;
      HWND                       m_hwnd;

      string                     m_strPlatform;

      static app *        s_papp;

      bool                       m_bFinished;
      string                     m_strSpaBootName;
      string                     m_strHtmlLog;

      string_map < string_map < string_map < md5_item > > > m_mapMd5;

      string_map < install >  m_mapInstall;

      stringa     m_straCommand;


      app();
      virtual ~app();


      void add_command_line(string str);
      string pick_command_line();

      void DragMainWindow();

      string spa_get_id()
      {
         return m_strId;
      }

      void spa_set_id(const char * psz)
      {
         m_strId = psz;
         m_strSpaBootName = psz;
      }

      virtual int32_t run() override;

      static UINT c_cdecl spa_main_proc(LPVOID);


      virtual int spa_main();
      virtual int spa_main_start(string strPlatform);
      virtual int spalib_main2();
      virtual int spalib_main32();
      virtual int spalib_main_plat();


      virtual void defer_show_debug_box();

      virtual int spaadmin_main(string strPlatform);
      virtual int do_spa(const char * psz, const char * pszParams = NULL);

      virtual string get_app_id(string wstr);
      virtual int check_soon_launch(string str, bool bLaunch);
      virtual int check_spa_installation(string strPlatform);
      virtual int show_spa_window(bool bShow = true);
      virtual int check_soon_file_launch(string wstr, bool bLaunch);
      virtual int check_soon_app_id(string wstr, bool bLaunch);
      virtual int check_soon_app_id1(string wstr, bool bLaunch);
      virtual int check_soon_app_id2(string wstr, bool bLaunch);
      virtual string spa_app_id_to_app_name(string strId);

      virtual void start_app_install_in_context(string strPlatform, bool bAlreadyElevated);
      virtual string get_latest_build_number(const char * pszVersion);

      virtual int check_user_service(string strPlatform, bool bLaunch);
      virtual int check_vcredist(string strPlatform);
      virtual int download_vcredist(string strPlatform);
      virtual string download_tmp_vcredist(string strPlatform);
      virtual int check_spa_bin(string strPlatform);
      virtual int download_spa_bin(string strPlatform);
      virtual string download_tmp_spa_bin(string strPlatform);
      virtual int check_spaadmin_bin(string strPlatform, bool bStartNok = false);
      virtual int download_spaadmin_bin(string strPlatform);
      virtual string download_tmp_spaadmin_bin(string strPlatform);
      virtual int check_install_bin_set(string strPlatofm);

      virtual bool is_file_ok(const char * path1, const char * pszTemplate, const char * pszFormatBuild, string pszPlatform);
      virtual bool is_file_ok(const stringa & straPath, const stringa & straTemplate, stringa & straMd5, const string & strFormatBuild, int iMd5Retry, string strPlatform);

      void trace(const string & str);
      void trace(double dRate);

      bool http_download(const char * pszUrl, const char * pszFile, bool bUrlEncode = true, int * piStatus = NULL);
      bool http_download_progress(const char * pszUrl, const char * pszFile, bool bProgress, bool bUrlEncode = true, int * piStatus = NULL);

      void http_get_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform);
      stringa get_reference_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform);

      string http_get(const char * pszUrl, bool bCache = false);

      static app * get();

      LRESULT window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

      int create_spa_window();

      virtual void start_app_app_web_server();

      void keep_drawing();

      string do_install(const char * psz);


      int spalib_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int       nCmdShow);

      void kill_process(string strImageName);
      int register_spa_file_type();
      int is_downloading_spaadmin();
      void end_spa(::aura::application * papp);
      bool low_is_spaadmin_running(string strPlatform);
      void defer_start_program_files_spa_admin(string strPlatform);
      void get_system_locale_schema(string & strLocale, string & strSchema);
      void app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild);
      void run_vcredist(string strPlatform);
      string get_module_path(HMODULE hmodule);
      string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);

      ATOM spa_register_class();



   };


} // namespace app_app






