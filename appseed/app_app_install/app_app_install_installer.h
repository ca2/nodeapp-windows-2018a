#pragma once



namespace app_app_install
{


   class installer :
      virtual public ::http::listener,
      virtual public int_scalar_source::listener
   {
   public:


      mutex                            m_mutex;

      stringa                          m_straTerminateProcesses;
      stringa                          m_straRestartCommandLine;
      stringa                          m_straRestartProcess;
      string                           m_strLastHost;
      string                           m_strCurrentHost;

#ifdef WINDOWS

      MESSAGE                          m_msg;

#endif

      string                           m_strPlat;
      string                           m_strPlat2;

      string                           m_strVersion;

      string                           m_strAppMatterList;

      uint32_t                         m_dwDownloadCallbackLast100k;
      uint32_t                         m_iDownloadCallbackColumn;
      uint64_t                         m_dwDownloadLen;
      int32_t                          m_cx;
      int32_t                          m_cy;
      string                           m_strHost;
      uint32_t                         m_dwInstallStartTime;

      bool                             m_bProgressModeAppInstall;
      double                           m_dAppInstallFileCount;

      ::file::path                     m_pathBaseUrl;

      string                           m_strLocale;
      string                           m_strSchema;

      string                           m_strCommand;
      
#ifdef WINDOWS

      MESSAGE                          m_msgSpaAdminWindow;

#endif

      ::user::primitive *              m_pwindow;

      string                           m_strBuild;
      string                           m_strBuildResource;
      string                           m_strApplicationId;
      string                           m_strApplicationType;

      machine_event                    m_machineevent;

      double                           m_dProgress;
      double                           m_dProgress1;
      double                           m_dProgress2;
      ::xml::document                  m_xmldocStringTable;
      double                           m_dDownloadRate;

      ::file::path                     m_pathIndex;
      ::file::path                     m_pathIndexCompressed;
      ::file::path                     m_pathInstall;

      bool                             m_bInstallSet;

      double                           m_dProgressStart;
      double                           m_dProgressEnd;
      stringa                          m_strSpa;
      stringa                          m_straHost;
      double                           m_dAnime;

      bool                             m_NeedRestartBecauseOfReservedFile;
      bool                             m_NeedRestartFatalError;
      string                           m_strTitle;
      string                           m_strApplicationName;
      string                           m_strSpaIgnitionBaseUrl;
      uint64_t                         m_dwCurFileLen;


      int32_t                          m_nCmdShow;

      int32_t                          m_iScreen;
      int32_t                          m_iProgressMode;
      int64_t                          m_iTotalGzLen2;
      int64_t                          m_iProgressTotalGzLen2;
      int64_t                          m_iGzLen2;

      int64_array                      m_iaProgress;
      double_array                     m_daProgress;
      double                           m_dAppInstallProgressBase;


      string                           m_strLogin;
      string                           m_strSessid;
      string                           m_strLoginFailed;
      oswindow                         m_oswindow;

      string                           m_strFile;

      stringa                          m_straHttpFailure;


      bool                             m_bLaunch;

      sp(::sockets::http_session)      m_phttpsession;
      ::sockets::socket_handler *      m_psockethandler;
      spa(::sockets::http_session)     m_httpsessionptra;
      mutex                            m_mutexOmp;

      int                              m_iInstallResult;


      installer(::aura::application * papp);
      virtual ~installer();


      string http_get(const string & strUrl, bool bScalarListener);

      void set_progress(double dProgress);

      ::count download_file_list(::file::patha & stringa, string_to_intptr & mapLen, string_to_string & mapCrc, string_to_intptr & mapGzLen, string_to_intptr & mapFlag);

      bool get(const string& url_in, bool bExist, int64_t iLength, const char * pszCrc, int64_t iGzLen);

      bool download_file(const string& inplace, const string& url_in, bool bExist, bool bCheck, int64_t iLength, const char * pszCrc, int64_t iGzLen, int_ptr & iFlag);

      ::file::path ca2_get_dir(LPCSTR lpcszUrl);

      ::file::path ca2bz_get_dir(LPCSTR lpcszUrl);

      ::file::path ca2unbz_get_dir(LPCSTR lpcszUrl);

      ::file::path ca2inplace_get_dir(LPCSTR lpcszUrl);

      ::file::path ca2_get_file(LPCSTR lpcszUrl);

      ::file::path ca2bz_get_file(LPCSTR lpcszUrl,const char * pszCrc);

      ::file::path ca2unbz_get_file(LPCSTR lpcszUrl);

      ::file::path ca2inplace_get_file(LPCSTR lpcszUrl);

      bool ca2_fy_url(string & str, LPCSTR lpcszPath, bool bExist, int64_t iLength, const char * pszCrc, int64_t iGzLen, bool bIndex = false, bool * pbFileOk = NULL);

      int32_t get_file_list(::file::patha & stringa,LPCSTR lpcszPath,string_to_intptr & mapLen,string_to_intptr & mapGzLen,string_to_string & mapCrc,string_to_intptr & mapFlag);

      int32_t get_file_list_ex(::file::patha & stringa,::file::patha & patha,string_to_intptr & mapLen,string_to_intptr & mapGzLen,string_to_string & mapCrc,string_to_intptr & mapFlag);

      int32_t get_local_file_list(::file::patha & stringa,LPCSTR lpcszUrl);

      ::count uncompress_file_list(::file::patha & stringa,string_to_string & strmapCrc);

      ::count copy_file_list(::file::patha & stringa,string_to_intptr & mapFlag);

      string load_string(const char * pszId, const char * pszDefault);

      void parse_index_file(const char * psz, string_to_intptr & mapLen, string_to_string & mapCrc, string_to_intptr & mapGzLen, string_to_intptr & mapFlag);

      bool spa_exec(const char * psz);

      bool launch_application();

      bool launch_applications();

      int32_t appmatter_list();

      int32_t ca2_build_version();

      int32_t ca2_build_version_etc(string & strSpaHost,int32_t &iHostRetry,stringa & straMd5AppInstall,int_array & iaLen);

      int32_t calc_host(string & strSpaHost, int32_t &iHostRetry);

      int32_t install(string strCommand);

      bool is_application_opened(const char * psz);

      bool is_there_application_opened();

      void machine_signalize_close_application();

      bool machine_unsignalize_close_application();

      bool machine_check_close_application(bool bDefault);

      void add_spa_start(const char * pszId);

      bool reboot();

      void remove_spa_start(const char * pszId);

      virtual void on_http_complete(::sockets::http_socket * psocket, ::http::e_status estatus);

      void new_progress_end(double dMilestone);

      int32_t run_ca2_application_installer(const char * id);

      int32_t final_launch_application();

      virtual int32_t app_install_synch(const char * pszCommandLine, uint32_t & dwStartError, bool bSynch);

      virtual void on_set_scalar(int_scalar_source * psource,e_scalar escalar,int64_t iValue,int iFlags);

   };


} // namespace install




