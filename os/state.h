#pragma once

#define slot_AFX_MODULE_THREAD_STATE 1
#define slot_AFX_THREAD_STATE 2

namespace win
{
   class thread;
} // namespace win

//#include "types.h"
//#include "template.h"

#ifndef __AFXTLS_H__
   #include "thread_slots.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// _AFX_DEBUG_STATE

#ifdef _DEBUG

class _AFX_DEBUG_STATE : public no_track_object
{
public:
   _AFX_DEBUG_STATE();
   virtual ~_AFX_DEBUG_STATE();
};

EXTERN_PROCESS_LOCAL(_AFX_DEBUG_STATE, afxDebugState)

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// _AFX_WIN_STATE


class _AFX_WIN_STATE : public no_track_object
{
public:
   // printing abort
   BOOL m_bUserAbort;
};

EXTERN_PROCESS_LOCAL(_AFX_WIN_STATE, _afxWinState)


/////////////////////////////////////////////////////////////////////////////
// AFX_MODULE_STATE : portion of state that is pushed/popped
// forward references required for AFX_MODULE_THREAD_STATE definition
//class CHandleMap;
class hwnd_map;
class hmenu_map;
//class hdc_map;
//class hgdiobj_map;
class himagelist_map;
class frame_window;
class CToolTipCtrl;
class ::userbase::control_bar;
class ::user::interaction;
namespace ca
{
   
   class thread;
   class window;
}

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif

template<class TYPE> 
class CEmbeddedButActsLikePtr
{
public:
   inline TYPE* operator->() { return &m_data; }
   inline operator TYPE*() { return &m_data; }
   TYPE m_data;
};

// AFX_MODULE_THREAD_STATE (local to thread *and* module)
class CLASS_DECL_VMSWIN AFX_MODULE_THREAD_STATE : 
   public no_track_object
{
public:
   AFX_MODULE_THREAD_STATE();
   virtual ~AFX_MODULE_THREAD_STATE();

   // current thread pointer
   class ::win::thread* m_pCurrentWinThread;



   // thread-local ca2 API new handler (separate from C-runtime)
   _PNH m_pfnNewHandler;


   // common controls thread state
   CToolTipCtrl* m_pToolTip;
   ::ca::window * m_pLastHit;       // last ::ca::window to own tooltip
   INT_PTR m_nLastHit;         // last hittest code
   TOOLINFO* m_pLastInfo;    // last TOOLINFO structure
   INT_PTR m_nLastStatus;      // last flyby status message
   ::userbase::control_bar* m_pLastStatus; // last flyby status control bar
};

// forward references required for AFX_MODULE_STATE definition
class application;


class ::ca::window;



class CDllIsolationWrapperBase;
#ifndef _AFX_NO_AFXCMN_SUPPORT
class CComCtlWrapper;
#endif
class CCommDlgWrapper;

// AFX_MODULE_STATE (global data for a module)
class CLASS_DECL_VMSWIN AFX_MODULE_STATE : public no_track_object
{
public:
   AFX_MODULE_STATE(BOOL bDLL, WNDPROC pfnAfxWndProc, DWORD dwVersion,
      BOOL bSystem = FALSE);
   ~AFX_MODULE_STATE();

   ::radix::application* m_pCurrentWinApp;
   HINSTANCE m_hCurrentInstanceHandle;
   HINSTANCE m_hCurrentResourceHandle;
   const char * m_lpszCurrentAppName;
   BYTE m_bDLL;    // TRUE if module is a DLL, FALSE if it is an EXE
   BYTE m_bSystem; // TRUE if module is a "system" module, FALSE if not
   BYTE m_bReserved[2]; // padding

   DWORD m_fRegisteredClasses; // flags for registered ::ca::window classes

   mutex       m_mutexRegClassList;

   hwnd_map       * m_pmapHWND;
//   hdc_map        * m_pmapHDC;
//   hgdiobj_map    * m_pmapHGDIOBJ;
   hmenu_map      * m_pmapHMENU;




   // number of locked OLE objects
   long m_nObjectCount;
   BOOL m_bUserCtrl;

   // AfxRegisterClass and System.RegisterWndClass data

   string * m_pstrUnregisterList;

   WNDPROC m_pfnAfxWndProc;
   DWORD m_dwVersion;  // version that module linked against

   // variables related to a given process in a module
   //  (used to be AFX_MODULE_PROCESS_STATE)
   void (PASCAL *m_pfnFilterToolTipMessage)(MSG*, ::ca::window *);


   // define thread local portions of module state
   thread_local<AFX_MODULE_THREAD_STATE, slot_AFX_MODULE_THREAD_STATE> m_thread;

   //Fusion: declare pointer to base_array of pointers to isolation aware dll wrappers (ex: comctl32).
   CDllIsolationWrapperBase** m_pDllIsolationWrappers;
   //Defaults to TRUE. When FALSE - ca2 API will not activate context in AFX_MAINTAIN_STATE2 (used by AFX_MANAGE_STATE).
   BOOL   m_bSetAmbientActCtx;
   //Handle of the module context.
   HANDLE   m_hActCtx;
   void CreateActivationContext();
};

CLASS_DECL_VMSWIN AFX_MODULE_STATE* AfxSetModuleState(AFX_MODULE_STATE* pNewState);
CLASS_DECL_VMSWIN AFX_MODULE_STATE* AfxGetModuleState();
CLASS_DECL_VMSWIN BOOL AfxIsModuleDll();
CLASS_DECL_VMSWIN BOOL AfxInitCurrentStateApp();
CLASS_DECL_VMSWIN AFX_MODULE_STATE* AfxGetStaticModuleState();
CLASS_DECL_VMSWIN HINSTANCE AfxGetInstanceHandleHelper();

CLASS_DECL_VMSWIN AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();

#define _AFX_CMDTARGET_GETSTATE() (m_pModuleState)

/////////////////////////////////////////////////////////////////////////////
// macros & classes to manage pushing/popping the module state

struct CLASS_DECL_VMSWIN AFX_MAINTAIN_STATE
{
   explicit AFX_MAINTAIN_STATE(AFX_MODULE_STATE* pModuleState) throw();
   ~AFX_MAINTAIN_STATE();

protected:
   AFX_MODULE_STATE* m_pPrevModuleState;
};

class _AFX_THREAD_STATE;
struct CLASS_DECL_VMSWIN AFX_MAINTAIN_STATE2
{
   explicit AFX_MAINTAIN_STATE2(AFX_MODULE_STATE* pModuleState);
   ~AFX_MAINTAIN_STATE2();

protected:
   AFX_MODULE_STATE* m_pPrevModuleState;
   _AFX_THREAD_STATE* m_pThreadState;

   ULONG_PTR m_ulActCtxCookie;
   BOOL m_bValidActCtxCookie;
};
#define AFX_MANAGE_STATE(p) _AfxInitManaged(); AFX_MAINTAIN_STATE2 _ctlState(p);

/////////////////////////////////////////////////////////////////////////////
// Thread global state

// forward references required for _AFX_THREAD_STATE definition
class view;
class CToolTipCtrl;
class ::userbase::control_bar;
class push_routing_frame;
class CPushRoutingView;

#define _AFX_TEMP_CLASS_NAME_SIZE 96
class CLASS_DECL_VMSWIN _AFX_THREAD_STATE : 
   public no_track_object,
   public ::ca::thread_state
{
public:
   _AFX_THREAD_STATE();
   virtual ~_AFX_THREAD_STATE();

   // override for m_pModuleState in _AFX_APP_STATE
   AFX_MODULE_STATE* m_pModuleState;
   AFX_MODULE_STATE* m_pPrevModuleState;



   // primitive::memory safety pool for temp maps
   void * m_pSafetyPoolBuffer;    // current buffer

   // thread local exception context
   AFX_EXCEPTION_CONTEXT m_exceptionContext;

   // ::ca::window create, gray dialog hook, and other hook data
   ::user::interaction * m_pWndInit;
   ::ca::window * m_pAlternateWndInit;      // special case commdlg hooking
   DWORD m_dwPropStyle;
   DWORD m_dwPropExStyle;
   HWND m_hWndInit;
   HHOOK m_hHookOldCbtFilter;
   HHOOK m_hHookOldMsgFilter;

   // message pump for run
   MSG m_msgCur;                   // current message
   point m_ptCursorLast;          // last mouse position
   UINT m_nMsgLast;                // last mouse message

#ifdef _DEBUG
      int m_nDisablePumpCount; // Diagnostic trap to detect illegal re-entrancy
#endif

   // other ::ca::window modal data
   MSG m_lastSentMsg;              // see ::ca::window::message_handler
   HWND m_hTrackingWindow;         // see ::ca::window::TrackPopupMenu
   HMENU m_hTrackingMenu;
   char m_szTempClassName[_AFX_TEMP_CLASS_NAME_SIZE];    // see System.RegisterWndClass
   HWND m_hLockoutNotifyWindow;    // see ::ca::window::OnCommand
   BOOL m_bInMsgFilter;

   // other framework modal data
   CPushRoutingView* m_pPushRoutingView;

   // ca2 API/DB thread-local data
   BOOL m_bWaitForDataSource;

   // OLE control thread-local data
   ::ca::window * m_pWndPark;       // "parking space" ::ca::window
   long m_nCtrlRef;        // reference count on parking ::ca::window
   BOOL m_bNeedTerm;       // TRUE if OleUninitialize needs to be called
};

EXTERN_THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState, slot_AFX_THREAD_STATE)

CLASS_DECL_VMSWIN _AFX_THREAD_STATE* AfxGetThreadState();
