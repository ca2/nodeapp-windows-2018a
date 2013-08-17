#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         callback_interface::callback_interface()
         {
         }

         callback_interface::~callback_interface()
         {
         }


         void callback_interface::OnMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd)
         {
            UNREFERENCED_PARAMETER(pSeq);
            UNREFERENCED_PARAMETER(lpmdd);
         }

         void callback_interface::OnMidiPlayerNotifyEvent(notify_event * pdata)
         {
            UNREFERENCED_PARAMETER(pdata);
         }

         void callback_interface::OnMidiLyricEvent(array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> * pevents)
         {
            UNREFERENCED_PARAMETER(pevents);
         }


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music









