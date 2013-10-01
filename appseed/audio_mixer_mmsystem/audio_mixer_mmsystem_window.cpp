#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      window::window(sp(base_application) papp) :
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
         IGUI_WIN_MSG_LINK(MM_MIXM_CONTROL_CHANGE, pinterface, this, &window::_001OnMixerControlChange);
         IGUI_WIN_MSG_LINK(MM_MIXM_LINE_CHANGE, pinterface, this, &window::_001OnMixerLineChange);
      }

      void window::_001OnMixerControlChange(::ca2::signal_object * pobj)
      {
         IGUI_WIN_CST(base);
         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerControlChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
         pbase->set_lresult(0);
      }

      void window::_001OnMixerLineChange(::ca2::signal_object * pobj)
      {
         IGUI_WIN_CST(base);
         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerLineChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
         pbase->set_lresult(0);
      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia






