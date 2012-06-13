﻿#include "framework.h"



HANDLE g_hinstancePluginbase = NULL; 
void * g_pvoidPluginSystem = NULL;
HANDLE g_hmutex = NULL;


int __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int nCmdShow)
{
	
   UNREFERENCED_PARAMETER(lpCmdLine);


   ::CoInitialize(NULL);


   if(!main_initialize())
      return -1;


   //_set_purecall_handler(_ca2_purecall);

   ::plane::system * psystem = new ::plane::system();

   ASSERT(hPrevInstance == NULL);

   int nReturnCode = 0;


   vsstring strChannel = get_command_line_param(::GetCommandLine(), "channel");


   g_hmutex = ::CreateMutex(NULL, FALSE, "Global\\ca2::fontopus::ca2plugin-container::" + strChannel);
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      return -1;
   }



   ::ca2plugin_container::application * papp = new ::ca2plugin_container::application();


   if(!papp->initialize(strChannel))
      return -1;




   nReturnCode = papp->run();


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
      delete psystem;
   }
   catch(...)
   {
   }

   psystem = NULL;


   try
   {
//      delete __get_module_state()->m_pmapHWND;
   }
   catch(...)
   {
   }
/*   try
   {
      delete __get_module_state()->m_pmapHDC;
   }
   catch(...)
   {
   }*/
/*   try
   {
      delete __get_module_state()->m_pmapHGDIOBJ;
   }
   catch(...)
   {
   }*/
//      delete __get_module_state()->m_pmapHMENU;

   try
   {
//      __get_module_state()->m_pmapHWND     = NULL;
   }
   catch(...)
   {
   }
/*   try
   {
      __get_module_state()->m_pmapHDC      = NULL;
   }
   catch(...)
   {
   }*/
/*   try
   {
      __get_module_state()->m_pmapHGDIOBJ  = NULL;
   }
   catch(...)
   {
   }*/


   set_heap_mutex(NULL);

   return nReturnCode;

}





/////////////////////////////////////////////////////////////////////////////
// export WinMain to force linkage to this module

extern "C" int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int nCmdShow)
{
   // call shared/exported WinMain
   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}














