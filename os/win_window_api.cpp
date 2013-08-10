#include "framework.h"


extern CLASS_DECL_win oswindow_map * g_pwindowmap;
extern const char * gen_OldWndProc;



/////////////////////////////////////////////////////////////////////////////
// Map from oswindow to sp(::ca2::window)

oswindow_map* get_oswindow_map(bool bCreate)
{

   return g_pwindowmap;

}


/////////////////////////////////////////////////////////////////////////////
// The WndProc for all window's and derived classes

#undef __window_procedure

LRESULT CALLBACK __window_procedure(oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   // special message which identifies the window as using __window_procedure
//   if (nMsg == WM_QUERYAFXWNDPROC)
  //    return 1;

   // all other messages route through message map
   sp(::ca2::window) pWnd = ::win::window::FromHandlePermanent(oswindow);
   //ASSERT(pWnd != NULL);               
   //ASSERT(pWnd==NULL || WIN_WINDOW(pWnd)->get_handle() == oswindow);
   if (pWnd == NULL || WIN_WINDOW(pWnd)->get_handle() != oswindow)
      return ::DefWindowProc(oswindow, nMsg, wParam, lParam);
   return win::__call_window_procedure(pWnd, oswindow, nMsg, wParam, lParam);
}

// always indirectly accessed via __get_window_procedure
WNDPROC CLASS_DECL_win __get_window_procedure()
{
   return __get_module_state()->m_pfn_window_procedure;
}
/////////////////////////////////////////////////////////////////////////////
// Special helpers for certain windows messages

__STATIC void CLASS_DECL_win __pre_init_dialog(
   sp(::user::interaction) pWnd, LPRECT lpRectOld, uint32_t* pdwStyleOld)
{
   ASSERT(lpRectOld != NULL);   
   ASSERT(pdwStyleOld != NULL);

   WIN_WINDOW(pWnd.m_p)->GetWindowRect(lpRectOld);
   *pdwStyleOld = WIN_WINDOW(pWnd.m_p)->GetStyle();
}

__STATIC void CLASS_DECL_win __post_init_dialog(
   sp(::user::interaction) pWnd, const RECT& rectOld, uint32_t dwStyleOld)
{
   // must be hidden to start with      
   if (dwStyleOld & WS_VISIBLE)
      return;

   // must not be visible after WM_INITDIALOG
   if (WIN_WINDOW(pWnd.m_p)->GetStyle() & (WS_VISIBLE|WS_CHILD))
      return;

   // must not move during WM_INITDIALOG
   rect rect;
   WIN_WINDOW(pWnd.m_p)->GetWindowRect(rect);
   if (rectOld.left != rect.left || rectOld.top != rect.top)
      return;

   // must be unowned or owner disabled
   sp(::user::interaction) pParent = WIN_WINDOW(pWnd.m_p)->GetWindow(GW_OWNER);
   if (pParent != NULL && pParent->IsWindowEnabled())
      return;

   if (!WIN_WINDOW(pWnd.m_p)->CheckAutoCenter())
      return;

   // center modal dialog boxes/message boxes
   //WIN_WINDOW(pWnd)->CenterWindow();
}



CLASS_DECL_win void hook_window_create(sp(::win::window) pwindow)
{

   ___THREAD_STATE* pThreadState = __get_thread_state();

   if (pThreadState->m_pWndInit == pwindow)
      return;

   if (pThreadState->m_hHookOldCbtFilter == NULL)
   {
      pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, win::__cbt_filter_hook, NULL, ::GetCurrentThreadId());
      if (pThreadState->m_hHookOldCbtFilter == NULL)
         throw memory_exception(pwindow->get_app());
   }

   ASSERT(pThreadState->m_hHookOldCbtFilter != NULL);
   
   ASSERT(pwindow != NULL);
   
   ASSERT(WIN_WINDOW(pwindow)->get_handle() == NULL);   // only do once

   ASSERT(pThreadState->m_pWndInit == NULL);   // hook not already in progress

   pThreadState->m_pWndInit = pwindow;

}

CLASS_DECL_win bool unhook_window_create()
{
   ___THREAD_STATE* pThreadState = __get_thread_state();
   if (pThreadState->m_pWndInit != NULL)
   {
      pThreadState->m_pWndInit = NULL;
      return FALSE;   // was not successfully hooked
   }
   return TRUE;
}



CLASS_DECL_win const char * __register_window_class(UINT nClassStyle,
                                                    HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
   // Returns a temporary string name for the class
   //  Save in a string if you want to use it for a long time
   LPTSTR lpszName = __get_thread_state()->m_szTempClassName;

   // generate a synthetic name for this class
   HINSTANCE hInst = g_hinstance;

   if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca2:::%p:%x", hInst, nClassStyle));
   }
   else
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca2:::%p:%x:%p:%p:%p", hInst, nClassStyle,
         hCursor, hbrBackground, hIcon));
   }

   // see if the class already exists
   WNDCLASS wndcls;
   if (::GetClassInfo(hInst, lpszName, &wndcls))
   {
      // already registered, assert everything is good
      ASSERT(wndcls.style == nClassStyle);

      // NOTE: We have to trust that the hIcon, hbrBackground, and the
      //  hCursor are semantically the same, because sometimes Windows does
      //  some internal translation or copying of those handles before
      //  storing them in the internal WNDCLASS retrieved by GetClassInfo.
      return lpszName;
   }

   // otherwise we need to register a new class
   wndcls.style = nClassStyle;
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
   wndcls.hInstance = hInst;
   wndcls.hIcon = hIcon;
   //wndcls.hCursor = hCursor;
   wndcls.hCursor = NULL;
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = NULL;
   wndcls.lpszClassName = lpszName;
   if (!__register_class(&wndcls))
      throw resource_exception(::ca2::get_thread_app());

   // return thread-local pointer
   return lpszName;
}


__STATIC void CLASS_DECL_win
   __handle_activate(sp(::ca2::window) pWnd, WPARAM nState, sp(::ca2::window) pWndOther)
{
   ASSERT(pWnd != NULL);      

   // send WM_ACTIVATETOPLEVEL when top-level parents change
   if (!(WIN_WINDOW(pWnd)->GetStyle() & WS_CHILD))
   {
      sp(::user::interaction) pTopLevel= WIN_WINDOW(pWnd)->GetTopLevelParent();
      if (pTopLevel && (pWndOther == NULL || !::IsWindow(WIN_WINDOW(pWndOther)->get_handle()) || pTopLevel != WIN_WINDOW(pWndOther)->GetTopLevelParent()))
      {
         // lParam points to window getting the WM_ACTIVATE message and
         //  oswindow_Other from the WM_ACTIVATE.
         oswindow oswindow_2[2];
         oswindow_2[0] = WIN_WINDOW(pWnd)->get_handle();
         if(pWndOther == NULL || WIN_WINDOW(pWndOther) == NULL)
         {
            oswindow_2[1] = NULL;
         }
         else
         {
            oswindow_2[1] = WIN_WINDOW(pWndOther)->get_handle();
         }
         // send it...
         pTopLevel->send_message(WM_ACTIVATETOPLEVEL, nState, (LPARAM)&oswindow_2[0]);
      }
   }
}

__STATIC bool CLASS_DECL_win
   __handle_set_cursor(sp(::ca2::window) pWnd, UINT nHitTest, UINT nMsg)
{
   if (nHitTest == HTERROR &&
      (nMsg == WM_LBUTTONDOWN || nMsg == WM_MBUTTONDOWN ||
      nMsg == WM_RBUTTONDOWN))
   {
      // activate the last active window if not active
      sp(::user::interaction) pLastActive = WIN_WINDOW(pWnd)->GetTopLevelParent();      
      if (pLastActive != NULL)
         pLastActive = pLastActive->GetLastActivePopup();
      if (pLastActive != NULL &&
         pLastActive != ::win::window::GetForegroundWindow() &&
         pLastActive->IsWindowEnabled())
      {
         pLastActive->SetForegroundWindow();
         return TRUE;
      }
   }
   return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
// Standard init called by WinMain

__STATIC bool CLASS_DECL_win __register_with_icon(WNDCLASS* pWndCls,
                                                  const char * lpszClassName, UINT nIDIcon)
{
   pWndCls->lpszClassName = lpszClassName;
   pWndCls->hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
   return __register_class(pWndCls);
}


string CLASS_DECL_win get_user_interaction_window_class(sp(::user::interaction) pui)
{
   ::user::interaction::e_type etype = pui->get_window_type();
   // mask off all classes that are already registered
   //__MODULE_STATE* pModuleState = __get_module_state();
/*   LONG fToRegister = fToRegisterParam & ~pModuleState->m_fRegisteredClasses;
   if (fToRegister == 0)
   {
      fToRegister = fToRegisterParam;
      if(ppszClass != NULL)
      {
         if(fToRegister & __WND_REG)
         {
            *ppszClass = gen_Wnd;
         }
         else if (fToRegister & __WNDOLECONTROL_REG)
         {
            *ppszClass = gen_WndOleControl;
         }
         else if (fToRegister & __WNDCONTROLBAR_REG)
         {
            *ppszClass = gen_WndControlBar;
         }
         else if(fToRegister & __WNDMDIFRAME_REG)
         {
            *ppszClass = gen_WndMDIFrame;
         }
         else if(fToRegister & __WNDFRAMEORVIEW_REG)
         {
            *ppszClass = gen_WndFrameOrView;
         }
      }
      return TRUE;
   }

   LONG fRegisteredClasses = 0;

   // common initialization
   */
   WNDCLASS wndcls;
   memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
   wndcls.lpfnWndProc = DefWindowProc;
   wndcls.hInstance = g_hinstance;
   //wndcls.hCursor = afxData.hcurArrow;

   INITCOMMONCONTROLSEX init;
   init.dwSize = sizeof(init);
   /*
   // work to register classes as specified by fToRegister, populate fRegisteredClasses as we go
   if (fToRegister & __WND_REG)
   {
      // Child windows - no brush, no icon, safest default class styles
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = gen_Wnd;
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WND_REG;
      }
   }
   if (fToRegister & __WNDOLECONTROL_REG)
   {
      // OLE control windows - use parent DC for speed
      wndcls.style |= CS_PARENTDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpszClassName = gen_WndOleControl;
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WNDOLECONTROL_REG;
      }
   }
   if (fToRegister & __WNDCONTROLBAR_REG)
   {
      // control bar windows
      wndcls.style = 0;   // control bars don't handle double click
      wndcls.lpszClassName = gen_WndControlBar;
      wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
      if (__register_class(&wndcls))
      {
         if(ppszClass != NULL)
         {
            *ppszClass = wndcls.lpszClassName;
         }
         fRegisteredClasses |= __WNDCONTROLBAR_REG;
      }
   }
   if (fToRegister & __WNDMDIFRAME_REG)
   {
      // MDI Frame window (also used for splitter window)
      wndcls.style = CS_DBLCLKS;
      wndcls.hbrBackground = NULL;
      /*      if (__register_with_icon(&wndcls, gen_WndMDIFrame, __IDI_STD_MDIFRAME))
      {
      if(ppszClass != NULL)
      {
      *ppszClass = gen_WndMDIFrame;
      }
      fRegisteredClasses |= __WNDMDIFRAME_REG;
      }
   }*/
   if (etype == ::user::interaction::type_frame || etype == ::user::interaction::type_view)
   {
      // SDI Frame or MDI Child windows or views - normal colors
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      if (__register_with_icon(&wndcls, gen_WndFrameOrView, 0))
      {
         return gen_WndFrameOrView;
      }
   }

   return __register_window_class(0);
   /*
   // save new state of registered controls
   pModuleState->m_fRegisteredClasses |= fRegisteredClasses;

   // special case for all common controls registered, turn on __WNDCOMMCTLS_REG
   if ((pModuleState->m_fRegisteredClasses & __WIN95CTLS_MASK) == __WIN95CTLS_MASK)
   {
      pModuleState->m_fRegisteredClasses |= __WNDCOMMCTLS_REG;
      fRegisteredClasses |= __WNDCOMMCTLS_REG;
   }

   // must have registered at least as mamy classes as requested
   return (fToRegister & fRegisteredClasses) == fToRegister;*/
}


/////////////////////////////////////////////////////////////////////////////
// Special WndProcs (activation handling & gray dialogs)


LRESULT CALLBACK
   __activation_window_procedure(oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
   WNDPROC oldWndProc = (WNDPROC)::GetProp(oswindow, gen_OldWndProc);
   ASSERT(oldWndProc != NULL);   

   LRESULT lResult = 0;
   try
   {
      bool bCallDefault = TRUE;
      switch (nMsg)
      {
      case WM_INITDIALOG:
         {
            uint32_t dwStyle;
            rect rectOld;
            sp(::ca2::window) pWnd = ::win::window::from_handle(oswindow);
            __pre_init_dialog(pWnd, &rectOld, &dwStyle);
            bCallDefault = FALSE;
            lResult = CallWindowProc(oldWndProc, oswindow, nMsg, wParam, lParam);
            __post_init_dialog(pWnd, rectOld, dwStyle);
         }
         break;

      case WM_ACTIVATE:
         __handle_activate(::win::window::from_handle(oswindow), wParam,
            ::win::window::from_handle((::oswindow) lParam));
         break;

      case WM_SETCURSOR:
         bCallDefault = !__handle_set_cursor(::win::window::from_handle(oswindow),
            (int16_t)LOWORD(lParam), HIWORD(lParam));
         break;

      case WM_NCDESTROY:
         SetWindowLongPtr(oswindow, GWLP_WNDPROC, reinterpret_cast<int_ptr>(oldWndProc));
         RemoveProp(oswindow, gen_OldWndProc);
         GlobalDeleteAtom(GlobalFindAtom(gen_OldWndProc));
         break;
      }

      // call original wndproc for default handling
      if (bCallDefault)
         lResult = CallWindowProc(oldWndProc, oswindow, nMsg, wParam, lParam);
   }
   catch(base_exception * pe)
   {
      // handle exception
      MSG msg;
      msg.hwnd = oswindow;
      msg.message = nMsg;
      msg.wParam = wParam;
      msg.lParam = lParam;

      //lResult = __process_window_procedure_exception(pe, &msg);
      //      TRACE(::ca2::trace::category_AppMsg, 0, "Warning: Uncaught exception in __activation_window_procedure (returning %ld).\n",
      //       lResult);
      pe->Delete();
   }


   return lResult;
}




/////////////////////////////////////////////////////////////////////////////
// Additional helpers for WNDCLASS init

// like RegisterClass, except will automatically call UnregisterClass
bool CLASS_DECL_win __register_class(WNDCLASS* lpWndClass)
{
   WNDCLASS wndcls;      
   if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName,
      &wndcls))
   {
      // class already registered
      return TRUE;
   }

   if (!::RegisterClass(lpWndClass))
   {
      //      TRACE(::ca2::trace::category_AppMsg, 0, "Can't register window class named %s\n",
      //       lpWndClass->lpszClassName);
      return FALSE;
   }

   bool bRet = TRUE;

   if (afxContextIsDLL)
   {

      try
      {
         // class registered successfully, add to registered list
         __MODULE_STATE* pModuleState = __get_module_state();
         single_lock sl(&pModuleState->m_mutexRegClassList, TRUE);
         if(pModuleState->m_pstrUnregisterList == NULL)
            pModuleState->m_pstrUnregisterList = new string;
         *pModuleState->m_pstrUnregisterList += lpWndClass->lpszClassName;
         *pModuleState->m_pstrUnregisterList +='\n';
      }
      catch(base_exception * pe)
      {
         ::ca2::rethrow(pe);
         // Note: DELETE_EXCEPTION not required.
      }

   }

   return bRet;
}



