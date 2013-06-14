#include "framework.h"
#include "sal.h"

extern CLASS_DECL_win oswindow_map * g_pwindowmap;

CLASS_DECL_ca2 int32_t ca2_main();

//CLASS_DECL_ca2 extern fixed_alloc_array * g_pfixedallocaWstring;

//CLASS_DECL_ca2 fixed_alloc_array * new_wstring_manager();

void CLASS_DECL_win __cdecl _ca2_purecall()
{
   throw simple_exception(::ca2::get_thread_app());
}


void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers);


//sp(::ca2::application)     win_application_create(sp(::ca2::application) pappSystem, const char * pszId);
//sp(::ca2::application)     win_instantiate_application(sp(::ca2::application) pappSystem, const char * pszId);
//int32_t                     win_application_procedure(sp(::ca2::application) pappSystem)
//UINT c_cdecl          win_application_thread_proc(LPVOID);
/////////////////////////////////////////////////////////////////////////////
// Standard WinMain implementation
//  Can be replaced as long as '::ca2::WinInit' is called first

int32_t CLASS_DECL_win __win_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   g_hinstance = hinstance;

   UNREFERENCED_PARAMETER(lpCmdLine);

   ::CoInitialize(NULL);

   if(!main_initialize())
      return -1;

   //Sleep(15 * 1000);


   _set_purecall_handler(_ca2_purecall);

   sp(::plane::system) psystem = canew(::plane::system());

   g_pwindowmap = new ::oswindow_map(psystem);

   ASSERT(hPrevInstance == NULL);

   int32_t nReturnCode = 0;


   ::win::main_init_data * pinitmaindata  = new ::win::main_init_data;


   pinitmaindata->m_hInstance             = hinstance;
   pinitmaindata->m_hPrevInstance         = hPrevInstance;
   pinitmaindata->m_vssCommandLine        = ::ca2::international::unicode_to_utf8(::GetCommandLineW());
   pinitmaindata->m_nCmdShow              = nCmdShow;

   
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
   catch(...)
   {
   }

   if(__get_thread_state()  != NULL)
   {
      __get_thread_state()->finalize();
      gen_ThreadState = NULL;
   }

   try
   {
      __win_term();
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
      delete __get_module_state()->m_pmapHWND;
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
      __get_module_state()->m_pmapHWND     = NULL;
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


int32_t CLASS_DECL_win ca2_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char * lpCmdLine, int32_t nCmdShow)
{
   try
   {

      _set_purecall_handler(_ca2_purecall);

      ::plane::system * psystem = new ::plane::system();


      //Sleep(30 * 1000);


      ASSERT(hPrevInstance == NULL);

      int32_t nReturnCode = 0;

      ::win::main_init_data * pinitmaindata  = new ::win::main_init_data;

      pinitmaindata->m_hInstance             = hInstance;
      pinitmaindata->m_hPrevInstance         = hPrevInstance;
      pinitmaindata->m_vssCommandLine        = lpCmdLine;
      pinitmaindata->m_nCmdShow              = nCmdShow;


      psystem->init_main_data(pinitmaindata);


      nReturnCode = psystem->main();


      __win_term();


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
         delete __get_module_state()->m_pmapHWND;
      }
      catch(...)
      {
      }
/*      try
      {
         delete __get_module_state()->m_pmapHDC;
      }
      catch(...)
      {
      }*/
/*      try
      {
         delete __get_module_state()->m_pmapHGDIOBJ;
      }
      catch(...)
      {
      }*/
//      delete __get_module_state()->m_pmapHMENU;
      try
      {
         __get_module_state()->m_pmapHWND     = NULL;
      }
      catch(...)
      {
      }
/*      try
      {
         __get_module_state()->m_pmapHDC      = NULL;
      }
      catch(...)
      {
      }
      try
      {
         __get_module_state()->m_pmapHGDIOBJ  = NULL;
      }
      catch(...)
      {
      }*/

      set_heap_mutex(NULL);


      return nReturnCode;
   }
   catch(...)
   {
   }
   return -1;
}



void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers)
{
   UNREFERENCED_PARAMETER(uiCode);
   UNREFERENCED_PARAMETER(ppointers);
}



HINSTANCE g_hinstance = NULL;