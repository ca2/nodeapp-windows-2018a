#include "framework.h"
#include <stddef.h>


#pragma warning(disable: 4074)
#pragma init_seg(compiler)

/////////////////////////////////////////////////////////////////////////////
// __MODULE_STATE push/pop implementation

CLASS_DECL_win __MODULE_STATE * __set_module_state(__MODULE_STATE* pNewState)
{
   ___THREAD_STATE* pState = gen_ThreadState;
   ASSERT(pState);
   if(pState)
   {
      __MODULE_STATE* pPrevState = pState->m_pModuleState;
      pState->m_pModuleState = pNewState;
      return pPrevState;
   }
   else
   {
      return ::null();
   }
}

__MAINTAIN_STATE::~__MAINTAIN_STATE()
{
   ___THREAD_STATE* pState = gen_ThreadState;
   ASSERT(pState);
   if(pState)
   {
   pState->m_pModuleState = m_pPrevModuleState;
}
}

__MAINTAIN_STATE2::__MAINTAIN_STATE2(__MODULE_STATE* pNewState)
{
   m_pThreadState = gen_ThreadState;
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
      m_pPrevModuleState=::null();
      m_pThreadState=::null();
   }

/*   if (__gen_get_ambient_act_ctx() && 
      pNewState->m_hActCtx != INVALID_HANDLE_VALUE)
   {
      m_bValidActCtxCookie = __activate_act_ctx(pNewState->m_hActCtx, &m_ulActCtxCookie);
   }
   else
   {
      m_bValidActCtxCookie = FALSE;
   }*/
}

/////////////////////////////////////////////////////////////////////////////
// ___THREAD_STATE implementation

void ___THREAD_STATE::initialize()
{
#ifdef DEBUG
   m_nDisablePumpCount = 0;
#endif
   m_msgCur.message = WM_NULL;
   m_nMsgLast = WM_NULL;
   //System.get_cursor_pos(&(m_ptCursorLast));
   m_ptCursorLast = point(0,0);
}

void ___THREAD_STATE::finalize()
{
   // unhook windows hooks
   if (m_hHookOldMsgFilter != ::null())
      ::UnhookWindowsHookEx(m_hHookOldMsgFilter);
   if (m_hHookOldCbtFilter != ::null())
      ::UnhookWindowsHookEx(m_hHookOldCbtFilter);

   // free safety pool buffer
   if (m_pSafetyPoolBuffer != ::null())
      free(m_pSafetyPoolBuffer);

   // parking ::ca::window must have already been cleaned up by now!
   ASSERT(m_pWndPark == ::null());


}

__declspec(thread) ___THREAD_STATE * gen_ThreadState = NULL;
__declspec(thread) BYTE gen_ThreadStateAlloc[sizeof(___THREAD_STATE)];

CLASS_DECL_win ___THREAD_STATE * __get_thread_state()
{
   ___THREAD_STATE *pState = gen_ThreadState;
   if(pState == NULL)
   {
      pState = (___THREAD_STATE *) gen_ThreadStateAlloc;
      pState->initialize();
   }
   return pState;
}



/////////////////////////////////////////////////////////////////////////////
// __MODULE_STATE implementation

__MODULE_STATE::__MODULE_STATE(bool bDLL, WNDPROC pfn_window_procedure, DWORD dwVersion, bool bSystem) :
   m_mutexRegClassList(::null())
{
   m_pmapHWND              = ::null();
//   m_pmapHDC               = ::null();
//   m_pmapHGDIOBJ           = ::null();
   m_pmapHMENU             = ::null();
   m_pstrUnregisterList    = ::null();
   /* xxx xxx xxx
   m_classList.Construct(offsetof(sp(::ca::type_info), m_pNextClass)); */

   m_fRegisteredClasses = 0;
   m_bDLL = (BYTE)bDLL;
   m_pfn_window_procedure = pfn_window_procedure;
   m_dwVersion = dwVersion;
   m_bSystem = (BYTE)bSystem;
//   bool bEnable = TRUE;
   try
   {
      //Preallocate the registered classes string, but CRT primitive::memory leak report is
      //called before the string frees primitive::memory, so need to disable tracking.
      //bEnable = __enable_memory_tracking(FALSE);
      //m_pstrUnregisterList->Preallocate(4096);
      //__enable_memory_tracking(bEnable);   
   }      
   catch(memory_exception * pe)
   {
      //__enable_memory_tracking(bEnable);   
      pe->Delete();
   }
   
   // cast starts out in "::fontopus::user control"
   m_bUserCtrl = TRUE;

   

   //bEnable = __enable_memory_tracking(FALSE);      
   //Fusion: allocate dll wrappers array.
   m_pDllIsolationWrappers = ::null();
   //__enable_memory_tracking(bEnable);
   m_bSetAmbientActCtx = TRUE;
   m_hActCtx = ::null();
}

/////////////////////////////////////////////////////////////////////////////
// Activation Context API wrappers

#define __ACTCTX_API_INIT_PROCPTR(hKernel,name) \
   pfn##name = (PFN_##name) GetProcAddress(hKernel, #name)\
   

/////////////////////////////////////////////////////////////////////////////
// Global function pointers for Context (WinSxS/Manifest) API, to be init during ca API global init.
#define __ACTCTX_API_PTR_DEFINE(name, type, params) \
   typedef type (WINAPI* PFN_##name)params; \
   PFN_##name pfn##name = ::null();

__ACTCTX_API_PTR_DEFINE(CreateActCtxW, HANDLE, (PCACTCTXW));
__ACTCTX_API_PTR_DEFINE(ReleaseActCtx, void, (HANDLE));
__ACTCTX_API_PTR_DEFINE(ActivateActCtx, bool, (HANDLE, uint_ptr*));
__ACTCTX_API_PTR_DEFINE(DeactivateActCtx, bool, (DWORD, uint_ptr));

__STATIC void CLASS_DECL_win __init_context_api()
{
   static HMODULE hKernel = ::null();
   if (hKernel == ::null())
   {
      hKernel = GetModuleHandle("KERNEL32");
      ENSURE(hKernel != ::null());
      __ACTCTX_API_INIT_PROCPTR(hKernel,CreateActCtxW);
      __ACTCTX_API_INIT_PROCPTR(hKernel,ReleaseActCtx);
      __ACTCTX_API_INIT_PROCPTR(hKernel,ActivateActCtx);
      __ACTCTX_API_INIT_PROCPTR(hKernel,DeactivateActCtx);   
   }
}

#if (_WIN32_WINNT >= 0x0500) || (_WIN32_FUSION >= 0x0100)
HANDLE CLASS_DECL_win __create_act_ctx_w(PCACTCTXW pActCtx)
{   
   HANDLE hCtx = pfnCreateActCtxW != 0 ? pfnCreateActCtxW(pActCtx) : INVALID_HANDLE_VALUE;
   return hCtx;
}
#else
HANDLE CLASS_DECL_win __create_act_ctx_w(void *pActCtx)
{   
   HANDLE hCtx = pfnCreateActCtxW != 0 ? pfnCreateActCtxW(pActCtx) : INVALID_HANDLE_VALUE;
   return hCtx;
}
#endif

void CLASS_DECL_win __release_act_ctx(HANDLE hActCtx)
{   
   if (pfnReleaseActCtx != 0)
   {
      pfnReleaseActCtx(hActCtx);
   }
}

CLASS_DECL_win bool __activate_act_ctx(HANDLE hActCtx, uint_ptr *lpCookie) 
{   
   bool rc = pfnActivateActCtx != 0 ? pfnActivateActCtx(hActCtx, lpCookie) : FALSE;   
   return rc;
}

CLASS_DECL_win bool __deactivate_act_ctx(DWORD dwFlags, uint_ptr ulCookie)
{   
   bool rc = pfnDeactivateActCtx != 0 ? pfnDeactivateActCtx(dwFlags, ulCookie) : FALSE;
   return rc;
}


void __MODULE_STATE::CreateActivationContext()
{
   __init_context_api();
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
   m_hActCtx = __create_act_ctx_w(&actCtx);
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {      
      actCtx.lpResourceName =  MAKEINTRESOURCEW(ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID);
      m_hActCtx = __create_act_ctx_w(&actCtx);
   }
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {      
      actCtx.lpResourceName =  MAKEINTRESOURCEW(CREATEPROCESS_MANIFEST_RESOURCE_ID);
      m_hActCtx = __create_act_ctx_w(&actCtx);
   }
   if (m_hActCtx == INVALID_HANDLE_VALUE)
   {
      m_hActCtx = ::null();
   }      
}

__MODULE_STATE::~__MODULE_STATE()
{

   if (m_hActCtx != ::null() && m_hActCtx != INVALID_HANDLE_VALUE)
   {
      __release_act_ctx(m_hActCtx);
      m_hActCtx = INVALID_HANDLE_VALUE;
   }
}


__MODULE_THREAD_STATE::__MODULE_THREAD_STATE()
{
   m_nLastHit = static_cast<int_ptr>(-1);
   m_nLastStatus = static_cast<int_ptr>(-1);
   m_pLastInfo = ::null();

   // Note: it is only necessary to initialize non-zero data
   //m_pfnNewHandler = &__new_handler;
}

__MODULE_THREAD_STATE::~__MODULE_THREAD_STATE()
{

   delete m_pLastInfo;


}

/////////////////////////////////////////////////////////////////////////////
// __MODULE_STATE for base application

LRESULT CALLBACK __window_procedure_base(oswindow, UINT, WPARAM, LPARAM);

class ___BASE_MODULE_STATE : public __MODULE_STATE
{
public:
   ___BASE_MODULE_STATE() : __MODULE_STATE(TRUE, __window_procedure_base, _MFC_VER)
      { }
};

PROCESS_LOCAL(___BASE_MODULE_STATE, gen_BaseModuleState)

#undef __window_procedure
LRESULT CALLBACK
__window_procedure_base(oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   return __window_procedure(oswindow, nMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// helper functions for module state

CLASS_DECL_win __MODULE_STATE * __get_app_module_state()
{
   return gen_BaseModuleState.get_data();
}

CLASS_DECL_win __MODULE_STATE * __get_module_state()
{
   ___THREAD_STATE* pState = __get_thread_state();
   ENSURE(pState);
   __MODULE_STATE* pResult;
   if (pState->m_pModuleState != ::null())
   {
      // thread state's module state serves as override
      pResult = pState->m_pModuleState;
   }
   else
   {
      // otherwise, use global cast state
      pResult = gen_BaseModuleState.get_data();
   }
   ENSURE(pResult != ::null());
   return pResult;
}

HINSTANCE CLASS_DECL_win __get_instance_handle_helper()
{
   return __get_module_state()->m_hCurrentInstanceHandle;
}

bool CLASS_DECL_win __is_module_dll()
{
   return __get_module_state()->m_bDLL;
}

bool CLASS_DECL_win __init_current_state_app()
{
   sp(::ca::application) pApp = __get_module_state()->m_pCurrentWinApp;
   if (pApp != ::null() && !pApp->initialize_instance())
   {
      // Init Failed
      try
      {
         pApp->exit();
      }
      catch(...)
      {
      }
      __win_term();
      return FALSE;
   }
   return TRUE;
}

CLASS_DECL_win __MODULE_THREAD_STATE * __get_module_thread_state()
{
   __MODULE_THREAD_STATE* pResult=__get_module_state()->m_thread.get_data();
   ENSURE(pResult != ::null());
   return pResult;
}

