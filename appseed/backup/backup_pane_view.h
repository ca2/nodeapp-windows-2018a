#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP pane_view : 
      public ::userex::pane_tab_view,
      public production::form_callback
   {
   public:
	   pane_view(sp(::ca::application) papp);
	   virtual ~pane_view();
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      void on_create_view(::user::view_creator_data * pcreatordata);
      void install_message_handling(::ca::message::dispatch * pinterface);

	   virtual void on_update(sp(::user::view) pSender, LPARAM lHint, ::ca::object* pHint);



	   DECL_GEN_SIGNAL(_001OnCreate)
   };


} // namespace backup



