#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA callback
   {
   public:


      callback();
      virtual ~callback();


      virtual ::audio_mixer::user::level_control * allocate_level_control();
      virtual ::audio_mixer::user::toggle_control * allocate_toggle_control();
      virtual ::audio_mixer::user::label * allocate_label();

   };


} // namespace audio_mixer






