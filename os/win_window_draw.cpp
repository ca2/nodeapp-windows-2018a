#include "framework.h"

extern void _001DeferPaintLayeredWindowBackground(oswindow oswindow, ::draw2d::graphics * pdc);



class keep_event_reset
{
public:
   
   
   event * m_pevent;


   keep_event_reset(event * pevent)
   {
      m_pevent = pevent;
      pevent->ResetEvent();
   }
   ~keep_event_reset()
   {
      m_pevent->SetEvent();
   }


};

namespace win
{

   window_draw::window_draw(sp(base_application) papp) : 
      element(papp),
      thread(papp),
      ::user::window_draw(papp),
      message_queue(papp),
      m_mutexRendering(papp),
      m_mutexRgnUpdate(papp),
      m_semaphoreBuffer(papp),
      m_mutexRender(papp),
      m_wndpaOut(papp)
   {
      m_dwLastRedrawRequest = ::get_tick_count();
      m_bRender = false;
      m_pbuffer = new user::buffer(papp);
      m_pbuffer->m_spdib.create(papp);
      m_dwLastUpdate = false;
   }

   
   window_draw::~window_draw()
   {

   }


   
   /*
   void window_draw::OnPaint(oswindow oswindow, CPaintDC & spgraphics)
   {
      UNREFERENCED_PARAMETER(oswindow);
      UNREFERENCED_PARAMETER(spgraphics);
   }
   */

   void window_draw::message_queue_message_handler(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_uiMessage == (WM_USER + 1984 + 1977))
      {
         _synch_redraw();
      }
   }

   void window_draw::asynch_redraw()
   {
      DWORD dwTick = ::get_tick_count();
      if(m_iFramesPerSecond == 0)
      {
         if(dwTick - m_dwLastUpdate < 1000)
            return;
      }
      else
      {
         if(dwTick - m_dwLastUpdate < 1000 / m_iFramesPerSecond)
            return;
      }
      _asynch_redraw();
   }

   void window_draw::_asynch_redraw()
   {
      if(!m_bProDevianMode)
      {
         m_spuiMessage->post_message(WM_USER + 1984 + 1977);
      }
   }

   void window_draw::synch_redraw()
   {
      if(!m_bProDevianMode && ::IsWindow(m_spuiMessage->get_handle()))
      {
         m_spuiMessage->send_message(WM_USER + 1984 + 1977);
      }
   }

   void window_draw::_synch_redraw()
   {
      
      keep_event_reset keepeventreset(&m_eventFree);

      static DWORD s_dwLastAnalysisFrame = 0;
      static DWORD s_dwLastFrameFrame = 0;
      static DWORD s_iAnalysisFrameFailureCount = 0;
      static DWORD s_iFrameFailureCount = 0;
      DWORD iFailureTime;
      if(m_iFramesPerSecond == 0)
      {
         iFailureTime = 1000 / 12;
      }
      else
      {
         iFailureTime = 1000 / m_iFramesPerSecond;
      }
      m_dwLastUpdate = ::get_tick_count();
      UpdateBuffer();
      return;
      if(m_pbuffer->GetBuffer()->get_os_data() != NULL)
      {
         //m_pbuffer->m_spdib->fill_channel(255, visual::rgba::channel_alpha);
         //ScreenOutput();
      }
      DWORD dwTakeTime = ::get_tick_count() - m_dwLastUpdate;
      m_dwLastDelay = dwTakeTime;
      if(dwTakeTime > iFailureTime)
      {
         s_dwLastAnalysisFrame += dwTakeTime;
         s_dwLastFrameFrame += dwTakeTime;
         TRACE("window_draw::_synch_redraw :: System has exceeded prodevian performance failure time - %d milliseconds", iFailureTime);
         TRACE("window_draw::_synch_redraw :: took - %d milliseconds", dwTakeTime);
         s_iAnalysisFrameFailureCount++;
         DWORD iTimeFrame = 2000;
         DWORD iMaxFailureCount = 3;
         if(s_dwLastAnalysisFrame > iTimeFrame ||
            dwTakeTime > (iFailureTime * iMaxFailureCount))
         {
            s_dwLastAnalysisFrame = 0;
            if(s_iAnalysisFrameFailureCount > iMaxFailureCount
            || (dwTakeTime > (iFailureTime * iMaxFailureCount)))
            {
               s_iFrameFailureCount++;
            }
            s_iAnalysisFrameFailureCount = 0;
         }
         DWORD iMaxFrameFailureCount = 3;
         DWORD iFrameTimeFrame = iTimeFrame * iMaxFrameFailureCount;
         if(s_dwLastFrameFrame > iFrameTimeFrame)
         {
            s_dwLastFrameFrame = 0;
            if(s_iFrameFailureCount > iMaxFailureCount)
            {
               TRACE("window_draw::_synch_redraw :: during prodevian Performance Analysis Time Frame - %d milliseconds -,", iTimeFrame);
               TRACE("window_draw::_synch_redraw :: failure ::count has exceeded the maximum count - %d", iMaxFailureCount);
               TRACE("window_draw::_synch_redraw :: Going to try to save some resource that may favor drawing perfomance");
               if(!System.savings().is_trying_to_save(::core::resource_blur_background))
               {
                  TRACE("window_draw::_synch_redraw :: System is not trying to save \"blur background\" resource");
                  TRACE("window_draw::_synch_redraw :: Going to try to save \"blur background\" resource");
                  System.savings().try_to_save(::core::resource_blur_background);
               }
               else if(!System.savings().is_trying_to_save(::core::resource_blurred_text_embossing))
               {
                  TRACE("window_draw::_synch_redraw :: System is not trying to save \"blurred text embossing\" resource");
                  TRACE("window_draw::_synch_redraw :: Going to try to save \"blurred text embossing\" resource");
                  System.savings().try_to_save(::core::resource_blurred_text_embossing);
               }
               else if(!System.savings().is_warning(::core::resource_processing))
               {
                  TRACE("window_draw::_synch_redraw :: System is not warning to save \"processing\" resource");
                  TRACE("window_draw::_synch_redraw :: Going to warn to save \"processing\" resource");
                  System.savings().warn(::core::resource_processing);
               }
               else if(!System.savings().is_trying_to_save(::core::resource_processing))
               {
                  TRACE("window_draw::_synch_redraw :: System is not trying to save \"processing\" resource");
                  TRACE("window_draw::_synch_redraw :: Going to try to save \"processing\" resource");
                  System.savings().try_to_save(::core::resource_blur_background);
               }
            }
            s_iFrameFailureCount = 0;
         }

      }
   }

   bool window_draw::to(
      ::draw2d::graphics *          pdc,
      LPCRECT        lpcrectUpdate,
      user::oswindow_tree::Array & oswindowtreea,
      bool           bGdiLocked,
      bool           bExcludeParamWnd)
   {
      UNREFERENCED_PARAMETER(bGdiLocked);
      UNREFERENCED_PARAMETER(bExcludeParamWnd);
      single_lock sl(&m_mutexRender, TRUE);
      rect rectUpdate(*lpcrectUpdate);

      rect rectChild;

      rect rectNewUpdate;
      
      for(int_ptr i = oswindowtreea.get_size() - 1; i >= 0; i--)
      {
         user::oswindow_tree & oswindowtreeChild = oswindowtreea[i];
         oswindow oswindowChild = oswindowtreeChild.m_oswindow;
         ::GetWindowRect(oswindowChild, rectChild);
         if(rectNewUpdate.intersect(rectChild, rectUpdate))
         {
            to(pdc, rectNewUpdate, oswindowtreeChild, true, false);
         }
       }
       return true;
   }

   bool window_draw::to(
      ::draw2d::graphics *          pdc,
      LPCRECT        lpcrectUpdate,
      user::oswindow_tree & oswindowtree,
      bool           bGdiLocked,
      bool           bExcludeParamWnd)
   {
      UNREFERENCED_PARAMETER(bGdiLocked);
      single_lock sl(&m_mutexRender, TRUE);
      rect rectUpdate(*lpcrectUpdate);

//      DWORD dwTimeIn = get_tick_count();

      oswindow oswindowParam = oswindowtree.m_oswindow;

      if(oswindowParam == NULL)
      {
         return false;
      }

      if(!::IsWindow(oswindowParam))
      {
         return false;
      }

      
            


      if(!::IsWindowVisible(oswindowParam))
      {
         return true;
      }




      if(oswindowtree.m_dwUser & 1)
      {
         ASSERT(TRUE);
      }
      else
      {
         ::user::window_interface * ptwi = System.user()->window_map().get((int_ptr) oswindowParam);
         sp(::user::interaction) pguie =  (ptwi);
         rect rectWindow;
         ::GetWindowRect(oswindowParam, rectWindow);
         //::GetClientRect(oswindowParam, rectWindow);
         //::ClientToScreen(oswindowParam, &rectWindow.top_left());
         //::ClientToScreen(oswindowParam, &rectWindow.bottom_right());
         
         pdc->SetViewportOrg(rectWindow.left, rectWindow.top);

      
         if(ptwi != NULL)
         {

       
            if(!bExcludeParamWnd &&
               pguie != NULL )
            {
               pguie->_001OnDraw(pdc);
            }


         }
         else
         {
            bool bWin4 = FALSE;
         //_gen::FillPSOnStack();
            /*::DefWindowProc(
               oswindowParam,
               (bWin4 ? WM_PRINT : WM_PAINT),
               (WPARAM)((dynamic_cast<::win::graphics * >(pdc))->get_os_data()),
               (LPARAM)(bWin4 ? PRF_CHILDREN | PRF_CLIENT : 0));*/
            //::RedrawWindow(oswindowParam, NULL, rgnClient, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
         }
      }


      

//      DWORD dwTimeOut = get_tick_count();
   //   TRACE("// Average Window Rendering time\n");
   //   TRACE("// Window Class: %s\n", (pwnd!=NULL) ? pwnd->GetRuntimeClass()->m_lpszClassName : "(Not available)");
   //   TRACE("// TickCount: %d \n", dwTimeOut - dwTimeIn);
   //   TRACE("// \n");


      return to(
         pdc,
         rectUpdate,
         oswindowtree.m_oswindowtreea,
         true,
         false);

   }


   UINT window_draw::RedrawProc()
   {
      if(!create_message_queue("::core::twf - core Transparent Window Framework"))
      {
         TRACE("Could not initialize ::core::twf - core Transparent Window Framework!");
         return 0;
      }
      ::AttachThreadInput(::GetCurrentThreadId(), WIN_THREAD(System.thread::m_p.m_p)->m_nThreadID, TRUE);
      MSG msg;
      s_bRunning = true;
      while(m_bRun)
      {
         try
         {
            if(m_bProDevianMode)
            {
               uint32_t ui1 = ::get_tick_count();
               _synch_redraw();
               uint32_t ui2 = ::get_tick_count();
               m_dwLastDelay = ui2 - ui1;
            }
         }
         catch(...)
         {
         }
         while(::PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE))
         {
            if(!__get_thread()->pump_message())
               break;
         }
         if(msg.message == WM_QUIT)
            break;
         int32_t iUiDataWriteWindowTimeForTheApplicationInThisMachine = 8;
         if(m_iFramesPerSecond == 0)
         {
            Sleep(1000);
         }
         else if((1000 / m_iFramesPerSecond) > m_dwLastDelay)
         {
            Sleep(max((DWORD) max(0, iUiDataWriteWindowTimeForTheApplicationInThisMachine), (1000 / m_iFramesPerSecond) - m_dwLastDelay));
         }
         else
         {
            Sleep(iUiDataWriteWindowTimeForTheApplicationInThisMachine);
         }
      }
      //delete this;
      s_bRunning = false;
      return 0;
   }

   int32_t window_draw::run()
   {
      return RedrawProc();
   }


   DWORD g_dwLastWindowDraw;
   // lprect should be in screen coordinates
   bool window_draw::UpdateBuffer()
   {
      if(m_bRender)
         return false;
      single_lock sl(&m_mutexRender, FALSE);
      if(!sl.lock(duration::zero()))
         return false;
      keeper<bool> keepRender(&m_bRender, true, false, true);
   //   TRACE("////////////////////////////////////////////////////\n");
   //   TRACE("// window_draw::TwfRender\n");
   //   TRACE("//\n");

//      DWORD dwTimeIn = get_tick_count();

      static bool bTest = false;
      semaphore * psemaphore = TwfGetBufferSemaphore();
      single_lock slSemaphoreBuffer(psemaphore, FALSE);
      if(!slSemaphoreBuffer.lock(duration::zero()))
      {
   //xxx      AddUpdateRect(rectClip, true);
   //      return false;
      }





      rect rectScreen;
      System.get_screen_rect(&rectScreen);
      m_pbuffer->UpdateBuffer(rectScreen.bottom_right());
      if(m_pbuffer->GetBuffer()->get_os_data() == NULL)
         return true;

      ::draw2d::graphics * pdc = m_pbuffer->GetBuffer();

      if(pdc == NULL)
      {
         return false;
      }

      synch_lock slUser(&user_mutex());
      //user::oswindow_array oswindowa;

      //get_wnda(oswindowa);

      user::interaction_ptr_array wndpa(get_app());

      get_wnda(wndpa);

      //user::window_util::SortByZOrder(oswindowa);

      user::oswindow_tree::Array oswindowtreea;
      //oswindowtreea = oswindowa;
      //oswindowtreea.EnumDescendants();


      

      rect rectUpdate;

      rectUpdate = rectScreen;

      rect rectOptimize;

      rectOptimize = rectUpdate;

      rect rectWindow;
      rect rect9;

      user::oswindow_array wndaApp;

      m_wndpaOut.remove_all();

//      ::draw2d::region_sp rgnWindow(get_app());
  //    ::draw2d::region_sp rgnIntersect(get_app());

//      rgnWindow->create_rect(0, 0, 0, 0);
  //    rgnIntersect->create_rect(0, 0, 0, 0);

      /*rect rectIntersect;
      ::draw2d::region_sp rgnUpdate(get_app());
      rgnUpdate->create_rect(rectUpdate);
      oswindow oswindowOrder = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
      for(;;)
      {
         if(oswindowOrder == NULL)
            break;
         //bool bVisible = ::IsWindowVisible(oswindowOrder);
         bool bVisible = ::IsWindowVisible(oswindowOrder);
         bool bIconic = ::IsIconic(oswindowOrder);
         if(!bVisible
         || bIconic
         || wndaApp.contains(oswindowOrder))
         {
         }
         else
         {
            rect rectWindow;
            ::GetWindowRect(oswindowOrder, rectWindow);
            char text[256];
            //::GetWindowText(oswindowOrder, text, sizeof(text));
            bool bLayered = ::GetWindowLong(oswindowOrder, GWL_EXSTYLE);
            rgnWindow->create_rect(rectWindow);
            rgnIntersect->create_rect(rect(0, 0, 0, 0));
            rgnIntersect->CombineRgn(rgnUpdate, rgnWindow, RGN_AND);
            rect rectIntersectBox;
            rgnIntersect->GetRgnBox(rectIntersectBox);
            if(rectIntersectBox.is_empty())
            {
            }
            else
            {
               if(!bLayered)
               {
                  rgnUpdate->CombineRgn(rgnUpdate, rgnWindow, ::draw2d::region::combine_exclude);
               }
               rect rectDiffBox;
               rgnUpdate->GetRgnBox(rectDiffBox);
               wndaApp.add(oswindowOrder);
               if(rectDiffBox.is_empty())
               {
                  break;
               }
            }
         }
         oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);
      }
         */
      
      for(int32_t l = 0; l < wndpa.get_count();)
      {
         try
         {
            if(wndpa[l].m_psession.is_null())
            {
               try
               {
                  ::win::window * pwindow = WIN_WINDOW(wndpa[l].m_pimpl);
                  if (pwindow != NULL)
                  {
                     pwindow->_001UpdateWindow();
                  }
                  
               }
               catch(...)
               {
               }


            }
            l++;
         }
         catch(simple_exception & se)
         {
            if(se.m_strMessage == "no more a window")
            {
               System.frames().remove(wndpa(l));
               wndpa.remove_at(l);
               
            }
         }
         catch(...)
         {
            System.frames().remove(wndpa(l));
            wndpa.remove_at(l);
         }
      }
      return true;
      
      for(index j = wndaApp.get_upper_bound(); j >= 0; j--)
      {
         oswindow oswindowTopic = wndaApp[j];

         sp(::user::window) pwnd = NULL;
         //sp(::user::window) pwnd = (System.user()->window_map().get((int_ptr) oswindowTopic));
         //if(pwnd == NULL)
         //{
         for(int32_t l = 0; l < wndpa.get_count(); l++)
         {
            if(wndpa[l].get_safe_handle() == oswindowTopic)
            {
               pwnd = (wndpa[l].m_pimpl.m_p);
               break;
            }
         }
         if(!::IsWindowVisible(wndaApp[j]) || ::IsIconic(wndaApp[j]) || pwnd == NULL)
            continue;



         /*pwnd->GetWindowRect(rectWindow);
         rectIntersect.intersect(rectWindow, rectUpdate);
         if(rectIntersect.area() <= 0)
         {
            pwnd->PostMessage(WM_USER + 184, 0, 0);
            // TODO: should use iMaxMonitor information to set window
            // to a more visible position in the monitor iMaxMonitor with greatest
            // area.
               
            //
            //
            // !!!! Warning !!!!
            //
            //

            // The implementation below potentially uses functions like SendMessage.
            // window_draw - The Transparent Window Interface - should never call
            // SendMessage or other functions that may lock other threads. 
            // This also applies to every _001OnDraw, _000OnDraw implementation.
            // 
            // e.g.: If the user interface thread waits for twf - wait_twf -
            //       which is a very common task for avoiding simultaneous
            //       access to user interface resources that may be
            //       updated by the user interface thread and drawn by
            //       by twf thread - following the prodevian painting model,
            //       where any time twf can request to draw a new frame
            //       to obbey certain rules like frames per second -
            //       and twf sends a message to the user interface thread,
            //       both threads get deadlocked.



            /*sp(simple_frame_window) pframe =  (pwnd);
            if(pframe != NULL)
            {
               pframe->InitialFramePosition(true);
            }
            else
            {
               class rect rect = System.m_monitorinfoa[0].rcMonitor;
               rect.deflate(rect.width() / 4, rect.height() / 4);
               pwnd->SetWindowPos(ZORDER_TOP, rect.left, rect.top, rect.width(), rect.height(), 0);
            }*/
         /*}
         pwnd->GetWindowRect(rectWindow);
         rectIntersect.intersect(rectWindow, rectUpdate);
         if(rectIntersect.area() > 0)
         {
            m_pbuffer->GetBuffer()->SetViewportOrg(0, 0);
            pwnd->_001Print(m_pbuffer->GetBuffer());
            m_wndpaOut.add(pwnd);
         }*/
      }

      //HDC hdc = (HDC) m_pbuffer->GetBuffer()->get_os_data();
      //::SetViewportOrgEx(hdc, 0, 0, NULL);





   //   if(TwfIsBuffered())
         /*{
            ScreenOutput(
               m_pbuffer,
               rgnClip);
         }*/

   //    TwfReleaseDC(pdc);

//      DWORD dwTimeOut = get_tick_count();
   //   TRACE("//\n");
   //   TRACE("// Rendering Finished\n");
   //   TRACE("// TickCount: %d \n", dwTimeOut - dwTimeIn);
   //   TRACE("////////////////////////////////////////////////////\n");

      return true;
   }

   bool window_draw::ScreenOutput()
   {
      
      if(m_bRender)
         return false;
      
      single_lock sl(&m_mutexRender, FALSE);
      
      if(!sl.lock(duration::zero()))
         return false;
   
      keeper<bool> keepRender(&m_bRender, true, false, true);
   
      static bool bTest = false;

      for(int32_t i = 0; i < m_wndpaOut.get_count(); i++)
      {
         sp(::user::interaction) pwnd = m_wndpaOut(i);

         ScreenOutput(m_pbuffer, pwnd);
      
      }

      return true;

   }


   semaphore * window_draw::TwfGetBufferSemaphore()
   {
      return &m_semaphoreBuffer;
   }

   // The first ::user::window handle in the array must belong
   // to the higher z order ::user::window.
   // The rectangle must contain all update region.
   // It must be in screen coordinates.



   // Remark: this optimization eliminates top level windows
   // that are lower z order siblings of a higher z order
   // top level ::user::window that contains all
   // the update region in a opaque area.
   // It doesn´t eliminates from the update parent windows
   // obscured by opaque children.

   window_draw::EOptimize window_draw::TwfOptimizeRender(
      user::oswindow_tree::Array & oswindowtreea,
      LPCRECT lpcrect)
   {
      const rect rectUpdate(*lpcrect);

      rect rectClient;
      for(int32_t i = 0; i < oswindowtreea.get_size();)
      {
         user::oswindow_tree & oswindowtree = oswindowtreea[i];
         switch(TwfOptimizeRender(
            oswindowtree,
            lpcrect))
         {
         case OptimizeNone:
            i++;
            break;
         case OptimizeThis:
            oswindowtreea.remove_at(i);
            break;
         case OptimizeAllNext:
            {
               while(oswindowtreea.get_size() > i + 1)
               {
                  oswindowtreea.remove_at(i + 1);
               }
               /*oswindowtreea.remove_at(i + 1);
               oswindow oswindow = oswindowtreea[i].m_oswindow;
               rect rect;
               ::GetClientRect(oswindow, rect);
               ::ClientToScreen(oswindow, &rect.top_left());
               ::ClientToScreen(oswindow, &rect.bottom_right());
               rect.union(rect, lpcrect);
               TwfOptimizeRenderRemoveNextProper(
                  oswindowtreea,
                  i + 1,
                  rect);*/
            }
            return OptimizeAllNext;
         }
      }
      return OptimizeNone;

   }

   window_draw::EOptimize window_draw::TwfOptimizeRenderRemoveNextProper(
      user::oswindow_tree::Array & oswindowtreea,
      int32_t iIndex,
      LPCRECT lpcrect)
   {

      
      const rect rectOptimize(*lpcrect);
      rect rectClient;
      for(int32_t i = iIndex; i < oswindowtreea.get_size();)
      {
         user::oswindow_tree & oswindowtree = oswindowtreea[i];
         ::oswindow oswindow = oswindowtree.m_oswindow;
         rect rect;
         ::GetClientRect(oswindow, rect);
         ::ClientToScreen(oswindow, &rect.top_left());
         ::ClientToScreen(oswindow, &rect.bottom_right());
         if(rectOptimize.contains(rect))
         {
            oswindowtreea.remove_at(i);
         }
         else
         {
            TwfOptimizeRenderRemoveNextProper(
               oswindowtree.m_oswindowtreea,
               0,
               lpcrect);

            i++;
         }
      }
      return OptimizeNone;

   }


   window_draw::EOptimize window_draw::TwfOptimizeRender(
      user::oswindow_tree & oswindowtree,
      LPCRECT lpcrect)
   {

      oswindowtree.m_dwUser = 0;

      oswindow oswindow = oswindowtree.m_oswindow;

      ::user::window_interface * ptwi = System.user()->window_map().get((int_ptr) oswindow);

      if(!::IsWindowVisible(oswindow))
      {
         return OptimizeThis;
      }
      const rect rectUpdate(*lpcrect);

      rect rectClient;

      sp(::user::interaction) pguie =  (ptwi);

      pguie->GetClientRect(rectClient);
      pguie->ClientToScreen(rectClient);

      


      rect rectIntersect;

      if(!rectIntersect.intersect(rectClient, rectUpdate))
      {
         return OptimizeThis;
      }

      if(!rectClient.contains(rectUpdate))
      {
         
         return OptimizeNone;
      }

   //    sp(::user::window) pwnd = window::FromHandlePermanent(oswindow);
       
      
      if(ptwi == NULL)
      {

   //      ::user::window_interface::GetProperty getp;
   //      getp.m_eproperty = CTransparentWndInterface::PropertyInterface;
   //      ::SendMessage(oswindow, CTransparentWndInterface::MessageGetProperty, 0, (LPARAM) &getp);
   //      ptwi = getp.m_pinterface;
   //      if(ptwi != NULL)
   //      {
   //         pwnd = ptwi->TwiGetWnd();
   //      }
      }
      
      EOptimize eoptimize = OptimizeNone;
      if(ptwi != NULL)
      {
         if(!ptwi->_001HasTranslucency())
            eoptimize = OptimizeAllNext;
      }

      EOptimize eoptimizeChildren = TwfOptimizeRender(
            oswindowtree.m_oswindowtreea,
            lpcrect);

      if(eoptimizeChildren == OptimizeAllNext)
      {
         oswindowtree.m_dwUser |= 1; // exclude from drawing;
         return OptimizeAllNext;
      }
      else if(eoptimize == OptimizeAllNext)
      {
         return OptimizeAllNext;
      }
      else
      {
         return OptimizeNone;
      }

      
   }


   bool window_draw::TwfGetTopWindow(
      oswindow oswindow,
      user::oswindow_array & oswindowa,
      array < HRGN, HRGN > & hrgna,
      user::oswindow_tree::Array & oswindowtreea,
      HRGN hrgn)
   {
      rect rectClient;
      for(int32_t i = 0; i < oswindowtreea.get_size(); i++)
      {
         user::oswindow_tree & oswindowtree = oswindowtreea[i];
         if(!TwfGetTopWindow(
            oswindow,
            oswindowa,
            hrgna,
            oswindowtree,
            hrgn))
            return false;

      }
      return true;

   }

   bool window_draw::TwfGetTopWindow(
      oswindow oswindowParam,
      user::oswindow_array & oswindowa,
      array < HRGN, HRGN > & hrgna,
      user::oswindow_tree & oswindowtree,
      HRGN hrgn)
   {
      oswindow oswindow = oswindowtree.m_oswindow;


      if(!::IsWindowVisible(oswindow))
      {
         return true;
      }




      rect rectWindow;

      ::GetWindowRect(oswindow, rectWindow);


   //   sp(::user::window) pwnd = ::win::window::from_handle;

      if(!TwfGetTopWindow(
            oswindowParam,
            oswindowa,
            hrgna,
            oswindowtree.m_oswindowtreea,
            hrgn))
      {
         return false;
      }

      HRGN hrgnIntersect;

      hrgnIntersect = ::CreateRectRgnIndirect(rectWindow);

      int32_t iCombine = ::CombineRgn(hrgnIntersect, hrgn, hrgnIntersect, RGN_AND);

      if(iCombine == NULLREGION)
      {
         ::DeleteObject(hrgnIntersect);
         return true;
      }

      

      ::user::window_interface * pwndi = System.user()->window_map().get((int_ptr) oswindow);

      if(pwndi == NULL)
      {
         ::SendMessage(
            oswindow,
            ::user::window_interface::MessageBaseWndGetProperty, 
            ::user::window_interface::PropertyDrawBaseWndInterface, 
            (LPARAM) &pwndi);
      }

      if(oswindowParam == oswindow ||
         (pwndi != NULL && 
            pwndi->_001HasTranslucency()))
      {
      

         iCombine = ::CombineRgn(hrgn, hrgn, hrgnIntersect, ::draw2d::region::combine_exclude);

         point ptOffset(0, 0);

         ::ScreenToClient(oswindow, &ptOffset);

         oswindowa.add(oswindow);
         ::OffsetRgn(hrgnIntersect, ptOffset.x, ptOffset.y);
         hrgna.add(hrgnIntersect);

         if(iCombine == NULLREGION)
         {
            return false;
         }
         
      }
      else
      {
         ::DeleteObject(hrgnIntersect);
         TwfGetTopWindowOptimizeOpaque(
            oswindow,
            oswindowa,
            hrgna);

      }
      if(oswindowParam == oswindow)
      {
         return false;
      }
      else
      {
         return true;
      }
   }

   // lpcrect must be in screen coordinates
   void window_draw::TwfGetTopWindow(
      oswindow oswindow,
      user::oswindow_array & oswindowa,
      array < HRGN, HRGN > & hrgna,
      user::oswindow_tree::Array & oswindowtreea,
      LPCRECT lpcrect)
   {
      HRGN hrgn = ::CreateRectRgnIndirect(lpcrect);
      TwfGetTopWindow(
         oswindow,
         oswindowa, 
         hrgna, 
         oswindowtreea, 
         hrgn);
      ::DeleteObject(hrgn);
   }

   void window_draw::TwfGetTopWindowOptimizeOpaque(
      oswindow oswindowOpaque,
      user::oswindow_array & oswindowa,
      array < HRGN, HRGN > & hrgna)
   {
      rect rectWindow;

   //   sp(::user::window) pwndOpaque = window::FromHandlePermanent(oswindowOpaque);

      ::GetWindowRect(oswindowOpaque, rectWindow);

      HRGN hrgnOpaque = ::CreateRectRgnIndirect(rectWindow);

      point ptOffset;
      for(int32_t i = 0; i < hrgna.get_size(); )
      {
         oswindow oswindow = oswindowa[i];
         HRGN hrgn = hrgna[i];
         ptOffset.x = 0;
         ptOffset.y = 0;
         ::ClientToScreen(oswindow, &ptOffset);
         ::OffsetRgn(hrgn, ptOffset.x, ptOffset.y);
         if(::CombineRgn(hrgn, hrgn, hrgnOpaque, ::draw2d::region::combine_exclude) == NULLREGION)
         {
            ::DeleteObject(hrgn);
            hrgna.remove_at(i);
            oswindowa.remove_at(i);
         }
         else
         {
            ::OffsetRgn(hrgn, -ptOffset.x, -ptOffset.y);
            i++;
         }
      }

      ::DeleteObject(hrgnOpaque);

   }


   void window_draw::get_wnda(user::interaction_ptr_array & wndpa)
   {
      wndpa = System.frames();
   }

   void window_draw::get_wnda(user::oswindow_array & oswindowa)
   {
      System.frames().get_wnda(oswindowa);
   }

   // Both the device context and clip region
   // should be in screen coordinates

   bool window_draw::ScreenOutput(
      user::buffer * pbuffer,
      ::draw2d::region & rgnUpdate)
   {
      UNREFERENCED_PARAMETER(pbuffer);
      UNREFERENCED_PARAMETER(rgnUpdate);
   //   TRACE("//\n");
   //   TRACE("window_draw::TwfScreenOutput\n");
   //   TRACE("//\n");

//      DWORD dwTimeIn = get_tick_count();

      user::oswindow_array oswindowa;
      
      get_wnda(oswindowa);

      user::window_util::SortByZOrder(oswindowa);

      ASSERT(FALSE);
      /*for(int32_t i = 0; i  < oswindowa.get_size(); i++)
      {
         oswindow oswindow = oswindowa[i];
         ScreenOutput(
            pbuffer,
            oswindow,
            rgnUpdate);
      }*/
//      DWORD dwTimeOut = get_tick_count();
   //   TRACE("//\n");
   //   TRACE("// window_draw::TwfRender\n");
   //   TRACE("// TickCount = %d \n", dwTimeOut - dwTimeIn);
   //   TRACE("//\n");

      return true;
   }



   bool window_draw::ScreenOutput(
      // pdc is the source primitive::memory device context
      // from which bitmap the screen is updated.
      user::buffer * pbuffer,
      // oswindowParam ::user::window device context
      // is used from screen output
      sp(::user::interaction) pwnd)
   {
      if(pwnd != NULL)
      {
         if(pwnd->m_bVoidPaint)
         {
            return true;
         }
         if(!pwnd->m_bVisible)
         {
            return true;
         }
         if(pwnd->IsIconic())
         {
            return true;
         }
         if(pwnd->m_pguie != NULL && pwnd->m_pguie != pwnd && !pwnd->m_pguie->m_bVisible)
         {
            return true;
         }
         if(pwnd->m_pimpl != NULL && pwnd->m_pimpl != pwnd && !pwnd->m_pimpl->m_bVisible)
         {
            return true;
         }
      }

      sp(::win::window) pwindow = pwnd;

      if(pwindow->m_spdib.is_null())
         pwindow->m_spdib.create(allocer());

      pwindow->m_spdib->update_window(pwindow, NULL);

      return true;


   }




   // The first ::user::window handle in the array must belong
   // to the higher z order ::user::window.
   // The rectangle must contain all update region.
   // It must be in screen coordinates.



   // Remark: this optimization eliminates top level windows
   // that are lower z order siblings of a higher z order
   // top level ::user::window that contains all
   // the update region in a opaque area.
   // It doesn´t eliminates from the update parent windows
   // obscured by opaque children.



} // namespace win