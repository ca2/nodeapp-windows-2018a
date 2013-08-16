#include "framework.h"


namespace audio
{


   wave_in_listener_set::wave_in_listener_set()
   {

   }


   wave_in_listener_set::~wave_in_listener_set()
   {

   }


   void wave_in_listener_set::WaveInDataProc(wave_in *pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr)
   {

      for(int32_t i = 0; i < this->get_size(); i++)
      {

         element_at(i)->WaveInDataProc(pwavein, dwSampleTime, lpwavehdr);

      }

   }


} // namespace audio




