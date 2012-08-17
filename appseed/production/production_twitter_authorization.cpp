#include "framework.h"
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>


typedef string ( *SALT)(::ca::application *, const char * , stringa &);

namespace twitter
{

   authorization::authorization(::ca::application * papp, const char * pszAuthorizationUrl, const char * pszForm, bool bVotagusAuth, bool bInteractive) :
      ca(papp)
   {
      m_strAuthorizationUrl=pszAuthorizationUrl;
      m_bInteractive    = bInteractive;
      m_bVotagusAuth    = bVotagusAuth;
      m_strForm         = pszForm;
      m_ptemplatePane   = new ::userbase::single_document_template(
         papp,
         "system/auth",
         System.template type_info < form_document > (),
         System.template type_info < simple_frame_window > (),
         System.template type_info < userex::pane_tab_view > ());
      m_pviewAuth       = NULL;
      m_pdocAuth        = NULL;
      m_pdoc            = NULL;
      m_ptabview        = NULL;
   }

   authorization::~authorization()
   {
   }

   string authorization::get_pin()
   {
      ensure_main_document();
      page1();
      m_pviewAuth->SetTimer(1984, 484, NULL);
      m_ptabview->get_wnd()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE);
      m_ptemplatePane->close_all_documents(FALSE);
      return m_strPin;
   }

   void authorization::ensure_main_document()
   {
      if(m_pdoc != NULL)
         return;
      ::ca::create_context_sp createcontext(get_app());
      createcontext->m_bMakeVisible = false;
      createcontext->m_puiParent = Sys(get_app()).oprop("top_parent").ca2 < ::user::interaction > ();
      createcontext->m_bOuterPopupAlertLike = true;
      m_pdoc = dynamic_cast < form_document * > (m_ptemplatePane->open_document_file(createcontext));
      userex::pane_tab_view * pview = m_pdoc->get_typed_view < userex::pane_tab_view >();
      pview->set_view_creator(this);
      m_ptabview = pview;
      pview->add_tab("ca2open", 1);
      pview->set_cur_tab_by_id(1);
   }


   void authorization::page1()
   {
      string strUrl;
      strUrl = "http://spaignition.api.laborserver.net/query?node=install_application&id=";
      string strAppName;
      if(System.m_strAppName == "winactionarea")
      {
         strAppName = "_set_windesk";
      }
      else
      {
         strAppName = System.m_strAppName;
      }
      strUrl += strAppName;
      strUrl += "&key=name";
      m_pdocAuth->get_html_data()->m_propertyset["application_name"] = System.http().get(strUrl);
      m_pdocAuth->get_html_data()->m_propertyset["authorization_link"] = m_strAuthorizationUrl;
      if(m_strLicense.is_empty())
      {
         m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Authenticating";
      }
      else
      {
         m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Licensing";
      }
      if(!m_pdocAuth->on_open_document(Application.dir().matter(m_strForm)))
      {
         return;
      }
      ::html::elemental * pelemental = m_pdocAuth->get_html_data()->get_element_by_id("authorization_link");
      ::html::impl::text * ptext = dynamic_cast < ::html::impl::text * > (pelemental->m_pimpl);
      ptext->m_strLink = "ext://" + m_strAuthorizationUrl;
      ptext->m_bLink = true;
      display_main_frame();
   }

   void authorization::display_main_frame()
   {
      rect rectOpen;
      if(m_ptabview->GetParentFrame()->GetParent() == NULL)
      {

         System.get_screen_rect(rectOpen);
      }
      else
      {
         m_ptabview->GetParentFrame()->GetParent()->GetClientRect(rectOpen);
      }
      int iWidth = rectOpen.width();
      int iHeight = rectOpen.height();
      rectOpen.deflate(iWidth / 5, iHeight / 50);
      rectOpen.top = iHeight * 2 / 3;
      simple_frame_window * pframe = dynamic_cast < simple_frame_window * > (m_pviewAuth->GetTopLevelParent());
      if(pframe != NULL)
      {
         pframe->m_etranslucency = ::user::interaction::TranslucencyPresent;
         pframe->m_bblur_Background = true;
      }
      if(&Bergedge != NULL && Bergedge.get_document() != NULL && Bergedge.get_document()->get_bergedge_view() != NULL)
      {
         Bergedge.get_document()->get_bergedge_view()->ShowWindow(SW_SHOW);
      }
      else
      {
         m_ptabview->GetTopLevelFrame()->ShowWindow(SW_SHOW);
      }
      m_ptabview->GetParentFrame()->SetWindowPos(
         ZORDER_TOP,
         rectOpen.left, rectOpen.top,
         rectOpen.width(), rectOpen.height(), 
         SWP_SHOWWINDOW);
      m_pviewAuth->GetTopLevelParent()->SetForegroundWindow();
      m_pviewAuth->GetTopLevelParent()->BringWindowToTop();
   }

   void authorization::pageMessage(const char * pszMatter, gen::property_set & set)
   {
      ensure_main_document();
      m_pdocAuth->get_html_data()->m_propertyset = set;
      m_pdocAuth->on_open_document(Application.dir().matter(pszMatter));
      display_main_frame();
      m_ptabview->get_wnd()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE);
      m_ptabview->get_wnd()->EndAllModalLoops(IDOK);
   }

   void authorization::on_create_view(::user::view_creator_data * pcreatordata)
   {
      switch(pcreatordata->m_id)
      {
      case 1:
         {
            m_pdocAuth = Cube.create_child_form(this, pcreatordata->m_pholder);
            if(m_pdocAuth != NULL)
            {
               m_pviewAuth = m_pdocAuth->get_typed_view < form_view > ();
               m_pviewAuth->m_pcallback = this;
               pcreatordata->m_pdoc = m_pdocAuth;
               pcreatordata->m_pwnd = m_pviewAuth->GetParentFrame();
            }
         }
         break;

         break;
      }
      if(pcreatordata->m_pwnd != NULL)
      {
         pcreatordata->m_eflag.signalize(::user::view_creator_data::flag_hide_all_others_on_show);
      }

   }
   void authorization::on_show_view()
   {
      switch(get_view_id())
      {
      case 1:
         {
         }
         break;

      }
   }








   bool authorization::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pview);
      if(pevent->m_eevent == ::user::event_button_clicked
         || pevent->m_eevent == ::user::event_enter_key)
      {
         if(pevent->m_puie->m_id == "submit" ||
            pevent->m_eevent == ::user::event_enter_key)
         {
            ::user::interaction * pguie = m_pviewAuth->get_child_by_name("pin");
            text_interface * ptext = dynamic_cast < text_interface * > (pguie);
            ptext->_001GetText(m_strPin);
            m_ptabview->get_wnd()->EndModalLoop(IDOK);
            m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
         }
      }
      return false;
   }




} // namespace twitter

