#include "framework.h"



HANDLE g_hinstancePluginbase = NULL;
void * g_pvoidPluginSystem = NULL;
HANDLE g_hmutex = NULL;


uint32_t plugin_container_app(const string & strChannel)
{

   ::plane::system * psystem = new ::plane::system(NULL);

   psystem->m_bShouldInitializeGTwf = false;

   psystem->m_hinstance = ::GetModuleHandle(NULL);

   manual_reset_event ev(psystem);

   psystem->m_peventReady = &ev;

   ev.ResetEvent();

   __start_core_system(psystem);

   if (!ev.wait(seconds(180)).signaled())
      return -1;

   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application(psystem, strChannel);

   papp->m_pbaseapp = psystem;

   papp->m_pbasesystem = psystem;

   papp->m_pplaneapp->m_psystem = psystem;

   papp->m_hinstance = psystem->m_hinstance;

   papp->m_pbasesession->m_pplanesession = psystem->get_session(0, NULL);

   try
   {

      if(!papp->pre_run())
      {

         return papp->m_iReturnCode;

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

   int32_t nReturnCode = 0;

   string strChannel(get_command_line_param(::GetCommandLine(), "channel").trimmed());

   g_hmutex = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::app.plugin.container::" + strChannel);

   if (::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      return -1;

   }

   if (file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {

      debug_box("app.plugin.container", "app.plugin.container", MB_OK);

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














