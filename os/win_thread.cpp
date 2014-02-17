#include "framework.h"
#include <ddeml.h>  // for MSGF_DDEMGR

namespace win
{
   class thread;
} // namespace win

bool CLASS_DECL_win __internal_pump_message();
LRESULT CLASS_DECL_win __internal_process_wnd_proc_exception(::exception::base*, const MSG* pMsg);
bool __internal_pre_translate_message(MSG* pMsg);
bool __internal_is_idle_message(MSG* pMsg);
__STATIC void CLASS_DECL_win __pre_init_dialog(sp(::user::interaction) pWnd, LPRECT lpRectOld, uint32_t * pdwStyleOld);
__STATIC void CLASS_DECL_win __post_init_dialog(sp(::user::interaction) pWnd, const RECT& rectOld, uint32_t dwStyleOld);

namespace core
{

   thread_startup::thread_startup()
   {
   }

   thread_startup::~thread_startup()
   {
   }

} // namespace core





/////////////////////////////////////////////////////////////////////////////
// Thread entry point

#ifdef _MT

struct ___THREAD_STARTUP : ::core::thread_startup
{
   // following are "in" parameters to thread startup
   ___THREAD_STATE* pThreadState;    // thread state of parent thread
   ::win::thread * pThread;    // thread for new thread
   DWORD dwCreateFlags;    // thread creation flags
   _PNH pfnNewHandler;     // new handler for new thread

   //HANDLE hEvent;          // event triggered after success/non-success
   //HANDLE hEvent2;         // event triggered after thread is resumed

   // strictly "out" -- set after hEvent is triggered
   bool bError;    // TRUE if error during startup
};

uint32_t __thread_entry(void * pParam)
{
   UINT uiRet = 0;

   try
   {

      ___THREAD_STARTUP* pStartup = (___THREAD_STARTUP*)pParam;
      ASSERT(pStartup != NULL);
      ASSERT(pStartup->pThreadState != NULL);
      ASSERT(pStartup->pThread != NULL);
      //ASSERT(pStartup->hEvent != NULL);
      ASSERT(!pStartup->bError);


      ::win::thread * pThread = pStartup->pThread;


      ::CoInitialize(NULL);


      pThread->::exception::translator::attach();

      try
      {
         // inherit parent's module state
         ___THREAD_STATE* pThreadState = __get_thread_state();
         pThreadState->m_pModuleState = pStartup->pThreadState->m_pModuleState;

         // set current thread pointer for System.GetThread
         __MODULE_STATE* pModuleState = __get_module_state();
         __MODULE_THREAD_STATE* pState = pModuleState->m_thread;
         pState->m_pCurrentWinThread = pThread;

         // forced initialization of the thread
         __init_thread();

         // thread inherits cast's main ::user::window if not already set
         //if (papp != NULL && GetMainWnd() == NULL)
         {
            // just attach the oswindow
            // trans         threadWnd.attach(pApp->GetMainWnd()->get_handle());
            //GetMainWnd() = pApp->GetMainWnd();
         }
      }
      catch(::exception::base *)
      {
         // Note: DELETE_EXCEPTION(e) not required.

         // exception happened during thread initialization!!
         //TRACE(::core::trace::category_AppMsg, 0, "Warning: Error during thread initialization!\n");

         // set error flag and allow the creating thread to notice the error
         //         threadWnd.detach();
         pStartup->bError = TRUE;
         VERIFY(::SetEvent(pStartup->hEvent));
         __end_thread((pThread->m_pbaseapp), (UINT)-1, FALSE);
         ASSERT(FALSE);  // unreachable
      }

      {
         single_lock sl(&::win::thread::s_mutex, true);
         ::win::thread::s_haThread.add(::GetCurrentThread());
         ::win::thread::s_threadptra.add(pThread);
      }


      pThread->thread_entry(pStartup);

      // pStartup is invlaid after the following
      // SetEvent (but hEvent2 is valid)
      HANDLE hEvent2 = pStartup->hEvent2;

      // allow the creating thread to return from thread::create_thread
      VERIFY(::SetEvent(pStartup->hEvent));

      // wait for thread to be resumed
      VERIFY(::WaitForSingleObject(hEvent2, INFINITE) == WAIT_OBJECT_0);
      ::CloseHandle(hEvent2);

      int32_t n;

      try
      {

         n = pThread->m_p->main();

      }
      catch(::exit_exception &)
      {

         Sys(pThread->get_app()).os().post_to_all_threads(WM_QUIT, 0, 0);

         return -1;

      }

      uiRet =  pThread->thread_term(n);


   }
   catch(...)
   {
      return -1;
   }
   
   if(__get_thread_state()  != NULL)
   {
      __get_thread_state()->finalize();
      gen_ThreadState = NULL;
   }

   return uiRet;

}

#endif //_MT

CLASS_DECL_win ::win::thread * __get_thread()
{
   // check for current thread in module thread state
   __MODULE_THREAD_STATE* pState = __get_module_thread_state();
   ::win::thread* pThread = pState->m_pCurrentWinThread;
   return pThread;
}


CLASS_DECL_win void __set_thread(thread * pthread)
{
   // check for current thread in module thread state
   __MODULE_THREAD_STATE* pState = __get_module_thread_state();
   pState->m_pCurrentWinThread = dynamic_cast < ::win::thread * > (pthread->thread::m_p.m_p);
}



CLASS_DECL_win MSG * __get_current_message()
{
   ___THREAD_STATE* pState = __get_thread_state();
   ASSERT(pState);
   return &(pState->m_msgCur);
}



CLASS_DECL_win void __internal_process_wnd_proc_exception(::exception::base*, signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   if (pbase->m_uiMessage == WM_CREATE)
   {
      pbase->set_lresult(-1);
      return;  // just fail
   }
   else if (pbase->m_uiMessage == WM_PAINT)
   {
      // force validation of ::user::window to prevent getting WM_PAINT again
      ValidateRect(pbase->m_pwnd->get_safe_handle(), NULL);
      pbase->set_lresult(0);
      return;
   }
   return;   // sensible default for rest of commands
}

CLASS_DECL_win void __process_window_procedure_exception(::exception::base* e, signal_details * pobj)
{
   thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->ProcessWndProcException( e, pobj );
   else
      return __internal_process_wnd_proc_exception( e, pobj );
}

void __internal_pre_translate_message(signal_details * pobj)
{
   try
   {
      SCAST_PTR(::message::base, pbase, pobj);

      //   ASSERT_VALID(this);

      thread * pthread = ::win::get_thread();
      if(pthread != NULL)
      {
         // if this is a thread-message, int16_t-circuit this function
         if (pbase->m_pwnd == NULL)
         {
            pthread->DispatchThreadMessageEx(pobj);
            if(pobj->m_bRet)
               return;
         }
      }

      sp(::user::interaction) puiTopic = pbase->m_pwnd->m_pguie;

      try
      {
         if(pthread->m_pbaseapp->m_pbasesession != NULL &&
            &pthread->m_pbaseapp->m_pbasesession->frames() != NULL)
         {
            try
            {
               for(int32_t i = 0; i < pthread->m_pbaseapp->m_pbasesession->frames().get_count(); i++)
               {
                  try
                  {
                     sp(::user::interaction) pui = pthread->m_pbaseapp->m_pbasesession->frames()(i);
                     if(pui != NULL)
                     {
                        if(pui->m_pguie != NULL)
                        {
                           pui->m_pguie->pre_translate_message(pobj);
                           if(pobj->m_bRet)
                              return;
                        }
                        else
                        {
                           pui->pre_translate_message(pobj);
                           if(pobj->m_bRet)
                              return;
                        }
                     }
                  }
                  catch(exit_exception & e)
                  {
                     throw e;
                  }
                  catch(...)
                  {
                  }
               }
            }
            catch(exit_exception & e)
            {
               throw e;
            }
            catch(...)
            {
            }
         }
      }
      catch(exit_exception & e)
      {
         throw e;
      }
      catch(...)
      {
      }
   }
   catch(exit_exception & e)
   {
      throw e;
   }
   catch(...)
   {
   }

   // no special processing
}

void __cdecl __pre_translate_message(signal_details * pobj)
{
   thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->pre_translate_message( pobj );
   else
      return __internal_pre_translate_message( pobj );
}

bool __internal_is_idle_message(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   // Return FALSE if the message just dispatched should _not_
   // cause on_idle to be run.  Messages which do not usually
   // affect the state of the ::fontopus::user interface and happen very
   // often are checked for.

   if(pbase == NULL)
      return FALSE;

   // redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
   if (pbase->m_uiMessage == WM_MOUSEMOVE || pbase->m_uiMessage == WM_NCMOUSEMOVE)
   {
      // mouse move at same position as last mouse move?
      ___THREAD_STATE *pState = __get_thread_state();
      point ptCursor;
      App(pobj->get_app()).get_cursor_pos(&ptCursor);
      if (pState->m_ptCursorLast == ptCursor && pbase->m_uiMessage == pState->m_nMsgLast)
         return FALSE;

      pState->m_ptCursorLast = ptCursor;  // remember for next time
      pState->m_nMsgLast = pbase->m_uiMessage;
      return TRUE;
   }

   // WM_PAINT and WM_SYSTIMER (caret blink)
   return pbase->m_uiMessage != WM_PAINT && pbase->m_uiMessage != 0x0118;
}



bool __internal_is_idle_message(LPMSG lpmsg)
{
   // Return FALSE if the message just dispatched should _not_
   // cause on_idle to be run.  Messages which do not usually
   // affect the state of the ::fontopus::user interface and happen very
   // often are checked for.

   if(lpmsg == NULL)
      return FALSE;

   // redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
   if (lpmsg->message == WM_MOUSEMOVE || lpmsg->message == WM_NCMOUSEMOVE)
   {
      // mouse move at same position as last mouse move?
      ___THREAD_STATE *pState = __get_thread_state();
      if (pState->m_ptCursorLast == lpmsg->pt && lpmsg->message == pState->m_nMsgLast)
         return FALSE;

      pState->m_ptCursorLast = lpmsg->pt;  // remember for next time
      pState->m_nMsgLast = lpmsg->message;
      return TRUE;
   }

   // WM_PAINT and WM_SYSTIMER (caret blink)
   return lpmsg->message != WM_PAINT && lpmsg->message != 0x0118;
}

bool __cdecl __is_idle_message(signal_details * pobj)
{
   thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->is_idle_message(pobj);
   else
      return __internal_is_idle_message(pobj);
}

bool __cdecl __is_idle_message(MSG* pMsg)
{
   win::thread * pThread = __get_thread();
   if(pThread)
      return pThread->is_idle_message( pMsg );
   else
      return __internal_is_idle_message( pMsg );
}


/*thread* CLASS_DECL_win __begin_thread(sp(base_application) papp, __THREADPROC pfnThreadProc, LPVOID pParam,
int32_t nPriority, UINT nStackSize, DWORD dwCreateFlags,
LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
ASSERT(pfnThreadProc != NULL);

thread* pThread = BASE_NEW thread(papp, pfnThreadProc, pParam);
ASSERT_VALID(pThread);

if (!pThread->create_thread(dwCreateFlags|CREATE_SUSPENDED, nStackSize,
lpSecurityAttrs))
{
pThread->Delete();
return NULL;
}
VERIFY(pThread->SetThreadPriority(nPriority));
if (!(dwCreateFlags & CREATE_SUSPENDED))
VERIFY(pThread->ResumeThread() != (DWORD)-1);

return pThread;
}*/
void CLASS_DECL_win __end_thread(sp(base_application) papp, UINT nExitCode, bool bDelete)
{
   // remove current thread object from primitive::memory
   __MODULE_THREAD_STATE* pState = __get_module_thread_state();
   ::win::thread* pThread = pState->m_pCurrentWinThread;
   if (pThread != NULL)
   {

      {
         single_lock sl(&::win::thread::s_mutex, true);
         ::win::thread::s_haThread.remove(::GetCurrentThread());
         ::win::thread::s_threadptra.remove(pThread);
      }

      ASSERT_VALID(pThread);
      //ASSERT(pThread != System::smart_pointer < sp(base_application)>::m_p);

      if (bDelete)
         pThread->Delete();
      pState->m_pCurrentWinThread = NULL;
   }

   // allow cleanup of any thread local objects
   __term_thread(papp);

}

extern thread_local_storage * __thread_data;

void CLASS_DECL_win __term_thread(sp(base_application) papp, HINSTANCE hInstTerm)
{
   UNREFERENCED_PARAMETER(papp);
   try
   {
      // cleanup thread local tooltip window
      if (hInstTerm == NULL)
      {
         //         __MODULE_THREAD_STATE* pModuleThreadState = __get_module_thread_state();
      }
   }
   catch( ::exception::base* e )
   {
      e->Delete();
   }

   try
   {
      // cleanup the rest of the thread local data
      if (__thread_data != NULL)
         __thread_data->delete_data();
   }
   catch( ::exception::base* e )
   {
      e->Delete();
   }
}


/////////////////////////////////////////////////////////////////////////////
// Global functions for thread initialization and thread cleanup

LRESULT CALLBACK __message_filter_hook(int32_t code, WPARAM wParam, LPARAM lParam);

void CLASS_DECL_win __init_thread()
{
   if (!afxContextIsDLL)
   {
      // set message filter proc
      ___THREAD_STATE* pThreadState = __get_thread_state();
      ASSERT(pThreadState->m_hHookOldMsgFilter == NULL);
      pThreadState->m_hHookOldMsgFilter = ::SetWindowsHookEx(WH_MSGFILTER,
         __message_filter_hook, NULL, ::GetCurrentThreadId());
   }
}

namespace win
{


   comparable_array < HANDLE > thread::s_haThread;
   comparable_array < thread * > thread::s_threadptra;
   mutex thread::s_mutex(NULL);



   void thread::set_p(::thread * p)
   {
      m_p = p;
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread construction


   void thread::construct(__THREADPROC pfnThreadProc, LPVOID pParam)
   {
      m_evFinish.SetEvent();
      if(System.GetThread() != NULL)
      {
         m_pAppThread = __get_thread()->m_pAppThread;
      }
      else
      {
         m_pAppThread = NULL;
      }
      m_pfnThreadProc = pfnThreadProc;
      m_pThreadParams = pParam;

      CommonConstruct();
   }

   thread::thread(sp(base_application) papp) :
      element(papp),
      message_queue(papp),//,
      //m_evFinish(FALSE, TRUE)
      ::thread(NULL),
      m_evFinish(papp),
      m_mutexUiPtra(papp)
      
   {
      m_evFinish.SetEvent();
      m_pAppThread = dynamic_cast < thread * > (papp.m_p);
      m_pThreadParams = NULL;
      m_pfnThreadProc = NULL;

      CommonConstruct();
   }

   void thread::CommonConstruct()
   {
      m_bCreatingMessageWindow = false;
      m_ptimera      = NULL; 
      m_puiptra      = NULL;
      m_puiMain      = NULL;
      m_puiActive    = NULL;


      m_nDisablePumpCount  = 0;

      // no HTHREAD until it is created
      m_hThread = NULL;
      m_nThreadID = 0;

      ___THREAD_STATE* pState = __get_thread_state();
      // initialize message pump
      m_nDisablePumpCount = 0;
      pState->m_nMsgLast = WM_NULL;
      System.get_cursor_pos(&(pState->m_ptCursorLast));

      // most threads are deleted when not needed
      m_bAutoDelete  = TRUE;

//      m_frameList.Construct(offsetof(::user::frame_window, m_pNextFrameWnd));
      m_ptimera = canew(::user::interaction::timer_array(get_app()));
      m_puiptra = canew(::user::interaction_ptr_array(get_app()));

   }


   thread::~thread()
   {
      /*
      if(m_puiptra != NULL)
      {
         single_lock sl(&m_mutexUiPtra, TRUE);
         ::user::interaction_ptr_array * puiptra = m_puiptra;
         m_puiptra = NULL;
         for(int32_t i = 0; i < puiptra->get_size(); i++)
         {
            sp(::user::interaction) pui = puiptra->element_at(i);
            if(pui->m_pthread != NULL)
            {
               try
               {
                  if(WIN_THREAD(pui->m_pthread) == this 
                     || WIN_THREAD(pui->m_p) == WIN_THREAD(m_p)
                     || WIN_THREAD(pui->m_pthread) == WIN_THREAD(m_p))
                  {
                     pui->m_pthread = NULL;
                  }
               }
               catch(...)
               {
               }
               try
               {
                  sp(::user::interaction) puie = pui->m_pguie;
                  if(puie != NULL 
                     && puie->m_pthread != NULL
                     && puie->m_pthread != NULL
                     && (WIN_THREAD(puie->m_pthread) == this 
                     || WIN_THREAD(puie->m_p) == WIN_THREAD(m_p)
                     || WIN_THREAD(puie->m_pthread) == WIN_THREAD(m_p)))
                  {
                     puie->m_pthread = NULL;
                  }
               }
               catch(...)
               {
               }
            }
         }
         sl.unlock();

         single_lock slWinThreadMutex(&::win::thread::s_mutex, true);
         ::win::thread::s_haThread.remove(m_hThread);
         ::win::thread::s_threadptra.remove(this);

      }
      */
      __MODULE_THREAD_STATE* pState = __get_module_thread_state();
      /*      // clean up temp objects
      pState->m_pmapHGDIOBJ->delete_temp();
      pState->m_pmapHDC->delete_temp();
      pState->m_pmapHWND->delete_temp();*/

      // free thread object
      if (m_hThread != NULL)
         CloseHandle(m_hThread);

      // cleanup module state
      if (pState->m_pCurrentWinThread == this)
         pState->m_pCurrentWinThread = NULL;

      window::DeleteTempMap();
      //      graphics::DeleteTempMap();
      //    object::DeleteTempMap();

      try
      {
         // cleanup temp/permanent maps (just the maps themselves)
         //         delete m_pmapHDC;
         //       delete m_pmapHGDIOBJ;
      }
      catch(...)
      {
      }

   }


   void * thread::get_os_data() const
   {

      return (void *) m_hThread;

   }


   int_ptr thread::get_os_int() const
   {

      return m_nThreadID;

   }


   HANDLE thread::item() const
   {
      return m_hThread;
   }

   bool thread::begin(int32_t epriority, uint_ptr nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {

      if(!create_thread(epriority, dwCreateFlags, nStackSize, lpSecurityAttrs))
      {
         Delete();
         return false;
      }

      return true;

   }

   sp(::user::interaction) thread::SetMainWnd(sp(::user::interaction) pui)
   {
      sp(::user::interaction) puiPrevious = m_puiMain;
      m_puiMain  = pui;
      return puiPrevious;
   }

   void thread::add(sp(::user::interaction) pui)
   {
      single_lock sl(&m_mutexUiPtra, TRUE);
      if(m_puiptra != NULL)
      {
         m_puiptra->add(pui);
      }
   }

   void thread::remove(::user::interaction * pui)
   {
      if(pui == NULL)
         return;
      if(GetMainWnd() == pui)
      {
         SetMainWnd(NULL);
      }
      single_lock sl(&m_mutexUiPtra, TRUE);
      if(m_puiptra != NULL)
      {
         m_puiptra->remove(pui);
         m_puiptra->remove(pui->m_pguie);
         m_puiptra->remove(pui->m_pimpl);
      }
      sl.unlock();
      if(m_ptimera != NULL)
      {
         m_ptimera->unset(pui);
         m_ptimera->unset(pui->m_pguie);
         m_ptimera->unset(pui->m_pimpl);
      }

      try
      {
         if(WIN_THREAD(pui->m_pthread.m_p) == this)
         {
            pui->m_pthread = NULL;
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pimpl != NULL && pui->m_pimpl != pui)
         {
            if(WIN_THREAD(pui->m_pimpl->m_pthread.m_p) == this)
            {
               pui->m_pimpl->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pguie != NULL && pui->m_pguie != pui)
         {
            if(WIN_THREAD(pui->m_pguie->m_pthread.m_p) == this)
            {
               pui->m_pguie->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
   }

   ::count thread::get_ui_count()
   {
      single_lock sl(&m_mutexUiPtra, TRUE);
      return m_puiptra->get_count();
   }

   sp(::user::interaction) thread::get_ui(index iIndex)
   {
      single_lock sl(&m_mutexUiPtra, TRUE);
      return m_puiptra->element_at(iIndex);
   }

   void thread::_001PostCreateMessageWindow()
   {
      post_thread_message(WM_USER + 123);
   }

   void thread::_001OnCreateMessageWindow(signal_details * pobj)
   {
      
      if(m_bCreatingMessageWindow)
         return;
      
      if(m_spuiMessage.is_set() && m_spuiMessage->IsWindow())
         return;
      
      keeper < bool > keepCreating(&m_bCreatingMessageWindow, true, false, true);

      try
      {

         if(!create_message_queue(get_app(), ""))
            return;

      }
      catch(...)
      {
         return;
      }

      if(m_spuiMessage->IsWindow())
      {
         single_lock sl(&m_ptimera->m_mutex, TRUE);
         int32_t iMin = 100;
         for(int32_t i = 0; i < m_ptimera->m_timera.get_count(); i++)
         {
            if(m_ptimera->m_timera.element_at(i)->m_uiElapse < natural(iMin))
            {
               iMin = m_ptimera->m_timera.element_at(i)->m_uiElapse;
            }
         }
         sl.unlock();
         m_spuiMessage->SetTimer((uint_ptr)-2, iMin, NULL);
      }

   }


   void thread::set_timer(sp(::user::interaction) pui, uint_ptr nIDEvent, UINT nEllapse)
   {

      m_ptimera->set(pui, nIDEvent, nEllapse);

      if(!m_bCreatingMessageWindow && m_spuiMessage.is_null())
      {
         _001PostCreateMessageWindow();
      }

   }

   void thread::unset_timer(sp(::user::interaction) pui, uint_ptr nIDEvent)
   {
      m_ptimera->unset(pui, nIDEvent);
   }

   void thread::set_auto_delete(bool bAutoDelete)
   {
      m_bAutoDelete = bAutoDelete;
   }

   void thread::set_run(bool bRun)
   {
      m_bRun = bRun;
   }

   event & thread::get_finish_event()
   {
      return m_evFinish;
   }

   bool thread::get_run()
   {
      return m_bRun;
   }

   ::thread * thread::get_app_thread()
   {
      return m_pAppThread;
   }

   sp(::user::interaction) thread::get_active_ui()
   {
      return m_puiActive;
   }

   sp(::user::interaction) thread::set_active_ui(sp(::user::interaction) pui)
   {
      sp(::user::interaction) puiPrevious = m_puiActive;
      m_puiActive = pui;
      return puiPrevious;
   }

   void thread::step_timer()
   {
      if(m_ptimera == NULL)
         return;
      m_ptimera->check();
   }

   bool thread::create_thread(int32_t epriority, uint32_t dwCreateFlagsParam, uint_ptr nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {

      DWORD dwCreateFlags = dwCreateFlagsParam;

      if(epriority != ::core::scheduling_priority_normal)
      {
         dwCreateFlags |= CREATE_SUSPENDED;
      }

      ENSURE(m_hThread == NULL);  // already created?

      // setup startup structure for thread initialization
      ___THREAD_STARTUP startup; 
      startup.bError = FALSE;
      startup.pfnNewHandler = NULL;
      //memset(&startup, 0, sizeof(startup));
      startup.pThreadState = __get_thread_state();
      startup.pThread = this;
      startup.m_pthread = NULL;
      startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
      startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
      startup.dwCreateFlags = dwCreateFlags;
      if (startup.hEvent == NULL || startup.hEvent2 == NULL)
      {
         TRACE(::core::trace::category_AppMsg, 0, "Warning: CreateEvent failed in thread::create_thread.\n");
         if (startup.hEvent != NULL)
            ::CloseHandle(startup.hEvent);
         if (startup.hEvent2 != NULL)
            ::CloseHandle(startup.hEvent2);
         return FALSE;
      }

      m_hThread = (HANDLE) (uint_ptr) ::create_thread(lpSecurityAttrs, nStackSize,  &__thread_entry, &startup, dwCreateFlags | CREATE_SUSPENDED, &m_nThreadID);

      if (m_hThread == NULL)
         return FALSE;

      // start the thread just for core API initialization
      VERIFY(::ResumeThread(m_hThread) != (DWORD)-1);
      VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
      ::CloseHandle(startup.hEvent);

      // if created suspended, suspend it until resume thread wakes it up
      if (dwCreateFlags & CREATE_SUSPENDED)
         VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);

      // if error during startup, shut things down
      if (startup.bError)
      {
         VERIFY(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0);
         ::CloseHandle(m_hThread);
         m_hThread = NULL;
         ::CloseHandle(startup.hEvent2);
         return FALSE;
      }

      // allow thread to continue, once resumed (it may already be resumed)
      ::SetEvent(startup.hEvent2);

      if(epriority != ::core::scheduling_priority_normal)
      {

         VERIFY(set_thread_priority(epriority));

         if (!(dwCreateFlagsParam & CREATE_SUSPENDED))
         {
            ENSURE(ResumeThread() != (DWORD)-1);
         }

      }

      return TRUE;

   }

   void thread::Delete()
   {
      if(m_bAutoDelete)
      {
         try
         {
            if(m_pappDelete != NULL)
               delete m_pappDelete;
         }
         catch(...)
         {
         }
      }
      else
      {
         try
         {
            m_hThread = NULL;
         }
         catch(...)
         {
         }
         try
         {
            m_evFinish.SetEvent();
         }
         catch(...)
         {
         }
      }
      try
      {
         if(m_p != NULL)
         {
            ::thread * pthread = thread::m_p;
            if(pthread != NULL && pthread->m_pbReady != NULL)
            {
               *pthread->m_pbReady = true;
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(m_pbReady != NULL)
         {
            *m_pbReady = true;
         }
      }
      catch(...)
      {
      }
      try
      {
         m_evFinish.SetEvent();
      }
      catch(...)
      {
      }

      if(m_bAutoDelete)
      {
         // delete thread if it is auto-deleting
         //pthread->smart_pointer < thread >::m_p = NULL;
         m_p.release();
         // delete_this();
      }
      else
      {
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread default implementation

   bool thread::PreInitInstance()
   {
      return true;
   }

   bool thread::initialize_instance()
   {
      ASSERT_VALID(this);

      return true;   // by default enter run loop
   }

   // main running routine until thread exits
   int32_t thread::run()
   {

      ASSERT_VALID(this);
      //      ___THREAD_STATE* pState = __get_thread_state();

      // for tracking the idle time state
      bool bIdle = TRUE;
      LONG lIdleCount = 0;

      // acquire and dispatch messages until a WM_QUIT message is received.
      MSG msg;
      while(m_bRun)
      {
         // phase1: check to see if we can do idle work
         while (bIdle &&
            !::PeekMessage(&msg, NULL, 0,0, PM_NOREMOVE))
         {
            // call on_idle while in bIdle state
            if (!on_idle(lIdleCount++))
               bIdle = FALSE; // assume "no idle" state


            if (!m_p->on_run_step())
               goto stop_run;

         }

         // phase2: pump messages while available
         do
         {

            // pump message, but quit on WM_QUIT
            if(!m_bRun || !pump_message())
            {
               try
               {
                  return exit();
               }
               catch(...)
               {
                  return -1;
               }
            }

            // reset "no idle" state after pumping "normal" message
            //if (is_idle_message(&m_msgCur))
            if (is_idle_message(&msg))
            {
               bIdle = TRUE;
               lIdleCount = 0;
            }

            if (!m_p->on_run_step())
               goto stop_run;

         }
         while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != FALSE);

      }
stop_run:

      return 0;
   }

   bool thread::is_idle_message(signal_details * pobj)
   {
      return __internal_is_idle_message(pobj);
   }

   bool thread::is_idle_message(LPMSG lpmsg)
   {
      return __internal_is_idle_message(lpmsg);
   }

   void thread::delete_temp()
   {

      //      object::DeleteTempMap();
      //    graphics::DeleteTempMap();
      window::DeleteTempMap();

   }


   int32_t thread::exit_instance()
   {
      ASSERT_VALID(this);


      try
      {
#ifdef DEBUG
         // Check for missing LockTempMap calls
         if(m_nTempMapLock != 0)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Temp map lock ::count non-zero (%ld).\n", m_nTempMapLock);
         }
         LockTempMaps();
         UnlockTempMaps(true);
#endif
      }
      catch(...)
      {
      }


      try
      {
         if(m_puiptra != NULL)
         {
            single_lock sl(&m_mutexUiPtra, TRUE);
            ::user::interaction_ptr_array * puiptra = m_puiptra;
            m_puiptra = NULL;
            for(int32_t i = 0; i < puiptra->get_size(); i++)
            {
               sp(::user::interaction) pui = puiptra->element_at(i);
               if(pui->m_pthread != NULL)
               {
                  if(WIN_THREAD(pui->m_pthread.m_p) == this 
                     || WIN_THREAD(pui->m_pthread->m_p.m_p) == WIN_THREAD(m_p.m_p)
                     || WIN_THREAD(pui->m_pthread.m_p) == WIN_THREAD(m_p.m_p))
                  {
                     pui->m_pthread = NULL;
                  }
               }
            }
            delete puiptra;
            sl.unlock();
         }
      }
      catch(...)
      {
      }

      try
      {
         ::user::interaction::timer_array * ptimera = m_ptimera;
         m_ptimera = NULL;
         delete ptimera;
      }
      catch(...)
      {
      }



      int32_t nResult = (int32_t)__get_current_message()->wParam;  // returns the value from PostQuitMessage
      return nResult;
   }

   bool thread::on_idle(LONG lCount)
   {



      ASSERT_VALID(this);

#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      // check core API's allocator (before idle)
      if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
         ASSERT(__check_memory());
#endif

      if(lCount <= 0 && m_puiptra != NULL)
      {
         for(int32_t i = 0; i < m_puiptra->get_count();)
         {
            sp(::user::interaction) pui = m_puiptra->element_at(i);
            bool bOk = false;
            try
            {
               
               bOk = pui != NULL && pui->IsWindowVisible();
            }
            catch(...)
            {
            }
            if (!bOk)
            {
               m_puiptra->remove_at(i);
            }
            else
            {
               try
               {
               /*__call_window_procedure(pMainWnd, pMainWnd->get_handle(),
               WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
               pui->send_message(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
               /*   pui->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
               (WPARAM)TRUE, 0, TRUE, TRUE);*/
               }
               catch (...)
               {

               }
               i++;
            }
         }


         // send WM_IDLEUPDATECMDUI to the main window
         /*
         sp(::user::interaction) pMainWnd = GetMainWnd();
         if (pMainWnd != NULL && pMainWnd->IsWindowVisible())
         {
         /*__call_window_procedure(pMainWnd, pMainWnd->get_handle(),
         WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
         /* pMainWnd->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
         pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
         (WPARAM)TRUE, 0, TRUE, TRUE);
         }
         */
         // send WM_IDLEUPDATECMDUI to all frame windows
         /* linux __MODULE_THREAD_STATE* pState = ___CMDTARGET_GETSTATE()->m_thread;
         sp(::user::frame_window) pFrameWnd = pState->m_frameList;
         while (pFrameWnd != NULL)
         {
         if (pFrameWnd->get_handle() != NULL && pFrameWnd != pMainWnd)
         {
         if (pFrameWnd->m_nShowDelay == SW_HIDE)
         pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
         if (pFrameWnd->IsWindowVisible() ||
         pFrameWnd->m_nShowDelay >= 0)
         {
         __call_window_procedure(pFrameWnd, pFrameWnd->get_handle(),
         WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
         pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
         (WPARAM)TRUE, 0, TRUE, TRUE);
         }
         if (pFrameWnd->m_nShowDelay > SW_HIDE)
         pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
         pFrameWnd->m_nShowDelay = -1;
         }
         pFrameWnd = pFrameWnd->m_pNextFrameWnd;
         }*/
      }
      else if (lCount >= 0)
      {
         /*         __MODULE_THREAD_STATE* pState = __get_module_thread_state();
         if (pState->m_nTempMapLock == 0)
         {
         // free temp maps, OLE DLLs, etc.
         ::core::LockTempMaps((m_p->m_pbaseapp));
         ::core::UnlockTempMaps((m_p->m_pbaseapp));
         }*/
      }

#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      // check core API's allocator (after idle)
      if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
         ASSERT(__check_memory());
#endif



      return lCount < 0;  // nothing more to do if lCount >= 0
   }

   message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      UNREFERENCED_PARAMETER(uiMessage);
      UNREFERENCED_PARAMETER(uiCode);
      return message::PrototypeNone;
   }


   void thread::DispatchThreadMessageEx(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(!pbase->m_bRet && pbase->m_uiMessage == WM_APP + 1984 && pbase->m_wparam == 77)
      {
         smart_pointer < ::user::message > spmessage(pbase->m_lparam);
         spmessage->send();
         pbase->m_uiMessage   = 0;    // ssshhhh.... - self-healing - sh...
         pbase->m_wparam      = 0;    // ssshhhh.... - self-healing - sh...
         pbase->m_bRet        = true;
         return;
      }
      /*   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
      const __MSGMAP_ENTRY* lpEntry;

      for (/* pMessageMap already init'ed *//*; pMessageMap->pfnGetBaseMap != NULL;
      pMessageMap = (*pMessageMap->pfnGetBaseMap)())
      {
      // Note: catch not so common but fatal mistake!!
      //       // BEGIN_MESSAGE_MAP(CMyThread, CMyThread)

      ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
      if (pMsg->message < 0xC000)
      {
      // constant window message
      if ((lpEntry = ::core::FindMessageEntry(pMessageMap->lpEntries,
      pMsg->message, 0, 0)) != NULL)
      goto LDispatch;
      }
      else
      {
      // registered windows message
      lpEntry = pMessageMap->lpEntries;
      while ((lpEntry = ::core::FindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
      {
      UINT* pnID = (UINT*)(lpEntry->nSig);
      ASSERT(*pnID >= 0xC000);
      // must be successfully registered
      if (*pnID == pMsg->message)
      goto LDispatch;
      lpEntry++;      // keep looking past this one
      }
      }
      }
      return FALSE;

      LDispatch:
      union MessageMapFunctions mmf;
      mmf.pfn = lpEntry->pfn;

      // always posted, so return value is meaningless

      (this->*mmf.pfn_THREAD)(pMsg->wParam, pMsg->lParam);*/

      LRESULT lresult;
      SignalPtrArray signalptra;
      m_signala.GetSignalsByMessage(signalptra, pbase->m_uiMessage, 0, 0);
      for(int32_t i = 0; i < signalptra.get_size(); i++)
      {
         Signal & signal = *signalptra[i];
         ::signal * psignal = signal.m_psignal;
         message::e_prototype eprototype = signal.m_eprototype;
         if(eprototype == message::PrototypeNone)
         {
            //message::base base(get_app());
            pbase->m_psignal = psignal;
            lresult = 0;
            //base.set(pmsg->message, pmsg->wParam, pmsg->lParam, lresult);
            psignal->emit(pbase);
            if(pbase->m_bRet)
               return;
         }
         break;
      }
      pbase->m_bRet = true;
   }

   void thread::pre_translate_message(signal_details * pobj)
   {
      ASSERT_VALID(this);
      return __internal_pre_translate_message(pobj);
   }

   void thread::ProcessWndProcException(::exception::base* e, signal_details * pobj)
   {
      return __internal_process_wnd_proc_exception(e, pobj);
   }

   __STATIC inline bool IsEnterKey(signal_details * pobj)
   { 
      SCAST_PTR(::message::base, pbase, pobj);
      return pbase->m_uiMessage == WM_KEYDOWN && pbase->m_wparam == VK_RETURN; 
   }

   __STATIC inline bool IsButtonUp(signal_details * pobj)
   { 
      SCAST_PTR(::message::base, pbase, pobj);
      return pbase->m_uiMessage == WM_LBUTTONUP; 
   }

   void thread::ProcessMessageFilter(int32_t code, signal_details * pobj)
   {

      if(pobj == NULL)
         return;   // not handled

      SCAST_PTR(::message::base, pbase, pobj);

      sp(::user::frame_window) pTopFrameWnd;
      sp(::user::interaction) pMainWnd;
      sp(::user::interaction) pMsgWnd;
      switch (code)
      {
      case MSGF_DDEMGR:
         // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
         //  never call the next hook.
         // By returning FALSE, the message will be dispatched
         //  instead (the default behavior).
         return;

      case MSGF_MENU:
         pMsgWnd = pbase->m_pwnd;
         if (pMsgWnd != NULL)
         {
            pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
            if (pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
               pTopFrameWnd->m_bHelpMode)
            {
               pMainWnd = __get_main_window();
               if ((GetMainWnd() != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
               {
                  //                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
                  pbase->m_bRet = true;
                  return;
               }
            }
         }
         // fall through...

      case MSGF_DIALOGBOX:    // handles message boxes as well.
         pMainWnd = __get_main_window();
         if (code == MSGF_DIALOGBOX && m_puiActive != NULL &&
            pbase->m_uiMessage >= WM_KEYFIRST && pbase->m_uiMessage <= WM_KEYLAST)
         {
            // need to translate messages for the in-place container
            ___THREAD_STATE* pThreadState = __get_thread_state();
            ENSURE(pThreadState);

            if (pThreadState->m_bInMsgFilter)
               return;
            pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
            if (m_puiActive->IsWindowEnabled())
            {
               pre_translate_message(pobj);
               if(pobj->m_bRet)
               {
                  pThreadState->m_bInMsgFilter = FALSE;
                  return;
               }
            }
            pThreadState->m_bInMsgFilter = FALSE;    // ok again
         }
         break;
      }
      // default to not handled
   }

   /////////////////////////////////////////////////////////////////////////////
   // Access to GetMainWnd() & m_pActiveWnd

   sp(::user::interaction) thread::GetMainWnd()
   {
      if (m_puiActive != NULL)
         return m_puiActive;    // probably in-place active

      // when not inplace active, just return main window
      if (m_puiMain != NULL)
         return m_puiMain;

      return System.get_active_guie();
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread implementation helpers

   bool thread::pump_message()
   {
      try
      {
         MSG msg;
         if(!::GetMessage(&msg, NULL, 0, 0))
         {
            TRACE(::core::trace::category_AppMsg, 1, "thread::pump_message - Received WM_QUIT.\n");
            m_nDisablePumpCount++; // application must die
            // Note: prevents calling message loop things in 'exit_instance'
            // will never be decremented
            return FALSE;
         }

         if(m_nDisablePumpCount != 0)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Error: thread::pump_message called when not permitted.\n");
            ASSERT(FALSE);
         }

         __trace_message("pump_message", &msg);

         if(msg.message != WM_KICKIDLE)
         {

            {

               smart_pointer < message::base > spbase;

               spbase = get_base(&msg);

               if(spbase.is_set())
               {

                  try
                  {
                     if(m_p != NULL)
                     {
                        m_p->pre_translate_message(spbase);
                        if(spbase->m_bRet)
                           return TRUE;
                     }
                  }
                  catch(exit_exception & e)
                  {
                     throw e;
                  }
                  catch(...)
                  {
                  }

                  if(true)
                  {
                     try
                     {
                        if(m_pbaseapp != NULL)
                        {
                           try
                           {
                              if(m_pbaseapp->m_pbasesystem != NULL)
                              {
                                 m_pbaseapp->m_pbasesystem->pre_translate_message(spbase);
                                 if(spbase->m_bRet)
                                    return TRUE;
/*                                 try
                                 {
                                    if(m_pbaseapp->m_pbasesystem->m_pcube != NULL)
                                    {
                                       m_pbaseapp->m_pbasesystem->m_pcubeInterface->pre_translate_message(spbase);
                                       if(spbase->m_bRet)
                                          return TRUE;
                                    }
                                 }
                                 catch(...)
                                 {
                                 }

                                 */
                              }
                           }
                           catch(...)
                           {
                           }
                           if(m_pbaseapp->m_pbasesession != NULL)
                           {
                              try
                              {
                                 m_pbaseapp->m_pbasesession->pre_translate_message(spbase);
                                 if(spbase->m_bRet)
                                    return TRUE;
                              }
                              catch(...)
                              {
                              }
/*                              try
                              {
                                 if(m_pbaseapp->m_pbasesession->m_pbergedge != NULL)
                                 {
                                    m_pbaseapp->m_pbasesession->m_pbergedgeInterface->pre_translate_message(spbase);
                                    if(spbase->m_bRet)
                                       return TRUE;
                                 }
                              }
                              catch(...)
                              {
                              }*/
                           }
                        }
                     }
                     catch(...)
                     {
                     }
                     try
                     {
                        if(!m_pbaseapp->is_system() && m_pbaseapp->is_session())
                        {
                           m_pbaseapp->pre_translate_message(spbase);
                           if(spbase->m_bRet)
                              return TRUE;
                        }
                     }
                     catch(...)
                     {
                     }

                  }



                  __pre_translate_message(spbase);
                  if(spbase->m_bRet)
                     return TRUE;

                  spbase.release();
               }
            }
            {
               ::TranslateMessage(&msg);
               ::DispatchMessage(&msg);
            }
         }
         return TRUE;
      }
      catch(exit_exception & e)
      {
         throw e;
      }
      catch(const ::exception::exception & e)
      {
         if(on_run_exception((::exception::exception &) e))
            return TRUE;
         // get_app() may be it self, it is ok...
         if(App(get_app()).final_handle_exception((::exception::exception & ) e))
            return TRUE;
         return FALSE;
      }
      catch(...)
      {
         return FALSE;
      }
   }


   /////////////////////////////////////////////////////////////////////////////
   // thread diagnostics


   void thread::assert_valid() const
   {
      command_target::assert_valid();
   }
   void thread::dump(dump_context & dumpcontext) const
   {
      command_target::dump(dumpcontext);
      ___THREAD_STATE *pState = __get_thread_state();

      dumpcontext << "m_pThreadParams = " << m_pThreadParams;
      dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
      dumpcontext << "\nm_bAutoDelete = " << m_bAutoDelete;
      dumpcontext << "\nm_hThread = " << (void *)m_hThread;
      dumpcontext << "\nm_nThreadID = " << m_nThreadID;
#ifdef DEBUG
      dumpcontext << "\nm_nDisablePumpCount = " << pState->m_nDisablePumpCount;
#endif
      if (__get_thread() == this)
         dumpcontext << "\nm_pMainWnd = " << m_puiMain.m_p;

      dumpcontext << "\nm_msgCur = {";
      dumpcontext << "\n\toswindow = " << (void *)pState->m_msgCur.hwnd;
      dumpcontext << "\n\tmessage = " << (UINT)pState->m_msgCur.message;
      dumpcontext << "\n\twParam = " << (UINT)pState->m_msgCur.wParam;
      dumpcontext << "\n\tlParam = " << (void *)pState->m_msgCur.lParam;
      dumpcontext << "\n\ttime = " << (uint_ptr) pState->m_msgCur.time;
      dumpcontext << "\n\tpt = " << point(pState->m_msgCur.pt);
      dumpcontext << "\n}";

      dumpcontext << "\nm_pThreadParams = " << m_pThreadParams;
      dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
      dumpcontext << "\nm_ptCursorLast = " << pState->m_ptCursorLast;
      dumpcontext << "\nm_nMsgLast = " << pState->m_nMsgLast;

      dumpcontext << "\n";
   }


   bool thread::post_message(sp(::user::interaction) pguie, UINT uiMessage, WPARAM wparam, lparam lparam)
   {
      if(m_hThread == NULL)
         return false;
      ::user::message * pmessage = new ::user::message;
      pmessage->m_pguie       = pguie;
      pmessage->m_uiMessage   = uiMessage;
      pmessage->m_wparam      = wparam;
      pmessage->m_lparam      = lparam;
      return post_thread_message(WM_APP + 1984, 77, (LPARAM) pmessage) != FALSE;
   }


   bool thread::on_run_exception(::exception::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      return false;
   }


   void thread::message_handler(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      // special message which identifies the window as using __window_procedure
//      if(pbase->m_uiMessage == WM_QUERYAFXWNDPROC)
  //    {
    //     pbase->set_lresult(0);
      //   return;
     // }

      // all other messages route through message map
      sp(::user::window) pwindow = pbase->m_pwnd->get_wnd();

      ASSERT(pwindow == NULL || pwindow == pbase->m_pwnd->m_pimpl);

      if(pwindow == NULL || pwindow != pbase->m_pwnd->m_pimpl)
      {
         pbase->set_lresult(::DefWindowProc(pbase->m_pwnd->get_safe_handle(), pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }

      ___THREAD_STATE* pThreadState = __get_thread_state();
      MSG oldState = pThreadState->m_lastSentMsg;   // save for nesting
      pThreadState->m_lastSentMsg.hwnd       = pbase->m_pwnd->get_safe_handle();
      pThreadState->m_lastSentMsg.message    = pbase->m_uiMessage;
      pThreadState->m_lastSentMsg.wParam     = pbase->m_wparam;
      pThreadState->m_lastSentMsg.lParam     = pbase->m_lparam;

      __trace_message("message_handler", pobj);

      // Catch exceptions thrown outside the scope of a callback
      // in debug builds and warn the ::fontopus::user.
      try
      {

         // special case for WM_INITDIALOG
         rect rectOld;
         uint32_t dwStyle = 0;
         if(pbase->m_uiMessage == WM_INITDIALOG)
            __pre_init_dialog(pwindow, &rectOld, &dwStyle);

         // delegate to object's message_handler
         if(pwindow->m_pguie != NULL && pwindow->m_pguie != pwindow)
         {
            pwindow->m_pguie->message_handler(pobj);
         }
         else
         {
            pwindow->message_handler(pobj);
         }

         // more special case for WM_INITDIALOG
         if(pbase->m_uiMessage == WM_INITDIALOG)
            __post_init_dialog(pwindow, rectOld, dwStyle);
      }
      catch(const ::exception::exception & e)
      {
         if(App(get_app()).on_run_exception((::exception::exception &) e))
            goto run;
         if(App(get_app()).final_handle_exception((::exception::exception &) e))
            goto run;
         __post_quit_message(-1);
         pbase->set_lresult(-1);
         return;
      }
      catch(::exception::base * pe)
      {
         __process_window_procedure_exception(pe, pbase);
         TRACE(::core::trace::category_AppMsg, 0, "Warning: Uncaught exception in message_handler (returning %ld).\n", pbase->get_lresult());
         pe->Delete();
      }
run:
      pThreadState->m_lastSentMsg = oldState;
   }


   thread::operator HANDLE() const
   { 

      return this == NULL ? NULL : m_hThread; 

   }

   bool thread::set_thread_priority(int32_t iCa2Priority)
   { 

      ASSERT(m_hThread != NULL); 

      int32_t nPriority = (int) get_os_thread_priority(iCa2Priority);


      bool bOk = ::SetThreadPriority(m_hThread, get_os_thread_priority(iCa2Priority))  != FALSE; 

      if(!bOk)
      {
         uint32_t dwLastError = ::GetLastError();
         ::OutputDebugString("thread::SetThreadPriority LastError = " + ::str::from(dwLastError));
      }

      return bOk;

   }


   int32_t thread::get_thread_priority()
   { 

      ASSERT(m_hThread != NULL); 

      int32_t nPriority = ::GetThreadPriority(m_hThread);

      int32_t iCa2Priority = ::get_os_thread_scheduling_priority(nPriority);

      return iCa2Priority;

   }


   uint32_t thread::ResumeThread()
   {
   
      ASSERT(m_hThread != NULL); 
      
      return ::ResumeThread(m_hThread); 
   
   }


   /*

   DWORD thread::SuspendThread()
   {
   
      ASSERT(m_hThread != NULL); 
      
      return ::SuspendThread(m_hThread); 
      
   }

   */


   bool thread::post_thread_message(UINT message, WPARAM wParam, lparam lParam)
   {

      if(m_hThread == NULL)
         return false;

      return ::PostThreadMessageA(m_nThreadID, message, wParam, lParam)  != FALSE;

   }


   void thread::set_os_data(void * pvoidOsData)
   {
      m_hThread = (HANDLE) pvoidOsData;
   }

   void thread::set_os_int(int_ptr iData)
   {
      m_nThreadID = (DWORD) iData;
   }

   void thread::message_queue_message_handler(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   CLASS_DECL_win ::thread * get_thread()
   {
      ::win::thread * pwinthread = __get_thread();
      if(pwinthread == NULL)
         return NULL;
      return pwinthread->m_p;
   }

   CLASS_DECL_win ::thread_state * get_thread_state()
   {
      return __get_thread_state();
   }

   void thread::LockTempMaps()
   {
      ++m_nTempMapLock;
   }
   bool thread::UnlockTempMaps(bool bDeleteTemp)
   {
      if (m_nTempMapLock != 0 && --m_nTempMapLock == 0)
      {
         if (bDeleteTemp)
         {
            // clean up temp objects
            //         object::DeleteTempMap();
            //       graphics::DeleteTempMap();
            window::DeleteTempMap();
         }



#ifndef ___PORTABLE
         sp(base_application) papp = (get_app());
         ___THREAD_STATE* pThreadState = __get_thread_state();
         if( pThreadState != NULL )
         {
            // restore safety pool after temp objects destroyed
            if(papp != NULL &&
               (pThreadState->m_pSafetyPoolBuffer == NULL ||
               _msize(pThreadState->m_pSafetyPoolBuffer) < papp->m_nSafetyPoolSize) &&
               papp->m_nSafetyPoolSize != 0)
            {
               // attempt to restore the safety pool to its max size
               size_t nOldSize = 0;
               if (pThreadState->m_pSafetyPoolBuffer != NULL)
               {
                  nOldSize = _msize(pThreadState->m_pSafetyPoolBuffer);
                  free(pThreadState->m_pSafetyPoolBuffer);
               }

               // undo handler trap for the following allocation
               //bool bEnable = __enable_memory_tracking(FALSE);
               try
               {
                  pThreadState->m_pSafetyPoolBuffer = malloc(papp->m_nSafetyPoolSize);
                  if (pThreadState->m_pSafetyPoolBuffer == NULL)
                  {
                     //                  TRACE(::core::trace::category_AppMsg, 0, "Warning: failed to reclaim %d bytes for primitive::memory safety pool.\n",
                     //                   pApp->m_nSafetyPoolSize);
                     // at least get the old buffer back
                     if (nOldSize != 0)
                     {
                        //get it back
                        pThreadState->m_pSafetyPoolBuffer = malloc(nOldSize);
                        ASSERT(pThreadState->m_pSafetyPoolBuffer != NULL);
                     }
                  }
               }
               catch( ::exception::base * )
               {
                  //__enable_memory_tracking(bEnable);
                  throw;
               }
               //__enable_memory_tracking(bEnable);
            }
         }
#endif  // !___PORTABLE
      }
      // return TRUE if temp maps still locked
      return m_nTempMapLock != 0;
   }

   int32_t thread::thread_entry(::core::thread_startup * pstartup)
   {

      ___THREAD_STARTUP* pStartup = (___THREAD_STARTUP*)pstartup;
      ASSERT(pStartup != NULL);
      ASSERT(pStartup->pThreadState != NULL);
      ASSERT(pStartup->pThread != NULL);
      ASSERT(!pStartup->bError);

      ::win::thread* pThread = pStartup->pThread;

      //      sp(base_application) papp = (get_app());
      m_evFinish.ResetEvent();
      install_message_handling(pThread);
      m_p->install_message_handling(pThread);

      IGUI_WIN_MSG_LINK(WM_USER + 123, pThread, pThread, &thread::_001OnCreateMessageWindow);

      return 0;   // not reached

   }

   int32_t thread::main()
   {

      /*      ___THREAD_STARTUP* pStartup = (___THREAD_STARTUP*)pstartup;
      ASSERT(pStartup != NULL);
      ASSERT(pStartup->pThreadState != NULL);
      ASSERT(pStartup->pThread != NULL);
      ASSERT(!pStartup->bError);*/

      if(!m_p->PreInitInstance())
      {
         return 0;
      }

      // first -- check for simple worker thread
      DWORD nResult = 0;
      if (m_pfnThreadProc != NULL)
      {
         nResult = (*m_pfnThreadProc)(m_pThreadParams);
      }
      // else -- check for thread with message loop
      else if (!m_p->initialize_instance())
      {
         try
         {
            nResult = exit();
         }
         catch(...)
         {
            nResult = (DWORD) -1;
         }
      }
      else
      {
         // will stop after PostQuitMessage called
         ASSERT_VALID(this);
         // let upper framework attach translator    
         //         translator::attach();
run:
         try
         {
            m_bReady = true;
            m_p->m_bReady = true;
            m_bRun = true;
            m_p->m_bRun = true;
            nResult = m_p->run();
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(const ::exception::exception & e)
         {
            if(on_run_exception((::exception::exception &) e))
               goto run;
            if(App(get_app()).final_handle_exception((::exception::exception & ) e))
               goto run;
            try
            {
               nResult = exit();
            }
            catch(...)
            {
               nResult = (DWORD) -1;
            }
         }
         catch(...)
         {
         }
         // let translator run undefinetely
         //translator::detach();
      }



      return 0;   // not reached
   }

   int32_t thread::thread_term(int32_t nResult)
   {
      try
      {
         destroy_message_queue();
      }
      catch(...)
      {
      }

      try
      {
         // cleanup and shutdown the thread
         //         threadWnd.detach();
         __end_thread((m_pbaseapp), nResult);
      }
      catch(...)
      {
      }
      return nResult;
   }


   /**
   * \file		src/lib/pal/windows/thread_windows.hpp
   * \brief	Platform independent threads and synchronization objects (windows version)
   */

   ///  \brief		starts thread on first call
   void thread::start()
   {
      ::ResumeThread(item());
   }


   void thread::wait()
   {
      ::WaitForSingleObject(item(),INFINITE);
   }

   ///  \brief		waits for signaling the thread for a specified time
   ///  \param		duration time period to wait for thread
   ///  \return	result of waiting action as defined in wait_result
   wait_result thread::wait(const duration & duration)
   {
      DWORD timeout = duration.is_pos_infinity() ? INFINITE : static_cast<DWORD>(duration.total_milliseconds());
      return wait_result((uint32_t) ::WaitForSingleObject(item(),timeout));
   }

   ///  \brief		sets thread priority
   ///  \param		new priority
   void thread::set_priority(int32_t priority)
   {
      if ( ::SetThreadPriority(item(), priority) == 0)
         throw runtime_error(get_app(), "Thread::set_priority: Couldn't set thread priority.");
   }

   ///  \brief		gets thread priority
   ///  \param		priority
   int32_t thread::priority()
   { 
      return ::GetThreadPriority(item());
   }


   bool thread::has_message()
   {
      ASSERT(GetCurrentThreadId() == m_nThreadID);
      MSG msg;
      return ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != FALSE;
   }


} // namespace win




bool CLASS_DECL_win __internal_pump_message();
LRESULT CLASS_DECL_win __internal_process_wnd_proc_exception(::exception::base*, const MSG* pMsg);
void __internal_pre_translate_message(signal_details * pobj);
bool __internal_is_idle_message(signal_details * pobj);
bool __internal_is_idle_message(LPMSG lpmsg);


/*thread* CLASS_DECL_win System.GetThread()
{
// check for current thread in module thread state
__MODULE_THREAD_STATE* pState = __get_module_thread_state();
//thread* pThread = pState->m_pCurrentWinThread;
return pThread;
}

MSG* CLASS_DECL_win __get_current_message()
{
___THREAD_STATE* pState = __get_thread_state();
ASSERT(pState);
return &(pState->m_msgCur);
}

bool CLASS_DECL_win __internal_pump_message()
{
___THREAD_STATE *pState = __get_thread_state();

if (!::GetMessage(&(pState->m_msgCur), NULL, NULL, NULL))
{
#ifdef DEBUG
TRACE(::core::trace::category_AppMsg, 1, "thread::pump_message - Received WM_QUIT.\n");
pState->m_nDisablePumpCount++; // application must die
#endif
// Note: prevents calling message loop things in 'exit_instance'
// will never be decremented
return FALSE;
}

#ifdef DEBUG
if (pState->m_nDisablePumpCount != 0)
{
TRACE(::core::trace::category_AppMsg, 0, "Error: thread::pump_message called when not permitted.\n");
ASSERT(FALSE);
}
#endif

#ifdef DEBUG
__trace_message("pump_message", &(pState->m_msgCur));
#endif

// process this message

if (pState->m_msgCur.message != WM_KICKIDLE && !__pre_translate_message(&(pState->m_msgCur)))
{
::TranslateMessage(&(pState->m_msgCur));
::DispatchMessage(&(pState->m_msgCur));
}
return TRUE;
}

bool CLASS_DECL_win ::core::PumpMessage()
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->pump_message();
else
return __internal_pump_message();
}

LRESULT CLASS_DECL_win __internal_process_wnd_proc_exception(::exception::base*, const MSG* pMsg)
{
if (pMsg->message == WM_CREATE)
{
return -1;  // just fail
}
else if (pMsg->message == WM_PAINT)
{
// force validation of ::user::window to prevent getting WM_PAINT again
ValidateRect(pMsg->oswindow, NULL);
return 0;
}
return 0;   // sensible default for rest of commands
}

LRESULT CLASS_DECL_win __process_window_procedure_exception(::exception::base* e, const MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->ProcessWndProcException( e, pMsg );
else
return __internal_process_wnd_proc_exception( e, pMsg );
}
bool __internal_pre_translate_message(MSG* pMsg)
{
//   ASSERT_VALID(this);

thread *pThread = System.GetThread();
if( pThread )
{
// if this is a thread-message, int16_t-circuit this function
if (pMsg->oswindow == NULL && pThread->DispatchThreadMessageEx(pMsg))
return TRUE;
}

// walk from target to main ::user::window
sp(::user::interaction) pMainWnd = System.GetMainWnd();
/* trans   if (::user::window::WalkPreTranslateTree(pMainWnd->GetSafeoswindow_(), pMsg))
return TRUE; */

// in case of modeless dialogs, last chance route through main
//   ::user::window's accelerator table
/*   if (pMainWnd != NULL)
{
sp(::user::window) pWnd = ::win::window::from_handle(pMsg->oswindow);
if (pWnd != NULL && WIN_WINDOW(pWnd)->GetTopLevelParent() != pMainWnd)
return pMainWnd->pre_translate_message(pMsg);
}

return FALSE;   // no special processing
}

bool __cdecl __pre_translate_message(MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->pre_translate_message( pMsg );
else
return __internal_pre_translate_message( pMsg );
}

bool __internal_is_idle_message(MSG* pMsg)
{
// Return FALSE if the message just dispatched should _not_
// cause on_idle to be run.  Messages which do not usually
// affect the state of the ::fontopus::user interface and happen very
// often are checked for.

// redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE)
{
// mouse move at same position as last mouse move?
___THREAD_STATE *pState = __get_thread_state();
if (pState->m_ptCursorLast == pMsg->pt && pMsg->message == pState->m_nMsgLast)
return FALSE;

pState->m_ptCursorLast = pMsg->pt;  // remember for next time
pState->m_nMsgLast = pMsg->message;
return TRUE;
}

// WM_PAINT and WM_SYSTIMER (caret blink)
return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

bool __cdecl __is_idle_message(MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->is_idle_message( pMsg );
else
return __internal_is_idle_message( pMsg );
}

/*
thread* CLASS_DECL_win __begin_thread(sp(type) pThreadClass,
int32_t nPriority, UINT nStackSize, DWORD dwCreateFlags,
LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
#ifndef _MT
pThreadClass;
nPriority;
nStackSize;
dwCreateFlags;
lpSecurityAttrs;

return NULL;
#else
ASSERT(pThreadClass != NULL);
ASSERT(pThreadClass->IsDerivedFrom(System.type_info < thread > ()));

thread* pThread = dynamic_cast < thread * > (App(get_app()).alloc(pThreadClass));
if (pThread == NULL)
throw memory_exception();
ASSERT_VALID(pThread);

pThread->m_pThreadParams = NULL;
if(pThread->begin(
nPriority, 
nStackSize, 
dwCreateFlags, 
lpSecurityAttrs))
return pThread;
else
return NULL;
#endif //!_MT
}*/

/*
void CLASS_DECL_win __end_thread(UINT nExitCode, bool bDelete)
{
#ifndef _MT
nExitCode;
bDelete;
#else
// remove current thread object from primitive::memory
__MODULE_THREAD_STATE* pState = __get_module_thread_state();
// thread* pThread = pState->m_pCurrentWinThread;
if (pThread != NULL)
{
ASSERT_VALID(pThread);
ASSERT(pThread != &System);

if (bDelete)
pThread->Delete();
// pState->m_pCurrentWinThread = NULL;
}

// allow cleanup of any thread local objects
__term_thread();

// allow C-runtime to cleanup, and exit the thread
_endthreadex(nExitCode);
#endif //!_MT
}

/////////////////////////////////////////////////////////////////////////////
// Global functions for thread initialization and thread cleanup

LRESULT CALLBACK __message_filter_hook(int32_t code, WPARAM wParam, LPARAM lParam);

void CLASS_DECL_win __init_thread()
{
if (!afxContextIsDLL)
{
// set message filter proc
___THREAD_STATE* pThreadState = __get_thread_state();
ASSERT(pThreadState->m_hHookOldMsgFilter == NULL);
pThreadState->m_hHookOldMsgFilter = ::SetWindowsHookEx(WH_MSGFILTER,
__message_filter_hook, NULL, ::GetCurrentThreadId());
}
}





bool thread::create_thread(DWORD dwCreateFlags, UINT nStackSize,
LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
#ifndef _MT
dwCreateFlags;
nStackSize;
lpSecurityAttrs;

return FALSE;
#else
ENSURE(m_hThread == NULL);  // already created?

// setup startup structure for thread initialization
___THREAD_STARTUP startup; memset(&startup, 0, sizeof(startup));
startup.pThreadState = __get_thread_state();
startup.pThread = this;
startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
startup.dwCreateFlags = dwCreateFlags;
if (startup.hEvent == NULL || startup.hEvent2 == NULL)
{
TRACE(::core::trace::category_AppMsg, 0, "Warning: CreateEvent failed in thread::create_thread.\n");
if (startup.hEvent != NULL)
::CloseHandle(startup.hEvent);
if (startup.hEvent2 != NULL)
::CloseHandle(startup.hEvent2);
return FALSE;
}

#ifdef _WIN32
//   m_thread = ::create_thread(NULL, 0, StartThread, this, 0, &m_dwThreadId);
// create the thread (it may or may not start to run)
m_hThread = (HANDLE)(uint_ptr)_beginthreadex(lpSecurityAttrs, nStackSize,  
&__thread_entry, &startup, dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
#else
pthread_attr_t attr;

pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
if (pthread_create(&m_thread,&attr, StartThread,this) == -1)
{
perror("Thread: create failed");
SetRunning(false);
}
//   pthread_attr_destroy(&attr);
#endif

if (m_hThread == NULL)
return FALSE;

// start the thread just for core API initialization
VERIFY(ResumeThread() != (DWORD)-1);
VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
::CloseHandle(startup.hEvent);

// if created suspended, suspend it until resume thread wakes it up
if (dwCreateFlags & CREATE_SUSPENDED)
VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);

// if error during startup, shut things down
if (startup.bError)
{
VERIFY(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0);
::CloseHandle(m_hThread);
m_hThread = NULL;
::CloseHandle(startup.hEvent2);
return FALSE;
}

// allow thread to continue, once resumed (it may already be resumed)
::SetEvent(startup.hEvent2);
return TRUE;
#endif //!_MT
}

void thread::Delete()
{

// delete thread if it is auto-deleting
if(m_bAutoDelete)
{
m_evFinish.SetEvent();
delete this;
}
else
{
m_hThread = NULL;
m_evFinish.SetEvent();
}
}

/////////////////////////////////////////////////////////////////////////////
// thread default implementation

bool thread::PreInitInstance()
{
return true;
}

bool thread::initialize_instance()
{
ASSERT_VALID(this);

return true;   // by default enter run loop
}

// main running routine until thread exits
int32_t thread::run()
{
ASSERT_VALID(this);
___THREAD_STATE* pState = __get_thread_state();

// for tracking the idle time state
bool bIdle = TRUE;
LONG lIdleCount = 0;

// acquire and dispatch messages until a WM_QUIT message is received.
while(m_bRun)
{
// phase1: check to see if we can do idle work
while (bIdle &&
!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
{
// call on_idle while in bIdle state
if (!on_idle(lIdleCount++))
bIdle = FALSE; // assume "no idle" state
}

// phase2: pump messages while available
do
{
// pump message, but quit on WM_QUIT
if (!pump_message())
return exit_instance();

// reset "no idle" state after pumping "normal" message
//if (is_idle_message(&m_msgCur))
if (is_idle_message(&(pState->m_msgCur)))
{
bIdle = TRUE;
lIdleCount = 0;
}


} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));

m_ptimera->check();
}
}

bool thread::is_idle_message(MSG* pMsg)
{
return __internal_is_idle_message(pMsg);
}

int32_t thread::exit_instance()
{
ASSERT_VALID(this);
ASSERT(&System != this);

for(int32_t i = 0; i < m_puieptra->get_count(); i++)
{
m_puieptra->element_at(i)->m_pthread = NULL;
}

delete m_ptimera;
delete m_puieptra;

int32_t nResult = (int32_t)__get_current_message()->wParam;  // returns the value from PostQuitMessage
return nResult;
}

bool thread::on_idle(LONG lCount)
{
ASSERT_VALID(this);

#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)
// check core API's allocator (before idle)
if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
ASSERT(__check_memory());
#endif

if (lCount <= 0)
{
// send WM_IDLEUPDATECMDUI to the main ::user::window
sp(::user::interaction) pMainWnd = GetMainWnd();
if (pMainWnd != NULL && pMainWnd->IsWindowVisible())
{
/*__call_window_procedure(pMainWnd, pMainWnd->get_handle(),
WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
/*       pMainWnd->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
(WPARAM)TRUE, 0, TRUE, TRUE);
}
// send WM_IDLEUPDATECMDUI to all frame windows
/* linux __MODULE_THREAD_STATE* pState = ___CMDTARGET_GETSTATE()->m_thread;
sp(::user::frame_window) pFrameWnd = pState->m_frameList;
while (pFrameWnd != NULL)
{
if (pFrameWnd->get_handle() != NULL && pFrameWnd != pMainWnd)
{
if (pFrameWnd->m_nShowDelay == SW_HIDE)
pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
if (pFrameWnd->IsWindowVisible() ||
pFrameWnd->m_nShowDelay >= 0)
{
__call_window_procedure(pFrameWnd, pFrameWnd->get_handle(),
WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
(WPARAM)TRUE, 0, TRUE, TRUE);
}
if (pFrameWnd->m_nShowDelay > SW_HIDE)
pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
pFrameWnd->m_nShowDelay = -1;
}
pFrameWnd = pFrameWnd->m_pNextFrameWnd;
}*/
/*}
else if (lCount >= 0)
{
__MODULE_THREAD_STATE* pState = __get_module_thread_state();
if (pState->m_nTempMapLock == 0)
{
// free temp maps, OLE DLLs, etc.
::core::LockTempMaps();
::core::UnlockTempMaps();
}
}

#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)
// check core API's allocator (after idle)
if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
ASSERT(__check_memory());
#endif

return lCount < 0;  // nothing more to do if lCount >= 0
}

message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
{
return message::PrototypeNone;
}


bool thread::DispatchThreadMessageEx(MSG* pmsg)
{
if(pmsg->message == WM_APP + 1984 && pmsg->wParam == 77)
{
smart_pointer < win::message > spmessage(pmsg->lParam);
spmessage->send();
return TRUE;
}
/*   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
const __MSGMAP_ENTRY* lpEntry;

for (/* pMessageMap already init'ed *//*; pMessageMap->pfnGetBaseMap != NULL;
pMessageMap = (*pMessageMap->pfnGetBaseMap)())
{
// Note: catch not so common but fatal mistake!!
//       // BEGIN_MESSAGE_MAP(CMyThread, CMyThread)

ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
if (pMsg->message < 0xC000)
{
// constant ::user::window message
if ((lpEntry = ::core::FindMessageEntry(pMessageMap->lpEntries,
pMsg->message, 0, 0)) != NULL)
goto LDispatch;
}
else
{
// registered windows message
lpEntry = pMessageMap->lpEntries;
while ((lpEntry = ::core::FindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
{
UINT* pnID = (UINT*)(lpEntry->nSig);
ASSERT(*pnID >= 0xC000);
// must be successfully registered
if (*pnID == pMsg->message)
goto LDispatch;
lpEntry++;      // keep looking past this one
}
}
}
return FALSE;

LDispatch:
union MessageMapFunctions mmf;
mmf.pfn = lpEntry->pfn;

// always posted, so return value is meaningless

(this->*mmf.pfn_THREAD)(pMsg->wParam, pMsg->lParam);*/

/*LRESULT lresult;
SignalPtrArray signalptra;
m_signala.GetSignalsByMessage(signalptra, pmsg->message, 0, 0);
for(int32_t i = 0; i < signalptra.get_size(); i++)
{
Signal & signal = *signalptra[i];
::signal * psignal = signal.m_psignal;
message::e_prototype eprototype = signal.m_eprototype;
if(eprototype == message::PrototypeNone)
{
message::base base;
base.m_psignal = psignal;
lresult = 0;
base.set(pmsg->message, pmsg->wParam, pmsg->lParam, lresult);
psignal->emit(&base);
if(base.m_bRet)
return true;
}
break;
}
return true;
}

bool thread::pre_translate_message(signal_details * pobj)
{
ASSERT_VALID(this);
return __internal_pre_translate_message( pMsg );
}

LRESULT thread::ProcessWndProcException(::exception::base* e, const MSG* pMsg)
{
return __internal_process_wnd_proc_exception( e, pMsg );
}
*/

/////////////////////////////////////////////////////////////////////////////
// Message Filter processing (WH_MSGFILTER)

LRESULT CALLBACK __message_filter_hook(int32_t code, WPARAM wParam, LPARAM lParam)
{
   thread* pthread;
   if (afxContextIsDLL || (code < 0 && code != MSGF_DDEMGR) || (pthread = dynamic_cast < thread * > (::win::get_thread())) == NULL)
   {
      return ::CallNextHookEx(gen_ThreadState->m_hHookOldMsgFilter, code, wParam, lParam);
   }
   ASSERT(pthread != NULL);
   smart_pointer < message::base > spbase;
   spbase = pthread->get_base((LPMSG)lParam);
   pthread->ProcessMessageFilter(code, spbase);
   LRESULT lresult = spbase->m_bRet ? 1 : 0;
   return lresult;
}

__STATIC bool CLASS_DECL_win IsHelpKey(LPMSG lpMsg)
   // return TRUE only for non-repeat F1 keydowns.
{
   return lpMsg->message == WM_KEYDOWN &&
      lpMsg->wParam == VK_F1 &&
      !(HIWORD(lpMsg->lParam) & KF_REPEAT) &&
      GetKeyState(VK_SHIFT) >= 0 &&
      GetKeyState(VK_CONTROL) >= 0 &&
      GetKeyState(VK_MENU) >= 0;
}

__STATIC inline bool IsEnterKey(LPMSG lpMsg)
{ return lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN; }

__STATIC inline bool IsButtonUp(LPMSG lpMsg)
{ return lpMsg->message == WM_LBUTTONUP; }

/*&bool thread::ProcessMessageFilter(int32_t code, LPMSG lpMsg)
{
if (lpMsg == NULL)
return FALSE;   // not handled

sp(::user::frame_window) pTopFrameWnd;
sp(::user::interaction) pMainWnd;
sp(::user::interaction) pMsgWnd;
switch (code)
{
case MSGF_DDEMGR:
// Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
//  never call the next hook.
// By returning FALSE, the message will be dispatched
//  instead (the default behavior).
return FALSE;

case MSGF_MENU:
pMsgWnd = ::win::window::from_handle(lpMsg->oswindow);
if (pMsgWnd != NULL)
{
pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
if (pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
pTopFrameWnd->m_bHelpMode)
{
pMainWnd = System.GetMainWnd();
if ((GetMainWnd() != NULL) && (IsEnterKey(lpMsg) || IsButtonUp(lpMsg)))
{
pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
return TRUE;
}
}
}
// fall through...

case MSGF_DIALOGBOX:    // handles message boxes as well.
pMainWnd = System.GetMainWnd();
if (code == MSGF_DIALOGBOX && m_pActiveWnd != NULL &&
lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
{
// need to translate messages for the in-place container
___THREAD_STATE* pThreadState = __get_thread_state();
ENSURE(pThreadState);

if (pThreadState->m_bInMsgFilter)
return FALSE;
pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
MSG msg = *lpMsg;
if (m_pActiveWnd->IsWindowEnabled() && pre_translate_message(&msg))
{
pThreadState->m_bInMsgFilter = FALSE;
return TRUE;
}
pThreadState->m_bInMsgFilter = FALSE;    // ok again
}
break;
}

return FALSE;   // default to not handled
}*/

/*
/////////////////////////////////////////////////////////////////////////////
// Access to GetMainWnd() & m_pActiveWnd

sp(::user::interaction) thread::GetMainWnd()
{
if (m_pActiveWnd != NULL)
return m_pActiveWnd;    // probably in-place active

// when not inplace active, just return main ::user::window
if (GetMainWnd() != NULL)
return GetMainWnd();

return System.get_active_guie();
}

/////////////////////////////////////////////////////////////////////////////
// thread implementation helpers

bool thread::pump_message()
{
try
{
return __internal_pump_message();
}
catch(const ::exception::exception &)
{
if(on_run_exception((::exception::exception &) e))
return TRUE;
// get_app() may be it self, it is ok...
if(App(get_app()).final_handle_exception((::exception::exception & ) e))
return TRUE;
return FALSE;
}
}

/////////////////////////////////////////////////////////////////////////////
// thread diagnostics


void thread::assert_valid() const
{
command_target::assert_valid();
}

void thread::dump(dump_context & dumpcontext) const
{
command_target::dump(dumpcontext);
___THREAD_STATE *pState = __get_thread_state();

dumpcontext << "m_pThreadParams = " << m_pThreadParams;
dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
dumpcontext << "\nm_bAutoDelete = " << m_bAutoDelete;
dumpcontext << "\nm_hThread = " << (void *)m_hThread;
dumpcontext << "\nm_nThreadID = " << m_nThreadID;
dumpcontext << "\nm_nDisablePumpCount = " << pState->m_nDisablePumpCount;
if (System.GetThread() == this)
dumpcontext << "\nm_pMainWnd = " << GetMainWnd();

dumpcontext << "\nm_msgCur = {";
dumpcontext << "\n\toswindow = " << (void *)pState->m_msgCur.oswindow;
dumpcontext << "\n\tmessage = " << (UINT)pState->m_msgCur.message;
dumpcontext << "\n\twParam = " << (UINT)pState->m_msgCur.wParam;
dumpcontext << "\n\tlParam = " << (void *)pState->m_msgCur.lParam;
dumpcontext << "\n\ttime = " << pState->m_msgCur.time;
dumpcontext << "\n\tpt = " << point(pState->m_msgCur.pt);
dumpcontext << "\n}";

dumpcontext << "\nm_pThreadParams = " << m_pThreadParams;
dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
dumpcontext << "\nm_ptCursorLast = " << pState->m_ptCursorLast;
dumpcontext << "\nm_nMsgLast = " << pState->m_nMsgLast;

dumpcontext << "\n";
}


bool thread::post_message(sp(::user::interaction) pguie, UINT uiMessage, WPARAM wparam, LPARAM lparam)
{
win::message * pmessage = new win::message;
pmessage->m_pguie       = pguie;
pmessage->m_uiMessage   = uiMessage;
pmessage->m_wparam      = wparam;
pmessage->m_lparam      = lparam;
return post_thread_message(WM_APP + 1984, 77, (LPARAM) pmessage);
}


bool thread::on_run_exception(::exception::exception &)
{
return false;
}


namespace win
{
LRESULT message::send()
{
return m_pguie->SendMessage(m_uiMessage, m_wparam, m_lparam);
}

UINT message::ThreadProcSendMessage(LPVOID lp)
{
message * pmessage = (message *) lp;
pmessage->send();
delete pmessage;
return 0;
}

void message::post(sp(::user::interaction) puie, UINT uiMessage, WPARAM wparam, LPARAM lparam, int32_t nPriority)
{
message * pmessage = new message;
pmessage->m_pguie = puie;
pmessage->m_uiMessage = uiMessage;
pmessage->m_wparam = wparam;
pmessage->m_lparam = lparam;
__begin_thread(puie->get_app(), &ThreadProcSendMessage, pmessage, nPriority);
}*/

// thread

