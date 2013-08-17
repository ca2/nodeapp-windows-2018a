#pragma once


namespace music
{


   namespace midi
   {


      namespace player
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA callback :
            virtual public callback_interface
         {
         public:



            friend class window;
            window m_wnd;

            callback(sp(::ca2::application) papp);
            virtual ~callback();

            virtual bool initialize();
            virtual bool finalize();

            virtual void OnMmsgDone(sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd);
            virtual void OnMidiPlayerNotifyEvent(notify_event * pdata); 
            virtual void OnMidiLyricEvent(array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> * pevents);


         };


      } // namespace player


   } // namespace midi


} // namespace music








