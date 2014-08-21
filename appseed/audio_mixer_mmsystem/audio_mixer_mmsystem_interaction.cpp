#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      interaction::interaction(sp(::axis::application) papp) :
         element(papp)
      {
         create_message_queue("");
      }

      interaction::~interaction()
      {
      }

      void interaction::install_message_handling(::message::dispatch * pinterface)
      {
         ::user::interaction::install_message_handling(pinterface);
         IGUI_WIN_MSG_LINK(MM_MIXM_CONTROL_CHANGE, pinterface, this, &interaction::_001OnMixerControlChange);
         IGUI_WIN_MSG_LINK(MM_MIXM_LINE_CHANGE, pinterface, this, &interaction::_001OnMixerLineChange);
      }

      void interaction::_001OnMixerControlChange(::signal_details * pobj)
      {
         IGUI_WIN_CST(base);
         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerControlChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
         pbase->set_lresult(0);
      }

      void interaction::_001OnMixerLineChange(::signal_details * pobj)
      {
         IGUI_WIN_CST(base);
         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerLineChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);
         pbase->set_lresult(0);
      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia






