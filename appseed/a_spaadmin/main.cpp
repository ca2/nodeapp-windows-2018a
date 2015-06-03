#include "ccvotagus_ca2_spa.h"

//bool g_bAdmin = true;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   spa_set_admin(true);
   return spalib_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}