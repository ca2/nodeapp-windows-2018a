#pragma once

#include "pane_view_update_hint.h"

namespace backup
{

   class CLASS_DECL_CA2_BACKUP pane_view : 
      public ::userex::pane_tab_view,
      public production::form_callback
   {
   public:
	   pane_view(::ca::application * papp);
	   virtual ~pane_view();
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      void on_create_view(::user::view_creator_data * pcreatordata);
      void install_message_handling(::gen::message::dispatch * pinterface);

	   virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint);



	   DECL_GEN_SIGNAL(_001OnCreate)
   };


} // namespace backup

