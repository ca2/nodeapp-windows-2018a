#include "../app_app_lib/app_app.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

   if(!defer_aura_init())
   {

      return -1;

   }
   
   int iRet = ::aura::app_main < ::app_app::app >(hInstance,hPrevInstance,lpCmdLine,nCmdShow, 
      [](auto papp)
   {

      papp->set_admin(false);

   });

   defer_aura_term();

   return iRet;

}