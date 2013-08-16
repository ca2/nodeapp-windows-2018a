#pragma once


namespace audio
{


   class wave_file
   {
   public:


      ::ca2::filesp  m_spfile;


      wave_file(sp(::ca2::file) pfile);
      ~wave_file();

      
   };



} // namespace audio



