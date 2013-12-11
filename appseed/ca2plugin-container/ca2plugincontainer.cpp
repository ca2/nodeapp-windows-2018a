#include "framework.h"



HANDLE g_hinstancePluginbase = NULL; 
void * g_pvoidPluginSystem = NULL;
HANDLE g_hmutex = NULL;


uint32_t thread_proc_app(void * lpParam)
{

   string * pstrChannel = (string *) lpParam;

//   _set_purecall_handler(_ca2_purecall);


   ::plane::system * psystem = new ::plane::system(NULL);

//   psystem->m_bExitIfNoApplications = false;

   psystem->m_hinstance = ::GetModuleHandle(NULL);

   manual_reset_event ev(psystem);

   psystem->m_peventReady = &ev;

   ev.ResetEvent();

   __start_system(psystem);

   if (!ev.wait(seconds(180)).signaled())
      return -1;

   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application(psystem, *pstrChannel);

   papp->m_pbaseapp = psystem;

   papp->m_pbasesystem = psystem;

   papp->m_pplaneapp->m_psystem = psystem;

   papp->m_hinstance = psystem->m_hinstance;

   papp->m_pplaneapp->m_psession = psystem->get_session(0, NULL);

   papp->m_pbasesession = papp->m_pplaneapp->m_psession;

   int32_t nReturnCode = papp->main();

   ExitProcess(nReturnCode);

   return nReturnCode;

}


int32_t __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   //Sleep(15 * 1000);

   xxdebug_box("ca2 plugin container win_main", "ca2 plugin container", 0);
	
   UNREFERENCED_PARAMETER(lpCmdLine);

   ::CoInitialize(NULL);

   if(!main_initialize())
      return -1;

   ASSERT(hPrevInstance == NULL);

   int32_t nReturnCode = 0;

   string * pstrChannel = new string(get_command_line_param(::GetCommandLine(), "channel"));

   g_hmutex = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::ca2plugin-container::" + *pstrChannel);

   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      return -1;

   }

   start_thread(&thread_proc_app, (LPVOID) pstrChannel);

   MSG msg;
      
   while(true)
	{

      if(!GetMessage(&msg, NULL, 0, 0xffffffffu))
         break;

      if(msg.message == WM_QUIT)
         break;

		TranslateMessage(&msg);

      DispatchMessage(&msg);

	}

   try
   {

      main_finalize();

   }
   catch(...)
   {

   }

   try
   {

      //__win_term();

   }
   catch(...)
   {

   }


   try
   {

      //delete psystem;
 
   }
   catch(...)
   {
   
   }

   
   //psystem = NULL;

   try
   {

      //delete __get_module_state()->m_pmapHWND;

   }
   catch(...)
   {

   }

/* 
   try
   {

      delete __get_module_state()->m_pmapHDC;

   }
   catch(...)
   {

   }
*/

/*
   try
   {

      delete __get_module_state()->m_pmapHGDIOBJ;

   }
   catch(...)
   {

   }
*/

   //delete __get_module_state()->m_pmapHMENU;

   try
   {

      //__get_module_state()->m_pmapHWND     = NULL;

   }
   catch(...)
   {

   }

/* 
   try
   {

      __get_module_state()->m_pmapHDC      = NULL;

   }
   catch(...)
   {

   }
*/

/* 
   try
   {

      __get_module_state()->m_pmapHGDIOBJ  = NULL;

   }
   catch(...)
   {

   }
*/

   //set_heap_mutex(NULL);

   return nReturnCode;

}


extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}














