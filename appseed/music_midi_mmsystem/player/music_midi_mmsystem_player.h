#pragma once


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class sequence_thread;


         namespace player
         {


            class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM player :
               virtual public ::music::midi::player::player
            {
            public:


               player(::aura::application * papp);
               virtual ~player();


               virtual void install_message_routing(::message::sender * pinterface) override;


               //virtual void SendMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd) override;
               virtual bool PlayRate(double dRate = 0.0, uint32_t dwEllapse = (uint32_t) -1) override;
               virtual bool Play(imedia_position tkStart, uint32_t dwEllapse = (uint32_t)-1) override;

               virtual bool init_thread() override;
               virtual void term_thread() override;
               virtual void pre_translate_message(::message::message * pobj) override;
               virtual void OnMmsgDone(::music::midi::sequence *pSeq) override;
               virtual void SaveFile(const char * lpszPathName) override;
               virtual void SetPosition(double dRate) override;
               virtual void pause() override;
               virtual void close_file() override;
               virtual bool ExecuteCommand(::music::midi::player::e_command ecommand, uint32_t dwEllapse) override;
               virtual void OnMidiOutDeviceChange() override;

               virtual uint32_t GetMidiOutDevice() override;
               virtual void PostNotifyEvent(::music::midi::player::e_notify_event eevent) override;

               virtual imedia_position RateToTicks(double dRate) override;

               virtual ::music::e_result SetTempoShift(double dTempoShift) override;

               virtual bool SetMidiOutDevice(uint32_t uiDevice) override;

               virtual ::multimedia::e_result set_client(::music::midi::player::player_client * pclient) override;

               virtual bool IsPlaying() override;

               virtual void PostGMReset() override;
               virtual void PostTempoChange() override;
               virtual void SendTempoChange() override;


               DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessageDone);
               DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessagePositionCB);
               DECL_GEN_SIGNAL(OnUserMessage);

               DECL_GEN_SIGNAL(on_attribute_change);


            };


         } // namespace player


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music






