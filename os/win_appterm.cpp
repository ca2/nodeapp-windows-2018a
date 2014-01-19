#include "framework.h"


#include <ddeml.h>


extern CLASS_DECL_win oswindow_map * g_pwindowmap;


/////////////////////////////////////////////////////////////////////////////
// other globals (internal library use)

/////////////////////////////////////////////////////////////////////////////
// Standard cleanup called by WinMain and __abort

void CLASS_DECL_win __gen_unregister_window_classes()
{
   
   // unregister Window classes
   __MODULE_STATE* pModuleState = __get_module_state();

   single_lock sl(&pModuleState->m_mutexRegClassList, TRUE);

   if(pModuleState->m_pstrUnregisterList != NULL) 
   {
      strsize start = 0;
      string className = pModuleState->m_pstrUnregisterList->Tokenize("\n",start);
      while (!className.is_empty())
      {
//         UnregisterClass(static_cast<const char *>(className), System.m_hInstance);
         className = pModuleState->m_pstrUnregisterList->Tokenize("\n",start);
      }
      pModuleState->m_pstrUnregisterList->Empty();
      pModuleState->m_pstrUnregisterList = NULL;
   }

}


void CLASS_DECL_win __win_term()
{   

   delete g_pwindowmap;

   g_pwindowmap = NULL;

/*   try
   {
      main_finalize();
   }
   catch (...)
   {
   }*/

   if (__get_thread_state() != NULL)
   {
      __get_thread_state()->finalize();
      gen_ThreadState = NULL;
   }


   __gen_unregister_window_classes();
   // cleanup OLE if required
//   thread* pThread = &System;

   // cleanup thread local tooltip ::user::window
//   __MODULE_THREAD_STATE* pModuleThreadState = __get_module_thread_state();
/*   if (pModuleThreadState->m_pToolTip != NULL)
   {
      if (pModuleThreadState->m_pToolTip->DestroyToolTipCtrl())
         pModuleThreadState->m_pToolTip = NULL;
   }*/

   ___THREAD_STATE* pThreadState = __get_thread_state();
   if (!afxContextIsDLL)
   {
      // unhook windows hooks
      if (pThreadState->m_hHookOldMsgFilter != NULL)
      {
         ::UnhookWindowsHookEx(pThreadState->m_hHookOldMsgFilter);
         pThreadState->m_hHookOldMsgFilter = NULL;
      }
      if (pThreadState->m_hHookOldCbtFilter != NULL)
      {
         ::UnhookWindowsHookEx(pThreadState->m_hHookOldCbtFilter);
         pThreadState->m_hHookOldCbtFilter = NULL;
      }
   }
    // We used to suppress all exceptions here. But that's the wrong thing
    // to do. If this process crashes, we should allow Windows to crash
    // the process and invoke watson.


   try
   {
      delete __get_module_state()->m_pmapHWND;
   }
   catch (...)
   {
   }
   /*   try
   {
   delete __get_module_state()->m_pmapHDC;
   }
   catch(...)
   {
   }*/
   /*   try
   {
   delete __get_module_state()->m_pmapHGDIOBJ;
   }
   catch(...)
   {
   }*/
   //      delete __get_module_state()->m_pmapHMENU;

   try
   {
      __get_module_state()->m_pmapHWND = NULL;
   }
   catch (...)
   {
   }
   /*   try
   {
   __get_module_state()->m_pmapHDC      = NULL;
   }
   catch(...)
   {
   }*/
   /*   try
   {
   __get_module_state()->m_pmapHGDIOBJ  = NULL;
   }
   catch(...)
   {
   }*/


}

/////////////////////////////////////////////////////////////////////////////
