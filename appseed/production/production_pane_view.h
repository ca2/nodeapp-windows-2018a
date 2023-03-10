#pragma once


namespace production
{


   class document;


   class CLASS_DECL_CA2_PRODUCTION pane_view :
      public ::userex::pane_tab_view,
      public ::filemanager::callback,
      public ::production::form_callback
   {
   public:


      sp(::user::form_view)      m_pviewOptions;


      pane_view(::aura::application * papp);
      virtual ~pane_view();


      DECL_GEN_SIGNAL(_001OnUserMessage);

      sp(::production::document) get_document();

      void on_control_event(::user::control_event * pevent);

      virtual void on_create_view(::user::view_creator_data * pcreatordata);
      virtual void on_show_view();

      virtual void install_message_routing(::message::sender * pinterface);

      virtual void on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint);
      virtual bool pre_create_window(::user::create_struct& cs);


      void OnFileManagerOpenFile(
      ::filemanager::data * pdata,
      ::fs::item_array & itema);

      DECL_GEN_SIGNAL(_001OnMenuMessage);
#ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
#endif

      DECL_GEN_SIGNAL(_001OnCreate);
   };


} // namespace production




