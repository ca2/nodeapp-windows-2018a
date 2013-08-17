#include "framework.h"



namespace audio_mmsystem
{

   wave_player_container::wave_player_container(sp(::ca2::application) papp) :
      ca2(papp)
   {
      m_pwaveplayer = __begin_thread < ::audio_mmsystem::wave_player > (get_app());
   }

   wave_player_container::~wave_player_container()
   {
   }

   ::audio_mmsystem::wave_player * wave_player_container::get_wave_player()
   {
      return m_pwaveplayer;
   }
}
