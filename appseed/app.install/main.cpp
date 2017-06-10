#include "framework.h"




#include <psapi.h>


#include <tlhelp32.h>




class installer :
   public ::base::simple_app,
   public ::aura::ipc::rx::receiver,
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
   ::aura::ipc::rx       m_rxchannel;
   
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

   virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

   virtual int32_t simple_app_pre_run();

   virtual bool intro();

   virtual bool end();

};


// if MSVC CRT is used
extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

   // call shared/exported WinMain
   return ::base::simple_app_main < installer > (hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}


// if MSVC CRT is stripped
/*extern "C" int32_t WinMainCRTStartup() \
{ 

   ExitProcess(simple_app::s_main < installer > ());

}*/


installer::installer() :
   ::object(this),
   ::aura::system(NULL, NULL),
   ::axis::system(NULL),
   ::base::system(NULL),
   ::install::installer(this),
   m_rxchannel(this)
{
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

installer::~installer()
{
}

int32_t installer::simple_app_pre_run()
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

         ca2_app_install_run(strCommandLine, dwStartError, true);

         return -1;

      }

   }

   return 0;

}

bool installer::intro()
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

void installer::install_defer_file_transfer()
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

void installer::on_receive(::aura::ipc::rx * prx, const char * pszMessage)
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



bool installer::are_there_user_files_in_use()
{
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "msvcp110d.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "msvcr110d.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca2.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca2.dll"))
      return true;
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








