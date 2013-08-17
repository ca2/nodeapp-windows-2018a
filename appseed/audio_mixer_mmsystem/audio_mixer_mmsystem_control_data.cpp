#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      int32_t control_data::GetObjectType()
      {

         return DataTypeBase;

      }


      int32_t control_data::get_type()
      {

         return m_iType;

      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




