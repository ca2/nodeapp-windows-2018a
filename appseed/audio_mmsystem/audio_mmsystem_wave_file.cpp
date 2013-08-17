#include "framework.h"


namespace audio_mmsystem
{


   wave_file::wave_file(sp(::ca2::file) pfile)
   {

      m_spfile = pfile;

   }


   wave_file::~wave_file()
   {
   }


} // namespace audio_mmsystem



