#include "framework.h"


namespace backup
{

   pane_view::pane_view(::aura::application * papp) :
      ::object(papp),
      ::user::tab(papp),
      
      ::userex::pane_tab_view(papp),
      ::user::tab_view(papp),
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

   void pane_view::_001OnCreate(::signal_details * pobj) 
   {
//      SCAST_PTR(::message::create, pcreate, pobj);

      if(pobj->previous())
         return;

      add_tab("backup", pane_view_backup);
      add_tab("Options", pane_view_options);
      
      set_cur_tab_by_id(pane_view_backup);
      
   }

   void pane_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint) 
   {
      ::user::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
         set_cur_tab_by_id(pane_view_backup);
      }
      if(pHint != NULL)
      {
         if(base_class <pane_view_update_hint >::bases(pHint))
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
   	   if(base_class <pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {
               
            }

         }

      }

   }




   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
//      sp(application) papp =  (get_app());
      switch(pcreatordata->m_id)
      {
      case pane_view_backup:
         {
            pcreatordata->m_pwnd = create_view < view > ();
            if(pcreatordata->m_pwnd != NULL)
            {
               pcreatordata->m_pdoc = get_document();
            }
         }
         break;
      case pane_view_options:
      {
         sp(::user::document) pdoc = Application.create_form(this, this);
         if(pdoc == NULL)
            return;
         sp(::user::impact) pview = pdoc->get_view();
         ::user::form_update_hint uh;
         uh.m_actioncontext = ::action::source::system_default();
         uh.m_etype = ::user::form_update_hint::type_browse;
         uh.m_strForm = "filemanager\\replace_name_in_file_system.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);
         
         uh.m_etype = ::user::form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = ::user::form_update_hint::type_after_browse;
         pdoc->update_all_views(NULL, 0, &uh);


         pcreatordata->m_pwnd = (pview->GetParentFrame());
//         sp(form_child_frame) pframe = (pcreatordata->m_pwnd);
         pcreatordata->m_pdoc = pdoc;

      }
      break;
      default:
         ASSERT(FALSE);
         break;
      }

   }


   void pane_view::install_message_handling(::message::dispatch * pinterface)
   {
      ::userex::pane_tab_view::install_message_handling(pinterface);
	   IGUI_WIN_MSG_LINK(WM_CREATE       , pinterface, this, &pane_view::_001OnCreate);
   }


} // namespace backup