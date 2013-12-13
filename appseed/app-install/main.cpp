﻿#include "framework.h"


#include <psapi.h>


#include <tlhelp32.h>




class installer :
   public simple_app,
   public small_ipc_rx_channel::receiver,
   public ::install::installer
{
public:

   
   enum e_message
   {
      message_none,
      message_ok,
      message_failed,
      message_unknown,
   };


   e_message                  m_emessage;
   HANDLE                     m_hmutexSpabootInstall;
   small_ipc_rx_channel       m_rxchannel;
   
   char *                     m_modpath;
   char *                     m_pszDllEnds;
   uint32_t *                    m_dwaProcess;
   int32_t                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   int32_t                        m_iSizeModule;
   bool                       m_bInstallerInstalling;

   installer();
   virtual ~installer();
   
   void install_defer_file_transfer();

   bool is_user_using(const char * pszDll);

   ATOM spaboot_message_register_class(HINSTANCE hInstance);

   int32_t cube_run(const char * id);

   void call_self_privileged_sync(const char * param);

   int32_t spaboot_registry_register(const char * lpszFile);

   void call_sync(const char * path, const char * param);

   void installer_call_sync(const char * path, const char * param);

   bool are_there_user_files_in_use();

   bool is_user_using(uint32_t processid, const char * pszDll);

   virtual void on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage);

   virtual int32_t simple_app_pre_run();

   virtual bool intro();

   virtual bool end();

};

// if MSVC CRT is used
extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow)
{
   // call shared/exported WinMain
   return simple_app::s_main < installer > ();
}

// if MSVC CRT is stripped
/*extern "C" int32_t WinMainCRTStartup() \
{ 

   ExitProcess(simple_app::s_main < installer > ());

}*/


installer::installer() :
   element(this),
   base_system(NULL),
   ::install::installer(this)
{
   ////////////////////////////////////////////////////////////
   // configuration encryption system : with C:\\" hardware :-)
   // short login               short password  access configuration
   // |                         |               |
   // -----------------------   --       --------
   //                       |    |       |
   if (file_exists_dup("C:\\ca2\\config\\beg_debug_box.txt"))
   {
      debug_box("app-install", "app", 0);
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
   m_bInstalling = false;

   construct();
}

installer::~installer()
{
}

int32_t installer::simple_app_pre_run()
{
   xxdebug_box("app-install", "app-install", MB_OK);

   if (__argc >= 2)
   {

      if (!strncmp_dup(__argv[1], "-install:", strlen_dup("-install:")))
      {

         //Sleep(15 * 1000);

         string strCommandLine;

         for (int32_t i = 1; i < __argc; i++)
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

         xxdebug_box(strCommandLine, "simple_app::body", 0);

         uint32_t dwStartError;

         strCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());

         strsize iFind;

         if ((iFind = strCommandLine.find_ci("-install:")) >= 0)
         {

            strCommandLine = strCommandLine.Left(iFind) + " " + strCommandLine.substr(iFind + strlen("-install:"));

         }

         xxdebug_box(strCommandLine, "simple_app::body", 0);

         ca2_app_install_run(strCommandLine, dwStartError, true);

         return -1;

      }

   }

   return 0;

}

bool installer::intro()
{



   m_hmutexSpabootInstall = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::ca2_spaboot_install::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      m_iError = -202;
      return false;
   }

   System.install().installation_file_lock(false);

   //Sleep(15 * 1000);

   m_modpath      = (char *) memory_alloc(MAX_PATH * 8);
   m_pszDllEnds   = (char *) memory_alloc(MAX_PATH * 8);
   m_iSizeProcess = 1024;
   m_dwaProcess   = (uint32_t *) memory_alloc(m_iSizeProcess);
   m_iSizeModule  = 1024;
   m_hmodulea     = (HMODULE *) memory_alloc(m_iSizeModule);

   m_rxchannel.m_preceiver = this;

   prepare_small_bell();

   if(!m_rxchannel.create("core/spaboot_install", "app-install.exe"))
   {
      m_iError = -1;
      return false;
   }

   return true;

}

void installer::install_defer_file_transfer()
{

   if (!m_bInstalling)
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


// non-thread safe
bool installer::is_user_using(uint32_t processid, const char * pszDll)
{

   HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
   MODULEENTRY32 me32;
   bool bFound = false;

   hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, processid );
   if( hModuleSnap == INVALID_HANDLE_VALUE )
   {
      return false;
   }

   me32.dwSize = sizeof(MODULEENTRY32);

   if(!Module32First(hModuleSnap, &me32))
   {
      ::CloseHandle(hModuleSnap);
      return false;
   }

   strcpy_dup(m_pszDllEnds, "\\");
   strcat_dup(m_pszDllEnds, pszDll);

   do
   {
      if(str_ends_ci_dup(me32.szModule, m_pszDllEnds) || stricmp_dup(me32.szModule, pszDll) == 0)
      {
         bFound = true;
         break;
      }

   }
   while(Module32Next(hModuleSnap, &me32));

   ::CloseHandle( hModuleSnap );
   return bFound;

}

// non-thread safe
bool installer::is_user_using(const char * pszDll)
{
   HANDLE hProcessSnap;
   PROCESSENTRY32 pe32;

   hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(hProcessSnap == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   
   pe32.dwSize = sizeof(PROCESSENTRY32);

   if(!Process32First(hProcessSnap, &pe32))
   {
      ::CloseHandle(hProcessSnap);
      return false;
   }

   bool bUsing = false;
   do
   {
      if(is_user_using(pe32.th32ProcessID, pszDll))
      {
         bUsing = true;
         break;
      }
   }
   while(Process32Next(hProcessSnap, &pe32));

  ::CloseHandle(hProcessSnap);

  return bUsing;

}

void installer::on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage)
{
   string strMessage(pszMessage);
   int32_t iRet = 0;
   const char * pszSuffix;
   if((pszSuffix = str_begins_inc_dup(strMessage, "synch_spaadmin:")) != NULL)
   {
      if (m_bInstalling)
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
      if(m_bInstalling)
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



bool installer::are_there_user_files_in_use()
{
#ifdef X86
   if(is_user_using(dir::element("stage\\x86\\msvcp110d.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x86\\msvcr110d.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x86\\ca.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x86\\ca2.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x86\\ca2.dll")))
      return true;
#else
   if(is_user_using(dir::element("stage\\x64\\msvcp110d.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x64\\msvcr110d.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x64\\ca.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x64\\ca2.dll")))
      return true;
   if(is_user_using(dir::element("stage\\x64\\ca2.dll")))
      return true;
#endif
   return false;
}


bool installer::end()
{

   simple_app::end();

   memory_free(m_hmodulea);
   memory_free(m_dwaProcess);
   memory_free(m_pszDllEnds);
   memory_free(m_modpath);

   return true;

}








