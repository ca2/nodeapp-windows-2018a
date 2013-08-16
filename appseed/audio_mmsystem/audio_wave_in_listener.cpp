#include "framework.h"


namespace audio
{


   wave_in_listener::wave_in_listener()
   {

   }

   wave_in_listener::~wave_in_listener()
   {
      for(int32_t i = 0; i < m_waveinptra.get_size(); i++)
      {
         m_waveinptra[i]->RemoveListener(this);
      }
   }

   void wave_in_listener::WaveInDataProc(wave_in *pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr)
   {
      UNREFERENCED_PARAMETER(pwavein);
      UNREFERENCED_PARAMETER(dwSampleTime);
      UNREFERENCED_PARAMETER(lpwavehdr);
   }

   void wave_in_listener::HookWaveIn(wave_in * pwavein)
   {
      m_waveinptra.add_unique(pwavein);
   }

   void wave_in_listener::UnhookWaveIn(wave_in * pwavein)
   {
      m_waveinptra.remove(pwavein);
   }



} // namespace audio



