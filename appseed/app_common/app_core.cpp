#include "framework.h"
#include "aura/node/windows/windows.h"


CLASS_DECL_AURA int32_t __win_main(sp(::aura::system) psystem,::windows::main_init_data * pmaininitdata);


extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow, app_core & appcore)
{

   //MessageBox(NULL, lpCmdLine, "msg", 0);

   UNREFERENCED_PARAMETER(lpCmdLine);

   ::aura::system * psystem                     = g_pfn_create_system();

   // what could influence time before main?
   // cold start (never previously called program and its Dlls...)?
   psystem->m_dwMainStartTime                   = appcore.m_dwStartTime;

   ASSERT(hPrevInstance == NULL);

   ::windows::main_init_data * pmaininitdata    = new ::windows::main_init_data;


   pmaininitdata->m_hInstance                   = hinstance;
   pmaininitdata->m_hPrevInstance               = hPrevInstance;
   pmaininitdata->m_vssCommandLine              = lpCmdLine;
   pmaininitdata->m_nCmdShow                    = nCmdShow;


   int32_t nReturnCode                          = __win_main(psystem, pmaininitdata);

   appcore.m_dwAfterApplicationFirstRequest     = psystem->m_dwAfterApplicationFirstRequest;

   try
   {

      delete psystem;

   }
   catch (...)
   {

   }

   psystem = NULL;

 
   return nReturnCode;

}




