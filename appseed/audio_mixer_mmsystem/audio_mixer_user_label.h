#pragma once


namespace audio_mixer
{


   namespace user
   {


      class CLASS_DECL_CA2_MULTIMEDIA label :
         public control
      {
      public:


         label();
         virtual ~label();


         virtual void SetText(const char * lpcszText) = 0;
         ::audio_mixer::control_data & get_data();


      };


   } // namespace user


} // namespace audio_mixer






