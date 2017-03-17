#pragma once


namespace a_spa
{


   class SPALIB_API simple_app :
      virtual public ::aura::simple_app
   {
   public:


      ::a_spa::socket_thread *   m_pthreadSsl;

      string                     m_strId;
      string                     m_strVersion;
      string                     m_strBuild;
      spa_canvas *               m_pcanvas;
      HWND                       m_hwnd;

      string                     m_strPlatform;

      static simple_app *        s_papp;

      bool                       m_bFinished;
      string                     m_strSpaBootName;
      string                     m_strHtmlLog;

      simple_app();
      virtual ~simple_app();


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
      virtual int check_soon_launch();
      virtual int check_spa_installation(string strPlatform);
      virtual int show_spa_window(bool bShow = true);
      virtual int check_soon_file_launch(string wstr);
      virtual int check_soon_app_id(string wstr);
      virtual int check_soon_app_id1(string wstr);
      virtual int check_soon_app_id2(string wstr);
      virtual string spa_app_id_to_app_name(string strId);

      virtual void start_app_install_in_context(string strPlatform);
      virtual string get_latest_build_number(const char * pszVersion);

      virtual int check_user_service(string strPlatform);
      virtual int check_vcredist(string strPlatform);
      virtual int download_vcredist(string strPlatform);
      virtual string download_tmp_vcredist(string strPlatform);
      virtual int check_spa_bin(string strPlatform);
      virtual int download_spa_bin(string strPlatform);
      virtual string download_tmp_spa_bin(string strPlatform);
      virtual int check_spaadmin_bin(string strPlatform);
      virtual int download_spaadmin_bin(string strPlatform);
      virtual string download_tmp_spaadmin_bin(string strPlatform);
      virtual int check_install_bin_set(string strPlatofm);

      virtual bool is_file_ok(const char * path1, const char * pszTemplate, const char * pszFormatBuild);
      virtual bool is_file_ok(const stringa & straPath, const stringa & straTemplate, stringa & straMd5, const string & strFormatBuild, int iMd5Retry, string strPlatform);

      void trace(const string & str);
      void trace(double dRate);

      bool ms_download(const char * pszUrl, const char * pszFile, bool bUrlEncode = true, int * piStatus = NULL);
      bool ms_download_progress(const char * pszUrl, const char * pszFile, bool bProgress, bool bUrlEncode = true, int * piStatus = NULL);

      string ms_get(const char * pszUrl, bool bCache = false);

      static simple_app * get();

      LRESULT window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

      int create_spa_window();

   };


} // namespace a_spa






