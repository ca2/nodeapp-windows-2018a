#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         enum e_notify_event
         {
            notify_event_playback_start,
            notify_event_playback_end,
            notify_event_playback_stop,
            notify_event_generic_mmsg_done,
            notify_event_position_set,
            notify_event_set_sequence,
            notify_event_midi_stream_out,
         };

         class notify_event
         {
         public:
            
            player                * m_pplayer;
            e_notify_event          m_enotifyevent;

         };


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music









