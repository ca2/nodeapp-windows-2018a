#include "framework.h"
//#include <Psapi.h>



namespace production
{


   view::view(sp(base_application) papp) :
      element(papp),
      ::user::interaction(papp), 
      ::user::scroll_view(papp),
      m_scrollbarVert(papp),
      m_scrollbarHorz(papp),
      m_dibV(allocer()),
      m_dibVs(allocer()),
      m_brushBkActive(allocer()),
      m_brushBkInactive(allocer())
   {
      m_pproduction = NULL;
      
      //System.file().ftd("C:\\home2\\ca2os\\ca2_spa\\stage", "C:\\teste.fileset");


      m_iLineHeight = 1;
      m_bLayout = false;
      m_brushBkActive->create_solid(RGB(150, 255, 130));
      m_brushBkInactive->create_solid(RGB(128, 208, 120));
      LOGFONT lf;
      memset(&lf, 0, sizeof(lf));

      strcpy(lf.lfFaceName, "Verdana");
      lf.lfHeight = 13;
      lf.lfHeight = 15;
      lf.lfWeight = 900;
      m_iV = 123;
      m_iVH = 49;
      m_iVW = 123;
      m_iVs = 124;
      m_iVsH = 49;
      m_iVsW = 41;

      

         m_pscrollbarVert  = &m_scrollbarVert;
         m_pscrollbarHorz  = &m_scrollbarHorz;

   }

   view::~view()
   {
   }

   void view::install_message_handling(::message::dispatch * pinterface)
   {
      ::user::scroll_view::install_message_handling(pinterface);

	   IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);
	   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &view::_001OnSize);
	   //IGUI_WIN_MSG_LINK(WM_PAINT, pinterface, this, &view::_001OnPaint);
	   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &view::_001OnCreate);
	   IGUI_WIN_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &view::_001OnContextMenu);
	   IGUI_WIN_MSG_LINK(WM_SETCURSOR, pinterface, this, &view::_001OnSetCursor);

   //   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
   //   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
      IGUI_WIN_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &view::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &view::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_RBUTTONUP, pinterface, this, &view::_001OnRButtonUp);
      IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &view::_001OnTimer);

      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &view::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);

      IGUI_WIN_MSG_LINK(WM_USER, pinterface, this, &view::_001OnUser);

      

   }

   #ifdef DEBUG
   void view::assert_valid() const
   {
	   ::user::impact::assert_valid();
   }

   void view::dump(dump_context & dumpcontext) const
   {
	   ::user::impact::dump(dumpcontext);
   }
   #endif //DEBUG

   bool view::pre_create_window(CREATESTRUCT& cs)
   {
      cs.style &= ~WS_EX_CLIENTEDGE;
	   return ::user::impact::pre_create_window(cs);
   }

   void view::_001OnInitialUpdate(::signal_details * pobj) 
   {
      ::user::impact::_001OnInitialUpdate(pobj);
   }

   void view::on_update(sp(::user::impact) pSender, LPARAM lHint, ::object * phint) 
   {
      UNREFERENCED_PARAMETER(pSender);
      if(lHint == 5432108)
      {
         SetTimer(5432108, 100, NULL);
      }
      else if(phint != NULL)
      {
         view_update_hint * puh = dynamic_cast < view_update_hint * > (phint);
         if(puh != NULL)
         {
            if(puh->m_eoption == view_update_hint::OptionClean)
            {
               m_pproduction->m_bClean = puh->m_bOption;
            }
            else if(puh->m_eoption == view_update_hint::OptionBuild)
            {
               m_pproduction->m_bBuild = puh->m_bOption;
            }
         }
      }
   	
   }

   void view::_001OnDestroy(::signal_details * pobj) 
   {
	   ::user::impact::_001OnDestroy(pobj);
   }

   void view::_001OnSize(::signal_details * pobj) 
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::size, psize, pobj)

      rect rectDesktop;
      Application.get_screen_rect(rectDesktop);

      m_iWScreen = rectDesktop.width();
      m_iHScreen = rectDesktop.height();
      rect rectClient;
      GetClientRect(rectClient);
      rectClient.deflate(2, 2);
      int32_t iW = rectClient.width() / 2;
      int32_t iH = rectClient.height() / 2;
      iH = min(iH, 120);
      double r = (double) iW / (double) iH;
      double rScreen = (double) rectDesktop.width() / (double) rectDesktop.height();
      if(r < rScreen)
      {
         iH = (int32_t) (iW / rScreen);
      }
      else if(r > rScreen)
      {
         iW = (int32_t) (iH * rScreen);
      }
      m_iW = iW;
      m_iH = iH;
   }


   void view:: _001OnDraw(::draw2d::graphics * pdc)
   {
      

      rect rectClient;
      GetClientRect(rectClient);

      pdc->SelectObject(System.visual().font_central().GetListCtrlFont());

      size sz = pdc->GetTextExtent("��qg");
      m_iLineHeight = max(1, sz.cy);

      pdc->FillSolidRect(rectClient, ARGB(255, 255, 255, 255));

      rect rectText(rectClient);

      rectText.bottom -= 84;

      rect rcItem;

      int32_t iStart = m_scrollinfo.m_ptScroll.y / m_iLineHeight;
      int32_t y = m_iLineHeight - m_scrollinfo.m_ptScroll.y % m_iLineHeight;
      if(m_scrollinfo.m_ptScroll.y > m_iLineHeight)
      {
         iStart--;
         y -= m_iLineHeight;
      }
      rect rectClip(rectText);
      //ClientToScreen(rectClip);
      ::draw2d::region_sp rgnClip(allocer());
      rgnClip->create_rect(rectClip);
      //pdc->Draw3dRect(rectText, RGB(200, 200, 200), RGB(200, 200, 200));
      pdc->SelectClipRgn(rgnClip);
      //single_lock sl(&m_pproduction->m_mutexStatus, TRUE);
      //pdc->set_color(ARGB(0xcc, 84, 84, 84));
      ::draw2d::brush_sp brush(allocer());
      brush->create_solid(ARGB(0xcc, 84, 84, 84));
      pdc->SelectObject(brush);
      for(int32_t i = iStart; i < m_pproduction->m_straStatus.get_size() && y < rectText.bottom; i++)
      {
         rcItem = rectText;
         rcItem.bottom = y + m_iLineHeight;
         rcItem.top = y ;
         pdc->draw_text(m_pproduction->m_straStatus[i], rcItem, DT_BOTTOM | DT_LEFT);
         y = rcItem.bottom;
      }
      pdc->SelectClipRgn(NULL);



      rect rectArea;

      GetAreaThumbRect(rectArea, m_iV);
      m_dibV->to(pdc, rectArea);

      GetAreaThumbRect(rectArea, m_iVs);
      m_dibVs->to(pdc, rectArea);

      if(!m_pproduction->m_bFinished)
      {
         string strTime;
         uint32_t dwMin = (m_pproduction->m_dwEndTick - m_pproduction->m_dwStartTick) / 1000 / 60;
         uint32_t dwSeg = ((m_pproduction->m_dwEndTick - m_pproduction->m_dwStartTick) / 1000) % 60;
         strTime.Format("%dm %ds", dwMin, dwSeg);
         pdc->TextOut(rectArea.right + 23, rectArea.top, strTime);
      }

   }

   void view::_001OnCreate(::signal_details * pobj) 
   {
      if(pobj->previous())
         return;

      class rect rect;
      rect.null();

	   if(!m_pscrollbarHorz->create(
		   ::user::scroll_bar::orientation_horizontal,
		   WS_CHILD 
		   | WS_VISIBLE,
		   rect,
		   this,
		   1024))
      {
		   return;
      }

      rect.null();

	   if(!m_pscrollbarVert->create(
		   ::user::scroll_bar::orientation_vertical,
		   WS_CHILD 
		   | WS_VISIBLE,
		   rect,
		   this,
		   1025))
      {
		   return;
      }

      SetTimer(31, 230, NULL);
      SetTimer(3003, 300, NULL);
      SetTimer(543218, 200, NULL);
      m_dibV.load_from_matter("wild_mountains_and_valleys-123.png");
      //System.visual().imaging().free(pfi);

      m_dibVs.load_from_matter("bambu49transr.png");
      //System.visual().imaging().free(pfi);

      

      //make_production();


      m_pproduction = create_production();

      m_pproduction->m_pview  = this;

      //m_pproduction->twitter_auth();
//      m_pproduction->twitter_twit("starting ca2 production application");



      if(Application.command()->m_varTopicQuery.has_property("start"))
      {
         make_production();
      }
      else if(Application.command()->m_varTopicQuery.has_property("start_deferred"))
      {
         production_loop(Application.command()->m_varTopicQuery["start_deferred"]);
      }

   }


   void view::_001OnContextMenu(::signal_details * pobj) 
   {
      SCAST_PTR(::message::context_menu, pcontextmenu, pobj)
      point point = pcontextmenu->GetPoint();
   }


   void view::_001OnTabClick(int32_t iTab)
   {
      if(iTab == 1)
      {
      }
   }


   void view::_001OnSetCursor(::signal_details * pobj) 
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->m_ecursor = ::visual::cursor_arrow;
   	
	   pobj->previous();

   }


   sp(::user::object) view::get_document()
   {
      return  (::user::impact::get_document());
   }

   void view::GetAreaThumbRect(LPRECT lprect, int32_t iArea)
   {
      rect rectClient;
      GetClientRect(rectClient);
      if(iArea == m_iV)
      {
         lprect->bottom = rectClient.bottom;
         lprect->top = lprect->bottom - m_iVH;
         lprect->left = 1;
         lprect->right = lprect->left + m_iVW;
      }
      else if(iArea == m_iVs)
      {
         lprect->bottom = rectClient.bottom;
         lprect->top = lprect->bottom - m_iVsH;
         lprect->left = 1 + m_iVW + 10;
         lprect->right = lprect->left + m_iVsW;
      }
   }

   index view::hit_test(point pt, ::user::control::e_element & eelement)
   {
      rect rectArea;
      GetAreaThumbRect(rectArea, m_iV);
      if(rectArea.contains(pt))
      {
         eelement = element_area;
         return m_iV;
      }
      GetAreaThumbRect(rectArea, m_iVs);
      if(rectArea.contains(pt))
      {
         eelement = element_area;
         return m_iVs;
      }
      eelement = element_none;
      return -1;
   }

   void view::_001OnLButtonDown(::signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
  //    SCAST_PTR(::message::mouse, pmouse, pobj)

//      int32_t iHitArea = hit_test(pmouse->m_pt);

   }

   void view::_001OnLButtonUp(::signal_details * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj)

      class point point = pmouse->m_pt;
      ScreenToClient(&point);
      ::user::control::e_element eelement;
      index iHitArea = hit_test(point, eelement);
      if(iHitArea == m_iV)
      {
         make_production();
      }
      else if(iHitArea == m_iVs)
      {
         make_production();
      }

   }

   void view::_001OnRButtonUp(::signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
  //    SCAST_PTR(::message::mouse, pmouse, pobj)

//      int32_t iHitArea = hit_test(pmouse->m_pt);
   /*   {
         ::user::menu menu;
         menu.LoadXmlMenu(get_app(), "production\\popup_production.xml");
         menu.set_app(get_app());
         ::user::menu menuPopup(menu.GetSubMenu(0));
         menuPopup.set_app(get_app());
         menuPopup.TrackPopupMenu(0, pmouse->m_pt.x, pmouse->m_pt.y, GetParentFrame()->GetSafeoswindow_());
      }
     */ 
   }




   void view::_001OnTimer(::signal_details * pobj)
   {
      SCAST_PTR(::message::timer, ptimer, pobj)
      if(ptimer->m_nIDEvent == 543218)
      {
         if(IsWindowVisible())
         {
            //RedrawWindow();
         }
      }
      else if(ptimer->m_nIDEvent == 5432108)
      {
         KillTimer(ptimer->m_nIDEvent);
      }
      else if(ptimer->m_nIDEvent == 5432180)
      {
         KillTimer(ptimer->m_nIDEvent);
      }
      else if(ptimer->m_nIDEvent == 3003)
      {
      }
      else if(ptimer->m_nIDEvent == 31)
      {
         if(!m_pproduction->m_bFinished)
         {
            m_pproduction->m_dwEndTick = ::GetTickCount();
            _001RedrawWindow();
         }

      }
   }





   void view::_001OnShowWindow(::signal_details * pobj) 
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::show_window, pshowwindow, pobj)
   }

   void view::make_production()
   {
      m_iStep = 1;
      sp(application) papp =  (get_app());
      m_pproduction->start_production(papp->m_eversion);
   }


   void view::production_loop(int32_t iLoopCount)
   {
      m_iStep = 1;
      sp(application) papp =  (get_app());
      m_pproduction->start_loop(papp->m_eversion, iLoopCount);
   }

   void view::_001OnUser(::signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_wparam == 1)
      {
         int32_t iLineHeight = m_iLineHeight;
         single_lock sl(&m_pproduction->m_mutexStatus, TRUE);
         if(m_pproduction->m_straStatus.get_size() > 0)
         {
            m_scrollinfo.m_sizeTotal.cx = 80;
            m_scrollinfo.m_sizeTotal.cy = (LONG) (m_pproduction->m_straStatus.get_size() * iLineHeight + 84);
            m_scrollinfo.m_ptScroll.y = max(0, m_scrollinfo.m_sizeTotal.cy - m_scrollinfo.m_sizePage.cy + iLineHeight);
            _001LayoutScrollBars();
         }
         else
         {
            m_scrollinfo.m_sizeTotal.cx = 80;
            m_scrollinfo.m_sizeTotal.cy = 80;
         }
         _001RedrawWindow();
      }
      else if(pbase->m_wparam == 2)
      {

         stringa stra;
         stringa straRelative;
         stringa straTitle;
         string strRemote;

         sp(pane_view) pview = GetTypedParent < pane_view > ();

         pview->set_cur_tab_by_id("tabbed_file_manager");

         sp(::filemanager::document) pdoc = pview->get_tabbed_filemanager_document();

         string strBase = m_pproduction->m_strBase;

         m_iStep = 2;

         pdoc->get_operation_doc(true)->m_thread.queue_copy(
            m_pproduction->m_straFiles, 
            "C:\\ca2\\vrel\\stage\\" + m_pproduction->m_strFormatBuild, 
            strBase,
            false, 
            this);

         pdoc->get_operation_doc(true)->m_thread.kick();
         
      }
      else if(pbase->m_wparam == 0x1000)
      {
         if(m_iStep == 2)
         {
            sp(pane_view) pview = GetTypedParent < pane_view > ();
            pview->set_cur_tab_by_id(PaneViewProduction);
            m_pproduction->step();
         }
      }
   }

   production * view::create_production()
   {
      production * pclass = new production(get_app());
      pclass->m_eversion = Application.m_eversion;
      return pclass;
   }


} // namespace production




