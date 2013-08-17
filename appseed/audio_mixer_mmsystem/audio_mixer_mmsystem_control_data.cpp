#include "framework.h"


namespace audio_mixer
{

   int32_t control_data::GetObjectType()
   {
      return DataTypeBase;
   }


   int32_t control_data::get_type()
   {
      return m_iType;
   }


} // namespace audio_mixer



