#include "nodeapp/os/os.h"




/////////////////////////////////////////////////////////////////////////////
// export WinMain to force linkage to this module
extern int32_t CLASS_DECL_win __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow);

extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow)
{
   // call shared/exported WinMain
   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
