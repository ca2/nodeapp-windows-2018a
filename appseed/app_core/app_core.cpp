#include "framework.h"

#ifdef CUBE
#include "freeimage/freeimage.h"
#endif



extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

#ifdef CUBE
   FreeImage_Initialise(FALSE);
#endif


   UNREFERENCED_PARAMETER(lpCmdLine);

   if (!__win_init())
      return -1;

   sp(::plane::system) psystem = canew(::plane::system);

   ASSERT(hPrevInstance == NULL);

   ::win::main_init_data * pmaininitdata = new ::win::main_init_data;


   pmaininitdata->m_hInstance = hinstance;
   pmaininitdata->m_hPrevInstance = hPrevInstance;
   pmaininitdata->m_vssCommandLine = lpCmdLine;
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
#ifdef CUBE
   FreeImage_DeInitialise();
#endif

   return nReturnCode;

}




