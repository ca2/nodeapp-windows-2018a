#pragma once


namespace audio
{


   class wave_in;


   class CLASS_DECL_CA2_MULTIMEDIA wave_in_listener
   {
   public:


      comparable_array < wave_in *, wave_in * > m_waveinptra;


      wave_in_listener();
      virtual ~wave_in_listener();

      virtual void WaveInDataProc(wave_in * pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr);

      void HookWaveIn(wave_in * pwavein);
      void UnhookWaveIn(wave_in * pwavein);


   };



} // namespace audio



