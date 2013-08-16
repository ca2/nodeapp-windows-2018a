#pragma once


namespace audio
{


   class CLASS_DECL_CA2_MULTIMEDIA wave_player_container :
      virtual public ::ca2::object
   {
   public:


      ::audio::wave_player *   m_pwaveplayer;


      wave_player_container(sp(::ca2::application) papp);
      virtual ~wave_player_container();


      ::audio::wave_player * GetWavePlayer();


   };


} // namespace audio




