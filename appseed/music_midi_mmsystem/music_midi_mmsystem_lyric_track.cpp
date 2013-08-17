#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      lyric_track::lyric_track(::ca2::application * papp) :
         ca2(papp),
         track_base(papp)
      {
         m_pfile = NULL;
         m_iDelay = 0;
      }

      lyric_track::lyric_track(::music::midi::file * pfile) :
         ca2(pfile->get_app()),
         track_base(pfile->get_app())
      {
         m_pfile = pfile;
         m_iDelay = 0;
      }

      lyric_track::lyric_track(const lyric_track & track) :
         ca2(track.get_app()),
         track_base(track.get_app())
      {
         operator = (track);
      }

      lyric_track::~lyric_track()
      {

      }

      e_file_result lyric_track::get_position(imedia::position & tkPositionParam, imedia::position tkMax)
      {
         if(m_iCurrentEvent >= m_trackWorkStorage.GetEventCount())
         {
            return SEndOfTrack;
         }
         //    event_v5 & event = m_trackWorkStorage.EventAt(m_iCurrentEvent);

         imedia::position tkPosition = -1;
         if(m_iCurrentEvent == m_iLastEvent &&
            m_iDelay == m_iLastDelay)
         {
            tkPosition = m_tkLastPosition;
         }
         else
         {
            /*tkPosition = m_tkPosition + event.GetDelta();
            int32_t iMsPosition  = m_pfile->TicksToMillisecs(tkPosition);
            iMsPosition += m_iDelay;
            tkPosition = m_pfile->MillisecsToTicks(iMsPosition);
            m_iLastEvent = m_iCurrentEvent;
            m_iLastDelay = m_iDelay;
            m_tkLastPosition = tkPosition;*/
            tkPosition = ApplyDelay(m_tkPosition);
            m_iLastEvent = m_iCurrentEvent;
            m_iLastDelay = m_iDelay;
            m_tkLastPosition = tkPosition;
         }

         if(tkPosition > tkMax)
         {
            return SReachedTkMax;
         }
         tkPositionParam = tkPosition;
         return Success;
      }

      file_flags & lyric_track::GetFlags()
      {
         return m_flagsFile;
      }

      const file_flags & lyric_track::GetFlags() const
      {
         return m_flagsFile;
      }

      e_file_result lyric_track::GetEvent(event_base *& pevent, imedia::position tkMax, bool bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(tkMax);
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         if(m_iCurrentEvent > m_trackWorkStorage.GetEventCount())
         {
            return SEndOfTrack;
         }
         if(m_tkPosition > tkMax)
         {
            return SReachedTkMax;
         }
         pevent = &m_trackWorkStorage.EventAt(m_iCurrentEvent);
         return Success;
      }

      e_file_result lyric_track::GetEvent(
         event_v1 *& pevent,
         imedia::position   tkMax,
         bool   bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         if(m_iCurrentEvent > m_trackWorkStorage.GetEventCount())
         {
            return SEndOfTrack;
         }
         if(m_tkPosition > tkMax)
         {
            return SReachedTkMax;
         }
         event_v5 & event = m_trackWorkStorage.EventAt(m_iCurrentEvent);
         m_eventv001 = event;
         pevent = &m_eventv001;
         return Success;
      }

      imedia::position lyric_track::get_position()
      {
         return ApplyDelay(m_tkPosition);
      }

      e_file_result lyric_track::seek_begin()
      {
         m_iCurrentEvent      = -1;
         m_tkPosition         = 0;
         m_tkLastPosition     = 0;
         m_iLastEvent         = -1;
         GetFlags().unsignalize_all();
         return MoveNext();
      }

      e_file_result lyric_track::seek_end()
      {
         m_iCurrentEvent = m_trackWorkStorage.GetEventCount();
         m_tkPosition = m_trackWorkStorage.EventAt(m_trackWorkStorage.GetEventCount() - 1).get_position();
         GetFlags().signalize(EndOfFile);
         m_tkLastPosition = m_trackWorkStorage.EventAt(m_trackWorkStorage.GetEventCount() - 2).get_position();
         m_iLastEvent = m_trackWorkStorage.GetEventCount() - 2;
         return Success;
      }


      VMSRESULT lyric_track::Prepare(sp(::ca2::application) papp, ::ikaraoke::lyric_events_v1 & events)
      {
         ::primitive::memory_file       memFile(papp);
         event_v5    event;
         imedia::position             tkPosition;
         imedia::position             tkLastPosition = 0;
         array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lvaPending;

         events.m_iCurrentToken = 0;


         TRACE("\n");
         TRACE("\n");
         m_trackWorkStorage.remove_all();
         while(events.m_iCurrentToken < events.m_tkaTokensPosition.get_size())
         {
            lvaPending.add(events);
            tkPosition = events.m_tkaTokensPosition.element_at(events.m_iCurrentToken);
            TRACE("lyric_track::Prepare V1 tkPosition %d", tkPosition);
            if(tkPosition >= tkLastPosition)
            {
               event._SetFlags(1); // Flag 1 means POSITIONCB
               event._SetDelta(tkPosition - tkLastPosition);
               event._SetPosition(tkPosition);
               TRACE(" Delta %d", event._GetDelta());
               TRACE(" Token %d", events.m_iCurrentToken);

               //         TRACE(" msPosition %d", events.m_msaNotesPosition.element_at(events.m_iCurrentNote));

               memFile.clear();
               /*xxx         CArchive ar(&memFile, CArchive::store);
               ar << EVENT_ID_LYRIC_V1;
               lvaPending.Serialize(ar);
               ar.Flush();
               uint32_t dwLength = memFile.get_length();
               LPBYTE lpData = memFile.get_data();
               event.SetParam(lpData, dwLength);
               //memFile.attach(lpData, dwLength);
               lvaPending.remove_all();
               m_trackWorkStorage.add(event);
               tkLastPosition = tkPosition;*/
            }
            TRACE("\n");
            events.m_iCurrentToken++;
         }

         return VMSR_SUCCESS;
      }

      VMSRESULT lyric_track::Prepare(sp(::ca2::application) papp, ::ikaraoke::lyric_events_v2 & events)
      {
         ::primitive::memory_file              memFile(papp);
         event_v5               event;
         imedia::position              tkPosition;
         imedia::position              tkLastPosition = 0;
         array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lvaPending;
         unique_index_sort_array       ia;

         events.m_iCurrentToken        = 0;
         events.m_iCurrentNote         = 0;

         m_trackWorkStorage.remove_all();



         TRACE("\n");
         TRACE("\n");
         while(events.m_iCurrentNote < events.m_msaNotesPosition.get_size())
         {
            ia.remove_all();
            ia  = events.GetNoteTokenArray(events.m_iCurrentNote);
            if(ia.get_size() > 0)
            {
               events.m_iCurrentToken = ia.get_at(0);
               lvaPending.add(events);
               tkPosition = events.m_tkaNotesPosition.element_at(events.m_iCurrentNote);
               TRACE("lyric_track::Prepare V2 tkPosition %d", tkPosition);
               if(tkPosition >= tkLastPosition)
               {
                  event._SetFlags(1); // Flag 1 means POSITIONCB
                  ASSERT(tkPosition >= tkLastPosition);
                  event._SetDelta(tkPosition - tkLastPosition);
                  event._SetPosition(tkPosition);
                  TRACE(" Delta %d", event._GetDelta());
                  TRACE(" Token %d", events.m_iCurrentToken);
                  TRACE(" Note %d", events.m_iCurrentNote);
                  //            TRACE(" msPosition %d", events.m_msaNotesPosition.element_at(events.m_iCurrentNote));

                  memFile.clear();
                  /*xxxx            CArchive ar(&memFile, CArchive::store);
                  ar << EVENT_ID_LYRIC_V1;
                  lvaPending.Serialize(ar);
                  ar.Flush();

                  uint32_t dwLength = memFile.get_length();
                  memFile.seek_to_begin();
                  event.SetParam(memFile);
                  m_trackWorkStorage.add(event);
                  lvaPending.remove_all();
                  tkLastPosition = tkPosition;*/
               }
               TRACE("\n");
            }
            events.m_iCurrentNote++;
         }
         return VMSR_SUCCESS;
      }

      e_file_result lyric_track::MoveNext()
      {
         m_iCurrentEvent++;
         if(m_iCurrentEvent >= m_trackWorkStorage.GetEventCount())
         {
            return SEndOfTrack;
         }

         m_tkPosition += m_trackWorkStorage.EventAt(m_iCurrentEvent)._GetDelta();
         //TRACE("lyric_track::MoveNext iEvent %d position %d Next %d\n",
         //   m_iCurrentEvent, m_tkPosition, m_tkNextPosition);
         return Success;

      }

      lyric_track & lyric_track::operator =(const lyric_track &track)
      {
         m_iCurrentEvent = track.m_iCurrentEvent;
         m_iDelay = track.m_iDelay;
         GetFlags() = track.GetFlags();
         m_iLastDelay = track.m_iLastDelay;
         m_iLastEvent = track.m_iLastEvent;
         m_pfile = track.m_pfile;
         m_tkLastPosition = track.m_tkLastPosition;
         m_tkPosition = track.m_tkPosition;
         m_trackWorkStorage = track.m_trackWorkStorage;
         return * this;


      }

      imedia::position lyric_track::WorkGetPosition()
      {
         imedia::position tkPosition = 0x7fffffff;
         get_position(tkPosition, 0x7fffffff);
         return tkPosition;
      }

      e_file_result lyric_track::WorkMoveNext()
      {
         return MoveNext();
      }

      void lyric_track::WorkGetEvent(
         event_base *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         WorkGetEvent((event_v5 *&) pevent, tkMax, bTkMaxInclusive);
      }

      void lyric_track::WorkGetEvent(
         event_v5 *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         if(m_iCurrentEvent > m_trackWorkStorage.GetEventCount())
         {
            return;
         }
         if(m_tkPosition > tkMax)
         {
            return;
         }
         event_v5 & event = m_trackWorkStorage.EventAt(m_iCurrentEvent);
         pevent = &event;
      }


      e_file_result lyric_track::WorkSeekBegin()
      {
         return seek_begin();
      }

      e_file_result lyric_track::WorkSeekEnd()
      {
         return seek_end();
      }

      track_base::e_type lyric_track::get_type()
      {
         return TypeLyric;
      }

      bool lyric_track::WorkIsEOT()
      {
         return m_iCurrentEvent >= m_trackWorkStorage.GetEventCount();
      }


      imedia::position lyric_track::ApplyDelay(imedia::position tkPosition)
      {
         return m_pfile->MillisecsToTicks(
            m_pfile->TicksToMillisecs(tkPosition)
            + m_iDelay);
      }

      void lyric_track::SetDelay(int32_t iMillis)
      {
         m_iDelay = iMillis;
      }


   } // namespace midi_mmsystem


} // namespace music









