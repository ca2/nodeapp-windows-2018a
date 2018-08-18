#include "framework.h"
//#include <Psapi.h>



namespace wordcard
{


   view::view(::aura::application * papp) :
      ::object(papp),
      ::user::interaction(papp), 
      m_dibV(allocer()),
      m_dibVs(allocer()),
      m_brushBkActive(allocer()),
      m_brushBkInactive(allocer())
   {

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

   }


   view::~view()
   {

   }


   void view::install_message_routing(::message::sender * pinterface)
   {
      ::user::scroll_view::install_message_routing(pinterface);

	   IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);
	   IGUI_MSG_LINK(WM_SIZE, pinterface, this, &view::_001OnSize);
	   //IGUI_MSG_LINK(WM_PAINT, pinterface, this, &view::_001OnPaint);
	   IGUI_MSG_LINK(WM_CREATE, pinterface, this, &view::_001OnCreate);
	   IGUI_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &view::_001OnContextMenu);
	   IGUI_MSG_LINK(WM_SETCURSOR, pinterface, this, &view::_001OnSetCursor);

   //   IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
   //   IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
      IGUI_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

      IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &view::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &view::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_RBUTTONUP, pinterface, this, &view::_001OnRButtonUp);
      

      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &view::_001OnShowWindow);
      IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);

      IGUI_MSG_LINK(WM_USER, pinterface, this, &view::_001OnUser);

      

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

   bool view::pre_create_window(::user::create_struct& cs)
   {
      cs.style &= ~WS_EX_CLIENTEDGE;
	   return ::user::impact::pre_create_window(cs);
   }

   void view::_001OnInitialUpdate() 
   {
      ::user::impact::_001OnInitialUpdate();
   }

   void view::on_update(::user::impact * pSender, LPARAM lHint, ::object * phint) 
   {
      UNREFERENCED_PARAMETER(pSender);
      if(lHint == 5432108)
      {
         SetTimer(5432108, 100, NULL);
      }
      else if(phint != NULL)
      {
      }
   	
   }

   void view::_001OnDestroy(::message::message * pobj) 
   {
	   ::user::impact::_001OnDestroy(pobj);
   }

   void view::_001OnSize(::message::message * pobj) 
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::size, psize, pobj);

      rect rectDesktop;
      best_monitor(rectDesktop);

      m_iWScreen = rectDesktop.width();
      m_iHScreen = rectDesktop.height();
      rect rectClient;
      GetClientRect(rectClient);
      rectClient.deflate(2, 2);
      int32_t iW = rectClient.width() / 2;
      int32_t iH = rectClient.height() / 2;
      iH = MIN(iH, 120);
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


   void view::on_viewport_offset(::draw2d::graphics * pgraphics)
   {

   }


   void view:: _001OnDraw(::draw2d::graphics * pgraphics)
   {

      
      

   }

   void view::_001OnCreate(::message::message * pobj) 
   {
      if(pobj->previous())
         return;

      class rect rect;
      rect.null();

	   //if(!m_pscrollbarHorz->create_window(
		  // orientation_horizontal,
		  // WS_CHILD 
		  // | WS_VISIBLE,
		  // rect,
		  // this,
		  // 1024))
    //  {
		  // return;
    //  }

    //  rect.null();

	   //if(!m_pscrollbarVert->create_window(
		  // orientation_vertical,
		  // WS_CHILD 
		  // | WS_VISIBLE,
		  // rect,
		  // this,
		  // 1025))
    //  {
		  // return;
    //  }

      SetTimer(31, 230, NULL);
      SetTimer(3003, 300, NULL);
      SetTimer(543218, 200, NULL);
      m_dibV.load_from_matter("wild_mountains_and_valleys-123.png");
      //System.visual().imaging().free(pfi);

      m_dibVs.load_from_matter("bambu49transr.png");
      //System.visual().imaging().free(pfi);

      

      //make_wordcard();




      Application.m_pview = this;

      //m_pwordcard->twitter_auth();
//      m_pwordcard->twitter_twit("starting ca2 wordcard application");




   }


   void view::_001OnContextMenu(::message::message * pobj) 
   {
      SCAST_PTR(::message::context_menu, pcontextmenu, pobj);
      point point = pcontextmenu->GetPoint();
   }


   void view::_001OnTabClick(int32_t iTab)
   {
      if(iTab == 1)
      {
      }
   }


   void view::_001OnSetCursor(::message::message * pobj) 
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->m_ecursor = ::visual::cursor_arrow;
   	
	   pobj->previous();

   }


   ::user::document * view::get_document()
   {

      return ::user::impact::get_document();

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

   void view::_001OnLButtonDown(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
  //    SCAST_PTR(::message::mouse, pmouse, pobj);

//      int32_t iHitArea = hit_test(pmouse->m_pt);

   }

   void view::_001OnLButtonUp(::message::message * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);

      class point point = pmouse->m_pt;
      ScreenToClient(&point);
      ::user::control::e_element eelement;
      index iHitArea = hit_test(point, eelement);
 

   }

   void view::_001OnRButtonUp(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
  //    SCAST_PTR(::message::mouse, pmouse, pobj);

//      int32_t iHitArea = hit_test(pmouse->m_pt);
   /*   {
         ::aura::menu menu;
         menu.LoadXmlMenu(get_app(), "wordcard\\popup_wordcard.xml");
         menu.set_app(get_app());
         ::aura::menu menuPopup(menu.GetSubMenu(0));
         menuPopup.set_app(get_app());
         menuPopup.track_popup_menu(0, pmouse->m_pt.x, pmouse->m_pt.y, GetParentFrame()->GetSafeoswindow_());
      }
     */ 
   }




   void view::_001OnTimer(::timer * ptimer) 
   {
      ::user::scroll_view::_001OnTimer(ptimer);
      if(ptimer->m_nIDEvent == 543218)
      {
         if(IsWindowVisible())
         {
            //set_need_redraw();
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
      }
   }





   void view::_001OnShowWindow(::message::message * pobj) 
   {
      UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::show_window, pshowwindow, pobj);
   }






   void view::_001OnUser(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_wparam == 1)
      {
         int32_t iLineHeight = m_iLineHeight;
         {
         }
         set_need_redraw();
      }
      else if(pbase->m_wparam == 2)
      {


      }
   }



   size view::get_total_size()
   {

      return m_sizeTotal;

   }


} // namespace wordcard




