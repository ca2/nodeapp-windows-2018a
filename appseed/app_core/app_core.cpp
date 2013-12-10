#include "framework.h"




extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   UNREFERENCED_PARAMETER(lpCmdLine);

   if (!__win_init())
      return -1;

   sp(::plane::system) psystem = canew(::plane::system);

   ASSERT(hPrevInstance == NULL);

   ::win::main_init_data * pmaininitdata = new ::win::main_init_data;


   pmaininitdata->m_hInstance = hinstance;
   pmaininitdata->m_hPrevInstance = hPrevInstance;
   pmaininitdata->m_vssCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());
   pmaininitdata->m_nCmdShow = nCmdShow;


   int32_t nReturnCode = __win_main(psystem, pmaininitdata);


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

   return nReturnCode;

}




