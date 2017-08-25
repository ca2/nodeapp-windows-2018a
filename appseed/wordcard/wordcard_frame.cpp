#include "framework.h"


namespace wordcard
{


   frame::frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp)
   {
      m_dataid = "::ca2::wordcard::frame";
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




   void frame::install_message_routing(::message::sender * pinterface)
   {
      simple_frame_window::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &frame::_001OnCreate);
   }

   void frame::_001OnCreate(::message::message * pobj)
   {

      pobj->previous();

   }


} // namespace wordcard



