#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA track :
         public track_base,
         public primitive::memory_container
      {
      public:

         enum State
         {
            StateOnDelta = 1,
            StateOnEvent = 2,
            StateOnEventRead = 3
         };

         enum CopyModes
         {
            CopyNormal = 1,
            CopyExcludeXFMetaLyrics = 2,
            CopyExcludeKarMetaLyrics = 4

         };
         uint32_t                         m_idxTrack;
         int32_t                           m_iIndex;
         imedia::position          m_tkPosition;
         uint32_t                         m_cbLeft;            
         bool                          m_bWorkTrackEnd;
         byte *                        m_hpbImage;
         uint32_t                           m_dwUsed;
         file_flags   m_flags;
         struct tag_smti
         {
            imedia::position       m_tkLength;
            uint32_t       m_cbLength;
         } m_smti;     

         imedia::position         m_tkDelta;
         byte *         m_hpbEventImage;

         int32_t               m_iWorkNextPositionEvent;
         imedia::position  m_tkWorkNextPosition;
         bool               m_bAutoAllocation;
         primitive::memory            m_memstorageHelper;
         State               m_estate;
         event_v1      m_event;
         event_v5      m_event8;
         tracks *      m_ptracks;
         track_v8     m_trackWorkStorage;
         int32_t               m_iCurrentEvent;


         track(sp(::ca2::application) papp);
         track(const track & track);
         virtual ~track();

         track & operator =(const track &eventSrc);


         void Initialize(tracks * pTracks);
         inline State GetState();
         inline event_v1 & GetEvent();


         e_file_result ReadDelta();

         e_file_result ReadEvent(
            imedia::position   tkMax,
            bool   bTkMaxInclusive);

         e_file_result ReadAnsiXFTokens(stringa & tokena, imedia::position_array * ptkaTicks);
         e_file_result ReadShiftJisXFTokens(stringa & tokena, imedia::position_array * lptkaTicks);
         e_file_result ReadKarTokens(stringa & tokena, imedia::position_array * lptkaTicks);
         e_file_result ReadTune1000Tokens(stringa & tokena, imedia::position_array * lptkaTicks);
         e_file_result ReadXFInfoHeader_(LPTSTR * lppStr, ::music::xf::info_header ** ppXfih, ::music::xf::info_header_ls ** ppXfihls);

         e_file_result GetNextXFInfoHeader(::music::xf::info_header * pxfih);
         e_file_result GetNextXFInfoHeaderLS(::music::xf::info_header_ls * pxfihls);
         e_file_result GetNextXFInfoHeaderLS(::music::xf::info_header_ls * pxfihls ,const string &strLanguage);
         e_file_result GetNextXFSongName(string &str);
         e_file_result GetNextKarInfo(::music::softkaraoke::info *pKarI);
         e_file_result GetNextTune1000Info(::music::tune1000::info *pTune1000I);


      protected:
         e_file_result WorkSetEOT();

         uint32_t GetVDWord(uint32_t * pDw);
         uint32_t GetVDWord(uint32_t dwLeft, uint32_t * pDw);

         uint32_t SetVDWord(uint32_t dwLeft, uint32_t dw);
         uint32_t SetVDWord(uint32_t dw);

      public:
         e_file_result _MoveNext(bool bWork);
         void _GetEvent(bool bWork, event_v5 *& pevent, imedia::position tkMax, bool bTkMaxInclusive);
         e_file_result _SeekBegin(bool bWork);
         virtual bool WorkIsEOT();
         virtual e_type get_type();
         file_flags & GetFlags();
         file_flags & _GetFlags();
         void WorkClear();
         virtual void allocate(uint32_t dwNewLength);
         e_file_result SeekXFLyricsHeader();
         e_file_result ReadXFLyricsHeader(xf::lyrics_id * pxflh);
         e_file_result GetDelta(imedia::position & tkDelta);
         e_file_result ReadEvent();
         virtual e_file_result MoveNext();
         int32_t GetMidiTrackIndex();
         imedia::position get_position();
         //int32_t GetFlags();


         e_file_result get_position(imedia::position & tkPosition, imedia::position  tkMax);

         e_file_result WorkWriteEvent();
         e_file_result WorkWriteEvent(event_v5 & event);

         VMSRESULT WorkSeek(uint32_t dwSeekWhat);
         void WorkDeleteEvent();

         ::index m_iWorkCurrentEvent;

         e_file_result WorkReadKarTokens(
            stringa & tokena,
            imedia::position_array *   lptkaTicks,
            stringa & stra);

         e_file_result WorkReadTune1000Tokens(
            stringa &  tokena,
            imedia::position_array * lptkaTicks);

         e_file_result CopyWork(
            track *pTrk,
            int32_t iMode);

         e_file_result WorkCopyWork(
            track *pTrk,
            int32_t iMode);

         imedia::position WorkGetPosition();


         virtual e_file_result WorkMoveNext();

         virtual void WorkGetEvent(
            event_base *&    pevent,
            imedia::position                  tkMax,
            bool                  bTkMaxInclusive);

         virtual void WorkGetEvent(
            event_v5 *&    pevent,
            imedia::position                  tkMax,
            bool                  bTkMaxInclusive);

         virtual e_file_result WorkSeekBegin();
         virtual e_file_result WorkSeekEnd();

         e_file_result WorkWriteXFInfoHeader(const char * pSongName, ::music::xf::info_header *pXfih, ::music::xf::info_header_ls * pXfihls);

         //e_file_result WorkWriteXFLyricEvent(
         //   string &str,
         //   imedia::position tkPosition);

         e_file_result WorkWriteXFLyricEvent(string &str, imedia::position tkPosition);

         e_file_result WorkReadXFTokens(uint32_t uiCodePage, stringa & tokena, imedia::position_array * lptkaTicks);

         e_file_result WorkReadXFTokens(uint32_t uiCodePage, stringa & tokena, imedia::position_array & positiona, int_array & iaTokenLine, bool bPlain);

         e_file_result WorkReadEmptyXFTokens(stringa & tokena, imedia::position_array & positiona);

         /*   e_file_result WorkReadAnsiXFTokens(stringa & tokena, imedia::position_array * lptkaTicks);*/

         e_file_result WriteXFLyricTrack(stringa & tokena, imedia::position_array & positiona, const char * psz);


         /*e_file_result WorkReadShiftJisXFTokens(
         stringa &  tokena,
         imedia::position_array *   lptkaTicks);*/

         e_file_result WorkGetLevel2Events(events_v1 * pevents, int32_t iTrack, bool bAnyTrack);
         e_file_result WorkGetNoteOnOffEventsV1(events_v1 * pEvents, int32_t iTrack, bool bAnyTrack);
         e_file_result WorkGetStandardEventsV1(events_v1 * pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack);
         e_file_result WorkGetStandardEventsV1OnlyMelodic(events_v1 * pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack);
         e_file_result WorkSeekXFLyricsHeader();
         e_file_result WorkReadXFLyricsHeader(xf::lyrics_id * pxflh);

         int32_t GetCurrentEvent();
         e_file_result WriteEvent(event_v5 & midiEvent);
         track_v8 & GetWorkTrack();
         VMSRESULT ToWorkStorage();
         VMSRESULT FromWorkTrack(bool bExcludeEOT);
         VMSRESULT FromWorkTrack(track_v8 & track);
         VMSRESULT ReadEvent(event_v5 & midiEvent);
         VMSRESULT ReadEvent(event_v1 & midiEvent);


         e_file_result seek(uint32_t dwSeekWhat);

         virtual e_file_result seek_begin();


         byte *   GetTrackImage();
         uint32_t   GetTrackImageLength();
         byte *   GetAllocationImage();
         byte *   GetTrackBody();
         uint32_t   GetTrackBodyLength();

         bool IsSoftKaraokeFile(bool bWork = false, stringa * pstra = NULL);

         bool IsXFFile();
         bool IsTune1000File();

         e_file_result GetStandardEventsV1(events_v1 * pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack);
         e_file_result GetNoteOnOffEventsV1(events_v1 * pEvents, int32_t iTrack, bool bAnyTrack);

         e_file_result GetLevel2Events(events_v1 *pevents, int32_t iTrack, bool bAnyTrack);

         void SetAutoAllocation(bool bValue = TRUE);

         e_file_result copy(track *pTrk, int32_t iMode);

         e_file_result WriteXFLyricEvent(string &str, imedia::position tkPosition);

         e_file_result WriteHeader(CHUNKHDR *pHdr);
         e_file_result WriteHeaderLength();
         e_file_result WriteCompleteEvent(imedia::position tkMax, bool bUseEventPosition);
         e_file_result WriteCompleteEvent(event_base & event, imedia::position tkMax, bool bUseEventPosition);
         e_file_result WriteDelta();

         e_file_result WriteXFInfoHeader(const char * pSongName, ::music::xf::info_header *pXfih, ::music::xf::info_header_ls *pXfihls);

         virtual e_file_result GetEvent(event_base * & pevent, imedia::position tkMax, bool bTkMaxInclusive);
         virtual e_file_result GetEvent(event_v1 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);
         void _WorkGetEvent(event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);
         bool _WorkIsEOT();
         imedia::position _WorkGetPosition();
         e_file_result _WorkMoveNext();

         void dump(dump_context & dumpcontext) const;

      };


   } // namespace midi


} // namespace music





