#pragma once


class stringa;


#define MessageKickBufferOut WM_APP + 120


namespace audio_decode
{


   class decoder;


} // namespace audio_decode


namespace audio_mmsystem
{


   class wave_out;
   class wave_pre_buffer;


   class  CLASS_DECL_AUDIO_MMSYSTEM wave_player :
      public ::ca2::thread
   {
   public:

      friend class wave_out;

      enum EDeviceState
      {
         DeviceStateInitial,
         DeviceStateOpened,
         DeviceStatePlaying,
         DeviceStateStopping,
         DeviceStatePaused,
      };

      enum EDecoderState
      {
         DecoderStateInitial,
         DecoderStateOpened,
      };

      enum e_state
      {
         state_initial,
         StatePlaying,
         StatePaused,
         StateFadingOutToStop,
      };

      enum e_event
      {
         EventOpenDecoder,
         EventOpenDevice,
         EventPlay,
         EventStop,
         EventExecutePause,
         EventExecuteRestart,
         EventEOF,
         EventPlaybackEnd,
         EventStopped,
         EventCloseDevice,
         EventCloseDecoder,
         EventFadeOutAndStop,
      };

      list < wave_player_command, wave_player_command & > m_commandlistStopOpen;
      list < wave_player_command, wave_player_command & > m_commandlistOpenPlay;

      ::multimedia::audio_decode::decoder_plugin_set          m_decoderset;
      sp(::user::interaction)                   m_oswindowCallback;
      sp(::multimedia::audio_decode::decoder)                 m_pdecoder;
      wave_out *                              m_pwaveout;

      EDeviceState                              m_edevicestate;
      EDecoderState                             m_edecoderstate;
      e_state                                   m_estate;
      int32_t                                       m_iOutBufferSampleCount;
      int32_t                                       m_iBufferId;


      wave_player(sp(::ca2::application) papp);
      virtual ~wave_player();


      bool DecoderOpen(wave_player_command & command);

      bool DecoderIsActive();
      void DecoderInitialize(sp(::ca2::file) pfile);
      void DecoderClose();
      void DecoderRun();

      void install_message_handling(::ca2::message::dispatch * pinterface);

      bool audCommandMessageProcedure(wave_player_command & command);
      void FillTitleInfo(stringa & wstraFormat, string2a & wstr2aTitle);
      void FadeOutAndStop();
      void Devocalize(bool bSet);
      bool IsDevocalized();
      void SetState(e_state estate);
      bool ExecuteIsPaused();
      void set_callback(sp(::user::interaction)  oswindow);
      bool GetStopEnable();
      bool GetPlayEnable();
      wave_out * get_wave_out();
      static wave_player * BeginThread(sp(::ca2::application) papp);
      bool IsPlaying();
      EDeviceState GetDeviceState();
      EDecoderState GetDecoderState();
      void SetDeviceState(EDeviceState estate);
      void SetDecoderState(EDecoderState estate);
      void OnEvent(e_event event);
      bool DeviceIsOpened();
      bool DecoderIsOpened();
      void AttachEndEvent(event * pevent);

      void ExecuteCommand(wave_player_command & command);



      virtual bool initialize_instance();
      virtual int32_t exit_instance();

      void _Stop();
      void ExecuteStop();
      DECL_GEN_SIGNAL(OnaudCommandMessage)

   };


} // namespace audio_mmsystem




