#pragma once


namespace iaudio
{


   class wave_stream_effect;


} // namespace iaudio


namespace audio
{

   class wave_out;
   class wave_player;

   /*
   typedef struct CLASS_DECL_CA2_MULTIMEDIA tagWaveProcDataMessage
   {
   uint32_t         m_dwBufferIndex;
   LPWAVEHDR   m_lpwavehdr;
   bool         m_bDelete;
   wave_out *   m_pwaveout;
   } WAVEOUTDATAMESSAGE, * LPWAVEOUTDATAMESSAGE;
   */

   class wave_buffer;


   class CLASS_DECL_CA2_MULTIMEDIA wave_out :
      public ::ca2::thread,
      public ::audio::wave_pre_buffer_callback
   {
   public:

      enum e_message
      {
         MessageBufferRequest = WM_APP + 101,
         MessageBufferReady = WM_APP + 102,
         MessageClose = WM_APP + 103,
      };

      enum  e_state
      {
         state_initial,
         StateOpened,
         StatePlaying,
         StateStopping,
         StateStopped,
         StatePaused,
      };

      wave_pre_buffer *                  m_pprebuffer;
      critical_section                    m_csOut;
      array < LPWAVEHDR, LPWAVEHDR > m_lphdraFree;
      wave_player *                     m_pplayer;

      critical_section                    m_csPreBuffer;
      int32_t                                 m_iLastBufferId;

      array < event *, event * >   m_evptraEnd;
      critical_section                    m_csHandle;
      ::ca2::thread *                   m_pthreadCallback;
      HWAVEOUT                            m_hwaveout;
      WAVEFORMATEX                        m_waveformatex;
      wave_buffer *                     m_pwavebuffer;
      e_state                             m_estate;
      MMRESULT                            m_mmr;
      int32_t                                 m_iBufferInMMSys;
      ::iaudio::wave_stream_effect *          m_peffect;
      uint32_t                               m_dwLostSampleCount;
      event                              m_eventStopped;


      wave_out(sp(::ca2::application) papp);
      virtual ~wave_out();


      audio_decode::decoder * SetDecoder(audio_decode::decoder * pdecoder);

      void install_message_handling(::ca2::message::dispatch * pinterface);

      int32_t GetBufferInMMSystemCount();
      imedia::time GetPositionMillis();
      imedia::time GetPositionMillisForSynch();
      imedia::position get_position();
      imedia::position get_position_for_synch();
      void AttachEndEvent(event * pevent);
      bool Stop();
      e_state GetState();
      void BufferReady(LPWAVEHDR lpwavehdr);
      MMRESULT Start(const imedia::position & position);
      wave_buffer & GetBuffer();
      MMRESULT open(::ca2::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
      MMRESULT OpenEx(::ca2::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample);
      MMRESULT close();

      int32_t GetBufferInMMSysCount();
      static wave_out * BeginThread(sp(::ca2::application) papp);
      void SetWaveStreamEffect(::iaudio::wave_stream_effect * peffect);
      bool Pause();
      bool Restart();
      MMRESULT GetLastMMResult();
      int32_t GetBufferSize();
      void * GetBuffer(int32_t iBuffer);
      HWAVEOUT GetSafeHandle();
      void PostClose();

      void OnPlaybackEnd();
      void _Free(LPWAVEHDR lpwavehdr);

      virtual bool initialize_instance();
      virtual int32_t exit_instance();

      DECL_GEN_SIGNAL(OnMultimediaOpen)
      DECL_GEN_SIGNAL(OnMultimediaDone)
      DECL_GEN_SIGNAL(OnMultimediaClose)
      DECL_GEN_SIGNAL(OnBufferReady)
      DECL_GEN_SIGNAL(OnClose)

      void OutBufferDone(LPWAVEHDR lpwavehdr);
      virtual void OnPreBufferDone(int32_t iId);

   };


} // namespace audio



