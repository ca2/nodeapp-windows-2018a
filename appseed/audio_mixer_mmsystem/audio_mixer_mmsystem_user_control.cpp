#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         control::control(::multimedia::audio_mixer::control_data * pdata)
         {

            m_pdata = pdata;

         }


         control::~control()
         {

            if(m_pdata != NULL)
            {

               delete m_pdata;

               m_pdata = NULL;

            }

         }


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




