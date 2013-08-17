#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM callback_interface :
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


   } //namespace midi_mmsystem


} // namespace music






