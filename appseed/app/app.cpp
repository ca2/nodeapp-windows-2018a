#pragma once


#include "nodeapp/appseed/app_core/app_core.h"


BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return app_core_main(hinstance, hPrevInstance, lpCmdLine, nCmdShow);
   
}

END_EXTERN_C