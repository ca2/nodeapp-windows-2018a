#include "framework.h"

#define COMPILE_MULTIMON_STUBS
//#include <multimon.h>

#include "sal.h"


__STATIC void CLASS_DECL_win __pre_init_dialog(
   sp(::user::interaction) pWnd, LPRECT lpRectOld, uint32_t* pdwStyleOld);
__STATIC void CLASS_DECL_win __post_init_dialog(
   sp(::user::interaction) pWnd, const RECT& rectOld, uint32_t dwStyleOld);
LRESULT CALLBACK
   __activation_window_procedure(oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam);

__STATIC_DATA const char gen_OldWndProc[] = "::ca::OldWndProc423";

const char gen_WndControlBar[] = __WNDCONTROLBAR;
const char gen_WndMDIFrame[] = __WNDMDIFRAME;
const char gen_WndFrameOrView[] = __WNDFRAMEORVIEW;
const char gen_WndOleControl[] = __WNDOLECONTROL;

struct __CTLCOLOR
{
   oswindow oswindow;
   HDC hDC;
   UINT nCtlType;
};

const char gen_Wnd[] = __WND;

namespace win
{

   void window::mouse_hover_add(sp(::user::interaction) pinterface)
   {
      m_guieptraMouseHover.add_unique(pinterface);
   }

   void window::mouse_hover_remove(sp(::user::interaction) pinterface)
   {
      m_guieptraMouseHover.remove(pinterface); 
   }

   window::window()
   {
      m_pcallback = ::null();
      m_pguie = this;
      set_handle(::null());
      m_pguieOwner = ::null();
      m_pguie->m_nFlags = 0;
      m_pfnSuper = ::null();
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = ::null();
      m_pguieCapture = ::null();
      m_hbitmap = ::null();
      ZERO(m_size);
      ZERO(m_pt);
      m_pcolorref = ::null();
      m_pbitmap = ::null();
      ZERO(m_bitmapinfo);
      m_pmutexGraphics = ::null();
   }

   void window::construct(oswindow oswindow)
   {
      m_pcallback = ::null();
      m_pguie = this;
      set_handle(oswindow);
      m_pguie->m_nFlags = 0;
      m_pfnSuper = ::null();
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = ::null();
      m_pguieCapture = ::null();
      m_hbitmap = ::null();
      ZERO(m_size);
      ZERO(m_pt);
      m_pcolorref = ::null();
      m_pbitmap = ::null();
      ZERO(m_bitmapinfo);
      m_pmutexGraphics = ::null();
   }

   window::window(sp(::ca::application) papp) :
      ca(papp),
      ::user::interaction(papp)
   {
      m_pcallback = ::null();
      m_pguie = this;
      set_handle(::null());
      m_pguieOwner = ::null();
      m_pguie->m_nFlags = 0;
      m_pfnSuper = ::null();
      m_nModalResult = 0;
      m_bMouseHover = false;
      m_pfont = ::null();
      m_pguieCapture = ::null();
      m_hbitmap = ::null();
      ZERO(m_size);
      ZERO(m_pt);
      m_pcolorref = ::null();
      m_pbitmap = ::null();
      ZERO(m_bitmapinfo);
      m_pmutexGraphics = ::null();
   }


   window::~window()
   {

      if(m_papp != ::null() && m_papp->m_psystem != ::null() && Sys(m_papp).user().is_set() && Sys(m_papp).user()->m_pwindowmap != ::null())
      {
         Sys(m_papp).user()->m_pwindowmap->m_map.remove_key((int_ptr) get_handle());
      }

      single_lock sl(m_pthread == ::null() ? ::null() : &m_pthread->m_pthread->m_mutex, TRUE);
      if(m_pfont != ::null())
      {
         delete m_pfont;
      }
      sl.unlock();
      if (get_handle() != ::null())
      {
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: calling DestroyWindow in window::~window; "
            "OnDestroy or PostNcDestroy in derived class will not be called.\n");
         m_pcallback = ::null();
         DestroyWindow();
      }
   }

   sp(::ca::window) window::from_os_data(void * pdata)
   {
      return from_handle((oswindow) pdata);   
   }


   void * window::get_os_data() const
   {

      return (void *) ((::win::window *) this)->get_handle();

   }



   // Change a window's style

   __STATIC bool CLASS_DECL_win __modify_style(oswindow oswindow, int32_t nStyleOffset,
      uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {
      ASSERT(oswindow != ::null());
      uint32_t dwStyle = ::GetWindowLong(oswindow, nStyleOffset);
      uint32_t dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
      if (dwStyle == dwNewStyle)
         return FALSE;

      ::SetWindowLong(oswindow, nStyleOffset, dwNewStyle);
      if (nFlags != 0)
      {
         ::SetWindowPos(oswindow, ::null(), 0, 0, 0, 0,
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
      }
      return TRUE;
   }

   bool window::ModifyStyle(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {
      return __modify_style(oswindow, GWL_STYLE, dwRemove, dwAdd, nFlags);
   }

   bool window::ModifyStyleEx(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {
      return __modify_style(oswindow, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
   }



   const MSG* window::GetCurrentMessage()
   {
      // fill in time and position when asked for
      ___THREAD_STATE* pThreadState = __get_thread_state();
      pThreadState->m_lastSentMsg.time = ::GetMessageTime();
      pThreadState->m_lastSentMsg.pt = point(::GetMessagePos());
      return &pThreadState->m_lastSentMsg;
   }

   LRESULT window::Default()
   {
      // call DefWindowProc with the last message
      ___THREAD_STATE* pThreadState = __get_thread_state();
      return DefWindowProc(pThreadState->m_lastSentMsg.message,
         pThreadState->m_lastSentMsg.wParam, pThreadState->m_lastSentMsg.lParam);
   }


   sp(::ca::window) window::from_handle(oswindow oswindow)
   {
      single_lock sl(afxMutexoswindow_(), TRUE);
      oswindow_map* pMap = get_oswindow_map(TRUE); //create map if not exist
      try
      {
         ASSERT(pMap != ::null());
         sp(::ca::window) pWnd =  pMap->from_handle(oswindow);
         if(pWnd != ::null() && WIN_WINDOW(pWnd.m_p)->get_handle() != oswindow)
            return ::null();
         return pWnd;
      }
      catch(...)
      {
         return ::null();
      }
   }

   sp(::ca::window) window::FromHandlePermanent(oswindow oswindow)
   {
      single_lock sl(afxMutexoswindow_(), TRUE);
      oswindow_map* pMap = get_oswindow_map();
      sp(::ca::window) pWnd = ::null();
      if (pMap != ::null())
      {
         // only look in the permanent map - does no allocations
         pWnd = pMap->lookup_permanent(oswindow);
         if(pWnd != ::null() && WIN_WINDOW(pWnd)->get_handle() != oswindow)
            return ::null();
      }
      return pWnd;
   }

   bool window::attach(oswindow oswindow_New)
   {
      ASSERT(get_handle() == ::null());     // only attach once, detach on destroy
      ASSERT(FromHandlePermanent(oswindow_New) == ::null());
      // must not already be in permanent map

      if (oswindow_New == ::null())
         return FALSE;
      single_lock sl(afxMutexoswindow_(), TRUE);
      oswindow_map * pMap = get_oswindow_map(TRUE); // create map if not exist
      ASSERT(pMap != ::null());

      pMap->set_permanent(set_handle(oswindow_New), this);
      if(m_pguie == ::null())
      {
         m_pguie = this;
      }

      return TRUE;
   }

   oswindow window::detach()
   {
      oswindow oswindow = get_handle();
      if (oswindow != ::null())
      {
         single_lock sl(afxMutexoswindow_(), TRUE);
         oswindow_map * pMap = get_oswindow_map(); // don't create if not exist
         if (pMap != ::null())
            pMap->remove_handle(get_handle());
         set_handle(::null());
      }

      return oswindow;
   }

   void window::pre_subclass_window()
   {
      // no default processing
   }


   /////////////////////////////////////////////////////////////////////////////
   // window creation

   bool window::CreateEx(uint32_t dwExStyle, const char * lpszClassName,
      const char * lpszWindowName, uint32_t dwStyle,
      const RECT& rect, sp(::user::interaction) pParentWnd, id id,
      LPVOID lpParam /* = ::null() */)
   {
      return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
         rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
         pParentWnd->get_safe_handle(), id, lpParam);
   }

   bool window::CreateEx(uint32_t dwExStyle, const char * lpszClassName,
      const char * lpszWindowName, uint32_t dwStyle,
      int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
      oswindow oswindow_Parent, id id, LPVOID lpParam)
   {
      UNREFERENCED_PARAMETER(id);
      ASSERT(lpszClassName == ::null() || __is_valid_string(lpszClassName) || 
         __is_valid_atom(lpszClassName));
      ENSURE_ARG(lpszWindowName == ::null() || __is_valid_string(lpszWindowName));

      // allow modification of several common create parameters
      CREATESTRUCT cs;
      cs.dwExStyle = dwExStyle;


      string strClass = calc_window_class();
      cs.lpszClass = strClass.is_empty() ? ::null() : (const char *) strClass;
      cs.lpszName = lpszWindowName;
      cs.style = dwStyle;
      cs.x = x;
      cs.y = y;
      cs.cx = nWidth;
      cs.cy = nHeight;
      cs.hwndParent = oswindow_Parent;
      //   cs.hMenu = oswindow_Parent == ::null() ? ::null() : nIDorHMenu;
      cs.hMenu = ::null();
      cs.hInstance = System.m_hInstance;
      cs.lpCreateParams = lpParam;

      if(m_pguie != ::null() && m_pguie != this)
      {
         if(!m_pguie->pre_create_window(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }
      else
      {
         if (!pre_create_window(cs))
         {
            PostNcDestroy();
            return FALSE;
         }
      }

      if(cs.hwndParent == ::null())
      {
         cs.style &= ~WS_CHILD;
      }

      hook_window_create(this);

      oswindow oswindow = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

#ifdef DEBUG
      if (oswindow == ::null())
      {
         uint32_t dwLastError = GetLastError();
         string strLastError = FormatMessageFromSystem(dwLastError);
         string strMessage;
         strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);

         TRACE(::ca::trace::category_AppMsg, 0, "Warning: Window creation failed: GetLastError returned:\n");
         TRACE(::ca::trace::category_AppMsg, 0, "%s\n", strMessage);
         try
         {
            if(dwLastError == 0x0000057e)
            {
               System.simple_message_box(::null(), "Cannot create a top-level child window.");
            }
            else
            {
               System.simple_message_box(::null(), strMessage);
            }
         }
         catch(...)
         {
         }
      }
#endif

      if (!unhook_window_create())
         PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

      if (oswindow == ::null())
         return FALSE;
      WNDCLASS wndcls;
      if(lpszClassName != ::null() &&
         GetClassInfo(System.m_hInstance, lpszClassName, &wndcls) &&
         wndcls.hIcon != ::null())
      {
         m_pguie->set_icon(new ::visual::icon(wndcls.hIcon), false);
         m_pguie->set_icon(new ::visual::icon(wndcls.hIcon), true);
      }
      //      oswindow oswindowHandle = get_handle();
      if(oswindow != get_handle())
      {
         ASSERT(FALSE); // should have been set in send msg hook
      }
      return TRUE;
   }

   // for child windows
   bool window::pre_create_window(CREATESTRUCT& cs)
   {
      return TRUE;
   }

   bool window::create(const char * lpszClassName,
      const char * lpszWindowName, uint32_t dwStyle,
      const RECT& rect,
      sp(::user::interaction) pParentWnd, id id,
      sp(::ca::create_context) pContext)
   {
      // can't use for desktop or pop-up windows (use CreateEx instead)
      ASSERT(pParentWnd != ::null());
      ASSERT((dwStyle & WS_POPUP) == 0);

      return CreateEx(0, lpszClassName, lpszWindowName,
         dwStyle | WS_CHILD,
         rect.left, rect.top,
         rect.right - rect.left, rect.bottom - rect.top,
         pParentWnd->get_handle(), id, (LPVOID)pContext);
   }

   bool window::create_message_window(const char * pszName, ::ca::window_callback * pcallback)
   {
      m_pcallback = pcallback;
      if(IsWindow())
      {
         SetWindowText(pszName);
      }
      else
      {
         string strName = "::ca::fontopus::message_wnd::winservice_1";
         if(!CreateEx(0, ::null(), pszName, WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, 0, ::null()))
         {
            return false;
         }
      }
      return true;
   }

   string window:: calc_window_class()
   {

      uint32_t uiStyle = m_pguie->get_window_default_style();

      string strIcon = m_pguie->get_window_icon_matter();

      if(strIcon.has_char())
      {

         return calc_icon_window_class(uiStyle, strIcon);

      }
      else
      {

         return get_user_interaction_window_class(m_pguie);

      }

   }


   string window::calc_icon_window_class(uint32_t dwDefaultStyle, const char * pszMatter)
   {

      string strPath = Application.dir().matter(pszMatter, "icon.ico");

      HICON hIcon = (HICON) ::LoadImage(::null(), strPath, IMAGE_ICON, 256, 256, LR_LOADFROMFILE);

      string strClass = get_user_interaction_window_class(m_pguie);

      if(hIcon != ::null())
      {


         // will fill lpszClassName with default WNDCLASS name
         // ignore instance handle from pre_create_window.

         WNDCLASS wndcls;
         if(strClass.has_char() &&  GetClassInfo(System.m_hInstance, strClass, &wndcls) && wndcls.hIcon != hIcon)
         {
            // register a very similar WNDCLASS
            return __register_window_class(wndcls.style, wndcls.hCursor, wndcls.hbrBackground, hIcon);
         }
      }

      return strClass;

   }


   void window::install_message_handling(::ca::message::dispatch * pinterface)
   {
      //m_pbuffer->InstallMessageHandling(pinterface);
      IGUI_WIN_MSG_LINK(WM_NCDESTROY         , pinterface, this, &window::_001OnNcDestroy);
      IGUI_WIN_MSG_LINK(WM_PAINT             , pinterface, this, &window::_001OnPaint);
      IGUI_WIN_MSG_LINK(WM_PRINT             , pinterface, this, &window::_001OnPrint);
      if(m_pguie != ::null() && m_pguie != this)
      {
         m_pguie->install_message_handling(pinterface);
      }
      IGUI_WIN_MSG_LINK(WM_CAPTURECHANGED    , pinterface, this, &window::_001OnCaptureChanged);
      IGUI_WIN_MSG_LINK(WM_CREATE            , pinterface, this, &window::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SETCURSOR         , pinterface, this, &window::_001OnSetCursor);
      IGUI_WIN_MSG_LINK(WM_ERASEBKGND        , pinterface, this, &window::_001OnEraseBkgnd);
      IGUI_WIN_MSG_LINK(WM_MOVE              , pinterface, this, &window::_001OnMove);
      IGUI_WIN_MSG_LINK(WM_SIZE              , pinterface, this, &window::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW        , pinterface, this, &window::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(ca2m_PRODEVIAN_SYNCH , pinterface, this, &window::_001OnProdevianSynch);
      IGUI_WIN_MSG_LINK(WM_DESTROY           , pinterface, this, &window::_001OnDestroy);
   }

   void window::_001OnMove(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      {
         class rect rectWindow;
         ::GetWindowRect(get_handle(), rectWindow);
         m_pguie->m_rectParentClient = rectWindow;
         m_rectParentClient = rectWindow;
      }
   }

   void window::win_update_graphics()
   {

      single_lock sl(mutex_graphics(), true);

      rect rectWindow;

      GetWindowRect(rectWindow);

      m_pt = rectWindow.top_left();

      if(rectWindow.area() <= 0)
         return;

      if(m_size != rectWindow.size())
      {

         m_spg.release();

         m_spb.release();

         //if(m_pbitmap != ::null())
         //{
         // delete m_pbitmap;
         //}

         if(m_hbitmap != ::null())
            ::DeleteObject(m_hbitmap);

         m_size = rectWindow.size();

         ZeroMemory(&m_bitmapinfo, sizeof (BITMAPINFO));

         m_bitmapinfo.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
         m_bitmapinfo.bmiHeader.biWidth         = m_size.cx;
         m_bitmapinfo.bmiHeader.biHeight        = -m_size.cy;
         m_bitmapinfo.bmiHeader.biPlanes        = 1;
         m_bitmapinfo.bmiHeader.biBitCount      = 32; 
         m_bitmapinfo.bmiHeader.biCompression   = BI_RGB;
         m_bitmapinfo.bmiHeader.biSizeImage     = m_size.cx * m_size.cy * 4;


         m_hbitmap = CreateDIBSection(::null(), &m_bitmapinfo, DIB_RGB_COLORS, (void **) &m_pcolorref, ::null(), 0);



#undef new
         m_pbitmap =  new  Gdiplus::Bitmap(m_size.cx, m_size.cy, m_size.cx *4 , PixelFormat32bppARGB, (BYTE *) m_pcolorref);
#define new DEBUG_NEW


         m_spg.create(allocer());

#undef new
         (dynamic_cast < ::win::graphics * > (m_spg.m_p))->attach(new Gdiplus::Graphics(m_pbitmap));
#define new DEBUG_NEW

         m_spb.create(allocer());

         m_spb->attach(m_pbitmap);

         m_spg->SelectObject(m_spb);

      }

   }

   void window::_001OnSize(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);


      if(!m_bRectOk && !(GetExStyle() & WS_EX_LAYERED))
      {
         class rect rectWindow;
         ::GetWindowRect(get_handle(), rectWindow);
         m_pguie->m_rectParentClient = rectWindow;
         m_rectParentClient = rectWindow;
      }

      /*      if(m_spdibMultAlphaWork.is_null())
      {
      m_spdibMultAlphaWork.create(allocer());
      }

      if(m_spdib.is_null())
      {
      m_spdib.create(allocer());
      }

      if(m_spdib.is_set() && m_rectParentClient.area() > 0)
      {
      m_spdib->create(m_rectParentClient.size());
      }*/



      m_pguie->layout();



   }

   void window::_001OnShowWindow(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::show_window, pshowwindow, pobj);
      m_bVisible = pshowwindow->m_bShow != FALSE;
      if(m_pguie != ::null() && m_pguie != this)
         m_pguie->m_bVisible = m_bVisible;
   }

   void window::_001OnDestroy(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();
      if(System.get_twf() != ::null())
      {
         sp(::win::window_draw) pdraw = System.get_twf();
         if(pdraw != ::null())
         {
            retry_single_lock sl(&pdraw->m_eventFree, millis(84), millis(84));
            pdraw->m_wndpaOut.remove(this);
            pdraw->m_wndpaOut.remove(m_pguie);
         }
      }
   }

   void window::_001OnCaptureChanged(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      m_pguieCapture = ::null();
   }

   // WM_NCDESTROY is the absolute LAST message sent.
   void window::_001OnNcDestroy(::ca::signal_object * pobj)
   {
      single_lock sl(m_pthread == ::null() ? ::null() : &m_pthread->m_pthread->m_mutex, TRUE);
      pobj->m_bRet = true;
      // cleanup main and active windows
      ::ca::thread* pThread = ::ca::get_thread();
      if (pThread != ::null())
      {
         if (pThread->GetMainWnd() == this)
         {
            if (!afxContextIsDLL)
            {
               // shut down current thread if possible
               if (pThread != &System)
                  __post_quit_message(0);
            }
            pThread->SetMainWnd(::null());
         }
         if (pThread->get_active_ui() == this)
            pThread->set_active_ui(::null());
      }

      // cleanup tooltip support
      if(m_pguie != ::null())
      {
         if (m_pguie->m_nFlags & WF_TOOLTIPS)
         {
         }
      }

      // call default, unsubclass, and detach from the map
      WNDPROC pfnWndProc = WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC));
      Default();
      if (WNDPROC(GetWindowLongPtr(get_handle(), GWLP_WNDPROC)) == pfnWndProc)
      {
         WNDPROC pfnSuper = *GetSuperWndProcAddr();
         if (pfnSuper != ::null())
            SetWindowLongPtr(get_handle(), GWLP_WNDPROC, reinterpret_cast<int_ptr>(pfnSuper));
      }
      detach();
      ASSERT(get_handle() == ::null());
      m_pfnDispatchWindowProc = &window::_start_user_message_handler;
      // call special post-cleanup routine
      PostNcDestroy();
      if(m_pguie != ::null() && m_pguie != this)
      {
         m_pguie->PostNcDestroy();
      }
   }

   void window::PostNcDestroy()
   {
      set_handle(::null());
      // default to nothing
   }

   void window::on_final_release()
   {
      if (get_handle() != ::null())
         DestroyWindow();    // will call PostNcDestroy
      else
         PostNcDestroy();
   }

   void window::assert_valid() const
   {
      if(((::win::window *)this)->get_handle() == ::null())
         return;     // null (unattached) windows are valid

      // check for special wnd??? values
      ASSERT(HWND_TOP == ::null());       // same as desktop
      if (((::win::window *)this)->get_handle() == HWND_BOTTOM)
      {
      }
      else if (((::win::window *)this)->get_handle() == HWND_TOPMOST)
      {
      }
      else if (((::win::window *)this)->get_handle() == HWND_NOTOPMOST)
      {
      }
      else
      {
         // should be a normal window
         ASSERT(::IsWindow(((::win::window *)this)->get_handle()));

         // should also be in the permanent or temporary handle map
         single_lock sl(afxMutexoswindow_(), TRUE);
         oswindow_map * pMap = get_oswindow_map();
         if(pMap == ::null()) // inside thread not having windows
            return; // let go
         ASSERT(pMap != ::null());

         //         ::ca::object* p=::null();
         /*if(pMap)
         {
         ASSERT( (p = pMap->lookup_permanent(get_handle())) != ::null() ||
         (p = pMap->lookup_temporary(get_handle())) != ::null());
         }*/

         //ASSERT((p) == this);   // must be us

         // Note: if either of the above asserts fire and you are
         // writing a multithreaded application, it is likely that
         // you have passed a C++ object from one thread to another
         // and have used that object in a way that was not intended.
         // (only simple inline wrapper functions should be used)
         //
         // In general, window objects should be passed by oswindow from
         // one thread to another.  The receiving thread can wrap
         // the oswindow with a window object by using ::win::window::from_handle.
         //
         // It is dangerous to pass C++ objects from one thread to
         // another, unless the objects are designed to be used in
         // such a manner.
      }
   }


   void window::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);

      dumpcontext << "\nm_oswindow_ = " << ((::win::window *)this)->get_handle();

      if (((::win::window *)this)->get_handle() == ::null() || ((::win::window *)this)->get_handle() == HWND_BOTTOM ||
         ((::win::window *)this)->get_handle() == HWND_TOPMOST || ((::win::window *)this)->get_handle() == HWND_NOTOPMOST)
      {
         // not a normal window - nothing more to dump
         return;
      }

      if (!::IsWindow(((::win::window *)this)->get_handle()))
      {
         // not a valid window
         dumpcontext << " (illegal oswindow)";
         return; // don't do anything more
      }

      sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(((::win::window *)this)->get_handle());
      if (pWnd.m_p != this)
         dumpcontext << " (Detached or temporary window)";
      else
         dumpcontext << " (permanent window)";

      // dump out window specific statistics
      char szBuf [64];    
      //if (!const_cast < ::ca::window * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pWnd == this)
        // ((::win::window *)this)->GetWindowText(szBuf, _countof(szBuf));
      //else
        // ::DefWindowProc(((::win::window *)this)->get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      //dumpcontext << "\ncaption = \"" << szBuf << "\"";

      ::GetClassName(((::win::window *)this)->get_handle(), szBuf, _countof(szBuf));
      dumpcontext << "\nclass name = \"" << szBuf << "\"";

      rect rect;
      ((::win::window *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent sp(::ca::window) = " << ((::win::window *)this)->get_parent().m_p;

      dumpcontext << "\nstyle = " << (uint_ptr)::GetWindowLong(((::win::window *)this)->get_handle(), GWL_STYLE);
      if (::GetWindowLong(((::win::window *)this)->get_handle(), GWL_STYLE) & WS_CHILD)
         dumpcontext << "\nid = " << __get_dialog_control_id(((::win::window *)this)->get_handle());

      dumpcontext << "\n";
   }

   bool window::DestroyWindow()
   {
      single_lock sl(m_pthread == ::null() ? ::null() : &m_pthread->m_pthread->m_mutex, TRUE);
      sp(::ca::window) pWnd;
      oswindow_map * pMap;
      oswindow oswindow_Orig;
      bool bResult;

      if ((get_handle() == ::null()) )
         return FALSE;

      bResult = FALSE;
      pMap = ::null();
      pWnd = ::null();
      oswindow_Orig = ::null();
      if (get_handle() != ::null())
      {
         single_lock sl(afxMutexoswindow_(), TRUE);
         pMap = get_oswindow_map();
         if(pMap != ::null())
         {
            pWnd = (pMap->lookup_permanent(get_handle()));
#ifdef DEBUG
            oswindow_Orig = get_handle();
#endif
         }
      }
      sl.unlock();
      if (get_handle() != ::null())
         bResult = ::DestroyWindow(get_handle()) != FALSE;
      sl.lock();
      if (oswindow_Orig != ::null())
      {
         // Note that 'this' may have been deleted at this point,
         //  (but only if pWnd != ::null())
         if (pWnd != ::null() && bResult)
         {
            // Should have been detached by OnNcDestroy
#ifdef DEBUG
            sp(::ca::window) pWndPermanent = (pMap->lookup_permanent(oswindow_Orig));;
            ASSERT(pWndPermanent == ::null());
            // It is important to call base class, including ca core
            // base classes implementation of install_message_handling
            // inside derived class install_message_handling
#endif
         }
         else
         {
#ifdef DEBUG
            ASSERT(get_handle() == oswindow_Orig);
#endif
            // detach after DestroyWindow called just in case
            detach();
         }
      }

      return bResult;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default window implementation


   LRESULT window::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
   {
      if (m_pfnSuper != ::null())
         return ::CallWindowProc(m_pfnSuper, get_handle(), nMsg, wParam, lParam);

      WNDPROC pfnWndProc;
      if ((pfnWndProc = *GetSuperWndProcAddr()) == ::null())
         return ::DefWindowProc(get_handle(), nMsg, wParam, lParam);
      else
         return ::CallWindowProc(pfnWndProc, get_handle(), nMsg, wParam, lParam);
   }

   WNDPROC* window::GetSuperWndProcAddr()
   {
      // Note: it is no longer necessary to override GetSuperWndProcAddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;
   }

   void window::pre_translate_message(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // no default processing
   }


   void window::GetWindowText(string & rString)
   {
      /*ASSERT(::IsWindow(get_handle()));

      int32_t nLen = ::GetWindowTextLength(get_handle());
      ::GetWindowText(get_handle(), rString.GetBufferSetLength(nLen), nLen+1);
      rString.ReleaseBuffer();*/
      rString = m_strWindowText;

   }

   int32_t window::GetChildByIdText(int32_t nID, string & rString) const
   {
      ASSERT(::IsWindow(((::win::window *)this)->get_handle()));
      rString = "";    // is_empty without deallocating

      oswindow oswindow = ::GetDlgItem(((::win::window *)this)->get_handle(), nID);
      if (oswindow != ::null())
      {
         int32_t nLen = ::GetWindowTextLength(oswindow);
         ::GetWindowText(oswindow, rString.GetBufferSetLength(nLen), nLen+1);
         rString.ReleaseBuffer();
      }

      return (int32_t)rString.get_length();
   }

   bool window::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) 
   {
      ASSERT(::IsWindow(get_handle()));
      lpwndpl->length = sizeof(WINDOWPLACEMENT);
      return ::GetWindowPlacement(get_handle(), lpwndpl) != FALSE;
   }

   bool window::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      ASSERT(::IsWindow(get_handle()));
      ((WINDOWPLACEMENT*)lpwndpl)->length = sizeof(WINDOWPLACEMENT);
      return ::SetWindowPlacement(get_handle(), lpwndpl) != FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // window will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
   void window::OnDrawItem(int32_t /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
   {

      // reflect notification to child window control
      if (ReflectLastMsg(lpDrawItemStruct->hwndItem))
         return;     // eat it

      // not handled - do default
      Default();
   }

   // Drawing: for all 4 control types
   int32_t window::OnCompareItem(int32_t /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   {
      // reflect notification to child window control
      LRESULT lResult;
      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
         return (int32_t)lResult;        // eat it

      // not handled - do default
      return (int32_t)Default();
   }

   void window::OnDeleteItem(int32_t /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   {
      // reflect notification to child window control
      if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
         return;     // eat it
      // not handled - do default
      Default();
   }

   bool window::_EnableToolTips(bool bEnable, UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
   void window::OnMeasureItem(int32_t /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   {
      if (lpMeasureItemStruct->CtlType == ODT_MENU)
      {
         ASSERT(lpMeasureItemStruct->CtlID == 0);
      }
      else
      {
         sp(::user::interaction) pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
         if (pChild != ::null() && pChild->SendChildNotifyLastMsg())
            return;     // eaten by child
      }
      // not handled - do default
      Default();
   }

   bool window::GetWindowInfo(PWINDOWINFO pwi) const
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::GetWindowInfo(get_handle(), pwi) != FALSE; 
   }

   sp(::ca::window) window::GetAncestor(UINT gaFlags) const
   { ASSERT(::IsWindow(get_handle())); return  ::win::window::from_handle(::GetAncestor(get_handle(), gaFlags)); }



   bool window::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
   {
      ASSERT(::IsWindow(get_handle())); 
      ASSERT(psbi != ::null());
      return ::GetScrollBarInfo(get_handle(), idObject, psbi) != FALSE;
   }

   bool window::GetTitleBarInfo(PTITLEBARINFO pti) const
   {
      ASSERT(::IsWindow(get_handle())); 
      ASSERT(pti != ::null());
      return ::GetTitleBarInfo(get_handle(), pti) != FALSE;
   }

   bool window::AnimateWindow(uint32_t dwTime, uint32_t dwFlags) 
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::AnimateWindow(get_handle(), dwTime, dwFlags) != FALSE;
   }

   bool window::FlashWindowEx(uint32_t dwFlags, UINT  uCount, uint32_t dwTimeout)
   {
      ASSERT(::IsWindow(get_handle())); 
      FLASHWINFO fwi;
      fwi.cbSize = sizeof(fwi);
      fwi.hwnd = get_handle();
      fwi.dwFlags = dwFlags;
      fwi.uCount = uCount;
      fwi.dwTimeout = dwTimeout;

      return ::FlashWindowEx(&fwi) != FALSE;
   }



   bool window::SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, uint32_t dwFlags)
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::SetLayeredWindowAttributes(get_handle(), crKey, bAlpha, dwFlags) != FALSE;
   }

   bool window::UpdateLayeredWindow(::ca::graphics * pDCDst, POINT *pptDst, SIZE *psize, 
      ::ca::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, uint32_t dwFlags)
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::UpdateLayeredWindow(get_handle(), WIN_HDC(pDCDst), pptDst, psize,
         WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;
   }


   bool window::GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, uint32_t *pdwFlags) const
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::GetLayeredWindowAttributes(get_handle(), pcrKey, pbAlpha, (LPDWORD) pdwFlags) != FALSE;
   }

   bool window::PrintWindow(::ca::graphics * pgraphics, UINT nFlags) const
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::PrintWindow(get_handle(), (HDC)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle(), nFlags) != FALSE;
   }


   LRESULT window::OnNTCtlColor(WPARAM wParam, LPARAM lParam)
   {
      // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
      __CTLCOLOR ctl;
      ctl.hDC = (HDC)wParam;
      ctl.oswindow = (oswindow) lParam;
      ___THREAD_STATE* pThreadState = __get_thread_state();
      ctl.nCtlType = pThreadState->m_lastSentMsg.message - WM_CTLCOLORMSGBOX;
      //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
      ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

      // Note: We call the virtual message_handler for this window directly,
      //  instead of calling ::ca::CallWindowProc, so that Default()
      //  will still work (it will call the Default window proc with
      //  the original Win32 WM_CTLCOLOR message).
      /*
      return message_handler(WM_CTLCOLOR, 0, (LPARAM)&ctl);*/
      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   // window extensions for help support

   void window::WinHelp(uint_ptr dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented(get_app());

      /*      application* pApp = &System;
      ASSERT_VALID(pApp);
      ASSERT(pApp->m_pszHelpFilePath != ::null());
      ASSERT(pApp->m_eHelpType == afxWinHelp);

      wait_cursor wait(get_app());

      PrepareForHelp();

      // need to use top level parent (for the case where get_handle() is in DLL)
      sp(::user::interaction) pWnd = EnsureTopLevelParent();

      TRACE(::ca::trace::category_AppMsg, 0, "WinHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

      // finally, run the Windows Help engine
      /* trans   if (!::WinHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
      {
      // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
      System.simple_message_box("Failed to launch help");
      }*/
   }

   //void window::HtmlHelp(uint_ptr dwData, UINT nCmd)
   //{
   // throw not_implemented(get_app());
   /*
   application* pApp = &System;
   ASSERT_VALID(pApp);
   ASSERT(pApp->m_pszHelpFilePath != ::null());
   // to call HtmlHelp the m_fUseHtmlHelp must be set in
   // the application's constructor
   ASSERT(pApp->m_eHelpType == afxHTMLHelp);

   wait_cursor wait(get_app());

   PrepareForHelp();

   // need to use top level parent (for the case where get_handle() is in DLL)
   sp(::user::interaction) pWnd = EnsureTopLevelParent();

   TRACE(::ca::trace::category_AppMsg, 0, "HtmlHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d.\n", pApp->m_pszHelpFilePath, dwData, nCmd);

   // run the HTML Help engine
   /* trans   if (!::ca::HtmlHelp(WIN_WINDOW(pWnd)->get_handle(), pApp->m_pszHelpFilePath, nCmd, dwData))
   {
   // linux System.simple_message_box(__IDP_FAILED_TO_LAUNCH_HELP);
   System.simple_message_box("Failed to launch help");
   }*/
   //}

   void window::PrepareForHelp()
   {
      if (is_frame_window())
      {
         // frame_window windows should be allowed to exit help mode first
         sp(frame_window) pFrameWnd = this;
         pFrameWnd->ExitHelpMode();
      }

      // cancel any tracking modes
      send_message(WM_CANCELMODE);
      SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // need to use top level parent (for the case where get_handle() is in DLL)
      sp(::user::interaction) pWnd = EnsureTopLevelParent();
      WIN_WINDOW(pWnd.m_p)->send_message(WM_CANCELMODE);
      WIN_WINDOW(pWnd.m_p)->SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

      // attempt to cancel capture
      oswindow oswindow_Capture = ::GetCapture();
      if (oswindow_Capture != ::null())
         ::SendMessage(oswindow_Capture, WM_CANCELMODE, 0, 0);
   }


   void window::WinHelpInternal(uint_ptr dwData, UINT nCmd)
   {
      UNREFERENCED_PARAMETER(dwData);
      UNREFERENCED_PARAMETER(nCmd);
      throw not_implemented(get_app());
      /*
      application* pApp = &System;
      ASSERT_VALID(pApp);
      if (pApp->m_eHelpType == afxHTMLHelp)
      {
      // translate from WinHelp commands and data to to HtmlHelp
      ASSERT((nCmd == HELP_CONTEXT) || (nCmd == HELP_CONTENTS) || (nCmd == HELP_FINDER));
      if (nCmd == HELP_CONTEXT)
      nCmd = HH_HELP_CONTEXT;
      else if (nCmd == HELP_CONTENTS)
      nCmd = HH_DISPLAY_TOC;
      else if (nCmd == HELP_FINDER)
      nCmd = HH_HELP_FINDER;
      HtmlHelp(dwData, nCmd);
      }
      else
      WinHelp(dwData, nCmd);*/
   }



   bool window::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
      if(command_target_interface::_001OnCmdMsg(pcmdmsg))
         return TRUE;

      //      bool b;

      //if(_iguimessageDispatchCommandMessage(pcommand, b))
      // return b;

      command_target * pcmdtarget = dynamic_cast < command_target * > (this);
      return pcmdtarget->command_target::_001OnCmdMsg(pcmdmsg);
   }


   bool window::BaseOnControlEvent(::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

   void window::_002OnDraw(::ca::graphics * pdc)
   {

      ::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::win::graphics * >(pdc))->get_handle()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }

   /////////////////////////////////////////////////////////////////////////////
   // main message_handler implementation

   void window::message_handler(::ca::signal_object * pobj)
   {

      SCAST_PTR(::ca::message::base, pbase, pobj);

      if(pbase->m_uiMessage == WM_SIZE || pbase->m_uiMessage == WM_MOVE)
      {

         win_update_graphics();

      }

      if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR)
      {

         SCAST_PTR(::ca::message::key, pkey, pobj);

         if(pbase->m_uiMessage == WM_KEYDOWN)
         {
            try
            {
               Application.set_key_pressed(pkey->m_ekey, true);
            }
            catch(...)
            {
            }
         }
         else if(pbase->m_uiMessage == WM_KEYUP)
         {
            try
            {
               Application.set_key_pressed(pkey->m_ekey, false);
            }
            catch(...)
            {
            }
         }
      }



      if(m_pguie != ::null())
      {
         m_pguie->pre_translate_message(pobj);
         if(pobj->m_bRet)
            return;
      }

      if(m_pcallback != ::null())
      {
         m_pcallback->message_window_message_handler(pobj);
         if(pobj->m_bRet)
            return;
      }
      if(pbase->m_uiMessage == WM_TIMER)
      {
         m_pthread->m_pthread->step_timer();
      }
      else if(pbase->m_uiMessage == WM_LBUTTONDOWN)
      {
         g_pwndLastLButtonDown = this;
      }
      /*      else if(pbase->m_uiMessage == CA2M_BERGEDGE)
      {
      if(pbase->m_wparam == BERGEDGE_GETAPP)
      {
      sp(::ca::application)* ppapp= (sp(::ca::application)*) pbase->m_lparam;
      *ppapp = get_app();
      pbase->m_bRet = true;
      return;
      }
      }*/
      pbase->set_lresult(0);

      if(pbase->m_uiMessage == WM_MOUSELEAVE)
      {
         m_bMouseHover = false;
         for(int32_t i = 0; i < m_guieptraMouseHover.get_size(); i++)
         {
            if(m_guieptraMouseHover(i) == this 
               || m_guieptraMouseHover[i].m_pimpl == this 
               || m_guieptraMouseHover[i].m_pguie == this)
               continue;
            m_guieptraMouseHover[i].send_message(WM_MOUSELEAVE);
         }
         m_guieptraMouseHover.remove_all();
      }

      if(pbase->m_uiMessage == WM_LBUTTONDOWN ||
         pbase->m_uiMessage == WM_LBUTTONUP ||
         pbase->m_uiMessage == WM_MBUTTONDOWN ||
         pbase->m_uiMessage == WM_MBUTTONUP ||
         pbase->m_uiMessage == WM_RBUTTONDOWN ||
         pbase->m_uiMessage == WM_RBUTTONUP ||
         pbase->m_uiMessage == WM_MOUSEMOVE ||
         pbase->m_uiMessage == WM_MOUSEWHEEL)
      {
         // user presence status activity reporting
         if(pbase->m_uiMessage == WM_LBUTTONDOWN 
            || pbase->m_uiMessage == WM_RBUTTONDOWN 
            || pbase->m_uiMessage == WM_MBUTTONDOWN
            || pbase->m_uiMessage == WM_MOUSEMOVE)
         {
            if(Session.fontopus()->m_puser != ::null())
            {
               if(&ApplicationUser != ::null())
               {
                  if(ApplicationUser.m_ppresence != ::null())
                  {
                     try
                     {
                        ApplicationUser.m_ppresence->report_activity();
                     }
                     catch(...)
                     {
                     }
                  }
               }
            }
         }
         ::ca::message::mouse * pmouse = (::ca::message::mouse *) pbase;

         Application.m_ptCursor = pmouse->m_pt;
         if(m_papp != ::null() && m_papp->m_psession != ::null())
         {
            Session.m_ptCursor = pmouse->m_pt;
            if(m_papp->m_psession->m_pbergedgeInterface != ::null())
            {
               m_papp->m_psession->m_pbergedgeInterface->m_ptCursor = pmouse->m_pt;
            }
         }
         if(m_pguie != ::null() && m_pguie != this && m_pguie->m_papp->m_psession != ::null() && m_pguie->m_papp->m_psession != m_papp->m_psession)
         {
            Sess(m_pguie->m_papp->m_psession).m_ptCursor = pmouse->m_pt;
         }

         sp(::plane::session) psession = ::null();
         if(m_papp->is_system())
         {
            psession = System.query_session(0);
            if(psession != ::null() && psession->m_bSessionSynchronizedCursor)
            {
               psession->m_ptCursor = pmouse->m_pt;
            }
         }

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {
            pmouse->m_bTranslated = true;
            rect64 rectWindow;
            if(m_bOSNativeMouseMessagePosition)
            {
               class rect rectWindow32;
               ::GetWindowRect(get_handle(), &rectWindow32);
               ::copy(rectWindow, rectWindow32);
            }
            else
            {
               m_pguie->GetWindowRect(rectWindow);   
            }
            if(System.get_monitor_count() > 0)
            {
               rect rcMonitor;
               System.get_monitor_rect(0, &rcMonitor);
               if(rectWindow.left >= rcMonitor.left)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= rcMonitor.top)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
            else
            {
               if(rectWindow.left >= 0)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= 0)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
         }

         if(pbase->m_uiMessage == WM_MOUSEMOVE)
         {
            // We are at the message_handler procedure.
            // mouse messages originated from message_handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;
         }
restart_mouse_hover_check:
         for(int32_t i = 0; i < m_guieptraMouseHover.get_size(); i++)
         {
            if(!m_guieptraMouseHover(i)->_001IsPointInside(pmouse->m_pt))
            {
               sp(::user::interaction) pui = m_guieptraMouseHover(i);
               pui->send_message(WM_MOUSELEAVE);
               m_guieptraMouseHover.remove(pui);
               goto restart_mouse_hover_check;
            }
         }
         if(!m_bMouseHover)
         {
            m_pguie->_001OnTriggerMouseInside();
         }
         if(m_pguieCapture != ::null())
         {
            if(m_pguieCapture->m_pimpl != ::null())
            {
               //m_pguieCapture->m_pimpl->SendMessage(pbase);
               try
               {
                  (m_pguieCapture->m_pimpl->*m_pguieCapture->m_pimpl->m_pfnDispatchWindowProc)(dynamic_cast < ::ca::signal_object * > (pmouse));
                  if(pmouse->get_lresult() != 0)
                     return;
               }
               catch(...)
               {
               }
               return;
            }
            else
            {
               //m_pguieCapture->SendMessage(pbase);
               try
               {
                  (m_pguieCapture->*m_pguieCapture->m_pfnDispatchWindowProc)(dynamic_cast < ::ca::signal_object * > (pmouse));
                  if(pmouse->get_lresult() != 0)
                     return;
               }
               catch(...)
               {
               }
               return;
            }
         }
         user::oswindow_array oswindowa;
         user::interaction_ptr_array wnda(get_app());
         wnda = System.frames();
         wnda.get_wnda(oswindowa);
         user::window_util::SortByZOrder(oswindowa);
         for(int32_t i = 0; i < oswindowa.get_size(); i++)
         {
            sp(::user::interaction) pguie = wnda.find_first(oswindowa[i]);
            if(pguie != ::null() && pguie->m_pguie != ::null())
            {
               pguie->m_pguie->_000OnMouse(pmouse);
               if(pmouse->m_bRet)
                  return;
            }
         }
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }
      else if(pbase->m_uiMessage == WM_KEYDOWN ||
         pbase->m_uiMessage == WM_KEYUP ||
         pbase->m_uiMessage == WM_CHAR)
      {

         ::ca::message::key * pkey = (::ca::message::key *) pbase;
         sp(::user::interaction) puiFocus = Application.user()->get_keyboard_focus();
         if(puiFocus != ::null() 
            && puiFocus->IsWindow()
            && puiFocus->GetTopLevelParent() != ::null())
         {
            puiFocus->send_message(pkey);
            if(pbase->m_bRet)
               return;
         }
         else if(!pkey->m_bRet)
         {
            if(m_pguie != this && m_pguie != ::null())
            {
               m_pguie->_000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }
            else
            {
               _000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }
         }
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }
      if(pbase->m_uiMessage == ::ca::message_event)
      {
         if(m_pguie != this && m_pguie != ::null())
         {
            m_pguie->BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         else
         {
            BaseOnControlEvent((::user::control_event *) pbase->m_lparam);
         }
         return;
      }
      (this->*m_pfnDispatchWindowProc)(pobj);
      if(pobj->m_bRet)
         return;
      /*
      if(m_pguie != ::null() && m_pguie != this)
      {
      m_pguie->_user_message_handler(pobj);
      if(pobj->m_bRet)
      return;
      }
      */
      if(m_pguie != ::null())
      {
         pbase->set_lresult(m_pguie->DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
      }
      else
      {
         pbase->set_lresult(DefWindowProc(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
      }
   }

   /*
   bool window::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
   LRESULT lResult = 0;
   union MessageMapFunctions mmf;
   mmf.pfn = 0;
   CInternalGlobalLock winMsgLock;
   // special case for commands
   if (message == WM_COMMAND)
   {
   if (OnCommand(wParam, lParam))
   {
   lResult = 1;
   goto LReturnTrue;
   }
   return FALSE;
   }

   // special case for notifies
   if (message == WM_NOTIFY)
   {
   NMHDR* pNMHDR = (NMHDR*)lParam;
   if (pNMHDR->oswindowFrom != ::null() && OnNotify(wParam, lParam, &lResult))
   goto LReturnTrue;
   return FALSE;
   }

   // special case for activation
   if (message == WM_ACTIVATE)
   __handle_activate(this, wParam, ::win::window::from_handle(lParam));

   // special case for set cursor HTERROR
   if (message == WM_SETCURSOR &&
   __handle_set_cursor(this, (int16_t)LOWORD(lParam), HIWORD(lParam)))
   {
   lResult = 1;
   goto LReturnTrue;
   }

   // special case for windows that contain windowless ActiveX controls
   bool bHandled;

   bHandled = FALSE;

   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
   UINT iHash; iHash = (LOWORD((uint_ptr)pMessageMap) ^ message) & (iHashMax-1);
   winMsgLock.lock(CRIT_WINMSGCACHE);
   __MSG_CACHE* pMsgCache; pMsgCache = &gen_MsgCache[iHash];
   const __MSGMAP_ENTRY* lpEntry;
   if (message == pMsgCache->nMsg && pMessageMap == pMsgCache->pMessageMap)
   {
   // cache hit
   lpEntry = pMsgCache->lpEntry;
   winMsgLock.unlock();
   if (lpEntry == ::null())
   return FALSE;

   // cache hit, and it needs to be handled
   if (message < 0xC000)
   goto LDispatch;
   else
   goto LDispatchRegistered;
   }
   else
   {
   // not in cache, look for it
   pMsgCache->nMsg = message;
   pMsgCache->pMessageMap = pMessageMap;

   for (/* pMessageMap already init'ed */ /*; pMessageMap->pfnGetBaseMap != ::null();
   pMessageMap = (*pMessageMap->pfnGetBaseMap)())
   {
   // Note: catch not so common but fatal mistake!!
   //       // BEGIN_MESSAGE_MAP(CMyWnd, CMyWnd)
   ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
   if (message < 0xC000)
   {
   // constant window message
   if ((lpEntry = ::ca::FindMessageEntry(pMessageMap->lpEntries,
   message, 0, 0)) != ::null())
   {
   pMsgCache->lpEntry = lpEntry;
   winMsgLock.unlock();
   goto LDispatch;
   }
   }
   else
   {
   // registered windows message
   lpEntry = pMessageMap->lpEntries;
   while ((lpEntry = ::ca::FindMessageEntry(lpEntry, 0xC000, 0, 0)) != ::null())
   {
   UINT* pnID = (UINT*)(lpEntry->nSig);
   ASSERT(*pnID >= 0xC000 || *pnID == 0);
   // must be successfully registered
   if (*pnID == message)
   {
   pMsgCache->lpEntry = lpEntry;
   winMsgLock.unlock();
   goto LDispatchRegistered;
   }
   lpEntry++;      // keep looking past this one
   }
   }
   }

   pMsgCache->lpEntry = ::null();
   winMsgLock.unlock();
   return FALSE;
   }

   LDispatch:
   ASSERT(message < 0xC000);

   mmf.pfn = lpEntry->pfn;

   switch (lpEntry->nSig)
   {
   default:
   ASSERT(FALSE);
   break;
   case ::ca::Sig_l_p:
   {
   point point(lParam);      
   lResult = (this->*mmf.pfn_l_p)(point);
   break;
   }      
   case ::ca::Sig_b_D_v:
   lResult = (this->*mmf.pfn_b_D)(::win::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
   break;

   case ::ca::Sig_b_b_v:
   lResult = (this->*mmf.pfn_b_b)(static_cast<bool>(wParam));
   break;

   case ::ca::Sig_b_u_v:
   lResult = (this->*mmf.pfn_b_u)(static_cast<UINT>(wParam));
   break;

   case ::ca::Sig_b_h_v:
   lResult = (this->*mmf.pfn_b_h)(reinterpret_cast<HANDLE>(wParam));
   break;

   case ::ca::Sig_i_u_v:
   lResult = (this->*mmf.pfn_i_u)(static_cast<UINT>(wParam));
   break;

   case ::ca::Sig_C_v_v:
   lResult = reinterpret_cast<LRESULT>((this->*mmf.pfn_C_v)());
   break;

   case ::ca::Sig_v_u_W:
   (this->*mmf.pfn_v_u_W)(static_cast<UINT>(wParam), 
   ::win::window::from_handle(reinterpret_cast<oswindow>(lParam)));
   break;

   case ::ca::Sig_u_u_v:
   lResult = (this->*mmf.pfn_u_u)(static_cast<UINT>(wParam));
   break;

   case ::ca::Sig_b_v_v:
   lResult = (this->*mmf.pfn_b_v)();
   break;

   case ::ca::Sig_b_W_uu:
   lResult = (this->*mmf.pfn_b_W_u_u)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)),
   LOWORD(lParam), HIWORD(lParam));
   break;

   case ::ca::Sig_b_W_COPYDATASTRUCT:
   lResult = (this->*mmf.pfn_b_W_COPYDATASTRUCT)(
   ::win::window::from_handle(reinterpret_cast<oswindow>(wParam)),
   reinterpret_cast<COPYDATASTRUCT*>(lParam));
   break;

   case ::ca::Sig_b_v_HELPINFO:
   lResult = (this->*mmf.pfn_b_HELPINFO)(reinterpret_cast<LPHELPINFO>(lParam));
   break;

   case ::ca::Sig_CTLCOLOR:
   {
   // special case for OnCtlColor to avoid too many temporary objects
   ASSERT(message == WM_CTLCOLOR);
   __CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lParam);
   ::ca::graphics_sp dcTemp; 
   dcTemp.set_handle1(pCtl->hDC);
   window wndTemp; 
   wndTemp.set_handle(pCtl->oswindow);
   UINT nCtlType = pCtl->nCtlType;
   // if not coming from a permanent window, use stack temporary
   sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(wndTemp.get_handle());
   if (pWnd == ::null())
   {
   pWnd = &wndTemp;
   }
   HBRUSH hbr = (this->*mmf.pfn_B_D_W_u)(&dcTemp, pWnd, nCtlType);
   // fast detach of temporary objects
   dcTemp.set_handle1(::null());
   wndTemp.set_handle(::null());
   lResult = reinterpret_cast<LRESULT>(hbr);
   }
   break;

   case ::ca::Sig_CTLCOLOR_REFLECT:
   {
   // special case for CtlColor to avoid too many temporary objects
   ASSERT(message == WM_REFLECT_BASE+WM_CTLCOLOR);
   __CTLCOLOR* pCtl = reinterpret_cast<__CTLCOLOR*>(lParam);
   ::ca::graphics_sp dcTemp; 
   dcTemp.set_handle1(pCtl->hDC);
   UINT nCtlType = pCtl->nCtlType;
   HBRUSH hbr = (this->*mmf.pfn_B_D_u)(&dcTemp, nCtlType);
   // fast detach of temporary objects
   dcTemp.set_handle1(::null());
   lResult = reinterpret_cast<LRESULT>(hbr);
   }
   break;

   case ::ca::Sig_i_u_W_u:
   lResult = (this->*mmf.pfn_i_u_W_u)(LOWORD(wParam),
   ::win::window::from_handle(reinterpret_cast<oswindow>(lParam)), HIWORD(wParam));
   break;

   case ::ca::Sig_i_uu_v:
   lResult = (this->*mmf.pfn_i_u_u)(LOWORD(wParam), HIWORD(wParam));
   break;

   case ::ca::Sig_i_W_uu:
   lResult = (this->*mmf.pfn_i_W_u_u)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)),
   LOWORD(lParam), HIWORD(lParam));
   break;

   case ::ca::Sig_i_v_s:
   lResult = (this->*mmf.pfn_i_s)(reinterpret_cast<LPTSTR>(lParam));
   break;

   case ::ca::Sig_l_w_l:
   lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);
   break;



   case ::ca::Sig_v_b_h:
   (this->*mmf.pfn_v_b_h)(static_cast<bool>(wParam), 
   reinterpret_cast<HANDLE>(lParam));
   break;

   case ::ca::Sig_v_h_v:
   (this->*mmf.pfn_v_h)(reinterpret_cast<HANDLE>(wParam));
   break;

   case ::ca::Sig_v_h_h:
   (this->*mmf.pfn_v_h_h)(reinterpret_cast<HANDLE>(wParam), 
   reinterpret_cast<HANDLE>(lParam));
   break;

   case ::ca::Sig_v_v_v:
   (this->*mmf.pfn_v_v)();
   break;

   case ::ca::Sig_v_u_v:
   (this->*mmf.pfn_v_u)(static_cast<UINT>(wParam));
   break;

   case ::ca::Sig_v_u_u:
   (this->*mmf.pfn_v_u_u)(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
   break;

   case ::ca::Sig_v_uu_v:
   (this->*mmf.pfn_v_u_u)(LOWORD(wParam), HIWORD(wParam));
   break;

   case ::ca::Sig_v_v_ii:
   (this->*mmf.pfn_v_i_i)(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
   break;

   case ::ca::Sig_v_u_uu:
   (this->*mmf.pfn_v_u_u_u)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
   break;

   case ::ca::Sig_v_u_ii:
   (this->*mmf.pfn_v_u_i_i)(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
   break;

   case ::ca::Sig_v_w_l:
   (this->*mmf.pfn_v_w_l)(wParam, lParam);
   break;

   case ::ca::Sig_MDIACTIVATE:
   (this->*mmf.pfn_v_b_W_W)(get_handle() == reinterpret_cast<oswindow>(lParam),
   ::win::window::from_handle(reinterpret_cast<oswindow>(lParam)),
   ::win::window::from_handle(reinterpret_cast<oswindow>(wParam)));
   break;

   case ::ca::Sig_v_D_v:
   (this->*mmf.pfn_v_D)(::win::graphics::from_handle(reinterpret_cast<HDC>(wParam)));
   break;


   case ::ca::Sig_v_W_v:
   (this->*mmf.pfn_v_W)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)));
   break;

   case ::ca::Sig_v_v_W:
   (this->*mmf.pfn_v_W)(::win::window::from_handle(reinterpret_cast<oswindow>(lParam)));
   break;

   case ::ca::Sig_v_W_uu:
   (this->*mmf.pfn_v_W_u_u)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)), LOWORD(lParam),
   HIWORD(lParam));
   break;

   case ::ca::Sig_v_W_p:
   {
   point point(lParam);
   (this->*mmf.pfn_v_W_p)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)), point);
   }
   break;

   case ::ca::Sig_v_W_h:
   (this->*mmf.pfn_v_W_h)(::win::window::from_handle(reinterpret_cast<oswindow>(wParam)),
   reinterpret_cast<HANDLE>(lParam));
   break;

   case ::ca::Sig_ACTIVATE:
   (this->*mmf.pfn_v_u_W_b)(LOWORD(wParam),
   ::win::window::from_handle(reinterpret_cast<oswindow>(lParam)), HIWORD(wParam));
   break;

   case ::ca::Sig_SCROLL:
   case ::ca::Sig_SCROLL_REFLECT:
   {
   // special case for WM_VSCROLL and WM_HSCROLL
   ASSERT(message == WM_VSCROLL || message == WM_HSCROLL ||
   message == WM_VSCROLL+WM_REFLECT_BASE || message == WM_HSCROLL+WM_REFLECT_BASE);
   int32_t nScrollCode = (int16_t)LOWORD(wParam);
   int32_t nPos = (int16_t)HIWORD(wParam);
   if (lpEntry->nSig == ::ca::Sig_SCROLL)
   (this->*mmf.pfn_v_u_u_W)(nScrollCode, nPos,
   ::win::window::from_handle(reinterpret_cast<oswindow>(lParam)));
   else
   (this->*mmf.pfn_v_u_u)(nScrollCode, nPos);
   }
   break;

   case ::ca::Sig_v_v_s:
   (this->*mmf.pfn_v_s)(reinterpret_cast<LPTSTR>(lParam));
   break;

   case ::ca::Sig_v_u_cs:
   (this->*mmf.pfn_v_u_cs)(static_cast<UINT>(wParam), reinterpret_cast<const char *>(lParam));
   break;

   case ::ca::Sig_OWNERDRAW:
   (this->*mmf.pfn_v_i_s)(static_cast<int32_t>(wParam), reinterpret_cast<LPTSTR>(lParam));
   lResult = TRUE;
   break;

   case ::ca::Sig_i_i_s:
   lResult = (this->*mmf.pfn_i_i_s)(static_cast<int32_t>(wParam), reinterpret_cast<LPTSTR>(lParam));
   break;

   case ::ca::Sig_u_v_p:
   {
   point point(lParam);
   lResult = (this->*mmf.pfn_u_p)(point);
   }
   break;

   case ::ca::Sig_u_v_v:
   lResult = (this->*mmf.pfn_u_v)();
   break;

   case ::ca::Sig_v_b_NCCALCSIZEPARAMS:
   (this->*mmf.pfn_v_b_NCCALCSIZEPARAMS)(static_cast<bool>(wParam), 
   reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam));
   break;

   case ::ca::Sig_v_v_WINDOWPOS:
   (this->*mmf.pfn_v_v_WINDOWPOS)(reinterpret_cast<WINDOWPOS*>(lParam));
   break;

   case ::ca::Sig_v_uu_M:
   (this->*mmf.pfn_v_u_u_M)(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HMENU>(lParam));
   break;

   case ::ca::Sig_v_u_p:
   {
   point point(lParam);
   (this->*mmf.pfn_v_u_p)(static_cast<UINT>(wParam), point);
   }
   break;

   case ::ca::Sig_SIZING:
   (this->*mmf.pfn_v_u_pr)(static_cast<UINT>(wParam), reinterpret_cast<LPRECT>(lParam));
   lResult = TRUE;
   break;

   case ::ca::Sig_MOUSEWHEEL:
   lResult = (this->*mmf.pfn_b_u_s_p)(LOWORD(wParam), (int16_t)HIWORD(wParam),
   point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
   if (!lResult)
   return FALSE;
   break;
   case ::ca::Sig_l:
   lResult = (this->*mmf.pfn_l_v)();
   if (lResult != 0)
   return FALSE;
   break;
   }
   goto LReturnTrue;

   LDispatchRegistered:    // for registered windows messages
   ASSERT(message >= 0xC000);
   ASSERT(sizeof(mmf) == sizeof(mmf.pfn));
   mmf.pfn = lpEntry->pfn;
   lResult = (this->*mmf.pfn_l_w_l)(wParam, lParam);

   LReturnTrue:
   if (pResult != ::null())
   *pResult = lResult;
   return TRUE;
   }
   */


   /////////////////////////////////////////////////////////////////////////////
   // window command handling

   bool window::OnCommand(WPARAM wParam, LPARAM lParam)
      // return TRUE if command invocation was attempted
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      /*   UINT nID = LOWORD(wParam);
      oswindow oswindow_Ctrl = lParam;
      int32_t nCode = HIWORD(wParam);

      // default routing for command messages (through closure table)

      if (oswindow_Ctrl == ::null())
      {
      // zero IDs for normal commands are not allowed
      if (nID == 0)
      return FALSE;

      // make sure command has not become disabled before routing
      CTestCmdUI state;
      state.m_id = nID;
      _001OnCommand(nID, CN_UPDATE_COMMAND_UI, &state, ::null());
      if (!state.m_bEnabled)
      {
      TRACE(::ca::trace::category_AppMsg, 0, "Warning: not executing disabled command %d\n", nID);
      return TRUE;
      }

      // menu or accelerator
      nCode = CN_COMMAND;
      }
      else
      {
      // control notification
      ASSERT(nID == 0 || ::IsWindow(oswindow_Ctrl));

      if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
      return TRUE;        // locked out - ignore control notification

      // reflect notification to child window control
      if (ReflectLastMsg(oswindow_Ctrl))
      return TRUE;    // eaten by child

      // zero IDs for normal commands are not allowed
      if (nID == 0)
      return FALSE;
      }

      #ifdef DEBUG
      if (nCode < 0 && nCode != (int32_t)0x8000)
      TRACE(::ca::trace::category_AppMsg, 0, "Implementation Warning: control notification = $%X.\n",
      nCode);
      #endif

      return _001OnCommand(nID, nCode, ::null(), ::null());*/
      return FALSE;
   }

   bool window::OnNotify(WPARAM, LPARAM lParam, LRESULT* pResult)
   {
      ASSERT(pResult != ::null());
      NMHDR* pNMHDR = (NMHDR*)lParam;
      oswindow oswindow_Ctrl = pNMHDR->hwndFrom;

      // get the child ID from the window itself
      //      uint_ptr nID = __get_dialog_control_id(oswindow_Ctrl);
      //      int32_t nCode = pNMHDR->code;

      ASSERT(oswindow_Ctrl != ::null());
      ASSERT(::IsWindow(oswindow_Ctrl));

      if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
         return true;        // locked out - ignore control notification

      // reflect notification to child window control
      if (ReflectLastMsg(oswindow_Ctrl, pResult))
         return true;        // eaten by child

      //      __NOTIFY notify;
      //    notify.pResult = pResult;
      //  notify.pNMHDR = pNMHDR;
      //xxx   return _001OnCommand((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, ::null());
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   // window extensions

   sp(frame_window) window::GetParentFrame()
   {
      if (get_handle() == ::null()) // no Window attached
      {
         return ::null();
      }

      ASSERT_VALID(this);

      sp(::user::interaction) pParentWnd = get_parent();  // start with one parent up
      while (pParentWnd != ::null())
      {
         if (pParentWnd->is_frame_window())
         {
            return (pParentWnd.m_p);
         }
         pParentWnd = pParentWnd->get_parent();
      }
      return ::null();
   }

   /* trans oswindow CLASS_DECL_win __get_parent_owner(sp(::user::interaction) oswindow)
   {
   // check for permanent-owned window first
   sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(oswindow);
   if (pWnd != ::null())
   return WIN_WINDOW(pWnd)->get_owner();

   // otherwise, return parent in the Windows sense
   return (::GetWindowLong(oswindow, GWL_STYLE) & WS_CHILD) ?
   ::get_parent: ::GetWindow(oswindow, GW_OWNER);
   }*/


   sp(::user::interaction) window::GetTopLevelParent()
   {
      if (get_handle() == ::null()) // no Window attached
         return ::null();

      ASSERT_VALID(this);

      sp(::user::interaction) puiParent = this;
      sp(::user::interaction) pui;
      while ((pui = ::user::get_parent_owner(puiParent)) != ::null())
         puiParent = pui;

      return puiParent;
   }

   sp(::user::interaction) window::GetTopLevelOwner()
   {

      if (get_handle() == ::null()) // no Window attached
         return ::null();

      ASSERT_VALID(this);

      oswindow oswindow_Owner = get_handle();
      oswindow oswindow_T;
      while ((oswindow_T = ::GetWindow(oswindow_Owner, GW_OWNER)) != ::null())
         oswindow_Owner = oswindow_T;

      return (::user::interaction *) ::win::window::from_handle(oswindow_Owner).m_p;

   }

   sp(::user::interaction) window::GetParentOwner()
   {
      if (get_handle() == ::null()) // no Window attached
         return ::null();

      ASSERT_VALID(this);

      oswindow oswindow_Parent = get_handle();
      oswindow oswindow_T;
      while ((::GetWindowLong(oswindow_Parent, GWL_STYLE) & WS_CHILD) &&
         (oswindow_T = ::GetParent(oswindow_Parent)) != ::null())
      {
         oswindow_Parent = oswindow_T;
      }

      return (::user::interaction *)::win::window::from_handle(oswindow_Parent).m_p;
   }

   bool window::IsTopParentActive()
   {
      ASSERT(get_handle() != ::null());
      ASSERT_VALID(this);

      sp(::user::interaction)pWndTopLevel=EnsureTopLevelParent();

      return window::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   }

   void window::ActivateTopParent()
   {
      // special activate logic for floating toolbars and palettes
      sp(::ca::window) pActiveWnd = GetForegroundWindow();
      if (pActiveWnd == ::null() || !(WIN_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(WIN_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevelParent()->SetForegroundWindow();
      }
   }

   sp(frame_window) window::GetTopLevelFrame()
   {
      if (get_handle() == ::null()) // no Window attached
         return ::null();

      ASSERT_VALID(this);

      sp(frame_window) pFrameWnd = ::null();
      if(m_pguie != this)
         pFrameWnd = m_pguie;
      else
         pFrameWnd = (this);

      bool bNull = pFrameWnd == ::null();

      if(bNull)
      {
         pFrameWnd = m_pguie->GetParentFrame();
      }
      else
      {
         bool bFrame = pFrameWnd->is_frame_window();
         if(!bFrame)
         {
            pFrameWnd = m_pguie->GetParentFrame();
         }
      }
         
      

      if (pFrameWnd != ::null())
      {
         sp(frame_window) pTemp;
         while ((pTemp = pFrameWnd->GetParentFrame()) != ::null())
            pFrameWnd = pTemp;
      }
      return pFrameWnd;
   }

   sp(::ca::window) window::get_safe_owner(sp(::ca::window) pParent, oswindow* pWndTop)
   {
      oswindow oswindow = get_safe_owner(pParent->get_handle(), pWndTop);
      return ::win::window::from_handle(oswindow);
   }

   int32_t window::message_box(const char * lpszText, const char * lpszCaption, UINT nType)
   {
      if (lpszCaption == ::null())
         lpszCaption = __get_app_name();
      int32_t nResult = ::MessageBox(get_handle(), lpszText, lpszCaption, nType);
      return nResult;
   }

   sp(::user::interaction) window::GetDescendantWindow(sp(::user::interaction) oswindow, id id)
   {
      single_lock sl(&oswindow->m_pthread->m_pthread->m_mutex, TRUE);
      // get_child_by_id recursive (return first found)
      // breadth-first for 1 level, then depth-first for next level

      // use get_child_by_id since it is a fast USER function
      sp(::user::interaction) pWndChild;
      if ((pWndChild = oswindow->get_child_by_id(id)) != ::null())
      {
         if (pWndChild->GetTopWindow() != ::null())
         {
            // children with the same ID as their parent have priority
            pWndChild = GetDescendantWindow(pWndChild, id);
            if (pWndChild != ::null())
               return pWndChild;
         }
         if (pWndChild != ::null())
            return pWndChild;
      }

      for(int32_t i = 0; i < oswindow->m_uiptraChild.get_count(); i++)
      {
         if(oswindow->m_uiptraChild[i].GetDlgCtrlId() == id)
         {
            if(oswindow->m_uiptraChild[i].GetDescendantWindow(id))
               return oswindow->m_uiptraChild[i].GetDescendantWindow(id);
            else
               return oswindow->m_uiptraChild(i);
         }
      }

      if(pWndChild == ::null())
         return ::null();

      // walk each child
      for (pWndChild = pWndChild->GetTopWindow(); pWndChild != ::null();
         pWndChild = pWndChild->GetNextWindow(GW_HWNDNEXT))
      {
         pWndChild = GetDescendantWindow(pWndChild, id);
         if (pWndChild != ::null())
            return pWndChild;
      }
      return ::null();    // not found
   }

   void window::SendMessageToDescendants(oswindow oswindow, UINT message,
      WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm)
   {
      // walk through HWNDs to avoid creating temporary window objects
      // unless we need to call this function recursively
      for (::oswindow oswindow_Child = ::GetTopWindow(oswindow); oswindow_Child != ::null();
         oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
      {
         // if bOnlyPerm is TRUE, don't send to non-permanent windows
         if (bOnlyPerm)
         {
            sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(oswindow_Child);
            if (pWnd != ::null())
            {
               // call window proc directly since it is a C++ window
               __call_window_procedure(pWnd, WIN_WINDOW(pWnd)->get_handle(), message, wParam, lParam);
            }
         }
         else
         {
            try
            {
               // send message with Windows SendMessage API
               ::SendMessage(oswindow_Child, message, wParam, lParam);
            }
            catch(...)
            {
            }
         }
         if (bDeep && ::GetTopWindow(oswindow_Child) != ::null())
         {
            // send to child windows after parent
            try
            {
               SendMessageToDescendants(oswindow_Child, message, wParam, lParam,
                  bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for window functions
   //    only works for derived class (eg: ::view) that override 'GetScrollBarCtrl'
   // if the window doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   CScrollBar* window::GetScrollBarCtrl(int32_t) const
   {
      return ::null();        // no special scrollers supported
   }

   int32_t window::SetScrollPos(int32_t nBar, int32_t nPos, bool bRedraw)
   {
      return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
   }

   int32_t window::GetScrollPos(int32_t nBar) const
   {
      return ::GetScrollPos(((::win::window *)this)->get_handle(), nBar);
   }

   void window::SetScrollRange(int32_t nBar, int32_t nMinPos, int32_t nMaxPos, bool bRedraw)
   {
      ::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void window::GetScrollRange(int32_t nBar, LPINT lpMinPos, LPINT lpMaxPos) const
   {
      ::GetScrollRange(((::win::window *)this)->get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void window::EnableScrollBarCtrl(int32_t nBar, bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar, bEnable);
   }

   bool window::SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
   {
      ASSERT(lpScrollInfo != ::null());

      oswindow oswindow = get_handle();
      lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
      ::SetScrollInfo(oswindow, nBar, lpScrollInfo, bRedraw);
      return true;
   }

   bool window::GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
   {
      UNREFERENCED_PARAMETER(nMask);
      ASSERT(lpScrollInfo != ::null());

      oswindow oswindow = get_handle();
      return ::GetScrollInfo(oswindow, nBar, lpScrollInfo) != FALSE;
   }

   int32_t window::GetScrollLimit(int32_t nBar)
   {
      int32_t nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
      SCROLLINFO info;
      if (GetScrollInfo(nBar, &info, SIF_PAGE))
      {
         nMax -= __max(info.nPage-1,0);
      }
      return nMax;
   }

   void window::ScrollWindow(int32_t xAmount, int32_t yAmount,
      LPCRECT lpRect, LPCRECT lpClipRect)
   {
      ASSERT(::IsWindow(get_handle()));

      if (IsWindowVisible() || lpRect != ::null() || lpClipRect != ::null())
      {
         // When visible, let Windows do the scrolling
         ::ScrollWindow(get_handle(), xAmount, yAmount, lpRect, lpClipRect);
      }
      else
      {
         // Windows does not perform any scrolling if the window is
         // not visible.  This leaves child windows unscrolled.
         // To ac::count for this oversight, the child windows are moved
         // directly instead.
         oswindow oswindow_Child = ::GetWindow(get_handle(), GW_CHILD);
         if (oswindow_Child != ::null())
         {
            for (; oswindow_Child != ::null();
               oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
            {
               rect rect;
               ::GetWindowRect(oswindow_Child, &rect);
               ScreenToClient(&rect);
               ::SetWindowPos(oswindow_Child, ::null(),
                  rect.left+xAmount, rect.top+yAmount, 0, 0,
                  SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
            }
         }
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // minimal layout support

   /*
   void window::RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
   UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
   {
   ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery || 
   (nFlags & ~reposNoPosLeftOver) == reposExtra);

   // walk kids in order, control bars get the resize notification
   //   which allow them to shrink the client area
   // remaining size goes to the 'nIDLeftOver' pane
   // NOTE: nIDFirst->nIDLast are usually 0->0xffff

   __SIZEPARENTPARAMS layout;
   sp(::user::interaction) oswindow_LeftOver = ::null();

   layout.bStretch = bStretch;
   layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
   if (lpRectClient != ::null())
   layout.rect = *lpRectClient;    // starting rect comes from parameter
   else
   {
   if(m_pguie != this)
   m_pguie->GetClientRect(&layout.rect);    // starting rect comes from client rect
   else
   GetClientRect(&layout.rect);    // starting rect comes from client rect
   }

   if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
   layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
   else
   layout.hDWP = ::null(); // not actually doing layout

   if(m_pguie != this && m_pguie != ::null())
   {
   for (sp(::user::interaction) oswindow_Child = m_pguie->GetTopWindow(); oswindow_Child != ::null();
   oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pWnd = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::ca::str::begins(strIdc, pszPrefix) && pWnd != ::null())
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
   }
   for (int32_t i = 0; i < m_pguie->m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) oswindow_Child = m_pguie->m_uiptra[i];
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pWnd = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::ca::str::begins(strIdc, pszPrefix) && pWnd != ::null())
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
   }
   }
   else
   {
   for (sp(::user::interaction) oswindow_Child = GetTopWindow(); oswindow_Child != ::null();
   oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
   {
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pWnd = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::ca::str::begins(strIdc, pszPrefix) && pWnd != ::null())
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
   }
   for (int32_t i = 0; i < m_uiptra.get_count();   i++)
   {
   sp(::user::interaction) oswindow_Child = m_uiptra[i];
   string strIdc = oswindow_Child->GetDlgCtrlId();
   sp(::user::interaction) pWnd = oswindow_Child;
   if (strIdc == pszIdLeftOver)
   oswindow_LeftOver = oswindow_Child;
   else if (::ca::str::begins(strIdc, pszPrefix) && pWnd != ::null())
   oswindow_Child->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&layout);
   }
   }

   // if just getting the available rectangle, return it now...
   if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
   {
   ASSERT(lpRectParam != ::null());
   if (bStretch)
   ::copy(lpRectParam, &layout.rect);
   else
   {
   lpRectParam->left = lpRectParam->top = 0;
   lpRectParam->right = layout.sizeTotal.cx;
   lpRectParam->bottom = layout.sizeTotal.cy;
   }
   return;
   }

   // the rest is the client size of the left-over pane
   if (pszIdLeftOver != ::null() && oswindow_LeftOver != ::null())
   {
   sp(::user::interaction) pLeftOver = oswindow_LeftOver;
   // allow extra space as specified by lpRectBorder
   if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
   {
   ASSERT(lpRectParam != ::null());
   layout.rect.left += lpRectParam->left;
   layout.rect.top += lpRectParam->top;
   layout.rect.right -= lpRectParam->right;
   layout.rect.bottom -= lpRectParam->bottom;
   }
   // reposition the window
   if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
   {
   pLeftOver->CalcWindowRect(&layout.rect);
   __reposition_window(&layout, pLeftOver, &layout.rect);
   }
   }

   // move and resize all the windows at once!
   if (layout.hDWP == ::null() || !::EndDeferWindowPos(layout.hDWP))
   TRACE(::ca::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }

   */

   void window::RepositionBars(UINT nIDFirst, UINT nIDLast, id nIdLeftOver,
      UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
   {
      UNREFERENCED_PARAMETER(nIDFirst);
      UNREFERENCED_PARAMETER(nIDLast);

      ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery || 
         (nFlags & ~reposNoPosLeftOver) == reposExtra);

      // walk kids in order, control bars get the resize notification
      //   which allow them to shrink the client area
      // remaining size goes to the 'nIDLeftOver' pane
      // NOTE: nIDFirst->nIDLast are usually 0->0xffff

      __SIZEPARENTPARAMS layout;
      sp(::user::interaction) oswindow_LeftOver = ::null();

      layout.bStretch = bStretch;
      layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
      if (lpRectClient != ::null())
         layout.rect = *lpRectClient;    // starting rect comes from parameter
      else
      {
         if(m_pguie != this)
            m_pguie->GetClientRect(&layout.rect);    // starting rect comes from client rect
         else
            GetClientRect(&layout.rect);    // starting rect comes from client rect
      }

      if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
         layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
      else
         layout.hDWP = ::null(); // not actually doing layout

      if(m_pguie != this && m_pguie != ::null())
      {
         for (sp(::user::interaction) oswindow_Child = m_pguie->GetTopWindow(); oswindow_Child != ::null();
            oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
         {
            id id = oswindow_Child->GetDlgCtrlId();
            sp(::user::interaction) pWnd = oswindow_Child;
            if (id == (int32_t) nIdLeftOver)
               oswindow_LeftOver = oswindow_Child;
            else if (pWnd != ::null())
               oswindow_Child->send_message(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (sp(::user::interaction) oswindow_Child = m_pguie->get_top_child(); oswindow_Child != ::null();
            oswindow_Child = oswindow_Child->under_sibling())
         {
            id id = oswindow_Child->GetDlgCtrlId();
            sp(::user::interaction) pWnd = oswindow_Child;
            if (id == nIdLeftOver)
               oswindow_LeftOver = oswindow_Child;
            else if (pWnd != ::null())
               oswindow_Child->send_message(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }
      else
      {
         for (sp(::user::interaction) oswindow_Child = GetTopWindow(); oswindow_Child != ::null();
            oswindow_Child = oswindow_Child->GetNextWindow(GW_HWNDNEXT))
         {
            id id = oswindow_Child->GetDlgCtrlId();
            sp(::user::interaction) pWnd = oswindow_Child;
            if (id == nIdLeftOver)
               oswindow_LeftOver = oswindow_Child;
            else if (pWnd != ::null())
               oswindow_Child->send_message(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
         for (sp(::user::interaction) oswindow_Child = m_pguie->get_top_child(); oswindow_Child != ::null();
            oswindow_Child = oswindow_Child->under_sibling())
         {
            id id = oswindow_Child->GetDlgCtrlId();
            sp(::user::interaction) pWnd = oswindow_Child;
            if (id == nIdLeftOver)
               oswindow_LeftOver = oswindow_Child;
            else if (pWnd != ::null())
               oswindow_Child->send_message(WM_SIZEPARENT, 0, (LPARAM)&layout);
         }
      }

      // if just getting the available rectangle, return it now...
      if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
      {
         ASSERT(lpRectParam != ::null());
         if (bStretch)
            ::CopyRect(lpRectParam, &layout.rect);
         else
         {
            lpRectParam->left = lpRectParam->top = 0;
            lpRectParam->right = layout.sizeTotal.cx;
            lpRectParam->bottom = layout.sizeTotal.cy;
         }
         return;
      }

      // the rest is the client size of the left-over pane
      if(nIdLeftOver != ::null() && oswindow_LeftOver != ::null())
      {
         sp(::user::interaction) pLeftOver = oswindow_LeftOver;
         // allow extra space as specified by lpRectBorder
         if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
         {
            ASSERT(lpRectParam != ::null());
            layout.rect.left += lpRectParam->left;
            layout.rect.top += lpRectParam->top;
            layout.rect.right -= lpRectParam->right;
            layout.rect.bottom -= lpRectParam->bottom;
         }
         // reposition the window
         if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
         {
            pLeftOver->CalcWindowRect(&layout.rect);
            __reposition_window(&layout, pLeftOver, &layout.rect);
         }
      }

      // move and resize all the windows at once!
      if (layout.hDWP == ::null() || !::EndDeferWindowPos(layout.hDWP))
         TRACE(::ca::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
   }



   void window::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
   {
      uint32_t dwExStyle = GetExStyle();
      if (nAdjustType == 0)
         dwExStyle &= ~WS_EX_CLIENTEDGE;
      ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special keyboard/system command processing

   bool window::HandleFloatingSysCommand(UINT nID, LPARAM lParam)
   {
      sp(::user::interaction) pParent = GetTopLevelParent();
      switch (nID & 0xfff0)
      {
      case SC_PREVWINDOW:
      case SC_NEXTWINDOW:
         if (LOWORD(lParam) == VK_F6 && pParent != ::null())
         {
            pParent->SetFocus();
            return true;
         }
         break;

      case SC_CLOSE:
      case SC_KEYMENU:
         // Check lParam.  If it is 0L, then the ::fontopus::user may have done
         // an Alt+Tab, so just ignore it.  This breaks the ability to
         // just press the Alt-key and have the first menu selected,
         // but this is minor compared to what happens in the Alt+Tab
         // case.
         if ((nID & 0xfff0) == SC_CLOSE || lParam != 0L)
         {
            if (pParent != ::null())
            {
               // Sending the above WM_SYSCOMMAND may destroy the cast,
               // so we have to be careful about restoring activation
               // and focus after sending it.
               oswindow oswindow_Save = get_handle();
               oswindow oswindow_Focus = ::GetFocus();
               pParent->SetActiveWindow();
               pParent->send_message(WM_SYSCOMMAND, nID, lParam);

               // be very careful here...
               if (::IsWindow(oswindow_Save))
                  ::SetActiveWindow(oswindow_Save);
               if (::IsWindow(oswindow_Focus))
                  ::SetFocus(oswindow_Focus);
            }
         }
         return true;
      }
      return false;
   }

   void window::WalkPreTranslateTree(sp(::user::interaction) puiStop, ::ca::signal_object * pobj)
   {
      ASSERT(puiStop == ::null() || puiStop->IsWindow());
      ASSERT(pobj != ::null());

      SCAST_PTR(::ca::message::base, pbase, pobj);
      // walk from the target window up to the oswindow_Stop window checking
      //  if any window wants to translate this message

      for (sp(::user::interaction) pui = pbase->m_pwnd; pui != ::null(); pui->get_parent())
      {

         pui->pre_translate_message(pobj);

         if(pobj->m_bRet)
            return; // trapped by target window (eg: accelerators)

         // got to oswindow_Stop window without interest
         if(pui == puiStop)
            break;

      }
      // no special processing
   }

   bool window::SendChildNotifyLastMsg(LRESULT* pResult)
   {
      ___THREAD_STATE* pThreadState = __get_thread_state();
      return OnChildNotify(pThreadState->m_lastSentMsg.message,
         pThreadState->m_lastSentMsg.wParam, pThreadState->m_lastSentMsg.lParam, pResult);
   }

   bool window::ReflectLastMsg(oswindow oswindow_Child, LRESULT* pResult)
   {
      // get the map, and if no map, then this message does not need reflection
      single_lock sl(afxMutexoswindow_(), TRUE);
      oswindow_map * pMap = get_oswindow_map();
      if (pMap == ::null())
         return FALSE;

      // check if in permanent map, if it is reflect it (could be OLE control)
      sp(::ca::window) pWnd = (pMap->lookup_permanent(oswindow_Child));
      ASSERT(pWnd == ::null() || WIN_WINDOW(pWnd)->get_handle() == oswindow_Child);
      if (pWnd == ::null())
      {
         return FALSE;
      }

      // only OLE controls and permanent windows will get reflected msgs
      ASSERT(pWnd != ::null());
      return WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
   }

   bool window::OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {

      return ReflectChildNotify(uMsg, wParam, lParam, pResult);
   }

   bool window::ReflectChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wParam);
      // Note: reflected messages are send directly to window::OnWndMsg
      //  and window::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch (uMsg)
      {
         // normal messages (just wParam, lParam through OnWndMsg)
      case WM_HSCROLL:
      case WM_VSCROLL:
      case WM_PARENTNOTIFY:
      case WM_DRAWITEM:
      case WM_MEASUREITEM:
      case WM_DELETEITEM:
      case WM_VKEYTOITEM:
      case WM_CHARTOITEM:
      case WM_COMPAREITEM:
         // reflect the message through the message map as WM_REFLECT_BASE+uMsg
         //return window::OnWndMsg(WM_REFLECT_BASE+uMsg, wParam, lParam, pResult);
         return FALSE;

         // special case for WM_COMMAND
      case WM_COMMAND:
         {
            // reflect the message through the message map as OCM_COMMAND
            /* xxx         int32_t nCode = HIWORD(wParam);
            if (window::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), ::null(), ::null()))
            {
            if (pResult != ::null())
            *pResult = 1;
            return TRUE;
            } */
         }
         break;

         // special case for WM_NOTIFY
      case WM_NOTIFY:
         {
            // reflect the message through the message map as OCM_NOTIFY
            NMHDR* pNMHDR = (NMHDR*)lParam;
            //            int32_t nCode = pNMHDR->code;
            //            __NOTIFY notify;
            //          notify.pResult = pResult;
            //        notify.pNMHDR = pNMHDR;
            // xxxx         return window::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, ::null());
         }

         // other special cases (WM_CTLCOLOR family)
      default:
         if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*__CTLCOLOR ctl;
            ctl.hDC = (HDC)wParam;
            ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
            //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
            ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

            // reflect the message through the message map as OCM_CTLCOLOR
            bool bResult = window::OnWndMsg(WM_REFLECT_BASE+WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
            if ((HBRUSH)*pResult == ::null())
            bResult = FALSE;
            return bResult;*/
            return false;
         }
         break;
      }

      return false;   // let the parent handle it
   }

   void window::OnParentNotify(UINT message, LPARAM lParam)
   {
      if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         if (ReflectLastMsg((oswindow) lParam))
            return;     // eat it
      }
      // not handled - do default
      Default();
   }

   void window::OnSetFocus(sp(::ca::window))
   { 
      bool bHandled;

      bHandled = FALSE;
      if( !bHandled )
      {
         Default();
      }
   }

   LRESULT window::OnActivateTopLevel(WPARAM wParam, LPARAM)
   {
      if (LOWORD(wParam) == WA_INACTIVE)
      {
         //         __MODULE_THREAD_STATE* pModuleThreadState = __get_module_thread_state();
      }

      return 0;
   }

   void window::OnSysColorChange()
   {
      throw not_implemented(get_app());

      /*      application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this)
      {
      // recolor global brushes used by control bars
      afxData.UpdateSysColors();
      }

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, TRUE);

      Default();*/
   }

   bool gen_GotScrollLines;

   void window::OnSettingChange(UINT uFlags, const char * lpszSection)
   {
      UNUSED_ALWAYS(uFlags);
      UNUSED_ALWAYS(lpszSection);

      // force refresh of settings that we cache
      gen_GotScrollLines = FALSE;


      window::OnDisplayChange(0, 0);    // to update system metrics, etc.
   }

   void window::OnDevModeChange(__in LPTSTR lpDeviceName)
   {
      UNREFERENCED_PARAMETER(lpDeviceName);
      throw not_implemented(get_app());
      /*application* pApp = &System;
      if (pApp != ::null() && pApp->GetMainWnd() == this)
      pApp->DevModeChange(lpDeviceName);

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
      const MSG* pMsg = GetCurrentMessage();
      SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
      TRUE, TRUE);
      }*/
   }

   bool window::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
   {
      if (!(GetStyle() & WS_CHILD))
      {
         sp(::user::interaction) pMainWnd = System.GetMainWnd();
         if (pMainWnd != ::null() &&
            GetKeyState(VK_SHIFT) >= 0 &&
            GetKeyState(VK_CONTROL) >= 0 &&
            GetKeyState(VK_MENU) >= 0)
         {
            //            pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
            return TRUE;
         }
      }
      return Default() != 0;
   }

   LRESULT window::OnDisplayChange(WPARAM, LPARAM)
   {
      // update metrics if this window is the main window
      if (System.GetMainWnd() == this)
      {
         // update any system metrics cache
         //         afxData.UpdateSysMetrics();
      }

      // forward this message to all other child windows
      if (!(GetStyle() & WS_CHILD))
      {
         const MSG* pMsg = GetCurrentMessage();
         SendMessageToDescendants(pMsg->message, pMsg->wParam, pMsg->lParam,
            TRUE, TRUE);
      }

      return Default();
   }

   LRESULT window::OnDragList(WPARAM, LPARAM lParam)
   {
      LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lParam;
      ASSERT(lpInfo != ::null());

      LRESULT lResult;
      if (ReflectLastMsg(lpInfo->hWnd, &lResult))
         return (int32_t)lResult;    // eat it

      // not handled - do default
      return (int32_t)Default();
   }

   void window::_001OnCreate(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Default();
   }

   void window::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
      UNREFERENCED_PARAMETER(pScrollBar);
      Default();
   }

   void window::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
   {
      UNREFERENCED_PARAMETER(pScrollBar);
      Default();
   }

   BOOL CALLBACK window::GetAppsEnumWindowsProc(oswindow oswindow, LPARAM lParam)
   {

      user::oswindow_array * poswindowa = (user::oswindow_array *) lParam;

      poswindowa->add(oswindow);

      return TRUE;

   }

   void window::get_app_wnda(user::oswindow_array & wnda)
   {
      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) &wnda);
   }

   /*   void window::_001OnDeferPaintLayeredWindowBackground(::ca::graphics * pdc)
   {
   _001DeferPaintLayeredWindowBackground(pdc);
   }*/


   class print_window :
      virtual ::ca::object
   {
   public:



      manual_reset_event m_event;
      oswindow m_oswindow;
      HDC m_hdc;

      print_window(sp(::ca::application) papp, oswindow oswindow, HDC hdc, uint32_t dwTimeout) :
         ca(papp),
         m_event(papp)
      {
         m_event.ResetEvent();
         m_oswindow = oswindow;
         m_hdc = hdc;
         __begin_thread(papp, &print_window::s_print_window, (LPVOID) this, ::ca::thread_priority_above_normal);
         if(m_event.wait(millis(dwTimeout)).timeout())
         {
            TRACE("print_window::time_out");
         }
      }


      static UINT c_cdecl s_print_window(LPVOID pvoid)
      {

         print_window * pprintwindow = (print_window *) pvoid;

         try
         {

            HANDLE hevent = (HANDLE) pprintwindow->m_event.get_os_data();

            ::PrintWindow(pprintwindow->m_oswindow, pprintwindow->m_hdc, 0);

            ::SetEvent(hevent);

         }
         catch(...)
         {

         }

         return 0;

      }

   };

   void window::_001DeferPaintLayeredWindowBackground(HDC hdc)
   {

      rect rectClient;

      GetClientRect(rectClient);


      //pdc->FillSolidRect(rectClient, 0x00000000);

      //return;
      rect rectUpdate;
      GetWindowRect(rectUpdate);
      SetViewportOrgEx(hdc, 0, 0, ::null());
      rect rectPaint;
      rectPaint = rectUpdate;
      ScreenToClient(rectPaint);
      user::oswindow_array wndaApp;


      HRGN rgnWindow;
      HRGN rgnIntersect;
      HRGN rgnUpdate = ::null();


      rgnWindow = CreateRectRgn(0, 0, 0, 0);
      rgnIntersect = CreateRectRgn(0, 0, 0, 0);

      //      int32_t iCount = wndaApp.get_count();

      try
      {

         if(GetWindowLong(GWL_EXSTYLE) & WS_EX_LAYERED)
         {
            rect rect5;
            rect rect9;

            rgnUpdate = ::CreateRectRgnIndirect(&rectUpdate);
            oswindow oswindowOrder = ::GetWindow(get_handle(), GW_HWNDNEXT);
            for(;;)
            {
               //            char szText[1024];
               //::GetWindowTextA(oswindowOrder, szText, sizeof(szText));
               if(oswindowOrder == ::null() || !::IsWindow(oswindowOrder))
                  break;

               if(!::IsWindowVisible(oswindowOrder) || ::IsIconic(oswindowOrder) || oswindowOrder == get_handle() || wndaApp.contains(oswindowOrder))
               {

                  if(oswindowOrder == get_handle())
                  {

                     // add as bookmark - doesn't paint it
                     wndaApp.add(oswindowOrder);

                  }

               }
               else
               {

                  rect rectWindow;

                  ::GetWindowRect(oswindowOrder, rectWindow);

                  SetRectRgn(rgnWindow, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);

                  SetRectRgn(rgnIntersect, 0, 0, 0, 0);

                  CombineRgn(rgnIntersect, rgnUpdate, rgnWindow, RGN_AND);

                  rect rectIntersectBox;

                  GetRgnBox(rgnIntersect, rectIntersectBox);

                  if(rectIntersectBox.is_empty())
                  {

                  }
                  else
                  {

                     CombineRgn(rgnUpdate, rgnUpdate, rgnWindow, ::ca::region::combine_exclude);

                     rect rectDiffBox;

                     GetRgnBox(rgnUpdate, rectDiffBox);

                     wndaApp.add(oswindowOrder);

                     if(rectDiffBox.is_empty())
                     {

                        break;

                     }

                  }

               }

               oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);

            }

            for(index j = wndaApp.get_upper_bound(); j >= 0; j--)
            {

               oswindow oswindow = wndaApp[j];

               if(oswindow == get_handle())
                  break;

               if(!::IsWindowVisible(oswindow) || ::IsIconic(oswindow))
                  continue;

               ::GetWindowRect(oswindow, rect5);

               rect9.intersect(rect5, rectUpdate);

               if(rect9.width() > 0 && rect9.height() > 0)
               {

                  /*sp(::ca::window) pwnd = (window::FromHandlePermanent);

                  if(pwnd == ::null())
                  {

                  for(int32_t l = 0; l < wndpa.get_count(); l++)
                  {
                  if(wndpa[l]->get_safe_handle() == oswindow)
                  {
                  pwnd = (wndpa[l]->m_pimpl);
                  break;
                  }
                  }
                  }
                  if(pwnd != ::null())
                  {
                  pwnd->_001Print(pdc);
                  }*/

                  //if(::GetWindowLong(wndaApp[j], GWL_EXSTYLE) & WS_EX_LAYERED)

                  if(true)
                  {

                     HDC hDCMem = CreateCompatibleDC(::null());

                     HBITMAP hBmp = ::null();

                     {

                        HDC hDC = ::GetWindowDC(oswindow);

                        hBmp = CreateCompatibleBitmap(hDC, rect5.width(), rect5.height());

                        ::ReleaseDC(oswindow, hDC);

                     }

                     HGDIOBJ hOld = SelectObject(hDCMem, hBmp);

                     //print_window printwindow(get_app(), oswindow, hDCMem, 284);

                     ::PrintWindow(oswindow, hDCMem, 0);

                     ::BitBlt(
                        hdc ,
                        //rect5.left,
                        //rect5.top,
                        0, 0,
                        rect5.width(), rect5.height(),
                        hDCMem,
                        rectUpdate.left - rect5.left, 
                        rectUpdate.top - rect5.top, 
                        SRCCOPY);

                     ::SelectObject(hDCMem, hOld);

                     ::DeleteObject(hDCMem);

                     ::DeleteObject(hBmp);

                  }
                  else
                  {

                     SetViewportOrgEx(hdc, 0, 0, ::null());

                     HDC hdcWindow = ::GetDCEx(wndaApp[j], ::null(), DCX_WINDOW);

                     if(hdcWindow == ::null())
                        hdcWindow = ::GetDCEx(wndaApp[j], ::null(), DCX_WINDOW | DCX_CACHE);

                     if(hdcWindow != ::null())
                     {

                        ::BitBlt(
                           hdc,
                           rect5.left - rectUpdate.left,
                           rect5.top - rectUpdate.top,
                           rect5.width(), rect5.height(),
                           hdcWindow,
                           rect5.left - rect5.left,
                           rect5.top - rect5.top,
                           SRCCOPY);

                        ::ReleaseDC(wndaApp[j], hdcWindow);

                     }
                     else
                     {

                        TRACE0("WARNING: failed to draw a background. this surface probably will be black.");

                     }

                  }

               }

            }

         }

      }
      catch(...)
      {

      }

      ::DeleteObject(rgnWindow);
      ::DeleteObject(rgnIntersect);
      ::DeleteObject(rgnUpdate);
   }

   void window::_001OnProdevianSynch(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      System.get_event(m_pthread)->SetEvent();
      //    System.get_event(System.get_twf())->wait(millis(8400));
   }

   void window::_001OnPaint(::ca::signal_object * pobj)
   {

      //lock lock(m_pguie, 1984);

      SCAST_PTR(::ca::message::base, pbase, pobj);

      PAINTSTRUCT paint;
      memset(&paint, 0, sizeof(paint));
      HDC hdc = ::BeginPaint(get_handle(), &paint);
      ::SelectClipRgn(hdc, ::null());

      try
      {

         ::ca::dib_sp dib(allocer());

         rect rectWindow;
         GetWindowRect(rectWindow);

         if(!dib->create(rectWindow.bottom_right()))
            return;

         ::ca::graphics * pdc = dib->get_graphics();

         if((dynamic_cast<::win::graphics * >(pdc))->get_handle() == ::null() 
            || (dynamic_cast<::win::graphics * >(pdc))->get_handle2() == ::null())
            return;

         rect rectPaint;
         rect rectUpdate;
         rectPaint = paint.rcPaint;
         if(rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
         {
            rectUpdate = rectWindow;
            rectPaint = rectWindow;
            ScreenToClient(rectPaint);
         }
         else
         {
            rectUpdate = rectPaint;
            ClientToScreen(rectUpdate);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         if(m_pguie != ::null() && m_pguie != this)
         {
            m_pguie->_001OnDeferPaintLayeredWindowBackground(pdc);
         }
         else
         {
            _001OnDeferPaintLayeredWindowBackground(pdc);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         _000OnDraw(pdc);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pdc))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         BitBlt(hdc, rectPaint.left, rectPaint.top, 
            rectPaint.width(), rectPaint.height(),
            (HDC) pdc->get_os_data(), rectUpdate.left, rectUpdate.top,
            SRCCOPY);

      }
      catch(...)
      {
      }

      ::EndPaint(get_handle(), &paint);
      pobj->m_bRet = true;
      pbase->set_lresult(0);
   }


   void window::_001OnPrint(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);

      if(pbase->m_wparam == ::null())
         return;

      ::ca::graphics_sp graphics(allocer());
      WIN_DC(graphics.m_p)->attach((HDC) pbase->m_wparam);
      rect rectx;
      ::ca::bitmap * pbitmap = &graphics->GetCurrentBitmap();
      ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);
      //      uint32_t dw = ::GetLastError();
      class size size = pbitmap->get_size();
      rectx.left = 0;
      rectx.top = 0;
      rectx.right = size.cx;
      rectx.bottom = size.cy;
      try
      {
         rect rectWindow;
         GetWindowRect(rectWindow);

         ::ca::dib_sp dib(allocer());
         if(!dib->create(rectWindow.bottom_right()))
            return;

         ::ca::graphics * pdc = dib->get_graphics();

         if(pdc->get_os_data() == ::null())
            return;

         rect rectPaint;
         rect rectUpdate;
         rectUpdate = rectWindow;
         rectPaint = rectWindow;
         rectPaint.offset(-rectPaint.top_left());
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         if(m_pguie != ::null() && m_pguie != this)
         {
            m_pguie->_001OnDeferPaintLayeredWindowBackground(pdc);
         }
         else
         {
            _001OnDeferPaintLayeredWindowBackground(pdc);
         }
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         _000OnDraw(pdc);
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pdc))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         (dynamic_cast<::win::graphics * >(pdc))->SelectClipRgn(::null());
         (dynamic_cast<::win::graphics * >(pdc))->SetViewportOrg(point(0, 0));

         graphics->SelectClipRgn( ::null());
         graphics->BitBlt(rectPaint.left, rectPaint.top, 
            rectPaint.width(), rectPaint.height(),
            pdc, rectUpdate.left, rectUpdate.top,
            SRCCOPY);

         graphics->TextOut(0, 0, "Te Amo CGCL", 11);
      }
      catch(...)
      {
      }
      graphics->FillSolidRect(rectx, RGB(255, 255, 255));
      WIN_DC(graphics.m_p)->detach();
      pobj->m_bRet = true;
      pbase->set_lresult(0);
   }


   void window::OnEnterIdle(UINT /*nWhy*/, sp(::ca::window) /*pWho*/)
   {
      // In some OLE inplace active scenarios, OLE will post a
      // message instead of sending it.  This causes so many WM_ENTERIDLE
      // messages to be sent that tasks running in the background stop
      // running.  By dispatching the pending WM_ENTERIDLE messages
      // when the first one is received, we trick Windows into thinking
      // that only one was really sent and dispatched.
      {
         MSG msg;
         while (PeekMessage(&msg, ::null(), WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
            DispatchMessage(&msg);
      }

      Default();
   }

   HBRUSH window::OnCtlColor(::ca::graphics *, sp(::ca::window) pWnd, UINT)
   {
      ASSERT(pWnd != ::null() && WIN_WINDOW(pWnd)->get_handle() != ::null());
      LRESULT lResult;
      if (WIN_WINDOW(pWnd)->SendChildNotifyLastMsg(&lResult))
         return (HBRUSH)lResult;     // eat it
      return (HBRUSH)Default();
   }

   // implementation of OnCtlColor for default gray backgrounds
   //   (works for any window containing controls)
   //  return value of FALSE means caller must call DefWindowProc's default
   //  TRUE means that 'hbrGray' will be used and the appropriate text
   //    ('clrText') and background colors are set.
   bool window::GrayCtlColor(HDC hDC, oswindow oswindow, UINT nCtlColor, HBRUSH hbrGray, COLORREF clrText)
   {
      if (hDC == ::null())
      {
         // sometimes Win32 passes a ::null() hDC in the WM_CTLCOLOR message.
         //         TRACE(::ca::trace::category_AppMsg, 0, "Warning: hDC is ::null() in window::GrayCtlColor; WM_CTLCOLOR not processed.\n");
         return FALSE;
      }

      if (hbrGray == ::null() ||
         nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
         nCtlColor == CTLCOLOR_SCROLLBAR)
      {
         return FALSE;
      }

      if (nCtlColor == CTLCOLOR_LISTBOX)
      {
         // only handle requests to draw the space between edit and drop button
         //  in a drop-down combo (not a drop-down list)
         if (!__is_combo_box_control(oswindow, (UINT)CBS_DROPDOWN))
            return FALSE;
      }

      // set background color and return handle to brush
      LOGBRUSH logbrush;
      VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
      ::SetBkColor(hDC, logbrush.lbColor);
      if (clrText == (COLORREF)-1)
         clrText = Session.get_default_color(COLOR_WINDOWTEXT);  // normal text
      ::SetTextColor(hDC, clrText);
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // 'dialog data' support

   /*bool window::update_data(bool bSaveAndValidate)
   {
   ASSERT(::IsWindow(get_handle())); // calling update_data before DoModal?

   CDataExchange dx(this, bSaveAndValidate);

   // prevent control notifications from being dispatched during update_data
   ___THREAD_STATE* pThreadState = __get_thread_state();
   oswindow oswindow_OldLockout = pThreadState->m_hLockoutNotifyWindow;
   ASSERT(oswindow_OldLockout != get_handle());   // must not recurse
   pThreadState->m_hLockoutNotifyWindow = get_handle();

   bool bOK = FALSE;       // assume failure
   try
   {
   do_data_exchange(&dx);
   bOK = TRUE;         // it worked
   }
   catch(user_exception * pe)
   {
   // validation failed - ::fontopus::user already alerted, fall through
   ASSERT(!bOK);                                 
   // Note: DELETE_EXCEPTION_(e) not required
   }
   catch(base_exception * pe)
   {
   // validation failed due to OOM or other resource failure
   //e->ReportError(MB_ICONEXCLAMATION, __IDP_INTERNAL_FAILURE);
   pe->ReportError(MB_ICONEXCLAMATION, "falha interna");
   ASSERT(!bOK);
   pe->Delete();
   }


   pThreadState->m_hLockoutNotifyWindow = oswindow_OldLockout;
   return bOK;
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Centering dialog support (works for any non-child window)

   void window::CenterWindow(sp(::user::interaction) pAlternateOwner)
   {
      ASSERT(::IsWindow(get_handle()));

      // determine owner window to center against
      uint32_t dwStyle = GetStyle();
      sp(::user::interaction) oswindow_Center = pAlternateOwner;
      if (pAlternateOwner == ::null())
      {
         if (dwStyle & WS_CHILD)
            oswindow_Center = get_parent();
         else
            oswindow_Center = GetWindow(GW_OWNER);
         if (oswindow_Center != ::null())
         {
            // let parent determine alternate center window
            sp(::user::interaction) oswindow_Temp =
               (sp(::user::interaction) )oswindow_Center->send_message(WM_QUERYCENTERWND, 0, 0);
            if (oswindow_Temp != ::null())
               oswindow_Center = oswindow_Temp;
         }
      }

      // get coordinates of the window relative to its parent
      rect rcDlg;
      GetWindowRect(&rcDlg);
      rect rcArea;
      rect rcCenter;
      sp(::user::interaction) oswindow_Parent;
      if (!(dwStyle & WS_CHILD))
      {
         // don't center against invisible or minimized windows
         if (oswindow_Center != ::null())
         {
            uint32_t dwAlternateStyle = oswindow_Center->GetWindowLong(GWL_STYLE);
            if (!(dwAlternateStyle & WS_VISIBLE) || (dwAlternateStyle & WS_MINIMIZE))
               oswindow_Center = ::null();
         }

         MONITORINFO mi;
         mi.cbSize = sizeof(mi);

         // center within appropriate monitor coordinates
         if (oswindow_Center == ::null())
         {
            oswindow hwDefault = System.GetMainWnd()->get_handle();

            :: GetMonitorInfo(::MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
            rcCenter = mi.rcWork;
            rcArea = mi.rcWork;
         }
         else
         {
            oswindow_Center->GetWindowRect(&rcCenter);
            ::GetMonitorInfo(::MonitorFromWindow(oswindow_Center->get_handle(), MONITOR_DEFAULTTONEAREST), &mi);
            rcArea = mi.rcWork;
         }
      }
      else
      {
         // center within parent client coordinates
         oswindow_Parent = get_parent();
         ASSERT(oswindow_Parent->IsWindow());

         oswindow_Parent->GetClientRect(&rcArea);
         ASSERT(oswindow_Center->IsWindow());
         oswindow_Center->GetClientRect(&rcCenter);
         ::MapWindowPoints(oswindow_Center->get_handle(), oswindow_Parent->get_handle(), (POINT*)&rcCenter, 2);
      }

      // find dialog's upper left based on rcCenter
      int32_t xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.width() / 2;
      int32_t yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.height() / 2;

      // if the dialog is outside the screen, move it inside
      if (xLeft < rcArea.left)
         xLeft = rcArea.left;
      else if (xLeft + rcDlg.width() > rcArea.right)
         xLeft = rcArea.right - rcDlg.width();

      if (yTop < rcArea.top)
         yTop = rcArea.top;
      else if (yTop + rcDlg.height() > rcArea.bottom)
         yTop = rcArea.bottom - rcDlg.height();

      // map screen coordinates to child coordinates
      SetWindowPos(0, xLeft, yTop, -1, -1,
         SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
   }

   bool window::CheckAutoCenter()
   {
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Dialog initialization support

   bool window::ExecuteDlgInit(const char * lpszResourceName)
   {
      // find resource handle
      LPVOID lpResource = ::null();
      HGLOBAL hResource = ::null();
      if (lpszResourceName != ::null())
      {
         //         HINSTANCE hInst = ::ca::FindResourceHandle(lpszResourceName, RT_DLGINIT);
         //       HRSRC hDlgInit = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
         /*     if (hDlgInit != ::null())
         {
         // load it
         hResource = LoadResource(hInst, hDlgInit);
         if (hResource == ::null())
         return FALSE;
         // lock it
         lpResource = LockResource(hResource);
         ASSERT(lpResource != ::null());
         }*/
      }

      // execute it
      bool bResult = ExecuteDlgInit(lpResource);

      // cleanup
      if (lpResource != ::null() && hResource != ::null())
      {
         UnlockResource(hResource);
         FreeResource(hResource);
      }
      return bResult;
   }

   bool window::ExecuteDlgInit(LPVOID lpResource)
   {
      bool bSuccess = TRUE;
      if (lpResource != ::null())
      {
         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
         while (bSuccess && *lpnRes != 0)   
         {
            WORD nIDC = *lpnRes++;
            WORD nMsg = *lpnRes++;
            DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;

            // In Win32 the WM_ messages have changed.  They have
            // to be translated from the 32-bit values to 16-bit
            // values here.

#define WIN16_LB_ADDSTRING  0x0401
#define WIN16_CB_ADDSTRING  0x0403
#define __CB_ADDSTRING   0x1234

            // unfortunately, WIN16_CB_ADDSTRING == CBEM_INSERTITEM
            if (nMsg == __CB_ADDSTRING)
               nMsg = CBEM_INSERTITEM;
            else if (nMsg == WIN16_LB_ADDSTRING)
               nMsg = LB_ADDSTRING;
            else if (nMsg == WIN16_CB_ADDSTRING)
               nMsg = CB_ADDSTRING;

            // check for invalid/unknown message types
            ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING ||
               nMsg == CBEM_INSERTITEM);

#ifdef DEBUG
            // For AddStrings, the ::count must exactly delimit the
            // string, including the ::null() termination.  This check
            // will not catch all mal-formed ADDSTRINGs, but will
            // catch some.
            if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING || nMsg == CBEM_INSERTITEM)
               ASSERT(*((LPBYTE)lpnRes + (UINT)dwLen - 1) == 0);
#endif

            if (nMsg == CBEM_INSERTITEM)
            {
               COMBOBOXEXITEM item = {0};
               item.mask = CBEIF_TEXT;
               item.iItem = -1;
               string strText(reinterpret_cast<LPTSTR>(lpnRes));            
               item.pszText = const_cast<LPTSTR>(strText.GetString());
               if (::SendDlgItemMessage(get_handle(), nIDC, nMsg, 0, (LPARAM) &item) == -1)
                  bSuccess = FALSE;
            }
            {
               // List/Combobox returns -1 for error
               if (::SendDlgItemMessageA(get_handle(), nIDC, nMsg, 0, (LPARAM) lpnRes) == -1)
                  bSuccess = FALSE;
            }


            // skip past data
            lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
         }
      }

      // send update message to all controls after all other siblings loaded
      if (bSuccess)
         SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);

      return bSuccess;
   }

   void window::UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);
      cmd_ui state(get_app());
      window wndTemp;       // very temporary window just for CmdUI update

      // walk all the kids - assume the IDs are for buttons
      /* xxx   for (oswindow oswindow_Child = ::GetTopWindow(get_handle()); oswindow_Child != ::null();
      oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
      {
      // send to buttons
      wndTemp.set_handle(oswindow_Child); // quick and dirty attach
      state.m_nID = __get_dialog_control_id(oswindow_Child);
      state.m_pOther = &wndTemp;

      // check for reflect handlers in the child window
      sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(oswindow_Child);
      if (pWnd != ::null())
      {
      // call it directly to disable any routing
      if (WIN_WINDOW(pWnd)->window::_001OnCommand(0, MAKELONG(0xffff,
      WM_COMMAND+WM_REFLECT_BASE), &state, ::null()))
      continue;
      }

      // check for handlers in the parent window
      if (window::_001OnCommand((UINT)state.m_nID, CN_UPDATE_COMMAND_UI, &state, ::null()))
      continue;

      // determine whether to disable when no handler exists
      bool bDisableTemp = bDisableIfNoHndler;
      if (bDisableTemp)
      {
      if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
      {
      // non-button controls don't get automagically disabled
      bDisableTemp = FALSE;
      }
      else
      {
      // only certain button controls get automagically disabled
      UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
      if (nStyle == (UINT)BS_AUTOCHECKBOX ||
      nStyle == (UINT)BS_AUTO3STATE ||
      nStyle == (UINT)BS_GROUPBOX ||
      nStyle == (UINT)BS_AUTORADIOBUTTON)
      {
      bDisableTemp = FALSE;
      }
      }
      }
      // check for handlers in the target (owner)
      state.DoUpdate(pTarget, bDisableTemp);
      }
      wndTemp.set_handle(::null());      // quick and dirty detach */
   }


   id window::RunModalLoop(uint32_t dwFlags, ::ca::live_object * pliveobject)
   {
      // for tracking the idle time state
      bool bIdle = TRUE;
      LONG lIdleCount = 0;
      bool bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
      oswindow oswindow_Parent = ::GetParent(get_handle());
      m_iModal = m_iModalCount;
      int32_t iLevel = m_iModal;
      oprop(string("RunModalLoop.thread(") + ::ca::str::from(iLevel) + ")") = System.GetThread();
      m_iModalCount++;

      m_iaModalThread.add(::GetCurrentThreadId());
      sp(::ca::application) pappThis1 = (m_pthread->m_pthread->m_p);
      sp(::ca::application) pappThis2 = (m_pthread->m_pthread);
      // acquire and dispatch messages until the modal state is done
      MSG msg;
      for (;;)
      {
         ASSERT(ContinueModal(iLevel));

         // phase1: check to see if we can do idle work
         while (bIdle && !::PeekMessage(&msg, ::null(), 0,0, PM_NOREMOVE))
         {
            ASSERT(ContinueModal(iLevel));

            // show the dialog when the message queue goes idle
            if (bShowIdle)
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            // call on_idle while in bIdle state
            if (!(dwFlags & MLF_NOIDLEMSG) && oswindow_Parent != ::null() && lIdleCount == 0)
            {
               // send WM_ENTERIDLE to the parent
               ::SendMessage(oswindow_Parent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)get_handle());
            }
            if ((dwFlags & MLF_NOKICKIDLE) ||
               !__call_window_procedure(this, get_handle(), WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
            {
               // stop idle processing next time
               bIdle = FALSE;
            }

            m_pthread->m_pthread->m_p->m_dwAlive = m_pthread->m_pthread->m_dwAlive = ::get_tick_count();
            if(pappThis1 != ::null())
            {
               pappThis1->m_dwAlive = m_pthread->m_pthread->m_dwAlive;
            }
            if(pappThis2 != ::null())
            {
               pappThis2->m_dwAlive = m_pthread->m_pthread->m_dwAlive;
            }
            if(pliveobject != ::null())
            {
               pliveobject->keep_alive();
            }
         }


         // phase2: pump messages while available
         do
         {
            if (!ContinueModal(iLevel))
               goto ExitModal;

            // pump message, but quit on WM_QUIT
            if (!m_pthread->m_pthread->pump_message())
            {
               __post_quit_message(0);
               return -1;
            }

            // show the window when certain special messages rec'd
            if (bShowIdle &&
               (msg.message == 0x118 || msg.message == WM_SYSKEYDOWN))
            {
               ShowWindow(SW_SHOWNORMAL);
               UpdateWindow();
               bShowIdle = FALSE;
            }

            if (!ContinueModal(iLevel))
               goto ExitModal;

            // reset "no idle" state after pumping "normal" message
            if (__is_idle_message(&msg))
            {
               bIdle = TRUE;
               lIdleCount = 0;
            }

            m_pthread->m_pthread->m_p->m_dwAlive = m_pthread->m_pthread->m_dwAlive = ::get_tick_count();
            if(pappThis1 != ::null())
            {
               pappThis1->m_dwAlive = m_pthread->m_pthread->m_dwAlive;
            }
            if(pappThis2 != ::null())
            {
               pappThis2->m_dwAlive = m_pthread->m_pthread->m_dwAlive;
            }
            if(pliveobject != ::null())
            {
               pliveobject->keep_alive();
            }

            /*            if(pliveobject != ::null())
            {
            pliveobject->keep();
            }*/

         } 
         while (::PeekMessage(&msg, ::null(), 0, 0, PM_NOREMOVE) != FALSE);


         if(m_pguie->m_pthread != ::null())
         {
            m_pguie->m_pthread->m_pthread->step_timer();
         }
         if (!ContinueModal(iLevel))
            goto ExitModal;


      }

ExitModal:
      m_iaModalThread.remove_first(::GetCurrentThreadId());
      m_iModal = m_iModalCount;
      return m_nModalResult;
   }

   bool window::ContinueModal(int32_t iLevel)
   {
      return iLevel < m_iModalCount;
   }

   void window::EndModalLoop(id nResult)
   {
      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from window::RunModalLoop
      m_nModalResult = (int32_t) nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {
         m_iModalCount--;
         for(index i = 0; i < m_iaModalThread.get_count(); i++)
         {
            ::PostThreadMessageA((DWORD) m_iaModalThread[i], WM_NULL, 0, 0);
         }
         PostMessage(WM_NULL);
         System.GetThread()->post_thread_message(WM_NULL, 0, 0);
      }
   }

   void window::EndAllModalLoops(id nResult)
   {
      ASSERT(::IsWindow(get_handle()));

      // this result will be returned from window::RunModalLoop
      m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      if(m_iModalCount > 0)
      {
         int32_t iLevel = m_iModalCount - 1;
         m_iModalCount = 0;
         PostMessage(WM_NULL);
         System.GetThread()->post_thread_message(WM_NULL, 0, 0);
         for(int32_t i = iLevel; i >= 0; i--)
         {
            ::ca::thread * pthread = oprop(string("RunModalLoop.thread(") + ::ca::str::from(i) + ")").ca < ::ca::thread > ();
            try
            {
               pthread->post_thread_message(WM_NULL, 0, 0);
            }
            catch(...)
            {
            }
         }
      }
   }


   /////////////////////////////////////////////////////////////////////////////
   // frame_window (here for library granularity)

   bool window::is_frame_window()
   {
      return FALSE;
   }
   /////////////////////////////////////////////////////////////////////////////
   // Extra window support for dynamic subclassing of controls

   bool window::subclass_window(oswindow oswindow)
   {
      if (!attach(oswindow))
         return FALSE;

      // allow any other subclassing to occur
      pre_subclass_window();

      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(oswindow, GWLP_WNDPROC);

      // now hook into the AFX WndProc
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(oswindow, GWLP_WNDPROC, (int_ptr)__get_window_procedure());
      ASSERT(oldWndProc != __get_window_procedure());

      if (*lplpfn == ::null())
         *lplpfn = oldWndProc;   // the first control of that type created
#ifdef DEBUG
      else if (*lplpfn != oldWndProc)
      {

         TRACE(::ca::trace::category_AppMsg, 0, "p: Trying to use subclass_window with incorrect window\n");
         TRACE(::ca::trace::category_AppMsg, 0, "\tderived class.\n");
         TRACE(::ca::trace::category_AppMsg, 0, "\toswindow_ = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(uint_ptr)oswindow, __get_dialog_control_id(oswindow), typeid(*this).name());

         ASSERT(FALSE);

         // undo the subclassing if continuing after assert

         ::SetWindowLongPtr(oswindow, GWLP_WNDPROC, (int_ptr)oldWndProc);

      }
#endif
      ::ca::message::size size(get_app());
      _001OnSize(&size);
      return TRUE;
   }

   bool window::SubclassDlgItem(UINT nID, sp(::ca::window) pParent)
   {
      ASSERT(pParent != ::null());
      ASSERT(::IsWindow(WIN_WINDOW(pParent)->get_handle()));

      // check for normal dialog control first
      oswindow oswindow_Control = ::GetDlgItem(WIN_WINDOW(pParent)->get_handle(), nID);
      if (oswindow_Control != ::null())
         return subclass_window(oswindow_Control);


      return FALSE;   // control not found
   }

   oswindow window::unsubclass_window()
   {
      ASSERT(::IsWindow(get_handle()));

      // set WNDPROC back to original value
      WNDPROC* lplpfn = GetSuperWndProcAddr();
      SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (int_ptr)*lplpfn);
      *lplpfn = ::null();

      // and detach the oswindow from the window object
      return detach();
   }


   /*   view_update_hint::view_update_hint(sp(::ca::application) papp) :
   ca(papp)
   {
   }
   */



   bool window::IsChild(sp(::user::interaction) pWnd)
   {
      ASSERT(::IsWindow(get_handle())); 
      if(WIN_WINDOW(pWnd.m_p)->get_handle() == ::null())
      {
         return ::user::interaction::IsChild(pWnd);
      }
      else
      {
         return ::IsChild(get_handle(), WIN_WINDOW(pWnd.m_p)->get_handle()) != FALSE;
      }
   }

   bool window::IsWindow()
   {
      return ::IsWindow(get_handle()) != FALSE;
   }

   oswindow window::get_handle() const
   {
      return ::win::oswindow_handle::get_handle();
   }

   bool window::SetWindowPos(int32_t z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
   {

      single_lock sl(mutex_graphics());

      /*
      bool b;
      bool * pb = &b;
      if(m_papp->s_ptwf != ::null())
      pb = &m_papp->s_ptwf->m_bProDevianMode;
      keeper < bool > keepOnDemandDraw(pb, false, *pb, true);
      */

      ASSERT(::IsWindow(get_handle())); 

      /*
      return ::SetWindowPos(get_handle(), pWndInsertAfter->get_handle(),
      x, y, cx, cy, nFlags) != FALSE; 
      */

      rect64 rectWindowOld = m_rectParentClient;

      if(nFlags & SWP_NOMOVE)
      {
         if(nFlags & SWP_NOSIZE)
         {
         }
         else
         {
            sl.lock();
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }
      else
      {
         if(nFlags & SWP_NOSIZE)
         {
            sl.lock();
            m_rectParentClient.offset(x - m_rectParentClient.left, y - m_rectParentClient.top);
         }
         else
         {
            sl.lock();
            m_rectParentClient.left    = x;
            m_rectParentClient.top     = y;
            m_rectParentClient.right   = m_rectParentClient.left + cx;
            m_rectParentClient.bottom  = m_rectParentClient.top + cy;
         }
      }

      if(m_pguie != this && m_pguie != ::null())
      {
         m_pguie->m_rectParentClient = m_rectParentClient;
      }

      if(GetExStyle() & WS_EX_LAYERED)
      {

         nFlags |= SWP_NOCOPYBITS;

         nFlags |= SWP_NOREDRAW;

         //nFlags |= SWP_NOMOVE;

         //nFlags |= SWP_NOSIZE;

         if(z == 0)
         {

            nFlags |= SWP_NOZORDER;

         }

         if(rectWindowOld.top_left() != m_rectParentClient.top_left())
         {

            send_message(WM_MOVE, 0, 0);

         }

         if(rectWindowOld.size() != m_rectParentClient.size())
         {

            send_message(WM_SIZE, 0, 0);

         }

         //nFlags |= SWP_FRAMECHANGED;

         if(nFlags & SWP_SHOWWINDOW)
         {

            if(!IsWindowVisible())
            {

               ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);

               ShowWindow(SW_SHOW);

            }

         }
         //else
         {

            // ::SetWindowPos(get_handle(),(oswindow)  z, x, y, cx, cy, nFlags);

         }
         /*         if(nFlags & SWP_SHOWWINDOW)
         {

         //            ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);
         if(!IsWindowVisible())
         {

         ShowWindow(SW_SHOW);

         }

         }
         else
         {

         //  ::SetWindowPos(get_handle(),(oswindow)  z, x, y, cx, cy, nFlags);

         }*/

         /*if(m_pguie != ::null())
         {
         m_pguie->oprop("pending_layout") = true;
         m_pguie->oprop("pending_zorder") = z;
         }*/
         /*if(&System != ::null() && System.get_twf() != ::null())
         {
         System.get_twf()->synch_redraw();
         }*/
      }
      else
      {

         if(z == -3)
         {

            ::SetWindowPos(get_handle(), (oswindow) 0, x, y, cx, cy, nFlags);

         }
         else
         {

            ::SetWindowPos(get_handle(), (oswindow) z, x, y, cx, cy, nFlags);

         }

      }
      return true;

   }

   void window::MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, bool bRepaint)
   {
      ASSERT(::IsWindow(get_handle())); 
      SetWindowPos(0, x, y, nWidth, nHeight, bRepaint ? SWP_SHOWWINDOW : 0);
   }

   void window::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   += (LONG) rectWindow.left;
      lprect->right  += (LONG) rectWindow.left;
      lprect->top    += (LONG) rectWindow.top;
      lprect->bottom += (LONG) rectWindow.top;

   }

   void window::ClientToScreen(LPPOINT lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     += (LONG) rectWindow.left;
      lppoint->y     += (LONG) rectWindow.top;
   }


   void window::ClientToScreen(__rect64 * lprect)
   {
      class rect rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   += rectWindow.left;
      lprect->right  += rectWindow.left;
      lprect->top    += rectWindow.top;
      lprect->bottom += rectWindow.top;

   }

   void window::ClientToScreen(__point64 * lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     += rectWindow.left;
      lppoint->y     += rectWindow.top;
   }


   void window::ScreenToClient(LPRECT lprect)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   -= (LONG) rectWindow.left;
      lprect->right  -= (LONG) rectWindow.left;
      lprect->top    -= (LONG) rectWindow.top;
      lprect->bottom -= (LONG) rectWindow.top;

   }

   void window::ScreenToClient(LPPOINT lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     -= (LONG) rectWindow.left;
      lppoint->y     -= (LONG) rectWindow.top;
   }


   void window::ScreenToClient(__rect64 * lprect)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lprect->left   -= rectWindow.left;
      lprect->right  -= rectWindow.left;
      lprect->top    -= rectWindow.top;
      lprect->bottom -= rectWindow.top;

   }

   void window::ScreenToClient(__point64 * lppoint)
   {
      class rect64 rectWindow;
      m_pguie->GetWindowRect(rectWindow);

      lppoint->x     -= rectWindow.left;
      lppoint->y     -= rectWindow.top;
   }


   void window::GetWindowRect(__rect64 * lprect)
   {

      if(!::IsWindow(get_handle()))
         return;

      //if(!::IsWindow(get_handle()))
      //throw simple_exception(get_app(), "no more a window");
      // if it is temporary window - probably not ca wrapped window
      if(m_pguie == ::null() || m_pguie == this)
      {
         rect rect32;
         ::GetWindowRect(get_handle(), rect32);
         ::copy(lprect, rect32);
      }
      else
      {
         interaction::GetWindowRect(lprect);
      }
   }

   void window::GetClientRect(__rect64 * lprect)
   {
      ASSERT(::IsWindow(get_handle())); 
      // if it is temporary window - probably not ca wrapped window
      if(m_pguie == ::null() || m_pguie == this)
      {
         rect rect32;
         ::GetClientRect(get_handle(), rect32);
         ::copy(lprect, rect32);
      }
      else
      {
         interaction::GetClientRect(lprect);
      }
   }

   id window::SetDlgCtrlId(id id)
   {
      m_id = id;
      return m_id;
   }

   id window::GetDlgCtrlId()
   {
      return m_id;
   }

   /*   guie_message_wnd::guie_message_wnd(sp(::ca::application) papp) :
   ca(papp)
   {
   m_pguieForward = ::null();
   }

   LRESULT guie_message_wnd::message_handler(::ca::signal_object * pobj)
   {
   if(m_pguieForward != ::null())
   {
   return m_pguieForward->message_handler(uiMessage, wparam, lparam);
   }
   else
   {
   return 0;
   }
   }*/

   void window::_001WindowMaximize()
   {
      ::user::interaction::_001WindowMaximize();
   }

   void window::_001WindowRestore()
   {
      m_eappearance = appearance_normal;
      if(m_pguie != ::null())
         m_pguie->m_eappearance = appearance_normal;
      ::ShowWindow(get_handle(), SW_RESTORE);
   }

   bool window::ShowWindow(int32_t nCmdShow)
   {
      if(!::IsWindow(get_handle()))
         return false;
      if(GetExStyle() & WS_EX_LAYERED)
      {
         if(nCmdShow == SW_HIDE)
         {
            ModifyStyle(get_handle(), WS_VISIBLE, 0, 0);
         }
         else
         {
            ModifyStyle(get_handle(), 0, WS_VISIBLE, 0);
         }
         if(nCmdShow == SW_MAXIMIZE)
         {
            _001WindowMaximize();
         }
         else if(nCmdShow == SW_RESTORE)
         {
            _001WindowRestore();
         }
         else
         {
            if(nCmdShow == SW_MINIMIZE)
            {
               m_pguie->m_eappearance = appearance_iconic;
               m_eappearance = appearance_iconic;
            }
            ::ShowWindow(get_handle(), nCmdShow);
         }
         m_bVisible = ::IsWindowVisible(get_handle()) != FALSE;
         if(m_pguie!= ::null() && m_pguie != this)
            m_pguie->m_bVisible = m_bVisible;
         if(!m_bVisible || IsIconic())
         {
            ::UpdateLayeredWindow(get_handle(), ::null(), ::null(), ::null(), ::null(), ::null(), 0, ::null(), 0);
         }
         return m_bVisible;
      }
      else
      {
         ::ShowWindow(get_handle(), nCmdShow);
         m_bVisible = ::IsWindowVisible(get_handle()) != FALSE;
         if(m_pguie!= ::null() && m_pguie != this)
            m_pguie->m_bVisible = m_bVisible;
         return m_bVisible;
      }
   }


   bool window::IsIconic()
   {
      ASSERT(::IsWindow(get_handle())); 
      if(GetExStyle() & WS_EX_LAYERED)
      {
         return m_pguie->m_eappearance == appearance_iconic;
      }
      else
      {
         return ::IsIconic(get_handle()) != FALSE;
      }
   }

   bool window::IsZoomed()
   {
      if(!::IsWindow(get_handle()))
         return false;
      return m_pguie->m_eappearance == appearance_zoomed;
   }


   sp(::user::interaction) window::get_parent() const
   {
      
      if(!::IsWindow(get_handle()))
         return ::null();
      
      if(get_handle() == ::null())
         return ::null();

      HWND hwndParent = ::GetParent(get_handle());
      
      if(hwndParent == ::null())
         return ::null();

      return (::user::interaction *) ::win::window::from_handle(hwndParent).m_p;

   }

   LONG window::GetWindowLong(int32_t nIndex)
   {
      return ::GetWindowLong(get_handle(), nIndex);
   }

   LONG window::SetWindowLong(int32_t nIndex, LONG lValue)
   {
      return ::SetWindowLong(get_handle(), nIndex, lValue);
   }


   sp(::user::interaction) window::release_capture()
   {
      oswindow oswindowCapture = ::GetCapture();
      if(oswindowCapture == ::null())
         return ::null();
      if(oswindowCapture == get_handle())
      {
         sp(::user::interaction) puieCapture = get_capture();
         if(::ReleaseCapture())
         {
            m_pguieCapture = ::null();
            return puieCapture;
         }
         else
         {
            return ::null();
         }
      }
      else
      {
         return window::GetCapture()->release_capture();
      }
   }

   sp(::user::interaction) window::get_capture()
   {
      oswindow oswindowCapture = ::GetCapture();
      if(oswindowCapture == ::null())
         return ::null();
      if(oswindowCapture == get_handle())
      {
         if(m_pguieCapture != ::null())
         {
            return m_pguieCapture;
         }
         else
         {
            if(m_pguie != ::null())
            {
               if(m_pguie->get_wnd() != ::null() && WIN_WINDOW(m_pguie->get_wnd())->m_pguieCapture != ::null())
               {
                  return WIN_WINDOW(m_pguie->get_wnd())->m_pguieCapture;
               }
               else
               {
                  return m_pguie;
               }
            }
            else
            {
               return this;
            }
         }
      }
      else
      {
         return window::GetCapture()->get_capture();
      }
   }



   // window
   /* window::operator oswindow() const
   { return this == ::null() ? ::null() : get_handle(); }*/
   bool window::operator==(const ::ca::window& wnd) const
   { 
      return (WIN_WINDOW((::ca::window *) &wnd)->get_handle()) == get_handle(); 
   }

   bool window::operator!=(const ::ca::window& wnd) const
   { 
      return (WIN_WINDOW((::ca::window *) &wnd)->get_handle()) != get_handle(); 
   }

   uint32_t window::GetStyle()
   {

      if(!::IsWindow(get_handle()))
         return 0;

      return (uint32_t)::GetWindowLong(get_handle(), GWL_STYLE); 

   }

   uint32_t window::GetExStyle()
   {

      if(!::IsWindow(get_handle()))
         return 0;

      return (uint32_t)::GetWindowLong(get_handle(), GWL_EXSTYLE); 

   }


   bool window::ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   { 

      if(!::IsWindow(get_handle()))
         return false;

      return ModifyStyle(get_handle(), dwRemove, dwAdd, nFlags); 

   }


   bool window::ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   { 

      if(!::IsWindow(get_handle()))
         return false;

      return ModifyStyleEx(get_handle(), dwRemove, dwAdd, nFlags); 

   }

   void window::set_owner(sp(::user::interaction) pOwnerWnd)
   { 
      m_pguieOwner = pOwnerWnd; 
   }

   LRESULT window::send_message(UINT message, WPARAM wParam, LPARAM lParam)
   {
      //ASSERT(::IsWindow(get_handle())); 
      return ::SendMessage(get_handle(), message, wParam, lParam);
   }

   bool window::PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
   { 
      return ::PostMessage(get_handle(), message, wParam, lParam) != FALSE; 
   }

   bool window::DragDetect(POINT pt) const
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::DragDetect(get_handle(), pt) != FALSE; 

   }

   void window::SetWindowText(const char * lpszString)
   { 
      m_strWindowText = lpszString;
   }

   strsize window::GetWindowText(LPTSTR lpszString, strsize nMaxCount)
   { 
      strncpy(lpszString, m_strWindowText, nMaxCount);
      return min(nMaxCount, m_strWindowText.get_length());
   }

   strsize window::GetWindowTextLength()
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::GetWindowTextLength(get_handle()); 

   }

   void window::SetFont(::ca::font* pfont, bool bRedraw)
   { 
      UNREFERENCED_PARAMETER(bRedraw);
      ASSERT(::IsWindow(get_handle())); m_pfont = new ::ca::font(*pfont); 
   }

   ::ca::font* window::GetFont()
   { 
      ASSERT(::IsWindow(get_handle())); 
      return m_pfont; 
   }

   void window::DragAcceptFiles(bool bAccept)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::DragAcceptFiles(get_handle(), bAccept); 
   }

   sp(::frame_window) window::EnsureParentFrame()
   {
      sp(::frame_window) pFrameWnd=GetParentFrame();
      ENSURE_VALID(pFrameWnd);
      return pFrameWnd;
   }

   sp(::user::interaction) window::EnsureTopLevelParent()
   {
      sp(::user::interaction)pWnd=GetTopLevelParent();
      ENSURE_VALID(pWnd);
      return pWnd;
   }

   void window::MoveWindow(LPCRECT lpRect, bool bRepaint)
   {
      MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint); 
   }

   UINT window::ArrangeIconicWindows()
   {
      ASSERT(::IsWindow(get_handle())); return ::ArrangeIconicWindows(get_handle()); 
   }

   int32_t window::SetWindowRgn(HRGN hRgn, bool bRedraw)
   {
      ASSERT(::IsWindow(get_handle())); return ::SetWindowRgn(get_handle(), hRgn, bRedraw); 
   }

   int32_t window::GetWindowRgn(HRGN hRgn)
   {
      ASSERT(::IsWindow(get_handle()) && hRgn != ::null()); return ::GetWindowRgn(get_handle(), hRgn); 
   }

   bool window::BringWindowToTop()
   {

      return ::BringWindowToTop(get_handle()) != FALSE; 

   }

   void window::MapWindowPoints(sp(::ca::window) pwndTo, LPPOINT lpPoint, UINT nCount)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::MapWindowPoints(get_handle(), pwndTo->get_handle(), lpPoint, nCount); 
   }

   void window::MapWindowPoints(sp(::ca::window) pwndTo, LPRECT lpRect)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::MapWindowPoints(get_handle(), pwndTo->get_handle(), (LPPOINT)lpRect, 2); 
   }

   ::ca::graphics * window::GetDC()
   { 
      ::ca::graphics_sp g(allocer());
      if(get_handle() == ::null())
      {
         (dynamic_cast < ::win::graphics * >(g.m_p))->Attach(::GetDC(::null()));
      }
      else
      {
         (dynamic_cast < ::win::graphics * >(g.m_p))->Attach(::GetDC(get_handle()));
      }
      return g.detach();
   }

   ::ca::graphics * window::GetWindowDC()
   {
      ASSERT(::IsWindow(get_handle())); 
      ::ca::graphics_sp g(allocer());
      (dynamic_cast < ::win::graphics * >(g.m_p))->Attach(::GetWindowDC(get_handle()));
      return g.detach();
   }

   bool window::ReleaseDC(::ca::graphics * pgraphics)
   { 

      if(pgraphics == ::null())
         return false;

      if(((Gdiplus::Graphics *)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle()) == ::null())
         return false;

      try
      {

         delete (dynamic_cast<::win::graphics * >(pgraphics))->m_pgraphics;

      }
      catch(...)
      {

         TRACE("::win::window::ReleaseDC : Failed to delete Gdiplus::Graphics");

      }

      (dynamic_cast<::win::graphics * >(pgraphics))->m_pgraphics = ::null();

      BOOL bOk = ::ReleaseDC(get_handle(), (dynamic_cast<::win::graphics * >(pgraphics))->m_hdc); 

      if(!bOk)
      {

         TRACE("::win::window::ReleaseDC : Failed to Release window dc");

      }

      (dynamic_cast<::win::graphics * >(pgraphics))->m_hdc = ::null();



      return true;

   }

   void window::UpdateWindow()
   {
      ::UpdateWindow(get_handle()); 
   }

   void window::SetRedraw(bool bRedraw)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0); 
   }

   bool window::GetUpdateRect(LPRECT lpRect, bool bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::GetUpdateRect(get_handle(), lpRect, bErase) != FALSE; 
   }

   int32_t window::GetUpdateRgn(::ca::region* pRgn, bool bErase)
   { 
      ASSERT(::IsWindow(get_handle())); 
      return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase); 
   }

   void window::Invalidate(bool bErase)
   { 
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRect(get_handle(), ::null(), bErase); 
   }

   void window::InvalidateRect(LPCRECT lpRect, bool bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRect(get_handle(), lpRect, bErase); 
   }

   void window::InvalidateRgn(::ca::region* pRgn, bool bErase)
   {
      ASSERT(::IsWindow(get_handle())); 
      ::InvalidateRgn(get_handle(), (HRGN)pRgn->get_os_data(), bErase); 
   }

   void window::ValidateRect(LPCRECT lpRect)
   { ASSERT(::IsWindow(get_handle())); ::ValidateRect(get_handle(), lpRect); }
   void window::ValidateRgn(::ca::region* pRgn)
   { ASSERT(::IsWindow(get_handle())); ::ValidateRgn(get_handle(), (HRGN)pRgn->get_os_data()); }

   bool window::IsWindowVisible()
   {

      if(!::IsWindow(get_handle()))
         return false;

      if(m_pguie != ::null())
      {

         if(!m_pguie->m_bVisible)
            return false;

         if(m_pguie->get_parent() != ::null() && !m_pguie->get_parent()->IsWindowVisible())
            return false;

      }

      if(!::IsWindowVisible(get_handle()))
         return false;

      return true;

   }


   void window::ShowOwnedPopups(bool bShow)
   { 

      ASSERT(::IsWindow(get_handle()));

      ::ShowOwnedPopups(get_handle(), bShow);

   }

   void window::SendMessageToDescendants(UINT message, WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm)
   { 
      ASSERT(::IsWindow(get_handle())); 
      //window::SendMessageToDescendants(get_handle(), message, wParam, lParam, bDeep, bOnlyPerm); 

      // walk through HWNDs to avoid creating temporary window objects
      // unless we need to call this function recursively
      sp(::user::interaction) pui = m_pguie->get_top_child();
      while(pui != ::null())
      {
         try
         {
            pui->send_message(message, wParam, lParam);
         }
         catch(...)
         {
         }
         if (bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->SendMessageToDescendants(message, wParam, lParam, bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
         try
         {
            pui = pui->under_sibling();
         }
         catch(...)
         {
            break;
         }
      }
   }

   sp(::user::interaction) window::GetDescendantWindow(id id)
   {
      ASSERT(::IsWindow(get_handle())); 
      return window::GetDescendantWindow(this, id); 
   }


   ::ca::graphics * window::GetDCEx(::ca::region* prgnClip, uint32_t flags)
   {

      ASSERT(::IsWindow(get_handle())); 
      ::ca::graphics_sp g(allocer());
      g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_os_data(), flags));
      return g.detach(); 

   }

   bool window::LockWindowUpdate()
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::LockWindowUpdate(get_handle()) != FALSE;

   }

   void window::UnlockWindowUpdate()
   {

      ASSERT(::IsWindow(get_handle()));

      ::LockWindowUpdate(::null());

   }

   bool window::RedrawWindow(LPCRECT lpRectUpdate, ::ca::region* prgnUpdate, UINT flags)
   { 

      if(System.get_twf() == ::null())
         return false;

      if(System.get_twf()->m_bProDevianMode)
         return true;

      ASSERT(::IsWindow(get_handle())); 

      return ::RedrawWindow(get_handle(), lpRectUpdate, prgnUpdate == ::null() ? ::null() : (HRGN)prgnUpdate->get_os_data(), flags) != FALSE;

   }

   bool window::EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags)
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags) != FALSE;

   }

   bool window::DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo)
   { 

      ASSERT(::IsWindow(get_handle()));

      return ::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo) != FALSE;

   }

   bool window::DrawCaption(::ca::graphics * pgraphics, LPCRECT lprc, UINT uFlags)
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle(), lprc, uFlags) != FALSE;

   }

   uint_ptr window::SetTimer(uint_ptr nIDEvent, UINT nElapse, void (CALLBACK* lpfnTimer)(oswindow, UINT, uint_ptr, uint32_t))
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::SetTimer(get_handle(), nIDEvent, nElapse, (TIMERPROC) lpfnTimer); 

   }

   bool window::KillTimer(uint_ptr nIDEvent)
   {

      ASSERT(::IsWindow(get_handle()));

      return ::KillTimer(get_handle(), nIDEvent)  != FALSE;

   }

   bool window::IsWindowEnabled()
   {

      if(!::IsWindow(get_handle()))
         return false;

      return ::IsWindowEnabled(get_handle()) != FALSE;

   }


   bool window::EnableWindow(bool bEnable)
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::EnableWindow(get_handle(), bEnable) != FALSE;

   }

   sp(::user::interaction) window::GetActiveWindow()
   {

      return (::user::interaction *) ::win::window::from_handle(::GetActiveWindow()).m_p;

   }

   sp(::user::interaction) window::SetActiveWindow()
   {

      ASSERT(::IsWindow(get_handle()));

      return (::user::interaction *) ::win::window::from_handle(::SetActiveWindow(get_handle())).m_p;

   }

   sp(::ca::window) window::GetCapture()
   {

      return ::win::window::from_handle(::GetCapture());

   }

   sp(::user::interaction) window::set_capture(sp(::user::interaction) pinterface)
   { 

      ASSERT(::IsWindow(get_handle())); 

      if(pinterface != ::null()) 
         m_pguieCapture = pinterface; 

      return  (::user::interaction *) ::win::window::from_handle(::SetCapture(get_handle())).m_p; 

   }

   sp(::user::interaction) window::GetFocus()
   { 

      return (::user::interaction *) ::win::window::from_handle(::GetFocus()).m_p;

   }

   sp(::user::interaction) window::SetFocus()
   { 

      ASSERT(::IsWindow(get_handle()));

      return (::user::interaction *) ::win::window::from_handle(::SetFocus(get_handle())).m_p;

   }

   sp(::ca::window) window::GetDesktopWindow()
   {

      return ::win::window::from_handle(::GetDesktopWindow());

   }


   // Helper for radio buttons
   int32_t window::GetCheckedRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton)
   {
      for (int32_t nID = nIDFirstButton; nID <= nIDLastButton; nID++)
      {
         if (IsDlgButtonChecked(nID))
            return nID; // id that matched
      }
      return 0; // invalid ID
   }

   void window::CheckDlgButton(int32_t nIDButton, UINT nCheck)
   { 

      ASSERT(::IsWindow(get_handle())); 

      ::CheckDlgButton(get_handle(), nIDButton, nCheck);

   }

   void window::CheckRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton, int32_t nIDCheckButton)
   { 

      ASSERT(::IsWindow(get_handle())); 

      ::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton); 

   }

   int32_t window::DlgDirList(__inout_z LPTSTR lpPathSpec, __in int32_t nIDListBox,
      __in int32_t nIDStaticPath, __in UINT nFileType)
   {
      ASSERT(::IsWindow(get_handle()));

      return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox, nIDStaticPath, nFileType); 

   }

   int32_t window::DlgDirListComboBox(__inout_z LPTSTR lpPathSpec, __in int32_t nIDComboBox,  __in int32_t nIDStaticPath, __in UINT nFileType)
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::DlgDirListComboBox(get_handle(), lpPathSpec, nIDComboBox, nIDStaticPath, nFileType); 

   }

   bool window::DlgDirSelect(LPTSTR lpString, int32_t nSize, int32_t nIDListBox)
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox) != FALSE;

   }

   bool window::DlgDirSelectComboBox(LPTSTR lpString, int32_t nSize, int32_t nIDComboBox)
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox) != FALSE;

   }

   void window::get_child_by_id(id id, oswindow* poswindow_) const
   {

      ASSERT(::IsWindow(get_handle())); 
      ASSERT(poswindow_ != ::null()); 
      *poswindow_ = ::GetDlgItem(get_handle(), (int32_t) id);

   }

   UINT window::GetChildByIdInt(int32_t nID, BOOL * lpTrans, bool bSigned) const
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::GetDlgItemInt(get_handle(), nID, lpTrans, bSigned);

   }

   int32_t window::GetChildByIdText(__in int32_t nID, __out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpStr, __in int32_t nMaxCount) const
   { ASSERT(::IsWindow(get_handle())); return ::GetDlgItemText(get_handle(), nID, lpStr, nMaxCount);}

   sp(::ca::window) window::GetNextDlgGroupItem(sp(::ca::window) pWndCtl, bool bPrevious) const
   {
      ASSERT(::IsWindow(get_handle()));
      return ::win::window::from_handle(::GetNextDlgGroupItem(get_handle(), pWndCtl->get_handle(), bPrevious)); 
   }

   sp(::ca::window) window::GetNextDlgTabItem(sp(::ca::window) pWndCtl, bool bPrevious) const
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::win::window::from_handle(::GetNextDlgTabItem(get_handle(), pWndCtl->get_handle(), bPrevious)); 
   }

   UINT window::IsDlgButtonChecked(int32_t nIDButton) const
   { ASSERT(::IsWindow(get_handle())); return ::IsDlgButtonChecked(get_handle(), nIDButton); }
   LPARAM window::SendDlgItemMessage(int32_t nID, UINT message, WPARAM wParam, LPARAM lParam)
   { ASSERT(::IsWindow(get_handle())); return ::SendDlgItemMessage(get_handle(), nID, message, wParam, lParam); }
   void window::SetDlgItemInt(int32_t nID, UINT nValue, bool bSigned)
   { ASSERT(::IsWindow(get_handle())); ::SetDlgItemInt(get_handle(), nID, nValue, bSigned); }
   void window::SetDlgItemText(int32_t nID, const char * lpszString)
   { ASSERT(::IsWindow(get_handle())); ::SetDlgItemText(get_handle(), nID, lpszString); }
   int32_t window::ScrollWindowEx(int32_t dx, int32_t dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::ca::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags)
   { ASSERT(::IsWindow(get_handle())); return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip,
   (HRGN)prgnUpdate->get_os_data(), lpRectUpdate, flags); }

   void window::ShowScrollBar(UINT nBar, bool bShow)
   { ASSERT(::IsWindow(get_handle())); ::ShowScrollBar(get_handle(), nBar, bShow); }
   sp(::user::interaction) window::ChildWindowFromPoint(POINT point)
   { 
      ASSERT(::IsWindow(get_handle())); 
      
      return (::user::interaction *) ::win::window::from_handle(::ChildWindowFromPoint(get_handle(), point)).m_p; 

   
   }

   sp(::user::interaction) window::ChildWindowFromPoint(POINT point, UINT nFlags)
   {
      ASSERT(::IsWindow(get_handle())); 
      
      return (::user::interaction *)  ::win::window::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags)).m_p;


   }

   sp(::ca::window) window::FindWindow(const char * lpszClassName, const char * lpszWindowName)
   { return ::win::window::from_handle(::FindWindow(lpszClassName, lpszWindowName)); }
   sp(::ca::window) window::FindWindowEx(oswindow oswindowParent, oswindow oswindowChildAfter, const char * lpszClass, const char * lpszWindow)
   { return ::win::window::from_handle(::FindWindowEx(oswindowParent, oswindowChildAfter, lpszClass, lpszWindow)); }

   sp(::user::interaction) window::GetNextWindow(UINT nFlag)
   { 
      ASSERT(::IsWindow(get_handle())); 
      
      return (::user::interaction *) ::win::window::from_handle(::GetNextWindow(get_handle(), nFlag)).m_p; 
   }

   sp(::user::interaction) window::GetTopWindow()
   { ASSERT(::IsWindow(get_handle())); return (::user::interaction *) ::win::window::from_handle(::GetTopWindow(get_handle())).m_p; }
   sp(::user::interaction) window::GetWindow(UINT nCmd)
   { ASSERT(::IsWindow(get_handle())); return (::user::interaction *) ::win::window::from_handle(::GetWindow(get_handle(), nCmd)).m_p; }
   sp(::user::interaction) window::GetLastActivePopup()
   { ASSERT(::IsWindow(get_handle())); return (::user::interaction *) ::win::window::from_handle(::GetLastActivePopup(get_handle())).m_p; }

   sp(::ca::window) window::set_parent(sp(::ca::window) pWndNewParent)
   {
      ASSERT(::IsWindow(get_handle())); 
      return ::win::window::from_handle(::SetParent(get_handle(), pWndNewParent->get_handle())); 
   }

   sp(::ca::window) window::WindowFromPoint(POINT point)
   { 

      return ::win::window::from_handle(::WindowFromPoint(point));

   }

   bool window::FlashWindow(bool bInvert)
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::FlashWindow(get_handle(), bInvert) != FALSE;

   }

   bool window::ChangeClipboardChain(oswindow oswindow_Next)
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::ChangeClipboardChain(get_handle(), oswindow_Next) != FALSE;

   }

   oswindow window::SetClipboardViewer()
   {

      ASSERT(::IsWindow(get_handle()));

      return ::SetClipboardViewer(get_handle());

   }

   bool window::OpenClipboard()
   { 

      ASSERT(::IsWindow(get_handle())); 

      return ::OpenClipboard(get_handle()) != FALSE;

   }

   sp(::ca::window) window::GetOpenClipboardWindow()
   {

      return ::win::window::from_handle(::GetOpenClipboardWindow());

   }

   sp(::ca::window) window::GetClipboardOwner()
   {

      return ::win::window::from_handle(::GetClipboardOwner());

   }

   sp(::ca::window) window::GetClipboardViewer()
   {

      return ::win::window::from_handle(::GetClipboardViewer());

   }

   void window::CreateCaret(::ca::bitmap* pBitmap)
   {

      ASSERT(::IsWindow(get_handle())); 

      ::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_os_data(), 0, 0);

   }

   void window::CreateSolidCaret(int32_t nWidth, int32_t nHeight)
   {

      ASSERT(::IsWindow(get_handle())); 

      ::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

   }

   void window::CreateGrayCaret(int32_t nWidth, int32_t nHeight)
   {

      ASSERT(::IsWindow(get_handle()));

      ::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

   }

   point window::GetCaretPos()
   { 

      point point;

      ::GetCaretPos((LPPOINT)&point); return point;

   }

   void window::SetCaretPos(POINT point)
   { 

      ::SetCaretPos(point.x, point.y);

   }

   void window::HideCaret()
   {

      ::HideCaret(get_handle());

   }

   void window::ShowCaret()
   { 

      ::ShowCaret(get_handle());

   }

   bool window::SetForegroundWindow()
   {

      return ::SetForegroundWindow(get_handle()) != FALSE; 

   }

   sp(::ca::window) window::GetForegroundWindow()
   {

      return ::win::window::from_handle(::GetForegroundWindow());

   }

   bool window::SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam)
   { 

      return ::SendNotifyMessage(get_handle(), message, wParam, lParam) != FALSE;

   }

   // Win4
   HICON window::SetIcon(HICON hIcon, bool bBigIcon)
   { 

      return (HICON)send_message(WM_SETICON, bBigIcon, (LPARAM)hIcon);

   }

   HICON window::GetIcon(bool bBigIcon) const
   { 

      ASSERT(::IsWindow(get_handle()));

      return (HICON)const_cast < ::win::window * > (this)->send_message(WM_GETICON, bBigIcon, 0);

   }

   void window::Print(::ca::graphics * pgraphics, uint32_t dwFlags) const
   { 

      ASSERT(::IsWindow(get_handle()));

      const_cast < ::win::window * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle(), dwFlags);

   }

   void window::PrintClient(::ca::graphics * pgraphics, uint32_t dwFlags) const
   { 

      ASSERT(::IsWindow(get_handle()));

      const_cast < ::win::window * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::win::graphics * >(pgraphics))->get_handle(), dwFlags);

   }

   bool window::SetWindowContextHelpId(uint32_t dwContextHelpId)
   { 

      ASSERT(::IsWindow(get_handle()));

      return ::SetWindowContextHelpId(get_handle(), dwContextHelpId) != FALSE;

   }

   uint32_t window::GetWindowContextHelpId() const
   {

      ASSERT(::IsWindow(get_handle()));

      return ::GetWindowContextHelpId(get_handle());

   }


   // Default message map implementations
   void window::OnActivateApp(bool, uint32_t)
   { Default(); }
   void window::OnActivate(UINT, sp(::ca::window), bool)
   { Default(); }
   void window::OnCancelMode()
   { Default(); }
   void window::OnChildActivate()
   { Default(); }
   void window::OnClose()
   { Default(); }
   void window::OnContextMenu(sp(::ca::window), point)
   { Default(); }

   bool window::OnCopyData(sp(::ca::window), COPYDATASTRUCT*)
   { 

      return Default() != FALSE;

   }

   void window::OnEnable(bool)
   { Default(); }
   void window::OnEndSession(bool)
   { Default(); }

   bool window::OnEraseBkgnd(::ca::graphics *)
   {

      return Default() != FALSE;

   }

   void window::OnGetMinMaxInfo(MINMAXINFO*)
   { Default(); }
   void window::OnIconEraseBkgnd(::ca::graphics *)
   { Default(); }
   void window::OnKillFocus(sp(::ca::window))
   { Default(); }
   LRESULT window::OnMenuChar(UINT, UINT, ::userbase::menu*)
   { return Default(); }
   void window::OnMenuSelect(UINT, UINT, HMENU)
   { Default(); }
   void window::OnMove(int32_t, int32_t)
   { Default(); }
   HCURSOR window::OnQueryDragIcon()
   { return (HCURSOR)Default(); }

   bool window::OnQueryEndSession()
   { 

      return Default() != FALSE;

   }

   bool window::OnQueryNewPalette()
   {

      return Default() != FALSE;

   }

   bool window::OnQueryOpen()
   {

      return Default() != FALSE;

   }

   void window::_001OnSetCursor(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);
      if(Session.get_cursor() != ::null() && Session.get_cursor()->m_ecursor != ::visual::cursor_system)
      {
         ::SetCursor(::null());
      }
      pbase->set_lresult(1);
      pbase->m_bRet = true;
      //(bool)Default(); 
   }


   void window::OnShowWindow(bool, UINT)
   { 

      Default(); 

   }


   void window::OnSize(UINT, int32_t, int32_t)
   { Default(); }
   void window::OnTCard(UINT, uint32_t)
   { Default(); }
   void window::OnWindowPosChanging(WINDOWPOS*)
   { Default(); }
   void window::OnWindowPosChanged(WINDOWPOS*)
   { Default(); }
   void window::OnDropFiles(HDROP)
   { Default(); }
   void window::OnPaletteIsChanging(sp(::ca::window))
   { Default(); }

   bool window::OnNcActivate(bool)
   {

      return Default() != FALSE;

   }

   void window::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
   { Default(); }

   bool window::OnNcCreate(LPCREATESTRUCT)
   {

      return Default() != FALSE;

   }

   LRESULT window::OnNcHitTest(point)
   { return Default(); }
   void window::OnNcLButtonDblClk(UINT, point)
   { Default(); }
   void window::OnNcLButtonDown(UINT, point)
   { Default(); }
   void window::OnNcLButtonUp(UINT, point)
   { Default(); }
   void window::OnNcMButtonDblClk(UINT, point)
   { Default(); }
   void window::OnNcMButtonDown(UINT, point)
   { Default(); }
   void window::OnNcMButtonUp(UINT, point)
   { Default(); }
   void window::OnNcMouseMove(UINT, point)
   { Default(); }
   void window::OnNcPaint()
   { Default(); }
   void window::OnNcRButtonDblClk(UINT, point)
   { Default(); }
   void window::OnNcRButtonDown(UINT, point)
   { Default(); }
   void window::OnNcRButtonUp(UINT, point)
   { Default(); }
   void window::OnSysChar(UINT, UINT, UINT)
   { Default(); }
   void window::OnSysCommand(UINT, LPARAM)
   { Default(); }
   void window::OnSysDeadChar(UINT, UINT, UINT)
   { Default(); }
   void window::OnSysKeyDown(UINT, UINT, UINT)
   { Default(); }
   void window::OnSysKeyUp(UINT, UINT, UINT)
   { Default(); }
   void window::OnCompacting(UINT)
   { Default(); }
   void window::OnFontChange()
   { Default(); }
   void window::OnPaletteChanged(sp(::ca::window))
   { Default(); }
   void window::OnSpoolerStatus(UINT, UINT)
   { Default(); }
   void window::OnTimeChange()
   { Default(); }
   void window::OnChar(UINT, UINT, UINT)
   { Default(); }
   void window::OnDeadChar(UINT, UINT, UINT)
   { Default(); }
   void window::OnKeyDown(UINT, UINT, UINT)
   { Default(); }
   void window::OnKeyUp(UINT, UINT, UINT)
   { Default(); }
   void window::OnLButtonDblClk(UINT, point)
   { Default(); }
   void window::OnLButtonDown(UINT, point)
   { Default(); }
   void window::OnLButtonUp(UINT, point)
   { Default(); }
   void window::OnMButtonDblClk(UINT, point)
   { Default(); }
   void window::OnMButtonDown(UINT, point)
   { Default(); }
   void window::OnMButtonUp(UINT, point)
   { Default(); }
   int32_t window::OnMouseActivate(sp(::ca::window), UINT, UINT)
   { return (int32_t)Default(); }
   void window::OnMouseMove(UINT, point)
   { Default(); }

   bool window::OnMouseWheel(UINT, int16_t, point)
   {

      return Default() != FALSE;

   }

   LRESULT window::OnRegisteredMouseWheel(WPARAM, LPARAM)
   { return Default(); }
   void window::OnRButtonDblClk(UINT, point)
   { Default(); }
   void window::OnRButtonDown(UINT, point)
   { Default(); }
   void window::OnRButtonUp(UINT, point)
   { Default(); }
   void window::OnTimer(uint_ptr)
   { Default(); }
   void window::OnInitMenu(::userbase::menu*)
   { Default(); }
   void window::OnInitMenuPopup(::userbase::menu*, UINT, bool)
   { Default(); }
   void window::OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR pszName)
   {
      (nMaxCount);
      if(nMaxCount>0)
      {
         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
         pszName[0]='\0';
      }
      Default(); 
   }
   void window::OnChangeCbChain(oswindow, oswindow)
   { Default(); }
   void window::OnDestroyClipboard()
   { Default(); }
   void window::OnDrawClipboard()
   { Default(); }
   void window::OnHScrollClipboard(sp(::ca::window), UINT, UINT)
   { Default(); }
   void window::OnPaintClipboard(sp(::ca::window), HGLOBAL)
   { Default(); }
   void window::OnRenderAllFormats()
   { Default(); }
   void window::OnRenderFormat(UINT)
   { Default(); }
   void window::OnSizeClipboard(sp(::ca::window), HGLOBAL)
   { Default(); }
   void window::OnVScrollClipboard(sp(::ca::window), UINT, UINT)
   { Default(); }
   UINT window::OnGetDlgCode()
   { return (UINT)Default(); }
   void window::OnMDIActivate(bool, sp(::ca::window), sp(::ca::window))
   { Default(); }
   void window::OnEnterMenuLoop(bool)
   { Default(); }
   void window::OnExitMenuLoop(bool)
   { Default(); }
   // Win4 support
   void window::OnStyleChanged(int32_t, LPSTYLESTRUCT)
   { Default(); }
   void window::OnStyleChanging(int32_t, LPSTYLESTRUCT)
   { Default(); }
   void window::OnSizing(UINT, LPRECT)
   { Default(); }
   void window::OnMoving(UINT, LPRECT)
   { Default(); }
   void window::OnCaptureChanged(sp(::ca::window))
   { Default(); }

   bool window::OnDeviceChange(UINT, uint_ptr)
   {

      return Default() != FALSE;

   }

   void window::OnWinIniChange(const char *)
   { Default(); }
   void window::OnChangeUIState(UINT, UINT)
   { Default(); }
   void window::OnUpdateUIState(UINT, UINT)
   { Default(); }
   UINT window::OnQueryUIState()
   { return (UINT)Default(); }

   // window dialog data support
   //    void window::do_data_exchange(CDataExchange*)
   //   { } // default does nothing

   // window modality support

   void window::BeginModalState()
   {

      ::EnableWindow(get_handle(), FALSE);

   }

   void window::EndModalState()
   {

      ::EnableWindow(get_handle(), TRUE);

   }

   // frame_window
   /*    void frame_window::DelayUpdateFrameTitle()
   { m_nIdleFlags |= idleTitle; }
   void frame_window::DelayRecalcLayout(bool bNotify)
   { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
   bool frame_window::InModalState() const
   { return m_cModalStack != 0; }
   void frame_window::set_title(const char * lpszTitle)
   { m_strTitle = lpszTitle; }
   string frame_window::get_title() const
   { return m_strTitle; }
   */



   void window::CloseWindow()
   { ASSERT(::IsWindow(get_handle())); ::CloseWindow(get_handle()); }

   bool window::OpenIcon()
   {

      ASSERT(::IsWindow(get_handle())); 

      return ::OpenIcon(get_handle()) != FALSE; 

   }

   ////////////////////////////////////////////////////////////////////////////
   // UI related ::ca::window functions

   oswindow window::get_safe_owner(oswindow hParent, oswindow* pWndTop)
   {
      // get ::ca::window to start with
      oswindow oswindow = hParent;
      if (oswindow == ::null())
      {
         /* trans      sp(frame_window) pFrame = command_target::GetRoutingFrame_();
         if (pFrame != ::null())
         oswindow = pFrame->get_handle();
         else
         oswindow = System.GetMainWnd()->get_handle();*/
      }

      // a popup ::ca::window cannot be owned by a child ::ca::window
      while (oswindow != ::null() && (::GetWindowLong(oswindow, GWL_STYLE) & WS_CHILD))
         oswindow = ::GetParent(oswindow);

      // determine toplevel ::ca::window to disable as well
      ::oswindow oswindow_Top = oswindow;
      ::oswindow oswindow_Temp = oswindow;
      for (;;)
      {
         if (oswindow_Temp == ::null())
            break;
         else
            oswindow_Top = oswindow_Temp;
         oswindow_Temp = ::GetParent(oswindow_Top);
      }

      // get last active popup of first non-child that was found
      if (hParent == ::null() && oswindow != ::null())
         oswindow = ::GetLastActivePopup(oswindow);

      // disable and store top level parent ::ca::window if specified
      if (pWndTop != ::null())
      {
         if (oswindow_Top != ::null() && ::IsWindowEnabled(oswindow_Top) && oswindow_Top != oswindow)
         {
            *pWndTop = oswindow_Top;
            ::EnableWindow(oswindow_Top, FALSE);
         }
         else
            *pWndTop = ::null();
      }

      return oswindow;    // return the owner as oswindow
   }

   void window::on_delete(sp(::ca::ca) pui)
   {
      UNREFERENCED_PARAMETER(pui);
   }


   /////////////////////////////////////////////////////////////////////////////
   // Official way to send message to a window

   CLASS_DECL_win LRESULT __call_window_procedure(sp(::user::interaction) pinteraction, oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam)
   {
      ___THREAD_STATE* pThreadState = __get_thread_state();
      MSG oldState = pThreadState->m_lastSentMsg;   // save for nesting
      pThreadState->m_lastSentMsg.hwnd = oswindow;
      pThreadState->m_lastSentMsg.message = nMsg;
      pThreadState->m_lastSentMsg.wParam = wParam;
      pThreadState->m_lastSentMsg.lParam = lParam;

      // Catch exceptions thrown outside the scope of a callback
      // in debug builds and warn the ::fontopus::user.
      ::ca::smart_pointer < ::ca::message::base > spbase;

      spbase = pinteraction->get_base(pinteraction, nMsg, wParam, lParam);

      __trace_message("WndProc", spbase);

      try
      {

         // special case for WM_INITDIALOG
         rect rectOld;
         uint32_t dwStyle = 0;
         if (nMsg == WM_INITDIALOG)
            __pre_init_dialog(pinteraction, &rectOld, &dwStyle);

         // delegate to object's message_handler
         if(pinteraction->m_pguie != ::null() && pinteraction->m_pguie != pinteraction)
         {
            pinteraction->m_pguie->message_handler(spbase);
         }
         else
         {
            pinteraction->message_handler(spbase);
         }
         // more special case for WM_INITDIALOG
         if (nMsg == WM_INITDIALOG)
            __post_init_dialog(pinteraction, rectOld, dwStyle);
      }
      catch(::exit_exception &)
      {

         Sys(pinteraction->m_papp).os().post_to_all_threads(WM_QUIT, 0, 0);

         return -1;

      }
      catch(const ::ca::exception & e)
      {

         if(!App(pinteraction->m_papp).on_run_exception((::ca::exception &) e))
         {

            Sys(pinteraction->m_papp).os().post_to_all_threads(WM_QUIT, 0, 0);

            return -1;

         }

         return -1;

      }
      catch(base_exception * pe)
      {
         __process_window_procedure_exception(pe, spbase);
         //         TRACE(::ca::trace::category_AppMsg, 0, "Warning: Uncaught exception in message_handler (returning %ld).\n", spbase->get_lresult());
         pe->Delete();
      }
      catch(...)
      {
      }

      try
      {
         pThreadState->m_lastSentMsg = oldState;
         LRESULT lresult = spbase->get_lresult();
         return lresult;
      }
      catch(...)
      {
         return 0;
      }
   }


   /*CDataExchange::CDataExchange(sp(::ca::window) pDlgWnd, bool bSaveAndValidate)
   {
   ASSERT_VALID(pDlgWnd);
   m_bSaveAndValidate = bSaveAndValidate;
   m_pDlgWnd = pDlgWnd;
   m_idLastControl = 0;
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // Window creation hooks

   LRESULT CALLBACK __cbt_filter_hook(int32_t code, WPARAM wParam, LPARAM lParam)
   {
      ___THREAD_STATE* pThreadState = __get_thread_state();
      if (code != HCBT_CREATEWND)
      {
         // wait for HCBT_CREATEWND just pass others on...
         return CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
            wParam, lParam);
      }

      ASSERT(lParam != ::null());
      LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
      ASSERT(lpcs != ::null());

      sp(::win::window) pWndInit = pThreadState->m_pWndInit;
      bool bContextIsDLL = afxContextIsDLL;
      if (pWndInit != ::null() || (!(lpcs->style & WS_CHILD) && !bContextIsDLL))
      {
         // Note: special check to avoid subclassing the IME window
         //if (gen_DBCS)
         {
            // check for cheap CS_IME style first...
            if (GetClassLong((oswindow) wParam, GCL_STYLE) & CS_IME)
               goto lCallNextHook;

            // get class name of the window that is being created
            const char * pszClassName;
            char szClassName[_countof("ime")+1];
            if (uint_ptr(lpcs->lpszClass) > 0xffff)
            {
               pszClassName = lpcs->lpszClass;
            }
            else
            {
               szClassName[0] = '\0';
               GlobalGetAtomName((ATOM)lpcs->lpszClass, szClassName, _countof(szClassName));
               pszClassName = szClassName;
            }

            // a little more expensive to test this way, but necessary...
            if (::__invariant_stricmp(pszClassName, "ime") == 0)
               goto lCallNextHook;
         }

         ASSERT(wParam != ::null()); // should be non-::null() oswindow
         ::oswindow oswindow = (::oswindow) wParam;
         WNDPROC oldWndProc;
         if (pWndInit != ::null())
         {
            // the window should not be in the permanent map at this time
            ASSERT(::win::window::FromHandlePermanent(oswindow) == ::null());

            pWndInit->m_pthread = dynamic_cast < ::ca::thread * > (::win::get_thread());
            if(pWndInit->m_pthread != ::null())
            {
               pWndInit->m_pthread->m_pthread->add(pWndInit);
            }
            pWndInit->m_pguie->m_pthread = pWndInit->m_pthread;
            if(pWndInit->m_pguie->m_pthread != ::null())
            {
               pWndInit->m_pguie->m_pthread->m_pthread->add(pWndInit->m_pguie);
            }
            pWndInit->m_pguie->m_pimpl = pWndInit;

            // connect the oswindow to pWndInit...
            pWndInit->attach(oswindow);
            // allow other subclassing to occur first
            pWndInit->pre_subclass_window();

            WNDPROC *pOldWndProc = pWndInit->GetSuperWndProcAddr();
            ASSERT(pOldWndProc != ::null());

            // subclass the window with standard __window_procedure
            WNDPROC afxWndProc = __get_window_procedure();
            oldWndProc = (WNDPROC)SetWindowLongPtr(oswindow, GWLP_WNDPROC,
               (uint_ptr)afxWndProc);
            ASSERT(oldWndProc != ::null());
            if (oldWndProc != afxWndProc)
               *pOldWndProc = oldWndProc;

            pThreadState->m_pWndInit = ::null();
         }
         else
         {
            ASSERT(!bContextIsDLL);   // should never get here

            static ATOM s_atomMenu = 0;
            bool bSubclass = true;         

            if (s_atomMenu == 0)
            {
               WNDCLASSEX wc;
               memset(&wc, 0, sizeof(WNDCLASSEX));
               wc.cbSize = sizeof(WNDCLASSEX);
               s_atomMenu = (ATOM)::GetClassInfoEx(::null(), "#32768", &wc);
            }

            // Do not subclass menus.
            if (s_atomMenu != 0)
            {
               ATOM atomWnd = (ATOM)::GetClassLongPtr(oswindow, GCW_ATOM);
               if (atomWnd == s_atomMenu)
                  bSubclass = false;
            }
            else
            {         
               char szClassName[256];
               if (::GetClassName(oswindow, szClassName, 256))
               {
                  szClassName[255] = NULL;
                  if (_tcscmp(szClassName, "#32768") == 0)
                     bSubclass = false;
               }
            }         
            if (bSubclass)
            {
               // subclass the window with the proc which does gray backgrounds
               oldWndProc = (WNDPROC)GetWindowLongPtr(oswindow, GWLP_WNDPROC);
               if (oldWndProc != ::null() && GetProp(oswindow, gen_OldWndProc) == ::null())
               {
                  SetProp(oswindow, gen_OldWndProc, oldWndProc);
                  if ((WNDPROC)GetProp(oswindow, gen_OldWndProc) == oldWndProc)
                  {
                     GlobalAddAtom(gen_OldWndProc);
                     SetWindowLongPtr(oswindow, GWLP_WNDPROC, (uint_ptr)__activation_window_procedure);
                     ASSERT(oldWndProc != ::null());
                  }
               }
            }
         }
      }

lCallNextHook:
      LRESULT lResult = CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
         wParam, lParam);

      return lResult;
   }



   void window::_001OnEraseBkgnd(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::erase_bkgnd, perasebkgnd, pobj);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(TRUE);
   }

   void window::_001BaseWndInterfaceMap()
   {
      System.user()->window_map().set((int_ptr)get_handle(), this);
   }


   void window::_001OnTriggerMouseInside()
   {

      m_bMouseHover = true;
      TRACKMOUSEEVENT tme = { sizeof(tme) };
      tme.dwFlags = TME_LEAVE;
      tme.hwndTrack = get_handle();
      TrackMouseEvent(&tme);

   }



   void window::set_view_port_org(::ca::graphics * pgraphics)
   {

      // graphics will be already set its view port to the window for linux - cairo with xlib

      pgraphics->SetViewportOrg(point(0, 0));
      pgraphics->SelectClipRgn(::null());

   }


} // namespace win


CTestCmdUI::CTestCmdUI(sp(::ca::application) papp) :
   ca(papp),
   cmd_ui(papp)
{
   m_bEnabled = TRUE;  // assume it is enabled
}

void CTestCmdUI::Enable(bool bOn)
{
   m_bEnabled = bOn;
   m_bEnableChanged = TRUE;
}

void CTestCmdUI::SetCheck(int32_t)
{
   // do nothing -- just want to know about calls to Enable
}

void CTestCmdUI::SetRadio(bool)
{
   // do nothing -- just want to know about calls to Enable
}

void CTestCmdUI::SetText(const char *)
{
   // do nothing -- just want to know about calls to Enable
}


/////////////////////////////////////////////////////////////////////////////
// Map from oswindow to sp(::ca::window)

oswindow_map* get_oswindow_map(bool bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      __MODULE_STATE* pState = __get_module_state();
      if(pState == ::null())
         return ::null();
      return pState->m_pmapHWND;
   }
   catch(...)
   {
      return ::null();
   }
}


mutex * afxMutexoswindow_()
{
   try
   {
      __MODULE_STATE* pState = __get_module_state();
      if(pState == ::null())
         return ::null();
      return pState->m_pmutexoswindow_;
   }
   catch(...)
   {
      return ::null();
   }
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
   sp(::ca::window) pWnd = ::win::window::FromHandlePermanent(oswindow);
   //ASSERT(pWnd != ::null());               
   //ASSERT(pWnd==::null() || WIN_WINDOW(pWnd)->get_handle() == oswindow);
   if (pWnd == ::null() || WIN_WINDOW(pWnd)->get_handle() != oswindow)
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
   ASSERT(lpRectOld != ::null());   
   ASSERT(pdwStyleOld != ::null());

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
   if (pParent != ::null() && pParent->IsWindowEnabled())
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

   if (pThreadState->m_hHookOldCbtFilter == ::null())
   {
      pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, win::__cbt_filter_hook, ::null(), ::GetCurrentThreadId());
      if (pThreadState->m_hHookOldCbtFilter == ::null())
         throw memory_exception(pwindow->get_app());
   }

   ASSERT(pThreadState->m_hHookOldCbtFilter != ::null());
   
   ASSERT(pwindow != ::null());
   
   ASSERT(WIN_WINDOW(pwindow)->get_handle() == ::null());   // only do once

   ASSERT(pThreadState->m_pWndInit == ::null());   // hook not already in progress

   pThreadState->m_pWndInit = pwindow;

}

CLASS_DECL_win bool unhook_window_create()
{
   ___THREAD_STATE* pThreadState = __get_thread_state();
   if (pThreadState->m_pWndInit != ::null())
   {
      pThreadState->m_pWndInit = ::null();
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

   if (hCursor == ::null() && hbrBackground == ::null() && hIcon == ::null())
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca:::%p:%x", hInst, nClassStyle));
   }
   else
   {
      C_RUNTIME_ERRORCHECK_SPRINTF(_sntprintf_s(lpszName, ___TEMP_CLASS_NAME_SIZE, ___TEMP_CLASS_NAME_SIZE - 1, "::ca:::%p:%x:%p:%p:%p", hInst, nClassStyle,
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
   wndcls.hCursor = ::null();
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = ::null();
   wndcls.lpszClassName = lpszName;
   if (!__register_class(&wndcls))
      throw resource_exception(::ca::get_thread_app());

   // return thread-local pointer
   return lpszName;
}


__STATIC void CLASS_DECL_win
   __handle_activate(sp(::ca::window) pWnd, WPARAM nState, sp(::ca::window) pWndOther)
{
   ASSERT(pWnd != ::null());      

   // send WM_ACTIVATETOPLEVEL when top-level parents change
   if (!(WIN_WINDOW(pWnd)->GetStyle() & WS_CHILD))
   {
      sp(::user::interaction) pTopLevel= WIN_WINDOW(pWnd)->GetTopLevelParent();
      if (pTopLevel && (pWndOther == ::null() || !::IsWindow(WIN_WINDOW(pWndOther)->get_handle()) || pTopLevel != WIN_WINDOW(pWndOther)->GetTopLevelParent()))
      {
         // lParam points to window getting the WM_ACTIVATE message and
         //  oswindow_Other from the WM_ACTIVATE.
         oswindow oswindow_2[2];
         oswindow_2[0] = WIN_WINDOW(pWnd)->get_handle();
         if(pWndOther == ::null() || WIN_WINDOW(pWndOther) == ::null())
         {
            oswindow_2[1] = ::null();
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
   __handle_set_cursor(sp(::ca::window) pWnd, UINT nHitTest, UINT nMsg)
{
   if (nHitTest == HTERROR &&
      (nMsg == WM_LBUTTONDOWN || nMsg == WM_MBUTTONDOWN ||
      nMsg == WM_RBUTTONDOWN))
   {
      // activate the last active window if not active
      sp(::user::interaction) pLastActive = WIN_WINDOW(pWnd)->GetTopLevelParent();      
      if (pLastActive != ::null())
         pLastActive = pLastActive->GetLastActivePopup();
      if (pLastActive != ::null() &&
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
   pWndCls->hIcon = ::LoadIcon(::null(), IDI_APPLICATION);
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
      if(ppszClass != ::null())
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
   memset(&wndcls, 0, sizeof(WNDCLASS));   // start with ::null() defaults
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
         if(ppszClass != ::null())
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
         if(ppszClass != ::null())
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
         if(ppszClass != ::null())
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
      wndcls.hbrBackground = ::null();
      /*      if (__register_with_icon(&wndcls, gen_WndMDIFrame, __IDI_STD_MDIFRAME))
      {
      if(ppszClass != ::null())
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
   ASSERT(oldWndProc != ::null());   

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
            sp(::ca::window) pWnd = ::win::window::from_handle(oswindow);
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
      //      TRACE(::ca::trace::category_AppMsg, 0, "Warning: Uncaught exception in __activation_window_procedure (returning %ld).\n",
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
      //      TRACE(::ca::trace::category_AppMsg, 0, "Can't register window class named %s\n",
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
         if(pModuleState->m_pstrUnregisterList == ::null())
            pModuleState->m_pstrUnregisterList = new string;
         *pModuleState->m_pstrUnregisterList += lpWndClass->lpszClassName;
         *pModuleState->m_pstrUnregisterList +='\n';
      }
      catch(base_exception * pe)
      {
         ::ca::rethrow(pe);
         // Note: DELETE_EXCEPTION not required.
      }

   }

   return bRet;
}


#undef new
#include <gdiplus.h>


namespace win
{

   /*   void window::_001DeferPaintLayeredWindowBackground(::ca::graphics * pdc)
   {


   }*/

   void window::_001UpdateWindow()
   {

      if(m_pcolorref == ::null())
         return;

      single_lock sl(mutex_graphics(), false);

      if(!sl.lock(millis(84)))
         return;



      {

         memset(m_pcolorref, 0, (size_t) (m_size.area() * 4));

         _001Print(m_spg);


      }

      if(GetExStyle() & WS_EX_LAYERED)
      {
         BYTE *dst=(BYTE*)m_pcolorref;
         int64_t size = m_size.area();


         // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255


         while (size >= 8)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);

            dst[4+0] = LOBYTE(((int32_t)dst[4+0] * (int32_t)dst[4+3])>> 8);
            dst[4+1] = LOBYTE(((int32_t)dst[4+1] * (int32_t)dst[4+3])>> 8);
            dst[4+2] = LOBYTE(((int32_t)dst[4+2] * (int32_t)dst[4+3])>> 8);

            dst[8+0] = LOBYTE(((int32_t)dst[8+0] * (int32_t)dst[8+3])>> 8);
            dst[8+1] = LOBYTE(((int32_t)dst[8+1] * (int32_t)dst[8+3])>> 8);
            dst[8+2] = LOBYTE(((int32_t)dst[8+2] * (int32_t)dst[8+3])>> 8);

            dst[12+0] = LOBYTE(((int32_t)dst[12+0] * (int32_t)dst[12+3])>> 8);
            dst[12+1] = LOBYTE(((int32_t)dst[12+1] * (int32_t)dst[12+3])>> 8);
            dst[12+2] = LOBYTE(((int32_t)dst[12+2] * (int32_t)dst[12+3])>> 8);

            dst[16+0] = LOBYTE(((int32_t)dst[16+0] * (int32_t)dst[16+3])>> 8);
            dst[16+1] = LOBYTE(((int32_t)dst[16+1] * (int32_t)dst[16+3])>> 8);
            dst[16+2] = LOBYTE(((int32_t)dst[16+2] * (int32_t)dst[16+3])>> 8);

            dst[20+0] = LOBYTE(((int32_t)dst[20+0] * (int32_t)dst[20+3])>> 8);
            dst[20+1] = LOBYTE(((int32_t)dst[20+1] * (int32_t)dst[20+3])>> 8);
            dst[20+2] = LOBYTE(((int32_t)dst[20+2] * (int32_t)dst[20+3])>> 8);

            dst[24+0] = LOBYTE(((int32_t)dst[24+0] * (int32_t)dst[24+3])>> 8);
            dst[24+1] = LOBYTE(((int32_t)dst[24+1] * (int32_t)dst[24+3])>> 8);
            dst[24+2] = LOBYTE(((int32_t)dst[24+2] * (int32_t)dst[24+3])>> 8);

            dst[28+0] = LOBYTE(((int32_t)dst[28+0] * (int32_t)dst[28+3])>> 8);
            dst[28+1] = LOBYTE(((int32_t)dst[28+1] * (int32_t)dst[28+3])>> 8);
            dst[28+2] = LOBYTE(((int32_t)dst[28+2] * (int32_t)dst[28+3])>> 8);

            dst += 4 * 8;
            size -= 8;
         }
         while(size--)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);
            dst += 4;
         }


         {

            HDC hdcScreen = ::GetDC(get_handle());

            HDC hdcMem = ::CreateCompatibleDC(::null());

            HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdcMem, m_hbitmap);

            BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

            POINT ptZero = { 0 };

            point ptSrc(0, 0);

            bool bOk = ::UpdateLayeredWindow(get_handle(), hdcScreen, &m_pt, &m_size, hdcMem, &ptSrc, RGB(0, 0, 0), &blendPixelFunction, ULW_ALPHA) != FALSE;

            ::SelectObject(hdcMem, hbitmapOld);

            ::DeleteDC(hdcMem);

            ::ReleaseDC(get_handle(), hdcScreen);

         }


      }
      else
      {

         {
            HDC hdcScreen = ::GetDC(get_handle());

            HDC hdcMem = ::CreateCompatibleDC(::null());

            HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdcMem, m_hbitmap);

            BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

            POINT ptZero = { 0 };

            point ptSrc(0, 0);

            ::BitBlt(hdcScreen, 0, 0, m_size.cx, m_size.cy, hdcMem, 0, 0, SRCCOPY);

            ::SelectObject(hdcMem, hbitmapOld);

            ::DeleteDC(hdcMem);

            ::ReleaseDC(get_handle(), hdcScreen);
         }

      }

   }

}



