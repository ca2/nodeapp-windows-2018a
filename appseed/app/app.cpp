#include "nodeapp/os/os.h"




/////////////////////////////////////////////////////////////////////////////
// export WinMain to force linkage to this module
extern int32_t CLASS_DECL_win __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow);

extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance,
__in LPTSTR lpCmdLine, int32_t nCmdShow)
{
   g_hinstance = hinstance;

   UNREFERENCED_PARAMETER(lpCmdLine);

   ::CoInitialize(NULL);

   if (!main_initialize())
      return -1;

   //Sleep(15 * 1000);

   _set_purecall_handler(_ca2_purecall);

   sp(::plane::system) psystem = canew(::plane::system());

   g_pwindowmap = new ::oswindow_map(psystem);


   ASSERT(hPrevInstance == NULL);

   int32_t nReturnCode = 0;


   ::win::main_init_data * pinitmaindata = new ::win::main_init_data;


   pinitmaindata->m_hInstance = hinstance;
   pinitmaindata->m_hPrevInstance = hPrevInstance;
   pinitmaindata->m_vssCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());
   pinitmaindata->m_nCmdShow = nCmdShow;


   psystem->init_main_data(pinitmaindata);

   xxdebug_box("box1", "box1", MB_ICONINFORMATION);

   set_main_thread(GetCurrentThread());
   set_main_thread_id(GetCurrentThreadId());

   nReturnCode = psystem->main();

   delete g_pwindowmap;

   g_pwindowmap = NULL;

   try
   {
      main_finalize();
   }
   catch (...)
   {
   }

   if (__get_thread_state() != NULL)
   {
      __get_thread_state()->finalize();
      gen_ThreadState = NULL;
   }

   try
   {
      __win_term();
   }
   catch (...)
   {
   }


   try
   {
      delete psystem;
   }
   catch (...)
   {
   }

   psystem = NULL;


   try
   {
      delete __get_module_state()->m_pmapHWND;
   }
   catch (...)
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
      __get_module_state()->m_pmapHWND = NULL;
   }
   catch (...)
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


   return nReturnCode;


}
