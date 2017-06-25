#pragma once


namespace app_app
{


   class SPALIB_API bootstrap :
      virtual public tool
   {
   public:



      bool                          m_NeedRestartBecauseOfReservedFile;
      bool                          m_NeedRestartFatalError;
      bool                          m_bLoginStartup;


      string                        m_strId;

      string                        m_strPlatform;

      string                        m_strSpaBootName;

      string_map < install >        m_mapInstall;

      stringa                       m_straCommand;

      sp(::thread)                  m_pthreadInstall;

      bool                          m_bForceUpdatedBuild;
      bool                          m_bInstalling;
      DWORD                         m_dwInstallStartTime;
      bool                          m_bShowPercentage;

      stringa                       m_strSpa;
      stringa                       m_straHost;

      bool                          m_bDownloadingAdmin;


      bootstrap(::aura::application * papp);
      virtual ~bootstrap();


      virtual int app_app();
      virtual int app_app_x86();
      virtual int app_app_proper();


      bool is_user_service_running();

      //string get_configuration(string strConfiguration);

      string get_title(string strTitle);

      virtual int admin_main(string strPlatform);
      virtual int do_app_app(const char * psz, const char * pszParams = NULL);

      virtual bool check_app_app_installation(string strPlatform);

      virtual int check_user_service(string strPlatform, bool bLaunch);
      virtual int check_vcredist(string strPlatform);
      virtual int download_vcredist(string strPlatform);
      virtual string download_tmp_vcredist(string strPlatform);
      virtual int check_app_app_bin(string strPlatform);
      virtual int download_app_app_bin(string strPlatform);
      virtual string download_tmp_app_app_bin(string strPlatform);
      virtual int check_app_app_admin_bin(string strPlatform, bool bStartNok = false);
      virtual int download_app_app_admin_bin(string strPlatform);
      virtual string download_tmp_app_app_admin_bin(string strPlatform);
      virtual int check_install_bin_set(string strPlatofm);

      virtual bool is_downloading_admin();

   };


} // namespace app_app






