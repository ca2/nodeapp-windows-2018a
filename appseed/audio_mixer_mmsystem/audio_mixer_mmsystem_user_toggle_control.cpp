#include "framework.h"


namespace audio_mixer
{


   namespace user
   {


      toggle_control::toggle_control() :
      control(new ::audio_mixer::control_data_switch())
      {

      }


      toggle_control::~toggle_control()
      {

      }


      ::audio_mixer::control_data_switch & toggle_control::get_data()
      {

         return *(::audio_mixer::control_data_switch* ) m_pdata;

      }


   } // namespace user


} // namespace audio_mixer





