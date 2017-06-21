#include "framework.h"


extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return ::base::app_main < ::app_app_install::app >(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}
