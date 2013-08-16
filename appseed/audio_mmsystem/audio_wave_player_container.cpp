#include "framework.h"

namespace audio
{

   wave_player_container::wave_player_container(sp(::ca2::application) papp) :
      ca2(papp)
   {
      m_pwaveplayer = __begin_thread < ::audio::wave_player > (get_app());
   }

   wave_player_container::~wave_player_container()
   {
   }

   ::audio::wave_player * wave_player_container::GetWavePlayer()
   {
      return m_pwaveplayer;
   }
}
