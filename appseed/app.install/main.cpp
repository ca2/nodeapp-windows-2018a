#include "framework.h"




#include <psapi.h>


#include <tlhelp32.h>




class app_install :
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


   e_message                     m_emessage;
   HANDLE                        m_hmutexSpabootInstall;
   ::aura::ipc::rx               m_rxchannel;
   
   char *                        m_modpath;
   char *                        m_pszDllEnds;
   uint32_t *                    m_dwaProcess;
   int32_t                       m_iSizeProcess;
   HMODULE *                     m_hmodulea;
   int32_t                       m_iSizeModule;
   bool                          m_bInstallerInstalling;

   ::install::installer *        m_pinstaller;


   app_install();
   virtual ~app_install();
   
   //void install_defer_file_transfer();

   ATOM spaboot_message_register_class(HINSTANCE hInstance);

   int32_t cube_run(const char * id);

   void call_self_privileged_sync(const char * param);

   int32_t spaboot_registry_register(const char * lpszFile);

   void call_sync(const char * path, const char * param);

   void installer_call_sync(const char * path, const char * param);

   virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

   virtual int32_t simple_app_pre_run();

   virtual bool intro();

   virtual bool end();

};


extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return ::base::simple_app_main < app_install > (hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}


app_install::app_install() :
   ::object(this),
   ::aura::system(NULL, NULL),
   ::axis::system(NULL),
   ::base::system(NULL),
   m_rxchannel(this)
{

   m_pinstaller = new ::install::installer(this);

   if (file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {
      debug_box("app.install", "app", 0);
   }
   
   m_hinstance             = ::GetModuleHandleA(NULL);
   m_hmutexSpabootInstall  = NULL;
   e_message m_emessage    = message_none;
   m_modpath               = NULL;
   m_pszDllEnds            = NULL;
   m_dwaProcess            = NULL;
   m_iSizeProcess          = 0;
   m_hmodulea              = NULL;
   m_iSizeModule           = 0;
   m_bInstallerInstalling  = false;
   m_bMatterFromHttpCache  = true;

   m_bAdvancedGUI          = false;

   construct(NULL);

}


app_install::~app_install()
{


}


int32_t app_install::simple_app_pre_run()
{

   System.oprop("do_not_initialize_user_presence") = true;

   xxdebug_box("app.install", "app.install", MB_OK);

   if (__argc >= 2)
   {

      if (!strncmp_dup(__argv[1], "-install:", strlen_dup("-install:")))
      {

         //Sleep(15 * 1000);

         string strCommandLine;

         /*for (int32_t i = 1; i < __argc; i++)
         {

            if (i == 1)
            {
               strCommandLine = &__argv[1][strlen_dup("-install:")];
            }
            else
            {
               strCommandLine = strCommandLine + " ";
               strCommandLine = strCommandLine + __argv[i];
            }

         }

         xxdebug_box(strCommandLine, "simple_app::body", 0);*/

         uint32_t dwStartError;

         strCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());

         strsize iFind;

         if ((iFind = strCommandLine.find_ci("-install:")) >= 0)
         {

            //strCommandLine = strCommandLine.Left(iFind) + " " + strCommandLine.substr(iFind + strlen("-install:"));

            strCommandLine = strCommandLine.substr(iFind + strlen("-install:"));

         }

         xxdebug_box(strCommandLine, "simple_app::body", 0);

         m_pinstaller->ca2_app_install_run(strCommandLine, dwStartError, true);

         return -1;

      }

   }

   return 0;

}

bool app_install::intro()
{

   string strMutex;

   strMutex = "Global\\::ca2::fontopus::ca2_spaboot_install_"+process_platform_dir_name2()+"::7807e510-5579-11dd-ae16-0800200c7784";

   m_hmutexSpabootInstall = ::CreateMutex(NULL, FALSE, strMutex);

   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      m_iReturnCode = -202;

      return false;

   }

   m_modpath      = (char *) memory_alloc(MAX_PATH * 8);
   m_pszDllEnds   = (char *) memory_alloc(MAX_PATH * 8);
   m_iSizeProcess = 1024;
   m_dwaProcess   = (uint32_t *) memory_alloc(m_iSizeProcess);
   m_iSizeModule  = 1024;
   m_hmodulea     = (HMODULE *) memory_alloc(m_iSizeModule);

   m_rxchannel.m_preceiver = this;

   prepare_small_bell();

   string strChannel;

   // "core/spaboot_install"

   strChannel = "::ca2::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784";

   if(!m_rxchannel.create(strChannel))
   {
      m_iReturnCode = -1;
      return false;
   }

   return true;

}

//void app_install::install_defer_file_transfer()
//{
//
//   if (!System.install().is_installing_ca2())
//   {
//
//      System.install().update_updated();
//
//      if(!System.install().is_updated() && !m_pinstaller->are_there_user_files_in_use())
//      {
//
//         m_pinstaller->install_synch("starter_start: : app=session session_start=session app_type=application install in background in spa");
//
//      }
//
//   }
//
//}


typedef int32_t (__cdecl * PFN_SPAADMIN_MAIN)(const char * pszCommandLine);



void app_install::on_receive(::aura::ipc::rx * prx, const char * pszMessage)
{
   
   string strMessage(pszMessage);

   int32_t iRet = 0;

   if(::str::begins_eat(strMessage, "app_install:"))
   {

      if (System.install().is_installing_ca2())
      {

         iRet = 1;

         return;

      }

      if(m_bInstallerInstalling)
      {

         iRet = 1;

         return;

      }
      
      m_bInstallerInstalling = true;
      
      m_pinstaller->install_synch(strMessage);

      m_bInstallerInstalling = false;

   }
   else if(::str::begins_eat(strMessage, "asynch_app_install:"))
   {

      if (System.install().is_installing_ca2())
      {

         iRet = 1;

         return;

      }

      if(m_bInstallerInstalling)
      {

         iRet = 1;

         return;

      }

      m_pinstaller->install_asynch(strMessage);

   }
   else if(strMessage == "ok")
   {

      m_emessage = message_ok;

   }
   else if(strMessage == "failed")
   {

      m_emessage = message_failed;

   }
   else if(strMessage == "exit")
   {

      PostQuitMessage(0);

   }
   else if(strMessage == "quit")
   {

      PostQuitMessage(0);

   }
   else
   {

      m_emessage = message_unknown;

   }

}




bool app_install::end()
{

   simple_app::end();

   memory_free(m_hmodulea);
   memory_free(m_dwaProcess);
   memory_free(m_pszDllEnds);
   memory_free(m_modpath);

   ::aura::del(m_pinstaller);

   return true;

}








