#include "StdAfx.h"
#include "sal.h"

CLASS_DECL_ca int ca2_main();

//CLASS_DECL_ca extern fixed_alloc_array * g_pfixedallocaWstring;

//CLASS_DECL_ca fixed_alloc_array * new_wstring_manager();

void CLASS_DECL_VMSWIN __cdecl _ca2_purecall(void)
{
    throw simple_exception();
}


void __cdecl _null_se_translator(unsigned int uiCode, EXCEPTION_POINTERS * ppointers);


//::ca::application *     win_application_create(::ca::application * pappSystem, const char * pszId);
//::ca::application *     win_instantiate_application(::ca::application * pappSystem, const char * pszId);
//int                     win_application_procedure(::ca::application * pappSystem)
//UINT AFX_CDECL          win_application_thread_proc(LPVOID);
/////////////////////////////////////////////////////////////////////////////
// Standard WinMain implementation
//  Can be replaced as long as 'AfxWinInit' is called first

int CLASS_DECL_VMSWIN AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{


   UNREFERENCED_PARAMETER(lpCmdLine);


   _set_purecall_handler(_ca2_purecall);

   ::plane::system * psystem = new ::plane::system();

   ASSERT(hPrevInstance == NULL);

   int nReturnCode = 0;


   ::win::main_init_data * pinitmaindata  = new ::win::main_init_data;


   pinitmaindata->m_hInstance             = hInstance;
   pinitmaindata->m_hPrevInstance         = hPrevInstance;
   pinitmaindata->m_vssCommandLine        = gen::international::unicode_to_utf8(::GetCommandLineW());
   pinitmaindata->m_nCmdShow              = nCmdShow;

   
   psystem->init_main_data(pinitmaindata);


   nReturnCode = psystem->main();


   try
   {
      AfxWinTerm();
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
      delete AfxGetModuleState()->m_pmapHWND;
   }
   catch(...)
   {
   }
   try
   {
      delete AfxGetModuleState()->m_pmapHDC;
   }
   catch(...)
   {
   }
   try
   {
      delete AfxGetModuleState()->m_pmapHGDIOBJ;
   }
   catch(...)
   {
   }
//      delete AfxGetModuleState()->m_pmapHMENU;

   try
   {
      AfxGetModuleState()->m_pmapHWND     = NULL;
   }
   catch(...)
   {
   }
   try
   {
      AfxGetModuleState()->m_pmapHDC      = NULL;
   }
   catch(...)
   {
   }
   try
   {
      AfxGetModuleState()->m_pmapHGDIOBJ  = NULL;
   }
   catch(...)
   {
   }

   set_heap_mutex(NULL);

   return nReturnCode;


}


int CLASS_DECL_VMSWIN ca2_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char * lpCmdLine, int nCmdShow)
{
   try
   {

      _set_purecall_handler(_ca2_purecall);

      ::plane::system * psystem = new ::plane::system();


      ASSERT(hPrevInstance == NULL);

      int nReturnCode = 0;

      ::win::main_init_data * pinitmaindata  = new ::win::main_init_data;

      pinitmaindata->m_hInstance             = hInstance;
      pinitmaindata->m_hPrevInstance         = hPrevInstance;
      pinitmaindata->m_vssCommandLine        = lpCmdLine;
      pinitmaindata->m_nCmdShow              = nCmdShow;


      psystem->init_main_data(pinitmaindata);


      nReturnCode = psystem->main();


      AfxWinTerm();


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
         delete AfxGetModuleState()->m_pmapHWND;
      }
      catch(...)
      {
      }
      try
      {
         delete AfxGetModuleState()->m_pmapHDC;
      }
      catch(...)
      {
      }
      try
      {
         delete AfxGetModuleState()->m_pmapHGDIOBJ;
      }
      catch(...)
      {
      }
//      delete AfxGetModuleState()->m_pmapHMENU;
      try
      {
         AfxGetModuleState()->m_pmapHWND     = NULL;
      }
      catch(...)
      {
      }
      try
      {
         AfxGetModuleState()->m_pmapHDC      = NULL;
      }
      catch(...)
      {
      }
      try
      {
         AfxGetModuleState()->m_pmapHGDIOBJ  = NULL;
      }
      catch(...)
      {
      }

      set_heap_mutex(NULL);


      return nReturnCode;
   }
   catch(...)
   {
   }
   return -1;
}



void __cdecl _null_se_translator(unsigned int uiCode, EXCEPTION_POINTERS * ppointers)
{
   UNREFERENCED_PARAMETER(uiCode);
   UNREFERENCED_PARAMETER(ppointers);
}
