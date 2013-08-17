#pragma once


#define C_TEMPO_MAP_CHK     16


namespace ikaraoke
{

   class lyric_event_v1;

} // namespace ikaraoke


namespace music
{


   namespace xf
   {

      class info_header;

   }

   namespace midi_mmsystem
   {


      class event_v1;
      class event_v2;
      class event_v5;
      class event_v7;
      class events_v1;
      class tracks;



      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM file :
         virtual public primitive::memory_container
      {
      public:

         class CMidiStreamEventHeader
         {
         public:
            uint32_t m_dwLength;
            uint32_t m_dwType;
         };

         ::music::e_storage                       m_estorage;
         imedia::position                    m_positionLastWorkRender;
         primitive::memory                   m_memstorageF1;
         file_flags                    m_flags;
         int32_t                                 m_iKeyShift;
         int32_t                                 m_iTempoShift;
         KEYFRAME                            m_keyframe;
         uint32_t                               m_dwMelodyTrack;
         uint32_t                               m_dwLyricTrack;
         int_array                           m_iaMuteTrack;
         imedia::position                     m_tkLastDelta;
         sequence *                     m_psequence;
         array < ::ikaraoke::events_tracks_v1 * >     m_evtrkv1ptraSeek;

         imedia::position                     m_tkLength;
         uint32_t                                 m_dwFormat;
         uint32_t                                 m_dwTracks;
         uint32_t                                 m_dwTimeDivision;

         int32_t                                 m_iOpenMode;

         string                              m_strName;

         CHUNKHDR *                           m_pMThd;
         MIDIFILEHDR *                        m_pFileHeader;

         ::music::midi::tempo_map              m_tempomap;

         uint32_t                                 m_dwPendingUserEvent;
         uint32_t                                 m_cbPendingUserEvent;
         byte *                              m_hpbPendingUserEvent;


         primitive_array < event_v5 *>  m_mepaImmediate;
         primitive_array < event_v5 *>  m_mepaOnQuarterNote;

         event_v5 *                     m_pmePendingLyric;
         bool                                m_bPendingLyric;

         tracks *                        m_ptracks;

         double GetTempoShiftRate();
         tracks & GetTracks();
         file_flags & GetFlags();
         bool IsTrackMute(int32_t iIndex);
         void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1);
         void MuteTrack(int32_t iIndex, bool bMute = true);

         e_file_result WorkSeekBegin();
         e_file_result CalcTkLength();
         e_file_result CreateTempoMap();
         ::multimedia::result ImmediatePutTempoChange();
         VMSRESULT GetTempoEvent(event_base & event);
         void OnStop();
         uint32_t GetImageSize();
         LPBYTE GetImage();
         VMSRESULT Format0();
         VMSRESULT InsertTempoEvent();
         ::music::midi::tempo_map_entry * GetTempoMapEntry(const imedia::position & position);
         int32_t GetTempoShift();
         VMSRESULT SetTempoShift(int32_t iTempoShift);
         void ToWorkStorage();
         void FromWorkStorage();
         int32_t GetKeyShift();
         int32_t SetKeyShift(int32_t iKeyShift);


         bool IsSoftKaraokeFile(bool bWork = false, stringa * pstra = NULL);
         bool IsXFFile();
         bool IsTune1000File();


         bool IsOpened();
         critical_section   m_cs;
         //e_file_result RebuildIndex();
         e_file_result WriteHeader(MIDIFILEHDR * lpmfh);
         e_file_result ChangeEventDelta(event_base *pEvent, imedia::position tkNewDelta);
         e_file_result ReplaceSameDeltaEvent(event_base & pEventNew);
         //   VMSRESULT allocate(uint32_t dwNewLength);
         //VMSRESULT allocate_add_up(uint32_t dwAddUp);

         e_file_result GetPreviousEvent(
            event_base * pPreviousEvent,
            event_base * pEvent);
         //e_file_result DeleteEvent(event_base  *pEvent);
         e_file_result SaveFile(const char * lpFilePath);


         e_file_result WorkWriteXFTracks(
            stringa &  tokena,
            imedia::position_array & positiona,
            ::music::xf::info_header & xfinfoheader);



         int32_t CalcMelodyTrack(events_v1 ** ppEvents, imedia::position_array * pTicks);

         int32_t WorkCalcMelodyTrack(
            events_v1 ** ppEvents, 
            imedia::position_array & positiona,
            int_array & iaTokenLine);



         int32_t GetFormat();
         //   e_file_result MergeTracks(
         //      CHUNKHDR *   pCh,
         //      MIDIFILEHDR *   pFh);
         //   e_file_result MergeWorkTracks(
         //      CHUNKHDR *   pCh,
         //      MIDIFILEHDR *   pFh);
         e_file_result MergeTracks();
         e_file_result WorkMergeTracks();

         e_file_result WorkStreamRender(
            LPMIDIHDR lpmh,
            imedia::position tkMax,
            uint32_t cbPrerollNomimalMax);

         e_file_result WorkGetNextEvent(
            event_v5 *&    pevent,
            imedia::position                tkMax,
            bool                  bTkMaxInclusive);

         e_file_result WorkGetNextRawEvent(
            event_v5 *&    pevent,
            imedia::position                tkMax,
            bool                  bTkMaxInclusive);

         e_file_result WorkGetNextRawMidiEvent(event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         e_file_result WorkSeek(imedia::position tkPosition, LPMIDIHDR lpmh);

         file(sp(::ca2::application) papp);
         virtual ~file();

         void delete_contents();


         int32_t GetDivision();
         imedia::position get_position();
         void AttachSeekSync(::ikaraoke::events_tracks_v1 * ptracksv1);

         bool IsNull();

         void WorkSeekEnd();

         e_file_result OpenFile(
            file &file,
            int32_t openMode);

         e_file_result OpenFile(
            ::ca2::file & file,
            int32_t openMode);

         e_file_result OpenFile(
            primitive::memory * pmemstorage,
            int32_t openMode,
            ::music::e_storage);

         e_file_result OpenFile(
            primitive::memory *  pmemstorage,
            e_file_open_mode eopenmode,
            ::music::e_storage estorage);

         e_file_result SaveFile(
            ::ca2::file & file);

         e_file_result CloseFile();

         e_file_result GetFileInfo(
            PSMFFILEINFO psfi);

         int_ptr TicksToMillisecs(
            imedia::position tkOffset);

         void TicksToMillisecs(
            imedia::time_array *pMillisArray,
            imedia::position_array *pTickArray,
            int_ptr tkOffset);

         void TicksToMillisecs(imedia::time_2darray *p2DMillisArray, imedia::position_2darray * p2DTicksArray, int_ptr tkOffset);

         void PositionToTime(imedia::time_2darray  & timea, imedia::position_2darray  & positiona, int_ptr tkOffset);

         void PositionToTime(imedia::time_array  & timea, imedia::position_array  & positiona, int_ptr tkOffset);

         imedia::position MillisecsToTicks(imedia::time msOffset);

         imedia::time PositionToTime(imedia::position msOffset);

         imedia::position TimeToPosition(imedia::time msOffset);

         void MillisecsToTicks(imedia::position_array * pTickArray, imedia::time_array *pMillisArray, imedia::time msOffset);

         void TimeToPosition(imedia::position_array & positiona, imedia::time_array & timea, imedia::time msOffset);

         e_file_result StreamRender(LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         VMSRESULT StreamEvent(imedia::position tkDelta, event_base * Event, LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         VMSRESULT StreamEventF1(imedia::position tkDelta, array < event_v5 *, event_v5 * > & eventptra, LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         e_file_result seek(imedia::position tkPosition, LPMIDIHDR lpmh);

         e_file_result seek(imedia::position tkPosition);

         e_file_result Build();

         e_file_result GetNextEvent(event_v1 *& pevent, imedia::position tkMax, bool   bTkMaxInclusive);

         e_file_result GetNextEventTkPosition(imedia::position * pTkPosition, imedia::position tkMax);

         void _SyncSeek(imedia::position tkPosition, ::ikaraoke::events_tracks_v1 * ptracksv1);
         void _SyncSeek(imedia::position tkPosition);
         e_file_result InsertParmData(imedia::position tkDelta, LPMIDIHDR lpmh);

         VMSRESULT InsertLyricEvents(::ikaraoke::lyric_event_v1 * pLyricEvent, LPMIDIHDR lpmh);

         e_file_result InsertPadEvent(imedia::position tkDelta, LPMIDIHDR lpmh);

         void SetOpened(bool bOpened = true);
      };


   } // namespace midi_mmsystem


} // namespace music





