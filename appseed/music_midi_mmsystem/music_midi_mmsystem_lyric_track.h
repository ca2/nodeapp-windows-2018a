#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM lyric_track : 
         public track_base
      {
      public:


         sp(::music::midi::file)          m_pfile;
         event_v1            m_eventv001;
         track_v8            m_trackWorkStorage;
         file_flags      m_flagsFile;
         imedia::position           m_tkPosition;
         ::index                    m_iCurrentEvent;
         int32_t                        m_iDelay; // in milliseconds

         imedia::position           m_tkLastPosition;
         ::index                    m_iLastEvent;
         int32_t                        m_iLastDelay;


         lyric_track(::ca2::application * papp);
         lyric_track(::music::midi::file * pfile);
         lyric_track(const lyric_track & track);
         virtual ~lyric_track();

         imedia::position ApplyDelay(imedia::position tkPosition);
         void SetDelay(int32_t iMillis);
         virtual bool WorkIsEOT();
         e_type get_type();
         e_file_result MoveNext();
         VMSRESULT Prepare(sp(::ca2::application) papp, ::ikaraoke::lyric_events_v1 & events);
         VMSRESULT Prepare(sp(::ca2::application) papp, ::ikaraoke::lyric_events_v2 & events);
         e_file_result seek_begin();
         e_file_result seek_end();
         imedia::position get_position();
         virtual e_file_result get_position(imedia::position &tkPosition, imedia::position tkMax);
         virtual const file_flags & GetFlags() const;
         virtual file_flags & GetFlags();

         virtual e_file_result GetEvent(event_base * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         virtual e_file_result GetEvent(event_v1 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         virtual imedia::position WorkGetPosition();
         virtual e_file_result WorkSeekBegin();
         virtual e_file_result WorkSeekEnd();

         virtual e_file_result WorkMoveNext();

         virtual void WorkGetEvent(event_base * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         virtual void WorkGetEvent(event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);


         lyric_track & operator =(const lyric_track &track);

      };


   } // namespace midi_mmsystem


} // namespace music





