#pragma once


#define MIDISEQUENCEMESSAGE_EVENT WM_USER + 30


#pragma once


namespace music
{

   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA sequence_thread : 
         public ::ca2::thread
      {
      public:


         enum e_message 
         {
            message_notify_event = WM_APP + 40,
         };


         sequence_thread(sp(::ca2::application) papp);

         player::player *                 m_pplayer;     
         sp(sequence)     m_psequence;
         event                        m_eventStop;

         void install_message_handling(::ca2::message::dispatch * pinterface);

         sequence * GetSequence();
         void Stop(imedia::time msEllapse);

         bool PostMidiSequenceEvent(sequence * pseq, sequence::e_event event);

         bool PostMidiSequenceEvent(sequence * pseq, sequence::e_event event, LPMIDIHDR lpmh);

         void PostNotifyEvent(player::e_notify_event eevent);

         void PrerollAndWait(double rate = 0.0);
         void PrerollAndWait(imedia::position tkStart);
         void Play(double dRate = 0.0);
         void Play(imedia::position tkStart);
         void PostGMReset();
         void PostTempoChange();
         void SendTempoChange();

         void ExecuteCommand(::ca::smart_pointer < player::command > pcommand);
         void _ExecuteCommand(::ca::smart_pointer < player::command > pcommand);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();
         virtual ~sequence_thread();

         DECL_GEN_SIGNAL(OnCommand)
            DECL_GEN_SIGNAL(OnMidiSequenceEvent)

      };


   } // namespace midi


} // namespace music





