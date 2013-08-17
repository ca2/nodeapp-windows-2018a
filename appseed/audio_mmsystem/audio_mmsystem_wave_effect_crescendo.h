#pragma once


namespace audio_mmsystem
{


   class wave_effect_crescendo :
      public ::iaudio::wave_stream_effect
   {
   public:


      int32_t      m_iLength; // in samples
      bool     m_bSustain;
      int16_t    m_shFinalScaleMul;
      int16_t    m_shFinalScaleDiv;
      bool     m_bTriggerEnd;
      int32_t      m_iStep;
      double   m_dDifferentialRate;
      double   m_dCurrentRate;


      wave_effect_crescendo();
      virtual ~wave_effect_crescendo();


      virtual bool IsEndOfEffect();
      virtual int32_t Process16bits(int16_t * pshStream, int32_t iCount);

      void SetFinalScale(int16_t shMul, int16_t shDiv);

      void SetLengthMillis(uint32_t dwMillis, uint32_t dwSamplesPerSec);
      void Initialize();


   };


} // namespace audio_mmsystem


