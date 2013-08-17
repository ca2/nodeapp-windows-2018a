#pragma once


namespace audio_mmsystem
{


   class  CLASS_DECL_AUDIO_MMSYSTEM wave_recorder_container :
      virtual public ::ca2::object
   {
   public:


      ::audio_mmsystem::wave_recorder *   m_pwaverecorder;


      wave_recorder_container(sp(::ca2::application) papp);
      virtual ~wave_recorder_container();


      ::audio_mmsystem::wave_recorder * GetWaveRecorder();


   };


} // audio_mmsystem



