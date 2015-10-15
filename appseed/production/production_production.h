#pragma once


namespace production
{

   class view;


   class CLASS_DECL_CA2_PRODUCTION production : 
      virtual public simple_thread 
   {
   public:

      enum e_version
      {
         version_backup,
         version_basis,
         version_stage,
      };


      class release :
         virtual public thread
      {
      public:
         

         production *   m_pproduction;
         string         m_strRelease;
         string         m_strServer;


         release(production * pproduction,const char * pszRelease,const char * pszServer);

         virtual bool initialize_instance();

         virtual int32_t run();

         virtual int32_t raw_run();

      };

      class compress_thread :
         virtual public thread
      {
      public:
         compress_thread(production * pproduction, manual_reset_event * peventFinished);

         manual_reset_event * m_pevFinished;
         production *   m_pproduction;
         uint32_t                m_dwThreadAffinityMask;
         int32_t run();
      };


      mutex                   m_mutexStatus;
      stringa                 m_straStatus;
      bool                    m_bLoop;
      int32_t                     m_iLoop;
      int32_t                     m_iLoopCount;
      string                  m_strSubversionRevision;
      stringa                 m_straCompress;
      mutex                   m_mutexCompress;
      string                  m_strBuildTook;
      string                  m_strSpc;
      string                  m_strSignTool;
      string                  m_strSignPass;
      bool                    m_bReleased;
      string                  m_strStartTime;
      string                  m_strStdPostColor;
      string                  m_strBackPostColor;
      string                  m_strEmpPostColor;
      string                  m_strVersion;
      string                  m_strStatusEmail;
      int32_t                 m_iGlobalRetry;
      string                  m_strTry;
      string                  m_strDownloadSite;

      string                  m_strTwitterConsumerKey;
      string                  m_strTwitterConsumerSecret;

      mutex                   m_mutexRelease;
      stringa                 m_straRelease;
      int32_t                 m_iRelease;

      e_version               m_eversion;

      ::datetime::time        m_timeStart;
      ::datetime::time        m_timeEnd;

      stringa                 m_straStageDirAcceptedFileExtensions;

      int32_t                     m_iStep;
                              
      sp(::user::impact)                m_pview;
      bool                    m_bFinished;
      uint32_t                   m_dwStartTick;
      uint32_t                   m_dwEndTick;
      stringa                 m_straCC;
      stringa                 m_straBB;
      string                  m_strBuild;
      string                  m_strFormatBuild;
      bool                    m_bEndProduction;
                              
      string                  m_strTag;
      ::file::path            m_strTagPath;
                              
      string                  m_strStatus;
      ::file::path            m_strBase;
      ::file::listing         m_straRoot;
      strsize                 m_iBaseLen;
      ::file::listing         m_straFiles;



      stringa                 m_straManifest;
      stringa                 m_straSignature;
      ::file::listing         m_straPath;


      bool                    m_bClean;
      bool                    m_bBuild;
                              
                              
      ::file::path            m_strVrel;
      ::file::path            m_strCCAuth;
      ::file::path            m_strCCVrel;
      string                  m_strCCVrelNew;
      string                  m_strIndexMd5;

      bool                    m_bEndStatus;
      manual_reset_event      m_evFinish;


      production(::aura::application * papp);
      virtual ~production();



      void start_production(e_version eversion);
      void start_loop(e_version eversion, int32_t iLoopCount);
      void step();

      void defer_quit();

      void add_status(const char * psz);
      void change_status(const char * psz);

      string version_to_international_datetime(const char * psz);


      virtual int32_t run();


      virtual int32_t produce();

      bool get_file_list(const char * pszBase, const char * pszDir, ::file::listing & stra, bool bFileSet = false);
      // bool ftp_put_dir(CFtpConnection & ftpconn, const char * pszBase, const char * pszDir, int32_t iMode);

      virtual void compress();
      virtual bool compress_next();

      virtual void generate_appmatter_spa();
      virtual void generate_appmatter_spa(const ::file::path & pszRoot);
      virtual void generate_appmatter_spa_folder(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa_locale(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa_style(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa(const ::file::path & pszRoot,const ::file::path & pszRelative);

      virtual void release_production();
      virtual bool release_npca2(const char * pszPlatform);
      virtual bool release_iexca2(const char * pszPlatform);
      virtual bool release_crxca2(const char * pszPlatform);

      virtual void add_path(const ::file::path & pszDir, const ::file::path & pszRelative);

      bool sync_source(const char * psz, const char * pszRevision);
      bool commit_for_new_build_and_new_release();
      bool commit_source(const char * psz);


      virtual bool twitter_auth();
      virtual string twitter_twit(const char * pszMessage);

      virtual bool facebook_auth();
      virtual string facebook_status(const char * pszMessage);

      void compress(const ::file::path & lpszRelative);

      void OnUpdateRelease();

      string xpi_digest(memory & mem);

      void xpi_sign_dir(const ::file::path & pszDir);
      void xpi_section(const char * pszManifest, const char * pszSignature);

      bool create_xpi(const char * pszPlatform, bool bSigned = true);
      bool create_unsigned_xpi(const char * pszPlatform);
      bool create_signed_xpi(const char * pszPlatform);

      void build(const char * psz);

      virtual void update_rc_file_version(const char * pszUrl);

      virtual string stage_platform(string strPlatform);

   };


} // namespace production


