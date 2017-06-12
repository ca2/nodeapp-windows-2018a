#include "framework.h"


#include <psapi.h>
#include <tlhelp32.h>



install_app::install_app() :
   ::object(this),
   ::aura::system(NULL, NULL),
   ::axis::system(NULL),
   ::base::system(NULL),
   m_rxchannel(this)
{

   m_pinstaller = new ::install::installer(this);

   ////////////////////////////////////////////////////////////
   // configuration encryption system : with C:\\" hardware :-)
   // short login               short password  access configuration
   // |                         |               |
   // -----------------------   --       --------
   //                       |    |       |
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

install_app::~install_app()
{



}

int32_t install_app::simple_app_pre_run()
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

         ::install::ca2_app_install_run(strCommandLine, dwStartError, true);

         return -1;

      }

   }

   return 0;

}

bool install_app::intro()
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

void install_app::install_defer_file_transfer()
{

   if (!System.install().is_installing_ca2())
   {

      System.install().update_updated();

      if(!System.install().is_updated() && !are_there_user_files_in_use())
      {

         // missing locale schema;

         throw "missing locale and schema parameters for installing";
         install_synch("starter_start: : app=session session_start=session app_type=application install in background in spa");

      }

   }

}


typedef int32_t (__cdecl * PFN_SPAADMIN_MAIN)(const char * pszCommandLine);



void install_app::on_receive(::aura::ipc::rx * prx, const char * pszMessage)
{
   string strMessage(pszMessage);
   int32_t iRet = 0;
   const char * pszSuffix;
   if((pszSuffix = str_begins_inc_dup(strMessage, "synch_spaadmin:")) != NULL)
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
      install_synch(pszSuffix);
      m_bInstallerInstalling = false;
   }
   else if((pszSuffix = str_begins_inc_dup(strMessage, "spaadmin:")) != NULL)
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
      install_asynch(pszSuffix);
   }
   else if(stricmp_dup(strMessage, "ok") == 0)
   {
      m_emessage = message_ok;
   }
   else if(stricmp_dup(strMessage, "failed") == 0)
   {
      m_emessage = message_failed;
   }
   else if(stricmp_dup(strMessage, "exit") == 0)
   {
      PostQuitMessage(0);
   }
   else if(stricmp_dup(strMessage, "quit") == 0)
   {
      PostQuitMessage(0);
   }
   else
   {
      m_emessage = message_unknown;
   }
}



bool install_app::are_there_user_files_in_use()
{

   if (is_shared_library_busy(
      dir::stage(process_platform_dir_name2()) / {"aura.dll", "axis.dll", "base.dll", "core.dll"}))
      return true;

   return false;

}


bool install_app::end()
{

   simple_app::end();

   memory_free(m_hmodulea);
   memory_free(m_dwaProcess);
   memory_free(m_pszDllEnds);
   memory_free(m_modpath);

   ::aura::del(m_pinstaller);

   return true;

}








