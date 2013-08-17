#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class control_data_switch;


      namespace user
      {


         class CLASS_DECL_AUDIO_MIXER_MMSYSTEM toggle_control :
            public control
         {
         public:


            toggle_control();
            virtual ~toggle_control();


            virtual check::e_check _001GetCheck() = 0;
            virtual void _001SetCheck(check::e_check echeck) = 0;
            virtual void set_label(const char * lpcszLabel) = 0;

            ::multimedia::audio_mixer::control_data_switch & get_data();


         };


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




