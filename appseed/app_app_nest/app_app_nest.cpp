#include "framework.h"



HANDLE g_hinstancePluginbase = NULL;
void * g_pvoidPluginSystem = NULL;
HANDLE g_hmutex = NULL;


uint32_t plugin_container_app(const string & strChannel)
{

   ::plugin::system * psystem = new ::plugin::system(NULL);

   psystem->m_hinstance = ::GetModuleHandle(NULL);

   ::set_thread(psystem);

   try
   {

      if(!psystem->pre_run())
      {

         return papp->get_exit_code();

      }

   }
   catch(...)
   {

      return -1;

   }

   __start_core_system_main(psystem);

   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application(psystem, strChannel);

   papp->m_paurasession = psystem->m_paurasession;

   papp->m_paxissession = psystem->m_paxissession;

   papp->m_pbasesession = psystem->m_pbasesession;

   papp->m_paxisapp = psystem->m_paxisapp;

   papp->m_pbaseapp = psystem;

   papp->m_pbasesystem = psystem;

   papp->m_pcoresystem = psystem;

   papp->m_hinstance = psystem->m_hinstance;

   papp->m_pbasesession->m_pcoresession = psystem->get_platform(0, NULL);

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

   int32_t nReturnCode = papp->main();

   ExitProcess(nReturnCode);

   return nReturnCode;

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














