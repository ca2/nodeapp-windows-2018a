#pragma once


namespace audio
{


   typedef int16_t WAVEBUFFERDATA;
   typedef WAVEBUFFERDATA * LPWAVEBUFFERDATA;


   typedef array < WAVEBUFFERDATA > wave_buffer_data_array;


   class CLASS_DECL_CA2_MULTIMEDIA wave_buffer :
      virtual public ::ca2::object
   {
   public:


      class buffer
      {
      protected:
         bool        m_bShortCompress;
      public:
         bool        m_bFFT;
         bool FFTProcessed();
         bool OnMultimediaDone();
         int32_t         m_iIndex;
         WAVEHDR     m_wavehdr;
         void *      m_pData;

      };


      class BufferArray :
         public array <buffer, buffer>
      {
      };


      uint32_t                  m_uiBufferSize;
      uint32_t                  m_uiAnalysisSize;
      uint32_t                  m_uiAnalysisSizeOrder;
      uint32_t                  m_uiAllocationSize;
      uint32_t                  m_uiInterestSize;
      uint32_t                  m_uiSkippedSamplesCount;

      uint_ptr                      m_dwCurrentBuffer;
      wave_buffer_data_array        m_bdaData;
      wave_buffer_data_array        m_bdaBuffer;
      array < byte >            m_baData;
      array < byte >            m_baBuffer;

      array < double >          m_daSetA;
      array < double >          m_daSetB;

      array < double >          m_daModuleSet;
      array < double >          m_daResultSet;

      critical_section              m_csResult;
      critical_section              m_csProcLevel1;
      critical_section              m_csProcLevel2;
      BufferArray                   m_buffera;
      kissfft::fftd *               m_pfft;
      array < kiss_fft_cpx >    m_pcpxa;

      double                        m_dNominalMaxLevel; // Divisor que faz o nível máximo ser um.

      WAVEFORMATEX                  m_waveformatexIn;
      WAVEFORMATEX                  m_waveformatexOut;


      wave_buffer(sp(::ca2::application) papp);
      virtual ~wave_buffer();


      double GetMaxLevel();
      void InterestCompressModuleSet();
      int32_t GetBufferCount();
      double PeakValueFrequency(double dMin, double dMax, double dSampleFrequency, double * dAmplitude);
      double PeakValueFrequency(double dSampleFrequency, double * dAmplitude);
      double PeakValueFrequency(double * dAmplitude);
      void HighPassFilterData();
      void PeakValueFrequencies(double_array & daFreq, double_array & daAmp, int32_t iCount);
      void FFTProcess(LPWAVEHDR lpwavehdr);
      void FFTProcess(int32_t iBuffer);
      void PCMOutProcessWAVEHDR(LPWAVEHDR lpwavehdr);
      void PCMOutProcess(int32_t iBuffer);
      LPBYTE PCMOutGetBuffer(int32_t iIndex);
      void InterestCopyModuleToResult(double * lpdMax);
      void InterestCalcDoubleSetBModule();
      void InterestCalcSetBModule();
      void FFTOpen(uint32_t uiAllocationSize, uint32_t uiBufferSize, uint32_t uiAnalisysSize, uint32_t uiInterestSize, uint32_t uiSkippedSamplesCount);
      void PCMOutOpen(uint32_t uiBufferSize, uint32_t uiBufferCount, LPWAVEFORMATEX lpwfexIn, LPWAVEFORMATEX lpwfexOut);
      void ZeroDoubleSetAImaginary();
      void CopyDataToDoubleSetA(int32_t iIndex);
      void CopyDataToSetA(int32_t iIndex);
      void FFTDoubleSetADoubleSetB(bool bInverseTransform);
      void FFTSetASetB();
      buffer & GetBuffer(int32_t iIndex);
      void * get_data(int32_t iIndex);
      buffer * get_item(int32_t iIndex);
      LPWAVEHDR GetHdr(int32_t iIndex);
      double * FFTGetResult();
      LPVOID   PCMOutGetInBuffer();
      uint32_t     PCMOutGetInBufferSize();


   };


} // namespace audio




