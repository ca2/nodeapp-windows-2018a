#pragma once


namespace app_app
{


   class SPALIB_API app :
      virtual public ::aura::app
   {
   public:


      window *                      m_pwindow;
      bool                          m_bRunMainLoop;
      bool                          m_bAdmin;

      double                        m_dProgress;
      double                        m_dProgress1;
      double                        m_dProgress2;

      string                        m_strId;

      ::app_app::socket_thread *    m_pthreadSsl;

      string                        m_strVersion;
      bool                          m_bFinished;

      string                        m_strBuild;

      string_map < bootstrap * >    m_bootstrap;

      string                        m_strTraceLabel;


      app();
      virtual ~app();


      void add_command_line(string str);
      string pick_command_line();


      string get_id();

      void set_id(const char * psz);

      virtual string get_latest_build_number(const char * pszVersion);


      virtual int32_t run() override;

      virtual int app_app_main();
      virtual int start_app_app(string strPlatform);

      virtual bool get_admin();
      virtual void set_admin(bool bSet);

      virtual bool show_window();
      virtual bool hide_window();

      virtual void end_app_app();


      virtual bool is_user_service_running();

      virtual string get_version(string strVersion);

      virtual string get_title(string strTitle);

      virtual string get_build();

      virtual string get_app_id(string wstr);
      virtual int check_soon_launch(string str, bool bLaunch);
      virtual int check_soon_file_launch(string wstr, bool bLaunch);
      virtual int check_soon_app_id(string wstr, bool bLaunch);
      virtual int check_soon_app_id1(string wstr, bool bLaunch);
      virtual int check_soon_app_id2(string wstr, bool bLaunch);
      virtual string app_id_to_app_name(string strId);

      virtual void trace(const string & str);
      virtual void trace(double dRate);

      virtual void start_web_server();

      virtual bool register_spa_file_type();
      virtual void defer_start_program_files_app_app_admin(string strPlatform);
      virtual void start_program_files_app_app_admin(string strPlatform);
      virtual void get_system_locale_schema(string & strLocale, string & strSchema);

      virtual void defer_show_debug_box();

      
      virtual bool low_is_app_app_admin_running(string strPlatform);


   };


} // namespace app_app






