#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM tracks :
         virtual public ::ca2::object
      {

      public:

         class TrackPtrArray;

         enum ETrackType
         {
            TypeMidiTrack,
            TypeMidiLyricTrack,
         };


         class TrackPtrAllocArray : 
            virtual public ::ca2::object
         {
            friend class TrackPtrArray;
         protected:

            smart_pointer_array < track_base >   m_tracka;
            smart_pointer_array < track >        m_miditracka;
            smart_pointer_array < lyric_track >   m_lyrictracka;
            tracks   *                       m_ptracks;

         public:
            ::count GetTrackCount() const;
            ::count GetMidiTrackCount() const;
            ::count GetLyricTrackCount();
            track_base * TrackAt(index iIndex);
            lyric_track * LyricTrackAt(index iIndex);
            track * MidiTrackAt(index iIndex);
            track * MidiTrackAt(index iIndex) const;

            TrackPtrAllocArray(sp(::ca2::application) papp);
            void Initialize(tracks * ptracks);
            void RemoveAllMidiTracks();
            void RemoveTrack(track_base * ptrack);
            void remove_all();
            void remove_at(index iIndex);

            track * CreateTrack(index iIndex, FOURCC fourcc = FOURCC_MTrk);
            track * CreateTrack(FOURCC fourcc = FOURCC_MTrk);
            lyric_track * CreateLyricTrack(index iIndex);
            lyric_track * CreateLyricTrack();

            void copy(TrackPtrAllocArray & array);
         };

         class TrackPtrArray
         {
         protected:

            smart_pointer_array < track_base >   m_tracka;
            smart_pointer_array < track >       m_miditracka;
            smart_pointer_array < lyric_track >  m_lyrictracka;
            tracks   *                       m_ptracks;

         public:
            ::count GetTrackCount() const;
            ::count GetMidiTrackCount() const;
            ::count GetLyricTrackCount();
            track_base * TrackAt(index iIndex);
            lyric_track * LyricTrackAt(index iIndex);
            track * MidiTrackAt(index iIndex);

         public:
            TrackPtrArray();
            void Initialize(tracks * ptracks);
            void RemoveAllMidiTracks();
            void RemoveTrack(track_base * ptrack);
            void remove_all();
            void remove_at(index iIndex);

            void copy(TrackPtrAllocArray & array);

            TrackPtrArray & operator = (TrackPtrAllocArray & array);
         };

         event_v5              m_midieventVolume;
         event_v1              m_midieventV001Volume;
         int32_t                        m_iLyricDelay;
         TrackPtrAllocArray         m_tracka;
         TrackPtrArray              m_trackaActive;
         int_array                  m_iaLevel;
         int_array                  m_iaNoteOn;
         int_array                  m_iaInstrument;
         imedia::position           m_tkLevelLastUpdateInterval;
         BYTE                       m_uchRunningStatus;
         imedia::position           m_tkPosition;
         file_flags    m_flagsFile;
         sp(::music::midi::file)        m_pFile;
         int32_t                        m_iMelodyTrackTipA; // set by karaoke to xf conversion
         int32_t                        m_iCurrentTrack;


         tracks(sp(::ca2::application) papp, sp(::music::midi::file) pFile = NULL);
         virtual ~tracks();


         void dump(dump_context & dumpcontext) const;

         bool IsSoftKaraokeFile(bool bWork = false, stringa * pstra = NULL);
         bool IsXFFile();
         bool IsTune1000File();

         VMSRESULT WorkCompactTracks(track & trackUnion, bool bOnlyMTrk);

         VMSRESULT WorkCompactMidiTracks(track & trackUnion, bool bOnlyMTrk);

         e_file_result WorkGetNextEvent(event_v5 * & ppevent, imedia::position tkMax, bool bTkMaxInclusive, bool bOnlyMTrk);

         e_file_result WorkGetNextRawEvent(event_v5 * & ppevent, imedia::position tkMax, bool bTkMaxInclusive, bool bOnlyMTrk);

         e_file_result WorkGetNextRawMidiEvent(event_v5 * & ppevent, imedia::position tkMax, bool bTkMaxInclusive, bool bOnlyMTrk);

         void WorkSeekBegin();
         void WorkSeekEnd();

         VMSRESULT WorkCompactTracksWork(track & trackUnion, bool bOnlyMTrk);

         VMSRESULT WorkCompactMidiTracksWork(track & trackUnion, bool bOnlyMTrk);

         e_file_result WorkGetXFTokens(uint32_t uiCodePage, string2a & token2a, imedia::position_2darray * p2DTicks, xf::lyrics_id_array * pxflh2a);

         e_file_result WorkGetXFTokens(uint32_t uiCodePage, string2a & token2a, imedia::position_2darray & p2DTicks, int2a & i2aTokenLine, xf::lyrics_id_array * pxflh2a, bool bPlain);

         e_file_result WorkGetEmptyXFTokens(int32_t iTrack, string2a & token2a, imedia::position_2darray & p2DTicks, xf::lyrics_id_array * pxflh2a);

         e_file_result WorkGetSoftKaraokeTokens(string2a & token2a, imedia::position_2darray *p2DTicks, xf::lyrics_id_array * pxflh2a);

         int32_t CalcMelodyTrack(events_v1 ** ppEvents, imedia::position_array *p1DTokens, WORD   wFileFormat);

         int32_t WorkCalcMelodyTrack(events_v1 ** ppEventsRet, imedia::position_array & positiona, int_array & iaTokenLine, WORD wFileFormat);

         static BYTE m_paramEventNoteOnPositionCB[];
         static int32_t m_iparamEventNoteOnPositionCB;

         e_file_result GetKarInfoHeader(::music::softkaraoke::info *pKarI);

         e_file_result GetTune1000InfoHeader(::music::tune1000::info *pTune1000I, int32_t iTrack);

         void RemoveAllMidiTracks();
         ::count GetTrackCount();
         track_base * TrackAt(index iIndex);
         track * CreateTrack(FOURCC fourcc = FOURCC_MTrk);
         void remove_all();
         void clear();
         track * CreateTrack(index iIndex, FOURCC fourcc = FOURCC_MTrk);
         bool WorkGetLongestXFLyrics(string &str, bool bPlain);
         e_file_result GetVolumeEvent(sp(::ca2::application) papp, event_v5 *& pevent);
         e_file_result GetVolumeEvent(sp(::ca2::application) papp, event_v1 *& pevent);
         void OnStop();
         track * seek(uint32_t dwSeekWhat);

         bool m_bVolumeEventSent;
         VMSRESULT GetLyricDelay(int32_t * lpiDelay);
         VMSRESULT SetLyricDelay(int32_t iDelay);
         index GetMidiTrackIndex(index iIndex);

         VMSRESULT CompactTracks(track & trackUnion, bool bOnlyMTrk, bool bOnlyMidiTrack);

         VMSRESULT ExpandTrack(index iTrackIndex);
         void ToWorkStorage();
         void FromWorkStorage();


         e_file_result GetKarTokens(string2a & token2a, imedia::position_2darray * pp2DTicks);

         e_file_result GetXFTokens(string2a & token2a, imedia::position_2darray *p2DTicks, xf::lyrics_id_array * pxflh2a);

         void seek_begin();

         e_file_result GetNextEvent(event_v1 * & pevent, imedia::position tkMax, bool bTkMaxInclusive, bool bOnlyMTrk, bool bOnlyMidiTrack);

         e_file_result GetNextEvent(event_v1 * & pevent, imedia::position tkMax, bool bTkMaxInclusive, int32_t iTrack);

         e_file_result GetXFInfoHeaders(::music::xf::info_headers * pxfihs);

         e_file_result GetNextEventTkPosition(imedia::position * pTkPosition, imedia::position tkMax);

         imedia::position CalcTkLength();
         imedia::position get_position();
         ::count GetMidiTrackCount() const;
         ::count GetActiveMidiTrackCount() const;
         ::count GetActiveTrackCount() const;

         track * MidiTrackAt(index iIndex);
         track * MidiTrackAt(index iIndex) const;
         track * ActiveMidiTrackAt(index iIndex);
         track_base * ActiveTrackAt(index iIndex);
         void DeactivateTrack(track_base * ptrack);

         file_flags & GetFlags();
         e_file_result WorkQuantizeVR();
         bool WorkGetLongestSoftKaraokeLyrics(string &str);
         void SetAutoAllocation(bool bSet = true);
         lyric_track * CreateLyricTrack();
         void remove_at(index iIndex);

      };


   } // namespace midi_mmsystem


} // namespace music





