#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         label::label() :
            control(new ::multimedia::audio_mixer::control_data)
         {

         }


         label::~label()
         {

         }


         ::multimedia::audio_mixer::control_data & label::get_data()
         {

            return *(::multimedia::audio_mixer::control_data* ) m_pdata;

         }


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia








