#include "../a_spalib/a_spa.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

   if(!defer_aura_init())
   {

      return -1;

   }
   
   spa_set_admin(false);
   
   int iRet = ::aura::simple_app_main < ::a_spa::simple_app >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

   defer_aura_term();

   return iRet;

}