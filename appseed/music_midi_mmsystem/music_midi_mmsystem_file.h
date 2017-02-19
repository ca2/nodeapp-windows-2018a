#pragma once


#define C_TEMPO_MAP_CHK     16


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM buffer :
            virtual public ::music::midi::file::buffer
         {
         public:



            buffer(::aura::application * papp);
            virtual ~buffer();


            virtual double GetTempoShiftRate();

            bool IsTrackMute(int32_t iIndex);

            void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1);

            void MuteTrack(int32_t iIndex, bool bMute = true);

            e_result WorkSeekBegin();

            e_result CalcTkLength();

            //e_result CreateTempoMap();

            ::multimedia::e_result ImmediatePutTempoChange();

            ::music::e_result GetTempoEvent(::music::midi::event & event);

            void OnStop();

            uint32_t GetImageSize();

            LPBYTE GetImage();

            ::music::e_result Format0();

            ::music::e_result InsertTempoEvent();

            ::music::midi::tempo_map_entry * GetTempoMapEntry(const imedia_position & position);

            virtual double GetTempoShift();

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

            e_result ChangeEventDelta(::music::midi::event *pEvent, imedia_position tkNewDelta);

            e_result ReplaceSameDeltaEvent(::music::midi::event & pEventNew);

            e_result GetPreviousEvent(::music::midi::event * pPreviousEvent, ::music::midi::event * pEvent);

            e_result SaveFile(const char * lpFilePath);

            e_result WorkWriteXFTracks(stringa & tokena, imedia::position_array & positiona, ::music::xf::info_header & xfinfoheader);

            //int32_t CalcMelodyTrack(::music::midi::events ** ppEvents, imedia::position_array * pTicks);

            //int32_t WorkCalcMelodyTrack(::music::midi::events ** ppEvents, imedia::position_array & positiona, int_array & iaTokenLine);

            int32_t GetFormat();

            e_result MergeTracks();

            e_result WorkMergeTracks();

            e_result WorkStreamRender(LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNomimalMax);

            e_result WorkGetNextEvent(::music::midi::event * & pevent, imedia_position tkMax, bool bTkMaxInclusive);

            e_result WorkGetNextRawEvent(::music::midi::event * & pevent, imedia_position tkMax, bool bTkMaxInclusive);

            e_result WorkGetNextRawMidiEvent(::music::midi::event * & pevent, imedia_position tkMax, bool bTkMaxInclusive);

            e_result WorkSeek(imedia_position tkPosition, LPMIDIHDR lpmh);

            void delete_contents();

            int32_t GetDivision();

            imedia_position GetPosition();

            void AttachSeekSync(::ikaraoke::events_tracks_v1 * ptracksv1);

            bool IsNull();

            void WorkSeekEnd();

            e_result OpenFile(::music::midi::file::buffer &file, int32_t openMode);

            e_result OpenFile(::file::file & file, int32_t openMode);

            e_result OpenFile(memory * pmemstorage, int32_t openMode, ::music::e_storage);

            e_result OpenFile(memory * pmemstorage, ::music::midi::file::e_open eopenmode, ::music::e_storage estorage);

            e_result SaveFile(::file::file & file);

            e_result CloseFile();

            e_result GetFileInfo(PSMFFILEINFO psfi);

            int_ptr TicksToMillisecs(imedia_position tkOffset);

            void TicksToMillisecs(imedia::time_array *pMillisArray, imedia::position_array *pTickArray, int_ptr tkOffset);

            void TicksToMillisecs(imedia::time_2darray *p2DMillisArray, imedia::position_2darray * p2DTicksArray, int_ptr tkOffset);

            void PositionToTime(imedia::time_2darray  & timea, imedia::position_2darray  & positiona, int_ptr tkOffset);

            void PositionToTime(imedia::time_array  & timea, imedia::position_array  & positiona, int_ptr tkOffset);

            imedia_position MillisecsToTicks(imedia_time msOffset);

            imedia_time PositionToTime(imedia_position msOffset);

            imedia_position TimeToPosition(imedia_time msOffset);

            void MillisecsToTicks(imedia::position_array * pTickArray, imedia::time_array *pMillisArray, imedia_time msOffset);

            void TimeToPosition(imedia::position_array & positiona, imedia::time_array & timea, imedia_time msOffset);

            e_result StreamRender(LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNomimalMax);

            ::music::e_result StreamEvent(imedia_position tkDelta, ::music::midi::event * Event, LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNomimalMax);

            ::music::e_result StreamEventF1(imedia_position tkDelta, array < ::music::midi::event *, ::music::midi::event * > & eventptra, LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNomimalMax);

            e_result seek(imedia_position tkPosition, LPMIDIHDR lpmh);

            e_result seek(imedia_position tkPosition);

            //e_result Build();

            e_result GetNextEvent(::music::midi::event *& pevent, imedia_position tkMax, bool   bTkMaxInclusive);

            e_result GetNextEventTkPosition(imedia_position * pTkPosition, imedia_position tkMax);

            void _SyncSeek(imedia_position tkPosition, ::ikaraoke::events_tracks_v1 * ptracksv1);

            void _SyncSeek(imedia_position tkPosition);

            e_result InsertParmData(imedia_position tkDelta, LPMIDIHDR lpmh);

            ::music::e_result InsertLyricEvents(::ikaraoke::lyric_event_v1 * pLyricEvent, LPMIDIHDR lpmh);

            e_result InsertPadEvent(imedia_position tkDelta, LPMIDIHDR lpmh);

            void SetOpened(bool bOpened = true);


         };


      } // namespacemmsystem


   } // namespace midi


} // namespace music





