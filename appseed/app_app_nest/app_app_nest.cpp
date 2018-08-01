#include "framework.h"



HANDLE g_hinstancePluginbase = NULL;
void * g_pvoidPluginSystem = NULL;
HANDLE g_hmutex = NULL;


uint32_t plugin_container_app(const string & strChannel)
{

   ::plugin::system * psystem = new ::plugin::system(NULL, NULL);

   psystem->m_hinstance = ::GetModuleHandle(NULL);

   ::set_thread(psystem);

   try
   {

      if(!psystem->pre_run())
      {

         return psystem->get_exit_code();

      }

   }
   catch(...)
   {

      return -1;

   }

   __start_core_system_main(psystem);

   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application(psystem, strChannel);

   papp->m_psession = psystem->m_psession;

   papp->m_psession = psystem->m_psession;

   papp->m_psession = psystem->m_psession;

   papp->m_papp = psystem->m_papp;

   papp->m_papp = psystem;

   papp->m_psystem = psystem;

   papp->m_psystem = psystem;

   papp->m_hinstance = psystem->m_hinstance;

   papp->m_psession->m_psession = psystem->get_platform(0, NULL);

   papp->create_host();

   try
   {

      if(!papp->pre_run())
      {

         return papp->get_exit_code();

      }

   }
   catch(...)
   {

      return -61;

   }

   papp->main();

   int iExitCode = papp->get_exit_code();

   ExitProcess(iExitCode);

   return iExitCode;

}


int32_t __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   UNREFERENCED_PARAMETER(lpCmdLine);

   if(!defer_core_init())
   {

      return -6;

   }


   //if(file_exists_dup("C:\\ca2\\config\\plugin\\npca2_beg_debug_box.txt"))
   //{

   //   debug_box("app_app_nest boxmain NP_Initialize","app_app_nest box",MB_OK);

   //}

   int32_t nReturnCode = 0;

   string strChannel(get_command_line_param(::GetCommandLine(), "channel"));

   g_hmutex = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::app_app_nest::" + strChannel);

   if (::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      return -1;

   }

   if (file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {

      debug_box("app_app_nest", "app_app_nest", MB_OK);

   }

   nReturnCode = plugin_container_app(strChannel);

   try
   {

      defer_core_term();

   }
   catch (...)
   {

   }


   return nReturnCode;

}


extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}














