#include "framework.h"


namespace audio
{

   wave::wave(sp(::ca2::application) papp) :
      ca2(papp)
   {
#ifdef WINDOWS
      m_uiWaveInDevice = WAVE_MAPPER;
#endif
      m_pwavein = NULL;
   }

   wave::~wave()
   {
   }


   wave_in * wave::GetWaveIn()
   {
      return m_pwavein;
   }

   bool wave::initialize()
   {

      if(m_pwavein != NULL)
         return true;

      System.factory().creatable_small < ::audio::wave_effect_devocalizer::Group >();
      System.factory().creatable_small < ::audio::wave_effect_devocalizer::Group::Channel >();

      m_pwavein = __begin_thread < wave_in > (get_app());
      if(m_pwavein != NULL)
      {
         m_pwavein->m_evInitialized.wait();
      }

      return m_pwavein != NULL;
   }

   bool wave::finalize()
   {

      return true;

   }

   void wave::close()
   {
      if(m_pwavein != NULL)
      {
         m_pwavein->Stop();
         m_pwavein->Reset();
         m_pwavein->close();
         m_pwavein->m_p->post_thread_message(WM_QUIT);
      }
   }


} // namespace audio


