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

            class player_callback;
            class player_interface;




            class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM player :
               virtual public ::music::midi::player::player
            {
            public:


               player(::aura::application * papp);
               virtual ~player();


               void install_message_handling(::message::dispatch * pinterface);


               void SendMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd);
               void SetCallbackUi(sp(::user::interaction) puie);
               bool Play(double dRate = 0.0, uint32_t dwEllapse = (uint32_t) -1);
               bool Play(imedia_position tkStart, uint32_t dwEllapse = (uint32_t)-1);

               virtual bool initialize_instance();
               virtual int32_t exit_instance();
               virtual void pre_translate_message(::signal_details * pobj);
               void OnMmsgDone(::music::midi::sequence *pSeq);
               DECL_GEN_SIGNAL(OnUserMessage);
                  void SaveFile(const char * lpszPathName);
               void SetPosition(double dRate);
               void Pause();
               void CloseFile();
               bool ExecuteCommand(::music::midi::player::e_command ecommand, uint32_t dwEllapse);
               virtual void OnMidiOutDeviceChange();

               uint32_t GetMidiOutDevice();
               void PostNotifyEvent(::music::midi::player::e_notify_event eevent);

               imedia_position RateToTicks(double dRate);

               virtual ::music::e_result SetTempoShift(double dTempoShift);

               bool SetMidiOutDevice(uint32_t uiDevice);

               virtual ::multimedia::e_result set_client(::music::midi::player::player_client * pclient);

               bool IsPlaying();

               void PostGMReset();
               void PostTempoChange();
               void SendTempoChange(); // verificar


               //DECL_GEN_SIGNAL(OnNotifyEvent);
                  DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessageDone);
                  DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessagePositionCB);

                  // midi central listener
                  DECL_GEN_SIGNAL(on_attribute_change);


            };


         } // namespace player


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music






