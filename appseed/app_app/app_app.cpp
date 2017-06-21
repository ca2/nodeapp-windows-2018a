#include "../app_app_lib/app_app.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

   if(!defer_aura_init())
   {

      return -1;

   }
   
   spa_set_admin(false);
   
   int iRet = ::aura::app_main < ::app_app::app >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

   defer_aura_term();

   return iRet;

}