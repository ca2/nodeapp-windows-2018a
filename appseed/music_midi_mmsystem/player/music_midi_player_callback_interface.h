#pragma once


namespace music
{


   namespace midi
   {


      namespace player
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA callback_interface :
            virtual public ::ca2::object
         {
         public:


            callback_interface();
            virtual ~callback_interface();


            virtual void OnMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd);
            virtual void OnMidiPlayerNotifyEvent(notify_event * pevent); 
            virtual void OnMidiLyricEvent(array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> * pevents);


         };


      } // namespace player


   } //namespace midi


} // namespace music






