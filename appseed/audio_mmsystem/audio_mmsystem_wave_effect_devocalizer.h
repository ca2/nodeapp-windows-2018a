#pragma once


namespace audio_mmsystem
{

   class wave_effect_devocalizer  :
      virtual public ::iaudio::wave_stream_effect 
   {
   public:


      class Chunk
      {
      public:


         wave_effect_devocalizer *       m_pparent;
         array < int16_t >              m_sha;


         void Initialize(wave_effect_devocalizer * pparent);
         bool Input(int16_t * pshStream);
      };

      class Group :
         virtual public ::ca2::object
      {
      public:


         class Channel :
            virtual public ::ca2::object
         {
         public:


            Group *                          m_pgroup;

            double_array                     m_da1;
            double_array                     m_da2;
            double_array                     m_da3;
            array < kiss_fft_cpx >       m_cpxa;
            sort_array < double, double >    m_faMax;
            int_array                        m_iaMax;


            Channel(sp(::ca2::application) papp);

            void Initialize(Group * pgroup);
            void Input(bool bFilter);
            void Output(sort_array < double, double > & faMax, int_array & iaMax);

         };

         wave_effect_devocalizer *                 m_pparent;
         spa(Channel)                              m_channela;
         array < kiss_fft_cpx >                m_cpxa;
         double_array                              m_daMod;
         double                                    m_dDivisor;

         Group(sp(::ca2::application) papp);
         void Initialize(wave_effect_devocalizer * pparent);
         bool Input(int16_t * pshStream);
         bool Output1(int16_t * pshStream);
         bool Output2(int16_t * pshStream);


      };

      sp(kissfft::fftd)                m_spfftd;
      sp(kissfft::ifftd)               m_spifftd;

      sort_array < sort_array < double, double >, sort_array < double, double > & > m_f2aMax;
      int2a                                  m_i2aMax;

      // Tuning Parameters
      int32_t                                   m_iCancelNoteCount;
      int32_t                                   m_iMin;
      int32_t                                   m_iMax;
      double                                    m_wDiv;

      bool                                      m_bEnable;

      int32_t                                   m_iDivisionCount;
      int32_t                                   m_iFirstOutput;
      int32_t                                   m_iOutputCount;
      int32_t                                   m_iOutDivision;
      int32_t                                   m_iOutChunk;


      int32_t                                   m_iSamplesPerSecond;
      int32_t                                   m_iSampleCount;
      int32_t                                   m_iChannelCount;
      int32_t                                   m_iGroupCount;

      int32_t                                   m_iInputDivisionCount;
      int32_t                                   m_iFirstInputDivision;
      int32_t                                   m_iFirstChunk;
      int32_t                                   m_iChunkCount;
      int32_t                                   m_iFirstInputGroup;
      int32_t                                   m_iInputGroupCount;
      int32_t                                   m_iOutputGroup;

      int32_t                                   m_iDivisionSampleCount;

      spa(Group)                                m_groupa;
      array < Chunk, Chunk & >             m_chunka;
      array < Chunk, Chunk & >             m_chunkaOut;
      Chunk                                     m_chunk;

      wave_effect_devocalizer(sp(::ca2::application) papp, bool bEnable, int32_t iSampleCount, int32_t iSamplesPerSecond, int32_t iDivisionCount, int32_t iGroupCount);
      virtual ~wave_effect_devocalizer();


      int32_t GetFrequencyIndex(double dFrequency);
      void ChunkToGroup();
      void InputChunk();
      void PrepareChunk(int32_t iDivision);
      void Enable(bool bSet = true);
      bool IsEnabled();
      bool GroupToOutput();
      bool Output(int16_t * pshIn);
      virtual int32_t Process16bits(int16_t * pshStream, int32_t iCount);
      virtual bool Process(int16_t * pshIn, int16_t * pshOut);


   };


} // namespace audio_mmsystem





