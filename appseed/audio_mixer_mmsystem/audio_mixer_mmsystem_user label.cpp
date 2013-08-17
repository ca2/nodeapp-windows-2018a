#include "framework.h"


namespace audio_mixer
{


   namespace user
   {


      label::label() :
         control(new ::audio_mixer::control_data)
      {

      }


      label::~label()
      {

      }


      ::audio_mixer::control_data & label::get_data()
      {

         return *(::audio_mixer::control_data* ) m_pdata;

      }


   } // namespace user


} // namespace audio_mixer






