#include "StdAfx.h"
#include <stddef.h>


#pragma warning(disable: 4074)
#pragma init_seg(compiler)

/////////////////////////////////////////////////////////////////////////////
// AFX_MODULE_STATE push/pop implementation

CLASS_DECL_VMSWIN AFX_MODULE_STATE * AfxSetModuleState(AFX_MODULE_STATE* pNewState)
{
   _AFX_THREAD_STATE* pState = _afxThreadState;
   ASSERT(pState);
   if(pState)
   {
      AFX_MODULE_STATE* pPrevState = pState->m_pModuleState;
      pState->m_pModuleState = pNewState;
      return pPrevState;
   }
   else
   {
      return NULL;
   }
}

AFX_MAINTAIN_STATE::~AFX_MAINTAIN_STATE()
{
   _AFX_THREAD_STATE* pState = _afxThreadState;
   ASSERT(pState);
   if(pState)
   {
   pState->m_pModuleState = m_pPrevModuleState;
}
}

AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2(AFX_MODULE_STATE* pNewState)
{
   m_pThreadState = _afxThreadState;
   ASSERT(m_pThreadState);
   if(m_pThreadState)
   {
   m_pPrevModuleState = m_pThreadState->m_pModuleState;
   m_pThreadState->m_pModuleState = pNewState;
   }
   else
   {
      // This is a very bad state; we have no good way to report the error at this moment
      // since exceptions from here are not expected
      m_pPrevModuleState=NULL;
      m_pThreadState=NULL;
   }

/*   if (AfxGetAmbientActCtx() && 
      pNewState->m_hActCtx != INVALID_HANDLE_VALUE)
   {
      m_bValidActCtxCookie = AfxActivateActCtx(pNewState->m_hActCtx, &m_ulActCtxCookie);
   }
   else
   {
      m_bValidActCtxCookie = FALSE;
   }*/
}

/////////////////////////////////////////////////////////////////////////////
// _AFX_THREAD_STATE implementation

_AFX_THREAD_STATE::_AFX_THREAD_STATE()
{
#ifdef _DEBUG
   m_nDisablePumpCount = 0;
#endif
   m_msgCur.message = WM_NULL;
   m_nMsgLast = WM_NULL;
   //System.get_cursor_pos(&(m_ptCursorLast));
   m_ptCursorLast = point(0,0);
}

_AFX_THREAD_STATE::~_AFX_THREAD_STATE()
{
   // unhook windows hooks
   if (m_hHookOldMsgFilter != NULL)
      ::UnhookWindowsHookEx(m_hHookOldMsgFilter);
   if (m_hHookOldCbtFilter != NULL)
      ::UnhookWindowsHookEx(m_hHookOldCbtFilter);

   // free safety pool buffer
   if (m_pSafetyPoolBuffer != NULL)
      free(m_pSafetyPoolBuffer);

   // parking ::ca::window must have already been cleaned up by now!
   ASSERT(m_pWndPark == NULL);


}

CLASS_DECL_VMSWIN _AFX_THREAD_STATE * AfxGetThreadState()
{
   _AFX_THREAD_STATE *pState =_afxThreadState.get_data();
   ENSURE(pState != NULL); 
   return pState;
}

THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState, slot_AFX_THREAD_STATE)

/////////////////////////////////////////////////////////////////////////////
// AFX_MODULE_STATE implementation

AFX_MODULE_STATE::AFX_MODULE_STATE(BOOL bDLL, WNDPROC pfnAfxWndProc,
   DWORD dwVersion, BOOL bSystem)
{
   m_pmapHWND              = NULL;
//   m_pmapHDC               = NULL;
//   m_pmapHGDIOBJ           = NULL;
   m_pmapHMENU             = NULL;
   m_pstrUnregisterList    = NULL;
   /* xxx xxx xxx
   m_classList.Construct(offsetof(::ca::type_info, m_pNextClass)); */

   m_fRegisteredClasses = 0;
   m_bDLL = (BYTE)bDLL;
   m_pfnAfxWndProc = pfnAfxWndProc;
   m_dwVersion = dwVersion;
   m_bSystem = (BYTE)bSystem;
//   BOOL bEnable = TRUE;
   try
   {
      //Preallocate the registered classes string, but CRT primitive::memory leak report is
      //called before the string frees primitive::memory, so need to disable tracking.
      //bEnable = AfxEnableMemoryTracking(FALSE);
      //m_pstrUnregisterList->Preallocate(4096);
      //AfxEnableMemoryTracking(bEnable);   
   }      
   catch(memory_exception * pe)
   {
      //AfxEnableMemoryTracking(bEnable);   
      pe->Delete();
   }
   
   // cast starts out in "::fontopus::user control"
   m_bUserCtrl = TRUE;

   

   //bEnable = AfxEnableMemoryTracking(FALSE);      
   //Fusion: allocate dll wrappers base_array.
   m_pDllIsolationWrappers = NULL;
   //AfxEnableMemoryTracking(bEnable);
   m_bSetAmbientActCtx = TRUE;
   m_hActCtx = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Activation Context API wrappers

#define AFX_ACTCTX_API_INIT_PROCPTR(hKernel,name) \
   pfn##name = (PFN_##name) GetProcAddress(hKernel, #name)\
   

/////////////////////////////////////////////////////////////////////////////
// Global function pointers for Context (WinSxS/Manifest) API, to be init during ca2 API global init.
#define AFX_ACTCTX_API_PTR_DEFINE(name, type, params) \
   typedef type (WINAPI* PFN_##name)params; \
   PFN_##name pfn##name = NULL;

AFX_ACTCTX_API_PTR_DEFINE(CreateActCtxW, HANDLE, (PCACTCTXW));
AFX_ACTCTX_API_PTR_DEFINE(ReleaseActCtx, void, (HANDLE));
AFX_ACTCTX_API_PTR_DEFINE(ActivateActCtx, BOOL, (HANDLE, ULONG_PTR*));
AFX_ACTCTX_API_PTR_DEFINE(DeactivateActCtx, BOOL, (DWORD, ULONG_PTR));

AFX_STATIC void CLASS_DECL_VMSWIN _AfxInitContextAPI()
{
   static HMODULE hKernel = NULL;
   if (hKernel == NULL)
   {
      hKernel = GetModuleHandle("KERNEL32");
      ENSURE(hKernel != NULL);
      AFX_ACTCTX_API_INIT_PROCPTR(hKernel,CreateActCtxW);
      AFX_ACTCTX_API_INIT_PROCPTR(hKernel,ReleaseActCtx);
      AFX_ACTCTX_API_INIT_PROCPTR(hKernel,ActivateActCtx);
      AFX_ACTCTX_API_INIT_PROCPTR(hKernel,DeactivateActCtx);   
   }
}

#if (_WIN32_WINNT >= 0x0500) || (_WIN32_FUSION >= 0x0100)
HANDLE CLASS_DECL_VMSWIN AfxCreateActCtxW(PCACTCTXW pActCtx)
{   
   HANDLE hCtx = pfnCreateActCtxW != 0 ? pfnCreateActCtxW(pActCtx) : INVALID_HANDLE_VALUE;
   return hCtx;
}
#else
HANDLE CLASS_DECL_VMSWIN AfxCreateActCtxW(void *pActCtx)
{   
   HANDLE hCtx = pfnCreateActCtxW != 0 ? pfnCreateActCtxW(pActCtx) : INVALID_HANDLE_VALUE;
   return hCtx;
}
#endif

void CLASS_DECL_VMSWIN AfxReleaseActCtx(HANDLE hActCtx)
{   
   if (pfnReleaseActCtx != 0)
   {
      pfnReleaseActCtx(hActCtx);
   }
}

CLASS_DECL_VMSWIN BOOL AfxActivateActCtx(HANDLE hActCtx, ULONG_PTR *lpCookie) 
{   
   BOOL rc = pfnActivateActCtx != 0 ? pfnActivateActCtx(hActCtx, lpCookie) : FALSE;   
   return rc;
}

CLASS_DECL_VMSWIN BOOL AfxDeactivateActCtx(DWORD dwFlags, ULONG_PTR ulCookie)
{   
   BOOL rc = pfnDeactivateActCtx != 0 ? pfnDeactivateActCtx(dwFlags, ulCookie) : FALSE;
   return rc;
}


void AFX_MODULE_STATE::CreateActivationContext()
{
   _AfxInitContextAPI();
   HMODULE hModule = m_hCurrentInstanceHandle;

   WCHAR rgchFullModulePath[MAX_PATH + 2];
   rgchFullModulePath[_countof(rgchFullModulePath) - 1] = 0;
   rgchFullModulePath[_countof(rgchFullModulePath) - 2] = 0;
   DWORD dw = GetModuleFileNameW(hModule, rgchFullModulePath, _countof(rgchFullModulePath)-1);
   if (dw == 0)
   {
      return;
   }
   if (rgchFullModulePath[_countof(rgchFullModulePath) - 2] != 0)
   {
      SetLastError(ERROR_BUFFER_OVERFLOW);
      return;
   }
   //First try ID 2 and then ID 1 - this is to consider also a.dll.manifest file
   //for dlls, which ID 2 ignores.
   ACTCTXW actCtx;
   actCtx.cbSize = sizeof(actCtx);
   actCtx.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID;
   actCtx.lpSource = rgchFullModulePath;
   actCtx.lpResourceName =  MAKEINTRESOURCEW(ISOLATIONAWARE_MANIFEST_RESOURCE_ID);
   actCtx.hModule = hModule;
   m_hActCtx = AfxCreateActCtxW(&actCtx);
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {      
      actCtx.lpResourceName =  MAKEINTRESOURCEW(ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID);
      m_hActCtx = AfxCreateActCtxW(&actCtx);
   }
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {      
      actCtx.lpResourceName =  MAKEINTRESOURCEW(CREATEPROCESS_MANIFEST_RESOURCE_ID);
      m_hActCtx = AfxCreateActCtxW(&actCtx);
   }
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {
      m_hActCtx = NULL;
   }      
}

AFX_MODULE_STATE::~AFX_MODULE_STATE()
{

   if (m_hActCtx != NULL && m_hActCtx != INVALID_HANDLE_VALUE)
   {
      AfxReleaseActCtx(m_hActCtx);
      m_hActCtx = INVALID_HANDLE_VALUE;
   }
}


AFX_MODULE_THREAD_STATE::AFX_MODULE_THREAD_STATE()
{
   m_nLastHit = static_cast<INT_PTR>(-1);
   m_nLastStatus = static_cast<INT_PTR>(-1);
   m_pLastInfo = NULL;

   // Note: it is only necessary to initialize non-zero data
   //m_pfnNewHandler = &AfxNewHandler;
}

AFX_MODULE_THREAD_STATE::~AFX_MODULE_THREAD_STATE()
{

   delete m_pLastInfo;


}

/////////////////////////////////////////////////////////////////////////////
// AFX_MODULE_STATE for base application

LRESULT CALLBACK AfxWndProcBase(HWND, UINT, WPARAM, LPARAM);

class _AFX_BASE_MODULE_STATE : public AFX_MODULE_STATE
{
public:
   _AFX_BASE_MODULE_STATE() : AFX_MODULE_STATE(TRUE, AfxWndProcBase, _MFC_VER)
      { }
};

PROCESS_LOCAL(_AFX_BASE_MODULE_STATE, _afxBaseModuleState)

#undef AfxWndProc
LRESULT CALLBACK
AfxWndProcBase(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   return AfxWndProc(hWnd, nMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// helper functions for module state

CLASS_DECL_VMSWIN AFX_MODULE_STATE * AfxGetAppModuleState()
{
   return _afxBaseModuleState.get_data();
}

CLASS_DECL_VMSWIN AFX_MODULE_STATE * AfxGetModuleState()
{
   _AFX_THREAD_STATE* pState = _afxThreadState;
   ENSURE(pState);
   AFX_MODULE_STATE* pResult;
   if (pState->m_pModuleState != NULL)
   {
      // thread state's module state serves as override
      pResult = pState->m_pModuleState;
   }
   else
   {
      // otherwise, use global cast state
      pResult = _afxBaseModuleState.get_data();
   }
   ENSURE(pResult != NULL);
   return pResult;
}

HINSTANCE CLASS_DECL_VMSWIN AfxGetInstanceHandleHelper()
{
   return AfxGetModuleState()->m_hCurrentInstanceHandle;
}

BOOL CLASS_DECL_VMSWIN AfxIsModuleDll()
{
   return AfxGetModuleState()->m_bDLL;
}

BOOL CLASS_DECL_VMSWIN AfxInitCurrentStateApp()
{
   ::radix::application* pApp = AfxGetModuleState()->m_pCurrentWinApp;
   if (pApp != NULL && !pApp->initialize_instance())
   {
      // Init Failed
      try
      {
         pApp->exit();
      }
      catch(...)
      {
      }
      AfxWinTerm();
      return FALSE;
   }
   return TRUE;
}

CLASS_DECL_VMSWIN AFX_MODULE_THREAD_STATE * AfxGetModuleThreadState()
{
   AFX_MODULE_THREAD_STATE* pResult=AfxGetModuleState()->m_thread.get_data();
   ENSURE(pResult != NULL);
   return pResult;
}

