#pragma once


namespace audio_mixer
{


   class control_data_switch;


   namespace user
   {


      class CLASS_DECL_CA2_MULTIMEDIA toggle_control :
         public control
      {
      public:


         toggle_control();
         virtual ~toggle_control();


         virtual check::e_check _001GetCheck() = 0;
         virtual void _001SetCheck(check::e_check echeck) = 0;
         virtual void set_label(const char * lpcszLabel) = 0;

         ::audio_mixer::control_data_switch & get_data();


      };


   } // namespace user


} // namespace audio_mixer





