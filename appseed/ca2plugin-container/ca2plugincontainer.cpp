#include "framework.h"



HANDLE g_hinstancePluginbase = ::null(); 
void * g_pvoidPluginSystem = ::null();
HANDLE g_hmutex = ::null();


uint32_t thread_proc_app(void * lpParam)
{

   vsstring * pstrChannel = (vsstring *) lpParam;

   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application();


   if(!papp->initialize(*pstrChannel))
      return -1;

   int32_t nReturnCode = papp->run();

   return nReturnCode;

}


int32_t __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   //Sleep(15 * 1000);

   xxdebug_box("ca plugin container win_main", "ca plugin container", 0);
	
   UNREFERENCED_PARAMETER(lpCmdLine);

   ::CoInitialize(::null());

   if(!main_initialize())
      return -1;

   //_set_purecall_handler(_ca2_purecall);

   //::plane::system * psystem = new ::plane::system();

   ASSERT(hPrevInstance == ::null());

   int32_t nReturnCode = 0;

   vsstring * pstrChannel = new vsstring(get_command_line_param(::GetCommandLine(), "channel"));

   g_hmutex = ::CreateMutex(::null(), FALSE, "Global\\::ca::fontopus::ca2plugin-container::" + *pstrChannel);

   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {

      return -1;

   }

   start_thread(&thread_proc_app, (LPVOID) pstrChannel);

   MSG msg;
      
   while(true)
	{

      if(!GetMessage(&msg, ::null(), 0, 0xffffffffu))
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

   
   //psystem = ::null();

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

      //__get_module_state()->m_pmapHWND     = ::null();

   }
   catch(...)
   {

   }

/* 
   try
   {

      __get_module_state()->m_pmapHDC      = ::null();

   }
   catch(...)
   {

   }
*/

/* 
   try
   {

      __get_module_state()->m_pmapHGDIOBJ  = ::null();

   }
   catch(...)
   {

   }
*/

   set_heap_mutex(::null());

   return nReturnCode;

}


extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}














