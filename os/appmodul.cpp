#include "framework.h"
#include "sal.h"

/////////////////////////////////////////////////////////////////////////////
// export WinMain to force linkage to this module
extern int CLASS_DECL_win __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int nCmdShow);

extern "C" int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int nCmdShow)
{
   // call shared/exported WinMain
   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/////////////////////////////////////////////////////////////////////////////
// initialize cast state such that it points to this module's core state

CLASS_DECL_win bool __initialize(bool bDLL, DWORD dwVersion)
{
   __MODULE_STATE* pModuleState = __get_module_state();
   pModuleState->m_bDLL = (BYTE)bDLL;
   ASSERT(dwVersion <= _MFC_VER);
   UNUSED(dwVersion);  // not used in release build
   pModuleState->m_dwVersion = dwVersion;
#ifdef _MBCS
   // set correct multi-byte code-page for Win32 apps
   if (!bDLL)
      _setmbcp(_MB_CP_ANSI);
#endif //_MBCS
   return TRUE;
}

// force initialization early
//#pragma warning(disable: 4074)
//#pragma init_seg(lib)


char gen_InitAppState = (char)(__initialize(FALSE, _MFC_VER));
