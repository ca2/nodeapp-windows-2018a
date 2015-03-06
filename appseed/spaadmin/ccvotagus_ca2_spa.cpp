#include "StdAfx.h"

bool g_bAdmin = true;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   return spalib_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}