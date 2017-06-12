#include "framework.h"


extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

   return ::base::simple_app_main < install_app >(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}
