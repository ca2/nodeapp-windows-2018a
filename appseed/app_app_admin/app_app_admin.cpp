#include "ccvotagus_ca2_spa.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
   
   if(!defer_aura_init())
   {

      return -1;

   }

   spa_set_admin(true);

 
   return ::aura::app_main < ::app_app::app >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

}



