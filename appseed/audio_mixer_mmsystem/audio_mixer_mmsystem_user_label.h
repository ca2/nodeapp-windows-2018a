#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         class CLASS_DECL_AUDIO_MIXER_MMSYSTEM label :
            public control
         {
         public:


            label();
            virtual ~label();


            virtual void SetText(const char * lpcszText) = 0;
            ::multimedia::audio_mixer::control_data & get_data();


         };


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia






