#include "framework.h"


namespace audio_mixer
{


   namespace user
   {


      control::control(::audio_mixer::control_data * pdata)
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


} // namespace audio_mixer



