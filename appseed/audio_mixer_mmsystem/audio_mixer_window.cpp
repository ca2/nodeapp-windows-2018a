#include "framework.h"


namespace audio_mixer
{


   window::window(sp(::ca2::application) papp) :
      ca2(papp)
   {
      create_message_window("");
   }

   window::~window()
   {
   }

   void window::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      ::user::interaction::install_message_handling(pinterface);
      #ifdef WINDOWS
      IGUI_WIN_MSG_LINK(MM_MIXM_CONTROL_CHANGE, pinterface, this, &window::_001OnMixerControlChange);
      IGUI_WIN_MSG_LINK(MM_MIXM_LINE_CHANGE, pinterface, this, &window::_001OnMixerLineChange);
      #endif
   }

   void window::_001OnMixerControlChange(::ca2::signal_object * pobj)
   {
      IGUI_WIN_CST(base);
      #ifdef WINDOWS
      m_pmixer->OnMixerControlChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
      pbase->set_lresult(0);
      #endif
   }

   void window::_001OnMixerLineChange(::ca2::signal_object * pobj)
   {
      IGUI_WIN_CST(base);
      #ifdef WINDOWS
      m_pmixer->OnMixerLineChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
      pbase->set_lresult(0);
      #endif
   }


} // namespace audio_mixer



