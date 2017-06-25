#include "framework.h"
#include "aura/node/windows/windows.h"



extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

   if (!defer_aura_init())
   {

      return -1;

   }

   ::app_app_install::app * psystem = new ::app_app_install::app;

   int iReturnCode = ::app_main(psystem, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

   defer_aura_term();

   return iReturnCode;

}
