#include "ccvotagus_ca2_spa.h"

//bool g_bAdmin = true;
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

   return ::aura::simple_app_main < a_spa >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

}