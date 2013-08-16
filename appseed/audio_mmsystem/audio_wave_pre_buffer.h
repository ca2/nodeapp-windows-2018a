#pragma once


namespace audio_decode
{


   class decoder;


} // namespace audio_decode


namespace iaudio
{


   class wave_stream_effect;


} // namespace iaudio


namespace audio
{


   class CLASS_DECL_CA2_MULTIMEDIA wave_pre_buffer :
      virtual public spa(audio_decode::decoder)
   {
   public:

      class Thread : public simple_thread
      {
      public:
         enum e_state
         {
            state_initial,
            StateStarting,
            StateRunning,
            StateStopping,
         };

         Thread(sp(::ca2::application) papp);
         ~Thread();
         void SetPreBuffer(wave_pre_buffer * pprebuffer);
         void Start();
         void Stop();
         void Step();
         e_state GetState();
      protected:
         e_state              m_estate;
         event                m_evStep;
         wave_pre_buffer *   m_pprebuffer;    

         virtual int32_t run();
      } m_thread;
      friend class Thread;



      class Chunk
      {
      public:
         void Reset();
         Chunk();
         int16_t *     m_psh;
         int32_t         m_iId;
      };

      class ChunkArray :
         public array < Chunk, Chunk & >
      {
      public:
         Chunk * GetChunkById(int32_t iId);
         void Reset();
      };


      array < int16_t >                 m_shaL1;
      array < int16_t >                 m_shaL2;
      ::audio::wave_pre_buffer_callback *          m_pcallback;
      int32_t                                 m_iChannelCount;
      int32_t                                 m_iChunkCount;
      int32_t                                 m_iChunkSampleCount;
      sp(audio_decode::decoder)           m_pdecoder;

      sp(::iaudio::wave_stream_effect)        m_speffect;
      ChunkArray                          m_chunkaL1;
      ChunkArray                          m_chunkaL2;

      int32_t                                 m_iOutCount;
      int32_t                                 m_iMinFreeBuffer;

      critical_section                    m_cs;
      bool                                m_bPlay;

      ::iaudio::wave_stream_effect *          m_pstreameffectOut;

      imedia::position                    m_position;

      wave_pre_buffer(sp(::ca2::application) papp);
      virtual ~wave_pre_buffer();


      int32_t _IdToChunk(int32_t iId);
      bool _PreOutputId(Chunk * pchunk);
      bool _FreeId(int32_t iId);
      bool _DeferKick();
      void _Kick();
      bool _UpdateBuffer(int32_t iChannelCount, int32_t iGroupCount, int32_t iGroupSampleCount, int32_t iMinL1BufferCount);
      void ClearBuffer();
      void Reset();
      bool IsEOF();
      void Devocalize(bool bSet);
      bool IsDevocalized();
      //void Kick();

      bool IdFree(int32_t iChunkId);
      int32_t GetChunkByteCount();
      int16_t * GetInBuffer(int32_t iChunk);
      int32_t GetTotalChunkCount();
      int16_t * GetOutBufferId(int32_t iId);
      void SetDecoder(audio_decode::decoder * pinterface);
      bool open(::audio::wave_pre_buffer_callback * pcallback, int32_t iChannelCount, int32_t iGroupCount, int32_t iGroupSampleCount);
      bool SetMinL1BufferCount(int32_t iCount);

      int64_t GetMillisLength();

      void Start(const imedia::position & position);
      void Stop();


   };


   inline wave_pre_buffer::Thread::e_state wave_pre_buffer::Thread::GetState()
   {
      return m_estate;
   }


} // namespace audio



