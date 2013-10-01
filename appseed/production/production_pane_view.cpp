#include "framework.h"


namespace production
{


   pane_view::pane_view(sp(base_application) papp) :
      element(papp),
      ::user::tab(papp),
      
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {

      m_etranslucency      = TranslucencyPresent;

   }


   pane_view::~pane_view()
   {

   }




   #ifdef DEBUG
   void pane_view::assert_valid() const
   {
	   ::user::view::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
	   ::user::view::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(::ca2::signal_object * pobj) 
   {
//      SCAST_PTR(::ca2::message::create, pcreate, pobj)
      if(pobj->previous())
         return;

      add_tab("Production", PaneViewProduction);
      add_tab("Options", PaneViewConfiguration);

      set_cur_tab_by_id(PaneViewProduction);
      
   }

   void pane_view::on_update(sp(::user::view) pSender, LPARAM lHint, ::object* pHint) 
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
         set_cur_tab_by_id(PaneViewProduction);
      }
      if(pHint != NULL)
      {
         if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeGetView))
            {
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeSetView))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
	      if(pHint != NULL)
      {
   	   if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               ( (GetTopLevelFrame()))->SetActiveView(this);
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {
               
//               int32_t iTab;
   //            if(puh->m_eview == PaneViewContextMenu)
     //          {
       //           m_tab._001AddSel(0);
         //      }
   //            else
               {
               }
               
            }
         }
      }


   }

   void pane_view::on_show_view()
   {
      ::userex::pane_tab_view::on_show_view();
      //sp(frame) pframe =  (dynamic_cast < ::ca2::window * > (GetParentFrame()));
      if(m_pviewdata->m_id == PaneViewFileManager)
      {
      }
      else if(m_pviewdata->m_id == PaneViewContextMenu)
      {
         sp(::filemanager::document) pdoc =  (m_pviewdata->m_pdoc);
         pdoc->FileManagerBrowse(Application.dir().userappdata("production\\menu"));
      }
      else
      {
      }
   }



   bool pane_view::pre_create_window(CREATESTRUCT& cs) 
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	

	   return ::user::view::pre_create_window(cs);
   }


   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
      sp(application) papp =  (( (GetParentFrame()))->get_app());
      switch(pcreatordata->m_id)
      {
      case PaneViewContextMenu:
         {
            sp(::filemanager::document) pdoc = papp->filemanager().std().open_child_list(false, true);
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_iIconSize = 16;
               pdoc->get_filemanager_data()->m_bListText = true;
               pdoc->get_filemanager_data()->m_bListSelection = false;
               pdoc->get_filemanager_data()->m_ptemplate->m_strFilePopup = "filemanager\\file_popup.xml";
               pdoc->get_filemanager_data()->m_strDISection = "production_menu";
               pdoc->get_filemanager_data()->m_pcallback = this;
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               pdoc->FileManagerBrowse(Application.dir().userappdata("production\\menu"));
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;
                  }
               }
            }
         }
         break;
      case PaneViewProduction:
         {
            sp(::user::interaction) puie = ::user::view::create_view < ::production::view > (get_document(), pcreatordata->m_pholder);
            if(puie != NULL)
            {
               pcreatordata->m_pdoc = get_document();
            }
         }
         break;
/*      case PaneViewFileManager:
         {
            sp(::create_context) cc(get_app());
            cc->m_bMakeVisible = false;
            cc->m_bTransparentBackground = true;
            cc->m_puiParent = this;

            get_document()->m_pfilemanagerdoc = papp->filemanager().std().open(papp, cc);
            sp(::filemanager::document) pdoc = get_document()->m_pfilemanagerdoc;
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_strDISection = "production_filemanager";
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                     //pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     //pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;
                  }
               }
            }
         }
         break;*/
      case PaneViewThreeActionLaunch:
         {
            sp(::filemanager::document) pdoc = papp->filemanager().std().open_child_list(false, true);
            if(pdoc != NULL)
            {
               pdoc->get_filemanager_data()->m_iIconSize = 48;
               pdoc->get_filemanager_data()->m_bListText = false;
               pdoc->get_filemanager_data()->m_bListSelection = false;
               pdoc->get_filemanager_data()->m_pcallback = this;
               pdoc->get_filemanager_data()->m_strDISection = "production_3-action-launch";
               pdoc->Initialize(true);
               pdoc->update_all_views(NULL, 1234);
               pdoc->update_all_views(NULL, 123458);
               sp(::user::view) pview = pdoc->get_view();
               pdoc->FileManagerBrowse(Application.dir().userappdata("production\\3-action-launch"));
               if(pview != NULL)
               {
                  sp(::user::frame_window) pframe =  (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                     pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                     pframe->set_parent(this);
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_pwnd = pframe;
                  }
               }
            }
         }
         break;
      case PaneViewConfiguration:
      {
         sp(form_document) pdoc = System.user()->create_form(this, pcreatordata->m_pholder);
         if(pdoc == NULL)
            return;
         sp(::user::view) pview = pdoc->get_view();
         m_pviewOptions =  (pview);

         m_pviewOptions->m_pcallback = this;
         
         form_update_hint uh;
         uh.m_etype = form_update_hint::type_browse;
         uh.m_strForm = "production\\options.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);
         
         uh.m_etype = form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = form_update_hint::type_after_browse;
         pdoc->update_all_views(NULL, 0, &uh);


         pcreatordata->m_pwnd = (pview->GetParentFrame());
         sp(form_child_frame) pframe = (pcreatordata->m_pwnd);
         pcreatordata->m_pdoc = pdoc;
         //pcreatordata->m_pwnd = pframe;

      }
      break;
      default:
         break;
      }
      ::userex::pane_tab_view::on_create_view(pcreatordata);
   }




   void pane_view::_001OnMenuMessage(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      ::user::tab_view::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_USER, pinterface, this, &pane_view::_001OnUserMessage);
	   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
   //	IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &pane_view::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_USER + 1122  , pinterface, this, &pane_view::_001OnMenuMessage);


   }


   void pane_view::OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      if(itema.get_size() > 0)
      {
         int32_t i = (int32_t) ::ShellExecuteW(
            NULL, 
            NULL, 
            ::ca2::international::utf8_to_unicode("\"" + itema[0].m_strPath + "\""),
            NULL,
            ::ca2::international::utf8_to_unicode("\"" + System.dir().name(itema[0].m_strPath) + "\""),
            SW_SHOWNORMAL);
         string str;
         str.Format("%d", i);
         //Application.simple_message_box(str);
         if(i == ERROR_FILE_NOT_FOUND)
         {
         }
      }
      GetParentFrame()->ShowWindow(SW_HIDE);
   }

     bool pane_view::BaseOnControlEvent(::user::control_event * pevent)
     {
      if(pevent->m_eevent == ::user::event_set_check)
      {
         if(pevent->m_puie->m_id == "clean")
         {
            sp(::user::interaction) pguie = m_pviewOptions->get_child_by_id("clean");
            sp(::user::check_box) pcheckbox =  (pguie);
            view_update_hint uh;
            uh.m_eoption = view_update_hint::OptionClean;
            uh.m_bOption = pcheckbox->_001GetCheck() == check::checked;
            get_document()->update_all_views(NULL, 0, &uh);
         }
         else if(pevent->m_puie->m_id == "build")
         {
            sp(::user::interaction) pguie = m_pviewOptions->get_child_by_id("build");
            sp(::user::check_box) pcheckbox =  (pguie);
            view_update_hint uh;
            uh.m_eoption = view_update_hint::OptionBuild;
            uh.m_bOption = pcheckbox->_001GetCheck() == check::checked;
            get_document()->update_all_views(NULL, 0, &uh);
         }
      }
      return false;
   }


     sp(::production::document) pane_view::get_document()
     {
        return  (::user::view::get_document());
     }

     void pane_view::_001OnUserMessage(::ca2::signal_object * pobj)
     {
        SCAST_PTR(::ca2::message::base, pbase, pobj)
           if(pbase->m_wparam == 1)
           {
               set_cur_tab_by_id("tabbed_file_manager");
           }
     }

} // namespace production