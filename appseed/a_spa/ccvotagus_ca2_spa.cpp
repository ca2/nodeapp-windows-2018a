#include "../a_spalib/a_spalib.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   spa_set_admin(false);
   return spalib_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}