#pragma once

namespace production
{

   class CLASS_DECL_CA2_PRODUCTION frame : 
      public simple_frame_window
   {
   public:
	   frame(sp(base_application) papp);
   public:
	   virtual ~frame();
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      void install_message_handling(::message::dispatch * pinterface);

      DECL_GEN_SIGNAL(_001OnCreate)
   };

} // namespace production