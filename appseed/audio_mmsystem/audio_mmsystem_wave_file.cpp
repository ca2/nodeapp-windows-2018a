#include "framework.h"


namespace audio
{


   wave_file::wave_file(sp(::ca2::file) pfile)
   {

      m_spfile = pfile;

   }


   wave_file::~wave_file()
   {
   }


} // namespace audio



