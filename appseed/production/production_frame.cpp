#include "framework.h"


namespace production
{


   frame::frame(::aura::application * papp) :
      element(papp),
      simple_frame_window(papp)
   {
      m_dataid = "::ca2::production::frame";
   }

   frame::~frame()
   {
   }


   #ifdef DEBUG
   void frame::assert_valid() const
   {
	   simple_frame_window::assert_valid();
   }

   void frame::dump(dump_context & dumpcontext) const
   {
	   simple_frame_window::dump(dumpcontext);
   }


   #endif //DEBUG




   void frame::install_message_handling(::message::dispatch * pinterface)
   {
      simple_frame_window::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &frame::_001OnCreate);
   }

   void frame::_001OnCreate(::signal_details * pobj)
   {

      pobj->previous();

   }


} // namespace production



