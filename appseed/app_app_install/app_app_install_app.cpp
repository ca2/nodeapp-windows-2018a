#include "framework.h"


namespace app_app_install
{


   app::app() :
      ::object(this),
      ::aura::system(NULL, NULL),
      m_rxchannel(this)
   {

      m_pinstaller = new ::app_app_install::installer(this);

      ////////////////////////////////////////////////////////////
      // configuration encryption system : with C:\\" hardware :-)
      // short login               short password  access configuration
      // |                         |               |
      // -----------------------   --       --------
      //                       |    |       |
      if (file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
      {

         debug_box("app_app_install", "app", 0);

      }

      m_hinstance = ::GetModuleHandleA(NULL);
      m_hmutexSpabootInstall = NULL;
      m_emessage = message_none;
      m_bInstallerInstalling = false;
      m_bMatterFromHttpCache = true;

      construct(NULL);

   }


   app::~app()
   {

   }


   int32_t app::simple_app_pre_run()
   {

      System.oprop("do_not_initialize_user_presence") = true;

      xxdebug_box("app_app_install", "app_app_install", MB_OK);

      if (__argc == 1)
      {

         ::file::path pathCa2 = (::dir::module() - 3);

         string str = file_as_string_dup(pathCa2 / "spa.appinstall");

         if (str.has_char())
         {

            return m_pinstaller->install(str);

         }

      }
      else if (__argc >= 2)
      {

         ::file::path path;

         path = __argv[1];

         if (path.extension().compare_ci("appinstall") == 0)
         {

            string str = file_as_string_dup(path);

            if (str.has_char())
            {

               return m_pinstaller->install(str);

            }

         }

         string strCommandLine;

         strCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());

         const char * pszCommand = strCommandLine;

         string strFile = ::str::consume_command_line_argument(pszCommand);

         string strCommand(pszCommand);

         if (strCommand.has_char())
         {

            return m_pinstaller->install(strCommand);

         }

      }


      return 0;

   }


   bool app::intro()
   {

      string strMutex;

      strMutex = "Global\\::ca2::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784";

      m_hmutexSpabootInstall = ::CreateMutex(NULL, FALSE, strMutex);

      if (::GetLastError() == ERROR_ALREADY_EXISTS)
      {

         m_iReturnCode = -202;

         return false;

      }

      m_rxchannel.m_preceiver = this;

      prepare_small_bell();

      string strChannel;

      strChannel = "::ca2::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784";

      if (!m_rxchannel.create(strChannel))
      {

         m_iReturnCode = -1;

         return false;

      }

      return true;

   }


   void app::on_receive(::aura::ipc::rx * prx, const char * pszMessage)
   {

      string strMessage(pszMessage);

      int32_t iRet = 0;

      if (::str::begins_eat_ci(strMessage, "appinstall:"))
      {

         if (System.install().is_installing_ca2())
         {

            iRet = 1;

            return;

         }

         if (m_bInstallerInstalling)
         {

            iRet = 1;

            return;

         }

         m_bInstallerInstalling = true;

         m_pinstaller->install(strMessage);

         m_bInstallerInstalling = false;

      }
      else if (stricmp_dup(strMessage, "ok") == 0)
      {

         m_emessage = message_ok;

      }
      else if (stricmp_dup(strMessage, "failed") == 0)
      {

         m_emessage = message_failed;

      }
      else if (stricmp_dup(strMessage, "exit") == 0)
      {

         post_quit();

      }
      else if (stricmp_dup(strMessage, "quit") == 0)
      {

         post_quit();

      }
      else
      {

         m_emessage = message_unknown;

      }

   }

   ///////////////////////////
   //
   // heuristic
   //
   bool app::are_ca2_framework_shared_libraries_busy()
   {

      stringa stra({ "aura.dll", "axis.dll", "base.dll", "core.dll" });

      ::file::patha patha;

      patha = dir::stage(process_platform_dir_name()) / stra;

      stra.copy_iter(patha);

      if (is_shared_library_busy(stra))
      {

         return true;

      }

      return false;

   }


   bool app::end()
   {

      ::aura::app::end();

      //memory_free(m_hmodulea);
      //memory_free(m_dwaProcess);
      //memory_free(m_pszDllEnds);
      //memory_free(m_modpath);

      ::aura::del(m_pinstaller);

      return true;

   }



} // namespace app_app_install







