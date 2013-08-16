#pragma once



namespace audio
{


   class wave_in;


   class CLASS_DECL_CA2_MULTIMEDIA wave_recorder : 
      public ::ca2::thread,
      public wave_in_listener
   {
   public:


      wave_in *                              m_pwavein;
      ::ca2::filesp                            m_spfile;
      audio_decode::encoder *                m_pencoder;
      audio_decode::encoder_plugin_set       m_pluginset;
      event                                  m_eventStopped;
      int32_t                                m_iOutBufferSampleCount;


      wave_recorder(sp(::ca2::application) papp); 
      virtual ~wave_recorder();


      void ExecuteStop();
      void ExecutePlay();

      bool IsRecording();

      virtual void WaveInDataProc(wave_in * pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr);

      void install_message_handling(::ca2::message::dispatch * pinterface);

      virtual bool initialize_instance();
      virtual int32_t exit_instance();

      bool audCommandMessageProcedure(wave_recorder_command &command);
      void ExecuteCommand(wave_recorder_command & command);
      DECL_GEN_SIGNAL(OnaudCommandMessage);


   };


} // namespace audio



