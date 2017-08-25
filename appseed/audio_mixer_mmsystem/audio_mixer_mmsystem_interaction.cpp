#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      interaction::interaction(::aura::application * papp) :
         ::object(papp)
      {
         create_message_queue("");
      }

      interaction::~interaction()
      {
      }

      void interaction::install_message_routing(::message::sender * pinterface)
      {
         ::user::interaction::install_message_routing(pinterface);
         IGUI_MSG_LINK(MM_MIXM_CONTROL_CHANGE, pinterface, this, &interaction::_001OnMixerControlChange);
         IGUI_MSG_LINK(MM_MIXM_LINE_CHANGE, pinterface, this, &interaction::_001OnMixerLineChange);
      }

      
      void interaction::_001OnMixerControlChange(::message::message * pmessage)
      {
         
         SCAST_MSG(base);

         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerControlChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);

         pbase->set_lresult(0);

      }


      void interaction::_001OnMixerLineChange(::message::message * pmessage)
      {
         
         SCAST_MSG(base);

         (dynamic_cast < ::multimedia::audio_mixer_mmsystem::audio_mixer * > (m_pmixer))->OnMixerLineChange((HMIXER) pbase->m_wparam, (uint32_t) pbase->m_lparam);

         pbase->set_lresult(0);

      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia






