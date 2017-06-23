#include "../app_app_lib/app_app.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

   if(!defer_aura_init())
   {

      return -1;

   }

   auto * psystem = new ::app_app::app;

   psystem->set_admin(false);
   
   int iRet = ::app_main(psystem, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
   
   defer_aura_term();

   return iRet;

}