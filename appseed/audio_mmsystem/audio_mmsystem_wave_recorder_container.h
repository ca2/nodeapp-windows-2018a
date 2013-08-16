#pragma once


namespace audio
{


   class CLASS_DECL_CA2_MULTIMEDIA wave_recorder_container :
      virtual public ::ca2::object
   {
   public:


      ::audio::wave_recorder *   m_pwaverecorder;


      wave_recorder_container(sp(::ca2::application) papp);
      virtual ~wave_recorder_container();


      ::audio::wave_recorder * GetWaveRecorder();


   };


} // audio



