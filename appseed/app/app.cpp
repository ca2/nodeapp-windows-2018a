#pragma once


#include "nodeapp/appseed/app_core/app_core.h"


BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   string str = ::str::international::unicode_to_utf8(::GetCommandLineW());

   return app_core_main(hinstance, hPrevInstance, (char *) (const char *) str, nCmdShow);
   
}

END_EXTERN_C