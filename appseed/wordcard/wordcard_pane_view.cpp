#include "framework.h"


namespace wordcard
{


   pane_view::pane_view(::aura::application * papp) :
      ::object(papp),
      ::user::tab(papp),

      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {



   }


   pane_view::~pane_view()
   {

   }




#ifdef DEBUG
   void pane_view::assert_valid() const
   {
      ::user::impact::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
      ::user::impact::dump(dumpcontext);
   }
#endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(::message::message * pobj)
   {
      //      SCAST_PTR(::message::create, pcreate, pobj);
      if (pobj->previous())
         return;

      add_tab("Wordcard", PaneViewWordcard);
      add_tab("Options", PaneViewConfiguration);

      set_cur_tab_by_id(PaneViewWordcard);

   }

   void pane_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if (lHint == 543218)
      {
         set_cur_tab_by_id(PaneViewWordcard);
      }
      if (pHint != NULL)
      {
         if (base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *)pHint;
            if (puh->is_type_of(pane_view_update_hint::TypeGetView))
            {
            }
            else if (puh->is_type_of(pane_view_update_hint::TypeSetView))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
      if (pHint != NULL)
      {
         if (base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *)pHint;
            if (puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               ((GetTopLevelFrame()))->SetActiveView(this);
            }
            else if (puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {

            }

         }

      }

   }


   void pane_view::on_show_view()
   {
      ::userex::pane_tab_view::on_show_view();
      //sp(frame) pframe =  (dynamic_cast < ::user::interaction_impl * > (GetParentFrame()));
      if (m_pviewdata->m_id == PaneViewFileManager)
      {
      }
      else if (m_pviewdata->m_id == PaneViewContextMenu)
      {
         sp(::filemanager::manager) pdoc = (m_pviewdata->m_pdoc);
         pdoc->FileManagerBrowse(System.dir().appdata() / "wordcard\\menu", ::action::source::system_default());
      }
      else
      {
      }
   }



   bool pane_view::pre_create_window(::user::create_struct& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return ::user::impact::pre_create_window(cs);
   }


   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
      switch (pcreatordata->m_id)
      {
      case PaneViewContextMenu:
      {
         sp(::filemanager::manager) pdoc = Session.filemanager()->open_child_list(false, true);
         if (pdoc != NULL)
         {
            pdoc->get_filemanager_data()->m_iIconSize = 16;
            pdoc->get_filemanager_data()->m_bListText = true;
            pdoc->get_filemanager_data()->m_bListSelection = false;
            pdoc->get_filemanager_template()->m_strFilePopup = "filemanager\\file_popup.xml";
            pdoc->get_filemanager_data()->m_strDataKeyModifier = "wordcard_menu";
            pdoc->get_filemanager_data()->m_pcallback = this;
            pdoc->Initialize(true);
            pdoc->update_all_views(NULL, 1234);
            pdoc->update_all_views(NULL, 123458);
            sp(::user::impact) pview = pdoc->get_view();
            pdoc->FileManagerBrowse(System.dir().appdata() / "wordcard\\menu", ::action::source::system_default());
            if (pview != NULL)
            {
               sp(::user::frame_window) pframe = (pview->GetParentFrame());
               if (pframe != NULL)
               {
                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                  pframe->SetParent(this);
                  pcreatordata->m_pdoc = pdoc;
                  pcreatordata->m_pwnd = pframe;
               }
            }
         }
      }
      break;
      case PaneViewWordcard:
      {

         m_pdocMenu = Application.create_child_form(this, pcreatordata->m_pholder);

         ::user::impact * pview = m_pdocMenu->get_view(0);

         prepare_wordcard_view();

         //m_pviewdata->m_pwnd = pview->GetParentFrame();

      }
      break;
      /*      case PaneViewFileManager:
               {
                  sp(::create) cc(get_app());
                  cc->m_bMakeVisible = false;
                  cc->m_bTransparentBackground = true;
                  cc->m_puiParent = this;

                  get_document()->m_pfilemanagerdoc = Sess(papp).filemanager()->open(papp, cc);
                  sp(::filemanager::manager) pdoc = get_document()->m_pfilemanagerdoc;
                  if(pdoc != NULL)
                  {
                     pdoc->get_filemanager_data()->m_strDataKeyModifier = "wordcard_filemanager";
                     pdoc->Initialize(true);
                     pdoc->update_all_views(NULL, 1234);
                     pdoc->update_all_views(NULL, 123458);
                     sp(::user::impact) pview = pdoc->get_view();
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
         sp(::filemanager::manager) pdoc = Session.filemanager()->open_child_list(false, true);
         if (pdoc != NULL)
         {
            pdoc->get_filemanager_data()->m_iIconSize = 48;
            pdoc->get_filemanager_data()->m_bListText = false;
            pdoc->get_filemanager_data()->m_bListSelection = false;
            pdoc->get_filemanager_data()->m_pcallback = this;
            pdoc->get_filemanager_data()->m_strDataKeyModifier = "wordcard_3-action-launch";
            pdoc->Initialize(true);
            pdoc->update_all_views(NULL, 1234);
            pdoc->update_all_views(NULL, 123458);
            sp(::user::impact) pview = pdoc->get_view();
            pdoc->FileManagerBrowse(System.dir().appdata() / "wordcard\\3-action-launch", ::action::source::system_default());
            if (pview != NULL)
            {
               sp(::user::frame_window) pframe = (pview->GetParentFrame());
               if (pframe != NULL)
               {
                  pframe->ModifyStyle(WS_CAPTION, WS_CHILD, 0);
                  pframe->SetParent(this);
                  pcreatordata->m_pdoc = pdoc;
                  pcreatordata->m_pwnd = pframe;
               }
            }
         }
      }
      break;
      case PaneViewConfiguration:
      {
         sp(::user::document) pdoc = Application.create_form(this, pcreatordata->m_pholder);
         if (pdoc == NULL)
            return;
         sp(::user::impact) pview = pdoc->get_view();
         m_pviewOptions = (pview);

         m_pviewOptions->m_pcallback = this;

         ::user::form_update_hint uh;
         uh.m_actioncontext = ::action::source::system_default();
         uh.m_etype = ::user::form_update_hint::type_browse;
         uh.m_strForm = "wordcard\\options.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = ::user::form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = ::user::form_update_hint::type_after_browse;
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


   bool pane_view::prepare_wordcard_view()
   {

      m_pdocMenu->open_document("matter://wordcard.html");

      ::user::form_view * pview = m_pdocMenu->get_typed_view<::user::form_view>(0);

      pview->m_pformcallback = this;

      m_pedit = dynamic_cast <::user::plain_edit *> (pview->get_child_by_id("name"));

      string str;

      data_get("name", str);

      if (m_pedit != NULL)
      {

         m_pedit->_001SetText(str, ::action::source_database);

      }

      return true;

   }



   void pane_view::_001OnMenuMessage(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_routing(::message::sender * pinterface)
   {
      ::user::tab_view::install_message_routing(pinterface);

      IGUI_MSG_LINK(WM_USER, pinterface, this, &pane_view::_001OnUserMessage);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
      //	IGUI_MSG_LINK(WM_SIZE, pinterface, this, &pane_view::_001OnSize);
      IGUI_MSG_LINK(WM_USER + 1122, pinterface, this, &pane_view::_001OnMenuMessage);


   }


   void pane_view::OnFileManagerOpenFile(
   ::filemanager::data * pdata,
   ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      if (itema.get_size() > 0)
      {
         int_ptr i = (int_ptr) ::ShellExecuteW(
                     NULL,
                     NULL,
                     ::str::international::utf8_to_unicode("\"" + itema[0]->m_filepath + "\""),
                     NULL,
                     ::str::international::utf8_to_unicode("\"" + itema[0]->m_filepath.folder() + "\""),
                     SW_SHOWNORMAL);
         //string str;
         //str.Format("%d", i);
         //Application.simple_message_box(str);
         if (i == ERROR_FILE_NOT_FOUND)
         {
         }
      }
      GetParentFrame()->ShowWindow(SW_HIDE);
   }

   bool pane_view::BaseOnControlEvent(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_button_clicked)
      {

         if (pevent->m_puie->m_id == "do_card")
         {

            string str;

            if (m_pedit != NULL)
            {

               m_pedit->_001GetText(str);

            }
            else
            {

               str = "(Nobody)";

            }

            ::fork(get_app(), [=]()
            {

               wordcard_do(str);

            });

         }

      }
      else if (pevent->m_eevent == ::user::event_after_change_text)
      {

         if (pevent->m_puie->m_id == "name" && pevent->m_actioncontext.is_user_source())
         {

            if (m_pedit != NULL)
            {

               string str;

               m_pedit->_001GetText(str);

               data_set("name", str);

            }

         }

      }
      else if (pevent->m_eevent == ::user::event_set_check)
      {
         if (pevent->m_puie->m_id == "clean")
         {
            sp(::user::interaction) pui = m_pviewOptions->get_child_by_id("clean");
            sp(::user::check_box) pcheckbox = (pui);
            view_update_hint uh;
            uh.m_eoption = view_update_hint::OptionClean;
            uh.m_bOption = pcheckbox->_001GetCheck() == check::checked;
            get_document()->update_all_views(NULL, 0, &uh);
         }
         else if (pevent->m_puie->m_id == "build")
         {
            sp(::user::interaction) pui = m_pviewOptions->get_child_by_id("build");
            sp(::user::check_box) pcheckbox = (pui);
            view_update_hint uh;
            uh.m_eoption = view_update_hint::OptionBuild;
            uh.m_bOption = pcheckbox->_001GetCheck() == check::checked;
            get_document()->update_all_views(NULL, 0, &uh);
         }
      }
      return false;
   }


   sp(::wordcard::document) pane_view::get_document()
   {
      return  (::user::impact::get_document());
   }

   void pane_view::_001OnUserMessage(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if (pbase->m_wparam == 1)
      {
         set_cur_tab_by_id("tabbed_file_manager");
      }
   }


} // namespace wordcard




