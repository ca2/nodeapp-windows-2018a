#pragma once


namespace twitter
{


   class CLASS_DECL_CA2_PRODUCTION authorization :
      virtual public ::user::form_callback,
      virtual public ::user::view_creator,
      virtual public ::user::tab_callback
   {
   public:


      string                                    m_strPin;
      string                                    m_strAuthorizationUrl;
      int                                       m_iView;
      ::userbase::single_document_template *    m_ptemplatePane;
      form_document *                           m_pdoc;
      ::userex::pane_tab_view *                   m_ptabview;
      form_document *                           m_pdocAuth;
      form_document *                           m_pdocMessage;
      form_view *                               m_pviewAuth;
      bool                                      m_bVotagusAuth;
      string                                    m_strForm;
      string                                    m_strLicense;
      bool                                      m_bLicense;
      bool                                      m_bInteractive;

      authorization(::ca::application * papp,const char * pszAuthorizationUrl, const char * pszForm, bool bVotagusAuth = false, bool bInteractive = true);
      virtual ~authorization();

      void ensure_main_document();
      void display_main_frame();
      void page1();
      void pageMessage(const char * pszMatter, gen::property_set & set);

      string get_pin();

      void on_create_view(::user::view_creator_data * pcreatordata);
      virtual void on_show_view();


      virtual bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };


} // namespace ca8

