#pragma once


namespace app_app_install
{


   class app :
      virtual public ::aura::app,
      virtual public ::aura::ipc::rx::receiver
   {
   public:


      enum e_message
      {
         message_none,
         message_ok,
         message_failed,
         message_unknown,
      };


      e_message                        m_emessage;
      HANDLE                           m_hmutexSpabootInstall;
      ::aura::ipc::rx                  m_rxchannel;

      bool                             m_bInstallerInstalling;

      ::app_app_install::installer *   m_pinstaller;



      app();
      virtual ~app();



      ::app_app_install::installer & installer() { return *m_pinstaller; }

      bool is_user_using(const char * pszDll);

      ATOM spaboot_message_register_class(HINSTANCE hInstance);

      int32_t cube_run(const char * id);

      void call_self_privileged_sync(const char * param);

      int32_t spaboot_registry_register(const char * lpszFile);

      void call_sync(const char * path, const char * param);

      void installer_call_sync(const char * path, const char * param);

      bool are_ca2_framework_shared_libraries_busy();

      virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

      virtual int32_t simple_app_pre_run();

      virtual bool intro();

      virtual bool end();

   };



} // namespace application



