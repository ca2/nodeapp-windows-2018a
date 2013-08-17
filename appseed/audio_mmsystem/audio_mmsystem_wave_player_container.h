#pragma once


namespace audio_mmsystem
{


   class  CLASS_DECL_AUDIO_MMSYSTEM wave_player_container :
      virtual public ::ca2::object
   {
   public:


      ::audio_mmsystem::wave_player *   m_pwaveplayer;


      wave_player_container(sp(::ca2::application) papp);
      virtual ~wave_player_container();


      ::audio_mmsystem::wave_player * get_wave_player();


   };


} // namespace audio_mmsystem




