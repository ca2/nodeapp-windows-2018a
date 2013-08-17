#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         toggle_control::toggle_control() :
            control(new ::multimedia::audio_mixer::control_data_switch())
         {

         }


         toggle_control::~toggle_control()
         {

         }


         ::multimedia::audio_mixer::control_data_switch & toggle_control::get_data()
         {

            return *(::multimedia::audio_mixer::control_data_switch* ) m_pdata;

         }


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





