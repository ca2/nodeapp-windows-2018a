#pragma once


/*typedef struct CLASS_DECL_CA2_MULTIMEDIA tagWaveProcDataMessage
{
uint32_t               m_msSampleTime;
uint32_t               m_dwBufferIndex;
LPWAVEHDR         m_lpwavehdr;
bool               m_bDelete;
wave_in *         m_pwavein;
} WAVEPROCDATAMESSAGE, * LPWAVEPROCDATAMESSAGE;*/


#define WAVM_WAVE_PROC_DATA 1000

namespace audio
{


   class wave_in;
   class wave_buffer;
   class wave_in_listener;


   class CLASS_DECL_CA2_MULTIMEDIA wave_in :
      public ::ca2::thread
   {
   public:


      enum  e_state
      {
         state_initial,
         StateOpened,
         StateRecording,
         state_stopping,
         StateStopped,
      };


      audio_decode::encoder *          m_pencoder;

      critical_section                 m_csHandle;
      bool                             m_bResetting;
      int32_t                              m_iRefCount;
      int32_t                              m_iBuffer;
      e_state                          m_estate;
      wave_buffer *                  m_pwavebuffer;

#ifdef WINDOWS
      HWAVEIN                          m_hWaveIn;
#else
      snd_pcm_t *                      m_ppcm;
      snd_pcm_hw_params_t *            m_phwparams;
#endif

      WAVEFORMATEX                     m_waveFormatEx;


      wave_in_listener_set             m_listenerset;

      event                            m_evInitialized;
      event                            m_eventExitInstance;
      event                            m_eventStopped;


      wave_in(sp(::ca2::application) papp);
      virtual ~wave_in();


      bool InitializeEncoder();

      MMRESULT AddBuffer(int32_t iBuffer);
      MMRESULT AddBuffer(LPWAVEHDR lpwavehdr);
      void RemoveListener(wave_in_listener * plistener);
      void AddListener(wave_in_listener * plistener);
      // Attributes
      critical_section & GetHandleCriticalSection();
      bool IsOpened();
      bool IsRecording();
#ifdef WINDOWS
      HWAVEIN GetSafeHwavein();
#endif
      uint32_t GetAnalysisMillis();
      bool IsResetting();

      uint32_t GetState();
      LPWAVEFORMATEX GetFormatEx();
      // Reference count
      int32_t Release();
      int32_t AddRef();


      wave_buffer & GetBuffer();

      // Operations
      MMRESULT open(int32_t iBufferCount, int32_t iBufferSampleCount);
      MMRESULT close();
      MMRESULT Stop();
      MMRESULT Start();
      MMRESULT Reset();

      // Event handlers
      virtual void TranslateWaveInMessage(::ca2::signal_object * pobj);

      virtual bool initialize_instance();
      virtual int32_t exit_instance();
      virtual void pre_translate_message(::ca2::signal_object * pobj);

#ifdef WINDOWS
      static void CALLBACK WaveInProc(HWAVEIN hwi, uint32_t uMsg,
         uint32_t dwInstance,
         uint32_t dwParam1,
         uint32_t dwParam2);
#endif

   };


} // namespace audio


