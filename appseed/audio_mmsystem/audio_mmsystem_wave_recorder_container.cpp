#include "framework.h"


namespace audio_mmsystem
{


   wave_recorder_container::wave_recorder_container(sp(::ca2::application) papp) :
      ca2(papp)
   {
      m_pwaverecorder = __begin_thread < ::audio_mmsystem::wave_recorder > (get_app());
   }

   wave_recorder_container::~wave_recorder_container()
   {
   }

   ::audio_mmsystem::wave_recorder * wave_recorder_container::GetWaveRecorder()
   {
      return m_pwaverecorder;
   }


} // namespace audio_mmsystem



