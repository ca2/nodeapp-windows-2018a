#pragma once


#include "nodeapp/appseed/app_core/app_core.h"





BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   if(!defer_core_init())
      return -1;

   int iRet = app_core_main(hinstance, hPrevInstance, NULL, nCmdShow);

   defer_core_term();

   return iRet;
   
}

END_EXTERN_C