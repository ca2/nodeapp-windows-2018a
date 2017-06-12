#pragma once



class install_app :
   public ::base::simple_app,
   public ::aura::ipc::rx::receiver
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

   char *                           m_modpath;
   char *                           m_pszDllEnds;
   uint32_t *                       m_dwaProcess;
   int32_t                          m_iSizeProcess;
   HMODULE *                        m_hmodulea;
   int32_t                          m_iSizeModule;
   bool                             m_bInstallerInstalling;

   ::install::installer *           m_pinstaller;



   install_app();
   virtual ~install_app();



   ::install::installer & installer() { return *m_pinstaller; }

   void install_defer_file_transfer();

   bool is_user_using(const char * pszDll);

   ATOM spaboot_message_register_class(HINSTANCE hInstance);

   int32_t cube_run(const char * id);

   void call_self_privileged_sync(const char * param);

   int32_t spaboot_registry_register(const char * lpszFile);

   void call_sync(const char * path, const char * param);

   void installer_call_sync(const char * path, const char * param);

   bool are_there_user_files_in_use();

   virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

   virtual int32_t simple_app_pre_run();

   virtual bool intro();

   virtual bool end();

};
