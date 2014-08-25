#include "framework.h"


CLASS_DECL_AURA int32_t __win_main(sp(::aura::system) psystem,::windows::main_init_data * pmaininitdata);



extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{


   UNREFERENCED_PARAMETER(lpCmdLine);

   sp(::core::system) psystem                  = canew(::core::system);

   ASSERT(hPrevInstance == NULL);

   ::windows::main_init_data * pmaininitdata    = new ::windows::main_init_data;


   pmaininitdata->m_hInstance                   = hinstance;
   pmaininitdata->m_hPrevInstance               = hPrevInstance;
   pmaininitdata->m_vssCommandLine              = lpCmdLine;
   pmaininitdata->m_nCmdShow                    = nCmdShow;


   int32_t nReturnCode                          = __win_main(psystem, pmaininitdata);


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




