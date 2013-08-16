#pragma once


namespace audio
{


   class wave_in;


   class CLASS_DECL_CA2_MULTIMEDIA wave :
      virtual public ::ca2::object
   {
   public:


      sp(wave_in)             m_pwavein;
      uint32_t                m_uiWaveInDevice;


      wave(sp(::ca2::application) papp);
      virtual ~wave();

      wave_in * GetWaveIn();


      void close();
      bool initialize();
      bool finalize();


   };


} // namespace audio



