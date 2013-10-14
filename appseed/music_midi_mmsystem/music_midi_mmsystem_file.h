#pragma once


#define C_TEMPO_MAP_CHK     16


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM file :
         virtual public midi::file::buffer
      {
      public:



         file(sp(base_application) papp);
         virtual ~file();


         double GetTempoShiftRate();

         bool IsTrackMute(int32_t iIndex);

         void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1);

         void MuteTrack(int32_t iIndex, bool bMute = true);

         e_result WorkSeekBegin();

         e_result CalcTkLength();

         e_result CreateTempoMap();

         ::multimedia::e_result ImmediatePutTempoChange();

         ::music::e_result GetTempoEvent(::music::midi::event_base & event);

         void OnStop();

         uint32_t GetImageSize();

         LPBYTE GetImage();

         ::music::e_result Format0();

         ::music::e_result InsertTempoEvent();

         ::music::midi::tempo_map_entry * GetTempoMapEntry(const imedia::position & position);

         int32_t GetTempoShift();

         ::music::e_result SetTempoShift(int32_t iTempoShift);

         void ToWorkStorage();

         void FromWorkStorage();

         int32_t GetKeyShift();

         int32_t SetKeyShift(int32_t iKeyShift);

         bool IsSoftKaraokeFile(bool bWork = false, stringa * pstra = NULL);

         bool IsXFFile();

         bool IsTune1000File();

         bool IsOpened();
         
         e_result RebuildIndex();

         e_result WriteHeader(MIDIFILEHDR * lpmfh);

         e_result ChangeEventDelta(::music::midi::event_base *pEvent, imedia::position tkNewDelta);

         e_result ReplaceSameDeltaEvent(::music::midi::event_base & pEventNew);

         e_result GetPreviousEvent(::music::midi::event_base * pPreviousEvent, ::music::midi::event_base * pEvent);

         e_result SaveFile(const char * lpFilePath);

         e_result WorkWriteXFTracks(stringa & tokena, imedia::position_array & positiona, ::music::xf::info_header & xfinfoheader);

         int32_t CalcMelodyTrack(::music::midi::events_v1 ** ppEvents, imedia::position_array * pTicks);

         int32_t WorkCalcMelodyTrack(::music::midi::events_v1 ** ppEvents, imedia::position_array & positiona, int_array & iaTokenLine);

         int32_t GetFormat();

         e_result MergeTracks();

         e_result WorkMergeTracks();

         e_result WorkStreamRender(LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         e_result WorkGetNextEvent(::music::midi::event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         e_result WorkGetNextRawEvent(::music::midi::event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         e_result WorkGetNextRawMidiEvent(::music::midi::event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive);

         e_result WorkSeek(::imedia::position tkPosition, LPMIDIHDR lpmh);

         void delete_contents();

         int32_t GetDivision();

         imedia::position get_position();

         void AttachSeekSync(::ikaraoke::events_tracks_v1 * ptracksv1);

         bool IsNull();

         void WorkSeekEnd();

         e_result OpenFile(::music::midi::file::buffer &file, int32_t openMode);

         e_result OpenFile(::file::stream_buffer & file, int32_t openMode);

         e_result OpenFile(primitive::memory * pmemstorage, int32_t openMode, ::music::e_storage);

         e_result OpenFile(primitive::memory * pmemstorage, midi::file::e_open eopenmode, ::music::e_storage estorage);

         e_result SaveFile(::file::stream_buffer & file);

         e_result CloseFile();

         e_result GetFileInfo(PSMFFILEINFO psfi);

         int_ptr TicksToMillisecs(imedia::position tkOffset);

         void TicksToMillisecs(imedia::time_array *pMillisArray, imedia::position_array *pTickArray, int_ptr tkOffset);

         void TicksToMillisecs(imedia::time_2darray *p2DMillisArray, imedia::position_2darray * p2DTicksArray, int_ptr tkOffset);

         void PositionToTime(imedia::time_2darray  & timea, imedia::position_2darray  & positiona, int_ptr tkOffset);

         void PositionToTime(imedia::time_array  & timea, imedia::position_array  & positiona, int_ptr tkOffset);

         imedia::position MillisecsToTicks(imedia::time msOffset);

         imedia::time PositionToTime(imedia::position msOffset);

         imedia::position TimeToPosition(imedia::time msOffset);

         void MillisecsToTicks(imedia::position_array * pTickArray, imedia::time_array *pMillisArray, imedia::time msOffset);

         void TimeToPosition(imedia::position_array & positiona, imedia::time_array & timea, imedia::time msOffset);

         e_result StreamRender(LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         ::music::e_result StreamEvent(imedia::position tkDelta, ::music::midi::event_base * Event, LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         ::music::e_result StreamEventF1(imedia::position tkDelta, array < ::music::midi::event_v5 *, ::music::midi::event_v5 * > & eventptra, LPMIDIHDR lpmh, imedia::position tkMax, uint32_t cbPrerollNomimalMax);

         e_result seek(imedia::position tkPosition, LPMIDIHDR lpmh);

         e_result seek(imedia::position tkPosition);

         e_result Build();

         e_result GetNextEvent(::music::midi::event_v1 *& pevent, imedia::position tkMax, bool   bTkMaxInclusive);

         e_result GetNextEventTkPosition(imedia::position * pTkPosition, imedia::position tkMax);

         void _SyncSeek(imedia::position tkPosition, ::ikaraoke::events_tracks_v1 * ptracksv1);

         void _SyncSeek(imedia::position tkPosition);

         e_result InsertParmData(imedia::position tkDelta, LPMIDIHDR lpmh);

         ::music::e_result InsertLyricEvents(::ikaraoke::lyric_event_v1 * pLyricEvent, LPMIDIHDR lpmh);

         e_result InsertPadEvent(imedia::position tkDelta, LPMIDIHDR lpmh);

         void SetOpened(bool bOpened = true);


      };


   } // namespace midi_mmsystem


} // namespace music





