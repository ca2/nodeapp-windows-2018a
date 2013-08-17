#pragma once


typedef struct tag_midicallbackdata MIDICALLBACKDATA;

typedef struct  tagDouble
{
   double d;
} DOUBLESTRUCT, * LPDOUBLESTRUCT;

//#define MIDIPLAYERMESSAGE_PLAYBACKEND WM_USER + 30
#define MIDISEQUENCEMESSAGE_EVENT WM_USER + 30

#define MIDIPLAYERMESSAGE_COMMAND WM_USER + 50
//#define MIDIPLAYERMESSAGE_STREAMOUT WM_USER + 31
//#define   MIDIPLAYERMESSAGE_STOP WM_USER + 32


#define MIDIPLAYERMESSAGE_NOTIFYEVENT WM_APP + 40







#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class sequence_thread;


      namespace player
      {

         class player_callback;
         class player_interface;




         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM player :
            public ::ca2::thread,
            public ::music::midi::midi_listener
         {
         public:

            enum e_message 
            {
               MessageNotifyEvent = WM_APP + 40,
            };

            event              m_eventPlaybackEnd;
            event              m_evInitialized;

            ::music::midi::sequence_thread *    m_psequencethread;
            sp(::user::interaction)                   m_puie;
            player_interface *   m_pinterface;
            double                  m_dNextPositionRate;


            player(sp(::ca2::application) papp);
            virtual ~player();


            void install_message_handling(::ca2::message::dispatch * pinterface);



            void SendMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd);
            void SetCallbackWindow(sp(::user::interaction) puie);
            ::music::midi::sequence & GetSequence();
            bool Play(double dRate = 0.0, uint32_t dwEllapse = 584);
            bool Play(imedia::position tkStart, uint32_t dwEllapse = 584);

            virtual bool initialize_instance();
            virtual int32_t exit_instance();
            virtual void pre_translate_message(::ca2::signal_object * pobj);
            void OnMmsgDone(::music::midi::sequence *pSeq);
            DECL_GEN_SIGNAL(OnUserMessage)
               void SaveFile(const char * lpszPathName);
            void SetPosition(double dRate);
            void Pause();
            void CloseFile();
            void SendReset();
            bool ExecuteCommand(e_command ecommand, uint32_t dwEllapse);
            virtual void OnMidiOutDeviceChange();

            uint32_t GetMidiOutDevice();
            void PostNotifyEvent(e_notify_event eevent);

            imedia::position RateToTicks(double dRate);

            bool SetTempoShift(int32_t iTempoShift);

            bool SetMidiOutDevice(uint32_t uiDevice);

            VMSRESULT SetInterface(player_interface * pinterface);

            VMSRESULT Initialize(::ca2::thread * pthread);

            bool IsPlaying();

            void PostGMReset();
            void PostTempoChange();
            void SendTempoChange(); // verificar


            DECL_GEN_SIGNAL(OnNotifyEvent)
               DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessageDone)
               DECL_GEN_SIGNAL(OnMultimediaMidiOutputMessagePositionCB)

               // midi central listener
               DECL_GEN_VSIGNAL(on_attribute_change);


         };


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music






