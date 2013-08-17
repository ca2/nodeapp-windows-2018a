#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      BYTE tracks::m_paramEventNoteOnPositionCB[] = {EVENT_ID_NOTE_ON};
      int32_t tracks::m_iparamEventNoteOnPositionCB = 1;

      tracks::tracks(sp(::ca2::application) papp, sp(::music::midi::file) pFile) :
         ca2(papp),
         m_tracka(papp)
      {

         m_tracka.Initialize(this);
         m_trackaActive.Initialize(this);

         m_tkLevelLastUpdateInterval = 0;
         m_iaLevel.set_size(16);
         m_iaNoteOn.set_size(16);
         m_iaInstrument.set_size(16);

         m_pFile = pFile;
         m_tkPosition = 0;
         m_iLyricDelay = 0;
         m_bVolumeEventSent = false;

      }

      tracks::~tracks()
      {
      }

      void tracks::dump(dump_context & dumpcontext) const
      {
         dumpcontext << "\n";
         dumpcontext << "\n";
         dumpcontext << "tracks";
         dumpcontext << "\n";
         for(int32_t i = 0; i < GetMidiTrackCount(); i++)
         {
            track * ptrack = (track *) MidiTrackAt(i);
            dumpcontext << "track " << i;
            dumpcontext << "\n";
            ptrack->dump(dumpcontext);
         }
      }

      //////////////////////////////////////////////////////////////////////
      // Operations
      //////////////////////////////////////////////////////////////////////

      /******************************************************************************
      *
      * smfGetNextEvent
      *
      * read the next event from the given spfile->
      *
      * pSmf                      - File to read the event from.
      *
      * pEvent                    - Pointer to an event structure which will receive
      *                             basic information about the event.
      *
      * tkMax                     - Tick destination. An attempt to read past this
      *                             position in the file will fail.
      *
      * Returns
      *   Success The events were successfully read.
      *   SEndOfFile There are no more events to read in this track.
      *   SReachedTkMax No event was read because <p tkMax> was reached.
      *   InvalidFile A disk or parse error occured on the spfile->
      *
      * This is the lowest level of parsing for a raw MIDI stream. The basic
      * information about one event in the file will be returned in pEvent.
      *
      * Merging data from all tracks into one stream is performed here.
      *
      * pEvent->tkDelta will contain the tick delta for the event.
      *
      * pEvent->abEvent will contain a description of the event.
      *  pevent->abEvent[0] will contain
      *    F0 or F7 for a System Exclusive message.
      *    FF for a MIDI file meta event.
      *    The status byte of any other MIDI message. (Running status will
      *    be tracked and expanded).
      *
      * pEvent->cbParm will contain the number of bytes of paramter data
      *   which is still in the file behind the event header already read.
      *   This data may be read with <f smfGetTrackEventData>. Any unread
      *   data will be skipped on the next call to <f smfGetNextTrackEvent>.
      *
      * Channel messages (0x8? - 0xE?) will always be returned fully in
      *   pevent->abEvent.
      *
      *  Meta events will contain the meta type in pevent->abEvent[1].
      *
      *  System exclusive events will contain only an 0xF0 or 0xF7 in
      *    pevent->abEvent[0].
      *
      *  The following fields in ptrack are used to maintain state and must
      *  be updated if a seek-in-track is performed:
      *
      *  bRunningStatus contains the last running status message or 0 if
      *   there is no valid running status.
      *
      *  hpbImage is a pointer into the file image of the first byte of
      *   the event to follow the event just read.
      *
      *  dwLeft contains the number of bytes from hpbImage to the end
      *   of the track.
      *
      *
      * get the next due event from all (in-use?) tracks
      *
      * For all tracks
      *  If not end-of-track
      *   decode event delta time without advancing through buffer
      *   event_absolute_time = track_tick_time + track_event_delta_time
      *   relative_time = event_absolute_time - last_stream_time
      *   if relative_time is lowest so far
      *    save this track as the next to pull from, along with times
      *
      * If we found a track with a due event
      *  Advance track pointer past event, saving ptr to parm data if needed
      *  track_tick_time += track_event_delta_time
      *  last_stream_time = track_tick_time
      * Else
      *  Mark and return end_of_file
      *
      *****************************************************************************/
      e_file_result tracks::GetNextEvent(
         event_v1 *&         pevent,
         imedia::position                   tkMax,
         bool                     bTkMaxInclusive,
         bool                    bOnlyMTrk,
         bool                    bOnlyMidiTrack)
      {
         track_base *             ptrack;
         track_base *             ptrkFound;
         int32_t                           idxTrackFound;
         int32_t                           idxTrack;
         imedia::position              tkEventPosition;
         imedia::position              tkEventFound;
         imedia::position              iRelPosition;
         imedia::position              iMinRelPosition;
         e_file_result    mfr;

         ASSERT(pevent != NULL);


         iMinRelPosition = 0x7fffffff;
         ptrkFound = NULL;

         for (idxTrack = 0; idxTrack < GetTrackCount(); idxTrack++)
         {
            ptrack = TrackAt(idxTrack);
            if(bOnlyMTrk)
            {
               if(ptrack->IsType(track_base::TypeMidi))
               {
                  track * ptrackMidi = (track *) ptrack;
                  CHUNKHDR FAR * pCh = (CHUNKHDR FAR *) ptrackMidi->GetTrackImage();
                  if (pCh->fourccType != FOURCC_MTrk)
                     continue;
               }
            }
            else if(bOnlyMidiTrack)
            {
               if(!ptrack->IsType(track_base::TypeMidi))
                  continue;
            }

            if (ptrack->GetFlags().is_signalized(EndOfTrack))
               continue;


            if(ptrack->get_position(tkEventPosition, tkMax) != VMSR_SUCCESS)
            {
               continue;
            }

            iRelPosition = tkEventPosition - m_tkPosition;
            ASSERT(iRelPosition >= 0);

            if(iRelPosition < iMinRelPosition)
            {
               iMinRelPosition = iRelPosition;
               ptrkFound = ptrack;
               idxTrackFound = idxTrack;
               tkEventFound = tkEventPosition;
            }
         }

         if (ptrkFound == NULL)
         {
            return SEndOfFile;
         }

         ptrack = ptrkFound;

         imedia::position tkPosition = tkEventFound;

         iRelPosition = tkPosition - m_tkPosition;
         ASSERT(iRelPosition >= 0);
         if(iRelPosition < 0)
            iRelPosition = 0;

         if(!GetFlags().is_signalized(DisablePlayLevel1Operations))
         {
            if(iRelPosition != 0)
            {
               m_tkLevelLastUpdateInterval += iRelPosition;
               if(m_tkLevelLastUpdateInterval > 24)
               {
                  m_tkLevelLastUpdateInterval = 0;
                  if(!m_bVolumeEventSent)
                  {
                     return GetVolumeEvent(get_app(), pevent);
                  }
                  else
                  {
                     m_bVolumeEventSent = false;
                  }
               }
            }
            else
            {
               m_bVolumeEventSent = false;
            }
         }


         if(ptrack->get_type() == track_base::TypeLyric)
         {
            ASSERT(true);
         }

         if(bTkMaxInclusive)
         {
            if (m_tkPosition + iMinRelPosition > tkMax)
            {
               return SReachedTkMax;
            }
         }
         else
         {
            if (m_tkPosition + iMinRelPosition >= tkMax)
            {
               return SReachedTkMax;
            }
         }

         mfr = ptrack->GetEvent(pevent, tkMax, bTkMaxInclusive);

         if(pevent->GetParamSize() == 21)
         {
            //      __debug_break();
         }
         if(mfr != Success)
            return mfr;

         //pevent->operator =(*ptrack->GetEvent());





         pevent->SetDelta(iRelPosition);
         m_tkPosition += iRelPosition;
         m_iCurrentTrack = idxTrackFound;
         ptrack->MoveNext();
         return Success;
      }

      e_file_result tracks::GetNextEvent(
         event_v1 *&    pevent,
         imedia::position                tkMax,
         bool                 bTkMaxInclusive,
         int32_t                  iTrack)
      {
         track_base *             ptrack;
         track_base *             ptrkFound;
         int32_t                           idxTrackFound;
         int32_t                           idxTrack;
         imedia::position              tkEventPosition;
         imedia::position              iRelPosition;
         imedia::position              iMinRelPosition;
         e_file_result    mfr;

         while(true)
         {
            iMinRelPosition = 0x7fffffff;
            ptrkFound = NULL;
            for (idxTrack = 0; idxTrack < GetTrackCount(); idxTrack++)
            {
               ptrack = TrackAt(idxTrack);

               if (ptrack->GetFlags().is_signalized(EndOfTrack))
                  continue;

               if(ptrack->get_position(tkEventPosition, tkMax) != VMSR_SUCCESS)
               {
                  continue;
               }

               iRelPosition = tkEventPosition - m_tkPosition;

               if(iRelPosition < iMinRelPosition)
               {
                  iMinRelPosition = iRelPosition;
                  ptrkFound = ptrack;
                  idxTrackFound = idxTrack;
               }
            }

            if (ptrkFound == NULL)
            {
               return SEndOfFile;
            }

            ptrack = ptrkFound;

            if(ptrack->get_type() == track_base::TypeLyric)
            {
               ASSERT(true);
            }

            if(bTkMaxInclusive)
            {
               if(m_tkPosition + iMinRelPosition > tkMax)
               {
                  return SReachedTkMax;
               }
            }
            else
            {
               if(m_tkPosition + iMinRelPosition >= tkMax)
               {
                  return SReachedTkMax;
               }
            }
            //Aten鈬o melhorar!!!!
            //em vez de pegar o evento, simplesmente pul・lo
            mfr = ptrack->GetEvent(pevent, tkMax, bTkMaxInclusive);

            if(mfr != Success)
               return mfr;

            imedia::position tkPosition = ptrack->get_position();

            iRelPosition = tkPosition - m_tkPosition;
            if(iRelPosition < 0)
               iRelPosition = 0;

            if(pevent != NULL)
            {
               pevent->SetDelta(iRelPosition);
            }
            m_tkPosition = m_tkPosition + iRelPosition;
            m_iCurrentTrack = idxTrackFound;
            if(m_iCurrentTrack == iTrack)
               break;
         }


         mfr = ptrack->MoveNext();
         if(mfr != Success)
            return mfr;

         return Success;
      }


      //
      //
      //
      //
      //
      //
      //
      //
      //
      e_file_result tracks::GetNextEventTkPosition(
         imedia::position *               pTkPosition,
         imedia::position                   tkMax)
      {
         track_base *         ptrack;
         track_base *         pTrkFound ;
         int32_t                  idxTrack;
         //imedia::position               tkEventDelta;
         imedia::position               tkEventPosition;
         imedia::position               tkEventPositionFound;
         imedia::position                   tkRelTime;
         imedia::position                   tkMinRelTime;
         //    uint32_t                   dwGot;
         VMSRESULT               vmsr;


         //    ASSERT(pSmf != NULL);
         ASSERT(pTkPosition != NULL);

         if (GetFlags().is_signalized(EndOfTrack))
         {
            return SEndOfFile;
         }

         pTrkFound       = NULL;
         tkMinRelTime    = MAX_TICKS;

         //for (ptrack = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrack++)
         for (idxTrack = 0; idxTrack < GetTrackCount(); idxTrack++)
         {
            ptrack = TrackAt(idxTrack);
            if (ptrack->GetFlags().is_signalized(EndOfTrack))
               continue;

            //      if(ptrack->GetState() == track::StateOnDelta)
            //      {
            //         if(ptrack->ReadDelta() != Success)
            //         {
            //            TRACE("Hit end of track w/o end marker!\n");
            //               return InvalidFile;
            //         }
            //      }

            //      tkEventDelta = ptrack->m_tkDelta;

            if (VMSR_SUCCESS != (vmsr = ptrack->get_position(tkEventPosition, tkMax)))
            {
               continue;
            }

            tkRelTime = tkEventPosition - m_tkPosition;

            if (tkRelTime < tkMinRelTime)
            {
               tkMinRelTime = tkRelTime;
               pTrkFound = ptrack;
               tkEventPositionFound = tkEventPosition;
            }
         }

         if (pTrkFound == NULL)
         {
            GetFlags().signalize(EndOfTrack);
            return SEndOfFile;
         }

         ptrack = pTrkFound;

         if (m_tkPosition + tkMinRelTime > tkMax)
         {
            return SReachedTkMax;
         }


         //    dwGot = ptrack->m_dwUsed;
         //   tkEventDelta = ptrack->m_tkDelta;
         //ptrack->cbLeft   -= dwGot;

         /* We MUST have at least three bytes here (cause we haven't hit
         ** the end-of-track meta yet, which is three bytes long). Checking
         ** against three means we don't have to check how much is left
         ** in the track again for any int16_t event, which is most cases.
         */
         //if (ptrack->cbLeft < 3)
         //{
         //return InvalidFile;
         //}

         //    *pTkPosition = ptrack->m_tkPosition + tkEventDelta;
         *pTkPosition = tkEventPositionFound;
         return Success;
      }

      void tracks::seek_begin()
      {
         for (int32_t i = 0; i < GetTrackCount(); i++)
         {
            track_base * ptrack = TrackAt(i);
            ptrack->seek_begin();
         }
         m_tkPosition = 0;
         GetFlags().unsignalize(EndOfTrack);
      }

      e_file_result tracks::GetXFTokens(
         string2a &
         token2a,
         imedia::position_2darray *p2DTicks,
         ::music::xf::lyrics_id_array * pxflh2a)
      {
         ASSERT(p2DTicks != NULL);
         ASSERT(pxflh2a != NULL);
         stringa  tokena;
         imedia::position_array * lptkaTicks = NULL;
         ::music::xf::lyrics_id xflh;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track *ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
            {
               continue;
            }

            ptrack->seek_begin();
            e_file_result smfr;
            smfr = ptrack->SeekXFLyricsHeader();
            if(smfr == Success)
            {
               smfr = ptrack->ReadXFLyricsHeader(&xflh);
            }


            tokena.remove_all();
            lptkaTicks = new imedia::position_array();
            if(xflh.m_strLanguage == L"JP")
            {
               smfr = ptrack->ReadShiftJisXFTokens(tokena, lptkaTicks);
            }
            else
            {
               smfr = ptrack->ReadAnsiXFTokens(tokena, lptkaTicks);
            }
            if(Success == smfr)
            {
               token2a.add(canew(stringa(tokena)));
               p2DTicks->add(lptkaTicks);
               pxflh2a->add(canew(::music::xf::lyrics_id(xflh)));
            }
            else
            {
               delete lptkaTicks;
            }
            lptkaTicks = NULL;
         }
         return Success;
      }

      e_file_result tracks::GetKarTokens(
         string2a &    token2a,
         imedia::position_2darray *       p2DTicks
         )
      {
         ASSERT(p2DTicks != NULL);
         stringa tokena;
         imedia::position_array * lptkaTicks = NULL;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track *ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->seek_begin();
            tokena.remove_all();
            lptkaTicks = new imedia::position_array();
            if(Success == ptrack->ReadKarTokens(tokena, lptkaTicks))
            {
               token2a.add(canew(stringa(tokena)));
               p2DTicks->add(lptkaTicks);
            }
            else
            {
               delete lptkaTicks;
            }
            lptkaTicks = NULL;
         }
         return Success;
      }


      e_file_result tracks::GetXFInfoHeaders(
         ::music::xf::info_headers *pxfihs)
      {
         ASSERT(pxfihs != NULL);
         pxfihs->Empty();
         ::index nIndex;
         ::count nSize = GetTrackCount();
         //string str lpsz;
         //::music::xf::info_header * pxfih;
         //::music::xf::info_header_ls * pxfihls;
         ::music::xf::info_header_ls xfihls;
         for(nIndex = 0; nIndex < nSize; nIndex++)
         {
            track *ptrack = (track *)TrackAt(nIndex);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            if(pxfihs->m_xfInfoHeader.is_empty())
            {
               ptrack->seek_begin();
               ptrack->GetNextXFInfoHeader(&pxfihs->m_xfInfoHeader);
               ptrack->seek_begin();
               ptrack->GetNextXFSongName(pxfihs->m_strSongName);
            }
            ptrack->seek_begin();

            if(Success == ptrack->GetNextXFInfoHeaderLS(&xfihls))
            {
               pxfihs->m_xfaInfoHeaderLS.add(xfihls);
            }
            if(pxfihs->m_strSongName.is_empty())
            {
               ptrack->seek_begin();
               ptrack->GetNextXFSongName(pxfihs->m_strSongName);
            }
         }
         return SEndOfFile;

      }


      e_file_result tracks::GetKarInfoHeader(
         ::music::softkaraoke::info *pKarI)
      {
         ASSERT(pKarI != NULL);
         if(GetTrackCount() >=3)
         {
            track *ptrack = (track *) TrackAt(2);
            ASSERT(ptrack->IsType(track_base::TypeMidi));
            return ptrack->GetNextKarInfo(pKarI);
         }
         else
         {
            track *ptrack = (track *) TrackAt(0);
            ASSERT(ptrack->IsType(track_base::TypeMidi));
            return ptrack->GetNextKarInfo(pKarI);
         }

      }

      e_file_result tracks::GetTune1000InfoHeader(
         ::music::tune1000::info *pTune1000I, int32_t iTrack)
      {
         ASSERT(pTune1000I != NULL);
         //   if(GetTrackCount() >=3)
         //   {
         //      track *ptrack = TrackAt(2);
         //      return ptrack->GetNextTune1000Info(pTune1000I);
         //   }
         //   else
         //   {
         if(iTrack >= 0 && iTrack < GetTrackCount())
         {
            track *ptrack = (track *) TrackAt(iTrack);
            ASSERT(ptrack->IsType(track_base::TypeMidi));
            return ptrack->GetNextTune1000Info(pTune1000I);
         }
         else
         {
            return SNotFound;
         }

         //   }

      }

      int32_t tracks::CalcMelodyTrack(
         events_v1 ** ppEventsRet,
         imedia::position_array *pTicks,
         WORD wFileFormat)
      {
         UNREFERENCED_PARAMETER(wFileFormat);
         ASSERT(*ppEventsRet == NULL);
         ::index i, j, k;
         double iMatches;
         ::index iBestMatch;
         uint32_array dwaBestMatchesIndexes;
         uint32_array dwaBestMatchesScores;
         imedia::position tkTolerance = 0;
         imedia::position tkPositionA, tkPositionB;
         const imedia::position tkToleranceAddUp = WORDSWAP(m_pFile->m_pFileHeader->wDivision) / 4;

         track * ptrack;
         events_v1 * pEvents;
         //   primitive_array < events_v1 *> eventsArray;
         //primitive_array < primitive_array < events_v1 *>> eventsArray2D;

         double2a i2aEventMatch;
         bool bAnyTrack = m_pFile->GetFormat() == 1;
         const int32_t max_tracks_search = 16;
         ::index iTrack;
         ::count iTrackCount;
         if(m_pFile->GetFormat() == 1)
         {
            iTrackCount = GetMidiTrackCount();
         }
         else
         {
            iTrackCount = max_tracks_search;
         }






         ::index iToleranceStep;
         ::count iToleranceStepCount;


         for(iTrack = 0; iTrack < iTrackCount; iTrack++)
         {
            i2aEventMatch.add_new();
            if(m_pFile->GetFormat() == 0)
            {
               ptrack = (track *) MidiTrackAt(0);
            }
            else
            {
               if(iTrack >= iTrackCount)
                  break;
               ptrack = (track *) MidiTrackAt(iTrack);
            }
            pEvents = new events_v1();
            ptrack->WorkSeekBegin();
            ptrack->WorkGetStandardEventsV1(pEvents, NoteOn, (int32_t) iTrack, bAnyTrack);
            //    eventsArray.add(pEvents);
            iToleranceStep = 0;

            for(tkTolerance = 0;
               tkTolerance <= WORDSWAP(m_pFile->m_pFileHeader->wDivision);
               tkTolerance += tkToleranceAddUp)
            {

               k = 0;
               iMatches = 0;
               for(j = 0; j < pTicks->get_size() - 1; j++)
               {
                  imedia::position tkStart = pTicks->get_at(j);
                  imedia::position tkEnd = pTicks->get_at(j);

                  while(k < pEvents->m_tkaEventsPosition.get_count() && (tkPositionB = pEvents->m_tkaEventsPosition.get_at(k)) < (tkStart - tkTolerance))
                  {
                     k++;
                     iMatches -= 3.0;
                  }

                  if(k >= pEvents->m_tkaEventsPosition.get_count())
                     break;

                  while(k < pEvents->m_tkaEventsPosition.get_count() && (tkPositionB = pEvents->m_tkaEventsPosition.get_at(k)) < tkEnd)
                  {
                     k++;
                     iMatches += 1;
                  }

                  if(k >= pEvents->m_tkaEventsPosition.get_count())
                     break;

               }

               iMatches -= 3 * (pTicks->get_size() - j);
               iMatches *= m_pFile->m_pFileHeader->wDivision;
               iMatches /= tkTolerance;
               i2aEventMatch[iTrack].add(iMatches);

            }

         }

         if(i2aEventMatch.get_size() > 0)
         {
            iToleranceStepCount = i2aEventMatch[0].get_size();
         }
         else
         {
            iToleranceStepCount = 0;
         }

         for(iToleranceStep = 0;
            iToleranceStep < iToleranceStepCount;
            iToleranceStep++)
         {
            double_array & iaEventMatch = i2aEventMatch[iToleranceStep];
            iMatches = 0x80000000;
            iBestMatch = 0x80000000;
            for(i = 0; i < iaEventMatch.get_size(); i++)
            {
               if(iaEventMatch.element_at(i) > iMatches)
               {
                  iBestMatch = i;
                  iMatches = iaEventMatch.element_at(i);
               }
            }
            if(iBestMatch >= 0)
            {
               dwaBestMatchesIndexes.add((dword) iBestMatch);
               dwaBestMatchesScores.add((dword) iMatches);
            }
            tkTolerance += tkToleranceAddUp;
         }



















         /*   while(tkTolerance <= DWORDSWAP(m_pFile->m_pFileHeader->wDivision))
         {
         for(i = 0; i < iSize; i++)
         {
         if(m_pFile->GetFormat() == 0)
         {
         ptrack = (track *) MidiTrackAt(0);
         }
         else
         {
         if(i >= GetTrackCount())
         break;
         ptrack = (track *) MidiTrackAt(i);
         }
         pEvents = new events_v1();
         ptrack->WorkSeekBegin();
         ptrack->WorkGetStandardEventsV1(pEvents, NoteOn, i, bAnyTrack);
         eventsArray.add(pEvents);
         k = 0;
         iMatches = 0;
         for(j = 0; j < pTicks->get_size(); j++)
         {
         if(k >= pEvents->m_tkaEventsPosition.get_size())
         {
         //iMatches -= 3;
         break;
         }
         tkPositionA = pTicks->get_at(j);
         while(tkPositionA - tkTolerance >
         (tkPositionB = pEvents->m_tkaEventsPosition.get_at(k)))
         {
         iMatches--; // 2001-05-05 Ser・que funciona?ｿ
         k++;
         if(k >= pEvents->m_tkaEventsPosition.get_size())
         break;
         }
         if(tkPositionB >= tkPositionA - tkTolerance &&
         tkPositionB <= tkPositionA + tkTolerance)
         {
         iMatches++;
         }
         else
         {
         //iMatches--;
         iMatches -= 3;
         }
         }
         iMatches -= 3 * (pTicks->get_size() - j);
         //         TRACE("iMatches %d iTrack %d\n", iMatches, i);
         eventsMatches.add(iMatches);
         }
         iMatches = 0x80000000;
         iBestMatch = 0x80000000;
         for(i = 0; i < eventsMatches.get_size(); i++)
         {
         if(eventsMatches.get_at(i) > iMatches)
         {
         iBestMatch = i;
         iMatches = eventsMatches.get_at(i);
         }
         }
         for(i = 0; i < iSize; i++)
         {
         delete eventsArray.get_at(i);
         }
         eventsArray.remove_all();
         if(iBestMatch >= 0)
         //      break;
         {
         dwaBestMatchesIndexes.add(iBestMatch);
         dwaBestMatchesScores.add(iMatches);
         }
         tkTolerance += tkToleranceAddUp;
         eventsMatches.remove_all();

         }*/



         //   if(iBestMatch >= 0)
         if(dwaBestMatchesIndexes.get_size() > 0)
         {

            int32_t iWorstMatches = 0xffffffff;
            for(i = 0; i < dwaBestMatchesIndexes.get_size(); i++)
            {
               if(((int32_t) dwaBestMatchesScores.get_at(i)) < iWorstMatches)
               {
                  iWorstMatches = (int32_t) dwaBestMatchesScores.get_at(i);
               }
            }
            if(iWorstMatches < 0)
            {
               for(i = 0; i < dwaBestMatchesIndexes.get_size(); i++)
               {
                  dwaBestMatchesScores.set_at(i,
                     (int32_t)dwaBestMatchesScores.get_at(i) -
                     iWorstMatches);
               }
            }
            iMatches = 0x80000000;
            iBestMatch = -1;
            for(i = 0; i < dwaBestMatchesIndexes.get_size(); i++)
            {
               if(((int32_t) dwaBestMatchesScores.get_at(i) ) > pow((double)iMatches, (double)i))
               {
                  iBestMatch =  dwaBestMatchesIndexes.get_at(i);
                  iMatches = (int32_t) dwaBestMatchesScores.get_at(i);
               }
            }
            /*      for(i = 0; i < iSize; i++)
            {
            if(i == iBestMatch)
            {
            *ppEventsRet = eventsArray.get_at(i);
            }
            else
            {
            delete eventsArray.get_at(i);
            }
            }*/
            if(iBestMatch >= 0)
            {
               if(m_pFile->GetFormat() == 0)
               {
                  ptrack = (track *) TrackAt(0);
               }
               else
               {
                  if(i >= GetTrackCount())
                     goto Skip090;
                  ptrack = (track *) TrackAt(iBestMatch);
               }
               pEvents = new events_v1();
               ptrack->WorkSeekBegin();
               ptrack->WorkGetStandardEventsV1(pEvents, NoteOn, (int32_t) iBestMatch, bAnyTrack);
               *ppEventsRet = pEvents;
            }
Skip090:;

         }

         return (int32_t) iBestMatch;

      }

      int32_t tracks::WorkCalcMelodyTrack(
         events_v1 ** ppEventsRet,
         imedia::position_array & positiona,
         int_array & iaTokenLine,
         WORD wFileFormat)
      {
         UNREFERENCED_PARAMETER(iaTokenLine);
         UNREFERENCED_PARAMETER(wFileFormat);
         ASSERT(*ppEventsRet == NULL);


         uint32_array dwaBestMatchesIndexes;
         uint32_array dwaBestMatchesScores;
         imedia::position tkTolerance = 0;
         imedia::position tkPositionA, tkPositionB;
         //const imedia::position tkToleranceAddUp = 60;
         const imedia::position tkToleranceTotal = m_pFile->GetDivision();
         const imedia::position tkToleranceAddUp = tkToleranceTotal / 4;
         track * ptrack;
         events_v1 * pEvents;
         primitive_array < events_v1 *> eventsArray;
         //primitive_array < primitive_array < events_v1 *>> eventsArray2D;
         int_array eventsMatches;
         bool bAnyTrack = m_pFile->GetFormat() == 1;
         const int32_t max_tracks_search = 16;
         /*   if(m_pFile->GetFormat() == 1)
         {
         iSize = GetMidiTrackCount();
         }
         else
         {
         iSize = max_tracks_search;
         }*/
         double_array daCorrel;
         //   bool bAnyTrack = m_pFile->GetFormat() == 1;
         //   const int32_t max_tracks_search = 16;
         ::index iTrack;
         ::count iTrackCount;
         if(m_pFile->GetFormat() == 1)
         {
            iTrackCount = GetMidiTrackCount();
         }
         else
         {
            iTrackCount = max_tracks_search;
         }
















         /// old begin /////////////////////////////////////////////

         /*   while(tkTolerance <= WORDSWAP(m_pFile->m_pFileHeader->wDivision))
         {
         for(i = 0; i < iSize; i++)
         {
         if(m_pFile->GetFormat() == 0)
         {
         ptrack = (track *) MidiTrackAt(0);
         }
         else
         {
         if(i >= GetTrackCount())
         break;
         ptrack = (track *) MidiTrackAt(i);
         }
         pEvents = new events_v1();
         ptrack->WorkSeekBegin();
         ptrack->WorkGetStandardEventsV1(pEvents, NoteOn, i, bAnyTrack);
         eventsArray.add(pEvents);
         k = 0;
         iMatches = 0;
         for(j = 0; j < positiona.get_size(); j++)
         {
         if(k >= pEvents->m_tkaEventsPosition.get_size())
         {
         //iMatches -= 3;
         break;
         }
         tkPositionA = positiona.get_at(j);
         while(tkPositionA - tkTolerance >
         (tkPositionB = pEvents->m_tkaEventsPosition.get_at(k)))
         {
         iMatches--; // 2001-05-05 Ser・que funciona?ｿ
         k++;
         if(k >= pEvents->m_tkaEventsPosition.get_size())
         break;
         }
         if(tkPositionB >= tkPositionA - tkTolerance &&
         tkPositionB <= tkPositionA + tkTolerance)
         {
         iMatches++;
         }
         else
         {
         //iMatches--;
         iMatches -= 3;
         }
         }
         iMatches -= 3 * (positiona.get_size() - j);
         //         TRACE("iMatches %d iTrack %d\n", iMatches, i);
         eventsMatches.add(iMatches);
         }
         iMatches = 0x80000000;
         iBestMatch = 0x80000000;
         for(i = 0; i < eventsMatches.get_size(); i++)
         {
         if(eventsMatches.get_at(i) > iMatches)
         {
         iBestMatch = i;
         iMatches = eventsMatches.get_at(i);
         }
         }
         for(i = 0; i < iSize; i++)
         {
         delete eventsArray.get_at(i);
         }
         eventsArray.remove_all();
         if(iBestMatch >= 0)
         //      break;
         {
         dwaBestMatchesIndexes.add(iBestMatch);
         dwaBestMatchesScores.add(iMatches);
         }
         tkTolerance += tkToleranceAddUp;
         eventsMatches.remove_all();

         }*/



         /// old end /////////////////////////////////////////////







































         double dCorrel;



         for(iTrack = 0; iTrack < iTrackCount; iTrack++)
         {
            if(m_pFile->GetFormat() == 0)
            {
               ptrack = (track *) MidiTrackAt(0);
            }
            else
            {
               if(iTrack >= GetTrackCount())
                  break;
               ptrack = (track *) MidiTrackAt(iTrack);
            }
            pEvents = new events_v1();
            ptrack->WorkSeekBegin();
            ptrack->WorkGetStandardEventsV1(pEvents, NoteOn, (int32_t) iTrack, bAnyTrack);
            dCorrel = 0.0;
            int32_t i = 0;
            int32_t j = 0;
            while(true)
            {
               if(i >= positiona.get_count())
                  break;
               imedia::position tkPosition = positiona[i];
               imedia::position tkNextPosition;
               if((i + 1) >= positiona.get_count())
                  tkNextPosition = tkPosition + 96 * 2;
               else
                  tkNextPosition = positiona[i+1];
               int32_t iTry = 0;
               while(true)
               {
                  if(j >= pEvents->m_tkaEventsPosition.get_count())
                     break;
                  imedia::position tkEvent = pEvents->m_tkaEventsPosition.element_at(j);
                  double dDiff = (double) (tkEvent - tkPosition);
                  // to favor a causal relation
                  if(dDiff < 0.0)
                  {
                     dCorrel += dDiff * 5.0 - 177.0;
                  }
                  // to favor near notes
                  else if(dDiff > 48.0)
                  {
                     if(iTry > 0)
                     {
                        break;
                     }
                     else
                     {
                        dCorrel -= dDiff * 5.0 + 177.0;
                     }
                  }
                  else if(tkNextPosition - tkEvent < 12)
                  {
                     break;
                  }
                  else
                  {
                     if(dDiff < 12.0 && iTry == 0)
                     {
                        dCorrel += 1984;
                     }
                     else
                     {
                        dCorrel += 1984 / (dDiff * 5.0 + (iTry * 77.0));
                     }
                  }
                  j++;
                  iTry++;
               }
               i++;
            }
            daCorrel.add(dCorrel);
         }


         int32_t iFound = -1;

         dCorrel = -1000.0 * 1000.0 * 100.0;

         for(int32_t i = 0; i < daCorrel.get_size(); i++)
         {
            if(daCorrel[i] > dCorrel)
            {
               dCorrel = daCorrel[i];
               iFound = i;
            }
         }

         return iFound;

      }

      bool tracks::IsTune1000File()
      {
         bool bIsTune1000File = false;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track * ptrack = (track *)TrackAt(i);
            ptrack->seek_begin();
            if(ptrack->IsType(track_base::TypeMidi) && (bIsTune1000File = ptrack->IsTune1000File()))
            {
               break;
            }
         }
         return bIsTune1000File;
      }

      bool tracks::IsSoftKaraokeFile(bool bWork, stringa * pstra)
      {
         if(GetTrackCount() < 2)
         {
            return false;
         }
         else
         {
            track * ptrack = (track *)TrackAt(1);
            ptrack->_SeekBegin(bWork);
            if(ptrack->IsType(track_base::TypeMidi)
               && ptrack->IsSoftKaraokeFile(bWork, pstra))
            {
               return true;
            }
            else
            {
               if(GetTrackCount() >= 3)
               {
                  track * ptrack = (track *)TrackAt(2);
                  if(!ptrack->IsType(track_base::TypeMidi))
                  {
                     return false;
                  }
                  else
                  {
                     ptrack->_SeekBegin(bWork);
                     stringa strArray;
                     imedia::position_array tkArray;
                     ptrack->seek_begin();
                     ptrack->ReadKarTokens(strArray, &tkArray);
                     if(strArray.get_size() > 0)
                        return true;
                     else
                        return false;
                  }
               }
               else
               {
                  return false;
               }
            }
         }
      }

      /*track * tracks::GetEventTrack(CMidiEventBase *pEvent)
      {
      int32_t i;
      for(i = 0; i < this->get_size(); i++)
      {
      track * ptrack = &TrackAt(i);
      if(ptrack->ContainsEvent(pEvent))
      return ptrack;
      }
      return NULL;
      } */

      track * tracks::seek(uint32_t dwSeekWhat)
      {
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track * ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->seek_begin();
            if(ptrack->seek(dwSeekWhat) == Success)
            {
               m_iCurrentTrack = i;
               return ptrack;
            }
         }
         return NULL;
      }

      bool tracks::IsXFFile()
      {
         if(GetTrackCount() <= 0)
            return false;
         else
         {
            for(int32_t i = 0; i < GetTrackCount(); i++)
            {
               track * ptrack = (track *)TrackAt(i);
               if(!ptrack->IsType(track_base::TypeMidi))
                  continue;
               if(ptrack->IsXFFile())
               {
                  return true;
               }
            }
            return false;
         }

      }



      void tracks::FromWorkStorage()
      {
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track * ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->FromWorkTrack(false);
         }

      }

      void tracks::ToWorkStorage()
      {
         for(int32_t i = 0; i < GetMidiTrackCount(); i++)
         {
            track * ptrack = MidiTrackAt(i);
            ptrack->ToWorkStorage();
         }
      }

      VMSRESULT tracks::CompactTracks(
         track & trackUnion,
         bool bOnlyMTrk,
         bool bOnlyMidiTrack)
      {
         //    track trackUnion;
         trackUnion.SetAutoAllocation();
         seek_begin();
         event_v1 * pevent;
         while(Success == GetNextEvent(pevent, 0x7fffffff, true, bOnlyMTrk, bOnlyMidiTrack))
         {
            if(pevent->GetFlags() != 0 ||
               (pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT))
            {
            }
            else
            {
               trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);
            }
         }
         pevent->SetFullType(Meta);
         pevent->SetMetaType(MetaEOT);
         pevent->SetParam(NULL, 0);
         trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);

         CHUNKHDR hdr;
         hdr.fourccType = FOURCC_MTrk;
         hdr.dwLength = trackUnion.m_smti.m_cbLength;
         trackUnion.WriteHeader(&hdr);


         return VMSR_SUCCESS;

      }

      VMSRESULT tracks::WorkCompactTracks(
         track & trackUnion,
         bool bOnlyMTrk)
      {
         //    track trackUnion;
         trackUnion.SetAutoAllocation();
         WorkSeekBegin();
         event_v5 * pevent;
         while(Success == WorkGetNextRawEvent(pevent, 0x7fffffff, true, bOnlyMTrk))
         {
            if(pevent->GetFlags() != 0 ||
               (pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT))
            {
            }
            else
            {
               if(pevent->_GetType() == NoteOn)
               {
                  if(pevent->_GetTrack() != 3)
                  {
                     //                    int32_t i = 1 + 2;
                  }
               }

               trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);
            }
         }
         pevent->SetFullType(Meta);
         pevent->SetMetaType(MetaEOT);
         pevent->SetParam(NULL, 0);
         trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);

         CHUNKHDR hdr;
         hdr.fourccType = FOURCC_MTrk;
         hdr.dwLength = trackUnion.m_smti.m_cbLength;
         trackUnion.WriteHeader(&hdr);


         return VMSR_SUCCESS;

      }

      VMSRESULT tracks::WorkCompactMidiTracks(
         track & trackUnion,
         bool bOnlyMTrk)
      {
         //    track trackUnion;
         trackUnion.SetAutoAllocation();
         WorkSeekBegin();
         event_v5 * pevent;
         while(Success == WorkGetNextRawMidiEvent(pevent, 0x7fffffff, true, bOnlyMTrk))
         {
            if(pevent->GetFlags() != 0 ||
               (pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT))
            {
            }
            else
            {
               if(pevent->_GetType() == NoteOn)
               {
                  if(pevent->_GetTrack() != 3)
                  {
                     //                    int32_t i = 1 + 2;
                  }
               }

               trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);
            }
         }
         pevent->SetFullType(Meta);
         pevent->SetMetaType(MetaEOT);
         pevent->SetParam(NULL, 0);
         trackUnion.WriteCompleteEvent(*pevent, 0x7fffffff, false);

         CHUNKHDR hdr;
         hdr.fourccType = FOURCC_MTrk;
         hdr.dwLength = trackUnion.m_smti.m_cbLength;
         trackUnion.WriteHeader(&hdr);


         return VMSR_SUCCESS;

      }

      VMSRESULT tracks::WorkCompactTracksWork(
         track & trackUnion,
         bool bOnlyMTrk)
      {
         trackUnion.GetWorkTrack().remove_all();
         WorkSeekBegin();
         event_v5 * pevent;
         imedia::position tkLastPosition = 0;
         imedia::position tkPosition = 0;
         while(WorkGetNextRawEvent(
            pevent,
            0x7fffffff,
            true,
            bOnlyMTrk)
            == Success)
         {
            tkPosition = get_position();
            if(pevent->GetFlags() != 0 ||
               (pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT))
            {
            }
            else
            {
               trackUnion.GetWorkTrack().add(
                  (imedia::position) (tkPosition - tkLastPosition),
                  *pevent);
            }
            tkLastPosition = tkPosition;
         }
         pevent->SetFullType(Meta);
         pevent->SetMetaType(MetaEOT);
         pevent->SetParam(NULL, 0);
         trackUnion.GetWorkTrack().add(*pevent);

         return VMSR_SUCCESS;

      }

      VMSRESULT tracks::WorkCompactMidiTracksWork(
         track & trackUnion,
         bool bOnlyMTrk)
      {
         trackUnion.GetWorkTrack().remove_all();
         WorkSeekBegin();
         event_v5 * pevent = NULL;
         imedia::position tkLastPosition = 0;
         imedia::position tkPosition = 0;

         while(
            WorkGetNextRawMidiEvent(
            pevent,
            0x7fffffff,
            true,
            bOnlyMTrk)
            == Success)
         {
            tkPosition = get_position();
            if(pevent->GetFlags() != 0 ||
               (pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT))
            {
            }
            else
            {
               trackUnion.GetWorkTrack().add(
                  (imedia::position) (tkPosition - tkLastPosition),
                  *pevent);
            }
            tkLastPosition = tkPosition;
         }
         event_v5 event;
         event.SetDelta(0);
         event.SetFullType(Meta);
         event.SetMetaType(MetaEOT);
         event.SetParam(NULL, 0);
         trackUnion.GetWorkTrack().add(event);

         return VMSR_SUCCESS;

      }

      // put in Format 1?ｿ
      VMSRESULT tracks::ExpandTrack(index iTrackIndex)
      {
         //    track trackUnion;
         track * ptrackParam = (track *) TrackAt(iTrackIndex);
         ASSERT(ptrackParam->IsType(track_base::TypeMidi));
         remove_at(iTrackIndex);
         ptrackParam->seek_begin();
         map<int32_t, int32_t, track *, track *> mapTracks;
         track * ptrack;
         ptrackParam->ToWorkStorage();
         ptrackParam->WorkSeekBegin();
         event_v5 * pevent;
         while(true)
         {
            ptrackParam->WorkGetEvent(
               pevent,
               0x7fffffff,
               true);
            if(pevent->GetFullType() == Meta &&
               pevent->GetMetaType() == MetaEOT)
            {
            }
            else if(pevent->GetFullType() < SysEx)
            {
               if(!mapTracks.Lookup(pevent->_GetTrack(), ptrack))
               {
                  ptrack = CreateTrack();
                  ptrack->SetAutoAllocation();
                  mapTracks.set_at(pevent->_GetTrack(), ptrack);
               }
               ptrack->GetWorkTrack().add(*pevent);
            }
            else
            {
               if(!mapTracks.Lookup(0, ptrack))
               {
                  ptrack = CreateTrack();
                  ptrack->SetAutoAllocation();
                  mapTracks.set_at(0, ptrack);
               }
               ptrack->GetWorkTrack().add(*pevent);
            }
            if(Success != ptrackParam->WorkMoveNext())
               break;
         }
         for(int32_t i = 0; i < 15; i++)
         {
            if(mapTracks.Lookup(i, ptrack))
            {
               ptrack->GetEvent().SetFullType(Meta);
               ptrack->GetEvent().SetMetaType(MetaEOT);
               ptrack->GetEvent().SetParam(NULL, 0);
               ptrack->GetEvent().SetDelta(0);
               ptrack->WorkWriteEvent();


               CHUNKHDR hdr;
               hdr.fourccType = FOURCC_MTrk;
               hdr.dwLength = ptrack->m_smti.m_cbLength;
               ptrack->WriteHeader(&hdr);
               ptrack->Initialize(this);
               ptrack->FromWorkTrack(false);
            }
         }
         return VMSR_SUCCESS;
      }

      index tracks::GetMidiTrackIndex(index iIndex)
      {
         if(TrackAt(iIndex)->get_type() == track_base::TypeMidi)
            return ((track*)TrackAt(iIndex))->GetMidiTrackIndex();
         else
            return -1;
      }

      VMSRESULT tracks::SetLyricDelay(int32_t iDelay)
      {
         m_iLyricDelay = iDelay;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            lyric_track * pltrack = dynamic_cast<lyric_track *>(TrackAt(i));
            if(pltrack != NULL)
            {
               pltrack->SetDelay(iDelay);
            }
         }
         return VMSR_SUCCESS;
      }

      VMSRESULT tracks::GetLyricDelay(int32_t *lpiDelay)
      {
         *lpiDelay = m_iLyricDelay;
         return VMSR_SUCCESS;
      }

      void tracks::OnStop()
      {
         int32_t i;
         m_tkLevelLastUpdateInterval = 0;

         for(i = 0; i < m_iaLevel.get_size(); i++)
         {
            m_iaLevel.element_at(i) = 0;
         }

         for(i = 0; i < m_iaNoteOn.get_size(); i++)
         {
            m_iaNoteOn.element_at(i) = 0;
         }

      }

      e_file_result tracks::GetVolumeEvent(sp(::ca2::application) papp, event_v5 *& pevent)
      {
         pevent = &m_midieventVolume;
         pevent->clear();
         pevent->SetFullType(Meta);
         pevent->SetFlags(1);
         pevent->SetDelta(0);
         pevent->SetPosition(get_position());
         ::ca2::byte_stream_memory_file memFile(papp);
         memFile << EVENT_ID_NOTE_ON;
         for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
         {
            memFile << (BYTE) m_iaLevel.element_at(i);
         }
         memFile.flush();
         file_size dwLength = memFile.get_length();
         LPBYTE lpData = dynamic_cast < ::primitive::memory * > (memFile.get_memory())->detach();
         pevent->SetParam(lpData, (int32_t) dwLength);

         m_bVolumeEventSent = true;

         return Success;
      }

      e_file_result tracks::GetVolumeEvent(sp(::ca2::application) papp, event_v1 *& pevent)
      {
         pevent = &m_midieventV001Volume;
         pevent->clear();
         pevent->SetFullType(Meta);
         pevent->SetFlags(1);
         pevent->SetDelta(0);
         pevent->SetPosition(get_position());
         ::ca2::byte_stream_memory_file memFile(papp);
         memFile << EVENT_ID_NOTE_ON;
         for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
         {
            memFile << (BYTE) m_iaLevel.element_at(i);
         }
         memFile.flush();
         file_size dwLength = memFile.get_length();
         LPBYTE lpData = memFile.detach_primitive_storage();
         pevent->SetParam(lpData, (int32_t) dwLength);

         m_bVolumeEventSent = true;

         return Success;
      }


      bool tracks::WorkGetLongestXFLyrics(string &wstrParam, bool bPlain)
      {
         string2a wstr2a;
         imedia::position_2darray position2a;
         int2a i2aTokenLine;
         if(WorkGetXFTokens((uint32_t) -1, wstr2a, position2a, i2aTokenLine, NULL, false) != Success)
            return false;

         ::index iMatch = -1;
         ::count iMaxSize = 0;
         ::count iSize;
         ::index i;
         for(i = 0; i <wstr2a.get_size(); i++)
         {
            iSize = wstr2a[i].get_size();
            if(iSize > iMaxSize)
            {
               iMaxSize = iSize;
               iMatch = i;
            }
         }
         if(iMatch < 0)
            return false;

         stringa stra = wstr2a.element_at(iMatch);

         int32_t            iTokens;
         int32_t            iChars;
         int32_t            iCharsFirst;
         WCHAR            wch;
         string         str;
         int32_t            iStr;
         int32_t            xPosition;
         bool            forceInsertion = FALSE;
         bool            bLineFirstToken;
         bool            bLineLastToken;


         iChars = -1;
         iCharsFirst = 0;
         iStr = 0;
         iTokens = -1;
         xPosition = 0;

         iSize = stra.get_size();
         strsize iLen;
         string wstrFinal;
         string wstrLine;
         bLineFirstToken = true;
         bLineLastToken = false;

         for(i = 0; i < iSize;)
         {
            str = stra.element_at(i);
            iLen = str.get_length();
            if(iLen <= 0)
            {
               i++;
               continue;
            }
            for(int32_t j = 0; j < iLen; j++)
            {
               wch = str[j];

               if(bLineFirstToken)
               {
                  if(wch == '\\')
                  {
                     forceInsertion = true;
                  }
                  else if(wch == '^')
                  {
                     if(!bPlain)
                     {
                        wstrLine += ' ';
                     }
                  }
                  else if(wch == '/')
                  {
                  }
                  else if(wch == '%')
                  {
                  }
                  else if(wch == '<')
                  {
                  }
                  else if(wch == '>')
                  {
                     if(!bPlain)
                     {
                        wstrLine += '\t';
                     }
                  }
                  else
                  {
                     wstrLine += wch;
                  }
               }
               else
               {
                  if(forceInsertion)
                  {
                     forceInsertion = false;
                     wstrLine += wch;
                  }
                  else if(wch == '\\')
                  {
                     forceInsertion = true;
                  }
                  else if(wch == '^')
                  {
                     if(!bPlain)
                     {
                        wstrLine += ' ';
                     }
                  }
                  else if(wch == '/')
                  {
                     bLineLastToken = true;
                     break;
                  }
                  else if(wch == '%')
                  {
                     bLineLastToken = true;
                     break;
                  }
                  else if(wch == '<')
                  {
                     if(j == 1)
                     {
                        bLineLastToken = true;
                        break;
                     }
                  }
                  else if(wch == '>')
                  {
                     if(!bPlain)
                     {
                        wstrLine += '\t';
                     }
                  }
                  else if(wch == '\n')
                  {
                     bLineLastToken = true;
                     break;
                  }
                  else if(wch == '\r')
                  {
                     bLineLastToken = true;
                     break;
                  }
                  else
                  {
                     wstrLine += wch;
                  }
               }
            }
            if(bLineLastToken)
            {
               wstrFinal += wstrLine;
               wstrLine.Empty();
               if(!bPlain)
               {
                  wstrFinal += "\r\n";
               }
               bLineFirstToken = true;
               bLineLastToken = false;
            }
            else
            {
               bLineFirstToken = false;
               i++;
            }
         }
         wstrFinal += wstrLine;
         if(!bPlain)
         {
            wstrFinal += "\r\n";
         }

         wstrParam = wstrFinal;
         return true;
      }

      bool tracks::WorkGetLongestSoftKaraokeLyrics(string &wstrParam)
      {
         string2a wstr2a;
         imedia::position_2darray tick2a;
         ::music::xf::lyrics_id_array xflh2a;
         if(WorkGetSoftKaraokeTokens(
            wstr2a,
            &tick2a,
            &xflh2a) != Success)
            return false;

         ::index iMatch = -1;
         ::count iMaxSize = 0;
         ::count iSize;
         ::index i;
         for(i = 0; i <wstr2a.get_size(); i++)
         {
            iSize = wstr2a[i].get_size();
            if(iSize > iMaxSize)
            {
               iMaxSize = iSize;
               iMatch = i;
            }
         }
         if(iMatch < 0)
            return false;

         stringa stra = wstr2a.element_at(iMatch);

         int32_t               iTokens;
         int32_t               iChars;
         int32_t               iCharsFirst;
         WCHAR            wch;
         string            str;
         int32_t               iStr;
         int32_t               xPosition;


         iChars = -1;
         iCharsFirst = 0;
         iStr = 0;
         iTokens = -1;
         xPosition = 0;

         iSize = stra.get_size();
         strsize iLen;
         string wstrFinal;

         for(i = 0; i < iSize; i++)
         {
            str = stra.element_at(i);
            iLen = str.get_length();
            for(int32_t j = 0; j < iLen; j++)
            {
               wch = str[j];

               if(wch == '\\')
               {
                  wstrFinal += "\r\n";
               }
               else if(wch == L'/')
               {
                  wstrFinal += "\r\n";
                  wstrFinal += "\r\n";
               }
               else
               {
                  wstrFinal += wch;
               }
            }
         }
         wstrFinal += "\r\n";

         wstrParam = wstrFinal;
         return true;

      }



      lyric_track * tracks::CreateLyricTrack()
      {
         return m_tracka.CreateLyricTrack();
      }

      e_file_result tracks::WorkGetNextEvent(
         event_v5 *&         pevent,
         imedia::position                   tkMax,
         bool                     bTkMaxInclusive,
         bool                    bOnlyMTrk)
      {
         track_base *          ptrack;
         track_base *          ptrkFound;
         int32_t                        idxTrackFound;
         int32_t                        idxTrack;
         imedia::position           tkEventPosition;
         imedia::position           tkEventFound;
         imedia::time               iRelTime;
         imedia::time               iMinRelTime;

         iMinRelTime = 0x7fffffff;
         ptrkFound = NULL;
         bool bNextEventLookUp;

         for(idxTrack = 0; idxTrack < GetActiveTrackCount(); idxTrack++)
         {
            ptrack = ActiveTrackAt(idxTrack);
            if(ptrack->WorkIsEOT())
            {
               DeactivateTrack(ptrack);
               idxTrack--;
               continue;
            }

            if(bOnlyMTrk)
            {
               if(ptrack->IsType(track_base::TypeMidi))
               {
                  track * pmiditrack = (track *) ptrack;
                  CHUNKHDR FAR * pCh = (CHUNKHDR FAR *) pmiditrack->GetTrackImage();
                  if (pCh->fourccType != FOURCC_MTrk)
                     continue;
               }
               else
               {
                  continue;
               }
            }

            bNextEventLookUp = true;
            while(true)
            {
               tkEventPosition = ptrack->WorkGetPosition();
               iRelTime = tkEventPosition - m_tkPosition;
               ASSERT(iRelTime >= 0);
               if(iRelTime < 0)
               {
                  if(ptrack->WorkMoveNext() != VMSR_SUCCESS)
                  {
                     if(ptrack->WorkIsEOT())
                     {
                        DeactivateTrack(ptrack);
                     }
                     bNextEventLookUp = false;
                     break;
                  }
               }
               else
               {
                  break;
               }
            }
            if(!bNextEventLookUp)
            {
               continue;
            }
            ASSERT(iRelTime >= 0);
            if (iRelTime < iMinRelTime)
            {
               iMinRelTime    = iRelTime;
               ptrkFound      = ptrack;
               idxTrackFound  = idxTrack;
               tkEventFound   = tkEventPosition;
               if(iRelTime == 0)
                  break;
            }
         }

         if (ptrkFound == NULL)
         {
            return SEndOfFile;
         }

         ptrack = ptrkFound;



         imedia::position tkPosition = tkEventFound;

         iRelTime = tkPosition - m_tkPosition;
         ASSERT(iRelTime >= 0);
         if(iRelTime < 0)
            iRelTime = 0;

         // XXX
         if(!GetFlags().is_signalized(DisablePlayLevel1Operations))
         {
            if(iRelTime != 0)
            {
               m_tkLevelLastUpdateInterval += iRelTime;
               if(m_tkLevelLastUpdateInterval > 24)
               {
                  m_tkLevelLastUpdateInterval = 0;
                  if(!m_bVolumeEventSent)
                  {
                     return GetVolumeEvent(get_app(), pevent);
                  }
                  else
                  {
                     m_bVolumeEventSent = false;
                  }
               }
            }
            else
            {
               m_bVolumeEventSent = false;
            }
         }


         if(ptrack->get_type() == track_base::TypeLyric)
         {
            ASSERT(true);
         }

         if(bTkMaxInclusive)
         {
            if (m_tkPosition + iMinRelTime > tkMax)
            {
               return SReachedTkMax;
            }
         }
         else
         {
            if (m_tkPosition + iMinRelTime >= tkMax)
            {
               return SReachedTkMax;
            }
         }

         ptrack->WorkGetEvent(pevent, tkMax, bTkMaxInclusive);

         if(!GetFlags().is_signalized(DisablePlayLevel1Operations))
         {
            if(pevent->_GetType() == ProgramChange)
            {
               m_iaInstrument.element_at(pevent->_GetTrack())
                  = pevent->GetChB1();
            }
         }

         m_iCurrentTrack = idxTrackFound;
         m_tkPosition = ptrack->get_position();
         ptrack->WorkMoveNext();


         return Success;
      }

      e_file_result tracks::WorkGetNextRawEvent(
         event_v5 *&         pevent,
         imedia::position                   tkMax,
         bool                     bTkMaxInclusive,
         bool                    bOnlyMTrk)
      {
         track_base *         ptrack;
         track_base *         ptrkFound;
         int32_t                     idxTrackFound;
         int32_t                     idxTrack;
         imedia::position                   tkEventPosition;
         imedia::position                     tkEventFound;
         imedia::time               iRelTime;
         imedia::time               iMinRelTime;



         iMinRelTime = 0x7fffffff;
         ptrkFound = NULL;
         bool bNextEventLookUp;
         for (idxTrack = 0; idxTrack < GetActiveTrackCount(); idxTrack++)
         {
            ptrack = ActiveTrackAt(idxTrack);
            ASSERT(!ptrack->WorkIsEOT());
            if(bOnlyMTrk)
            {
               if(ptrack->IsType(track_base::TypeMidi))
               {
                  track * ptrackMidi = (track *) ptrack;
                  CHUNKHDR FAR * pCh = (CHUNKHDR FAR *) ptrackMidi->GetTrackImage();
                  if (pCh->fourccType != FOURCC_MTrk)
                     continue;
               }
            }
            bNextEventLookUp = true;
            while(true)
            {
               tkEventPosition = ptrack->WorkGetPosition();
               iRelTime = tkEventPosition - m_tkPosition;
               ASSERT(iRelTime >= 0);
               if(iRelTime < 0)
               {
                  if(ptrack->WorkMoveNext() != VMSR_SUCCESS)
                  {
                     if(ptrack->WorkIsEOT())
                     {
                        DeactivateTrack(ptrack);
                     }
                     bNextEventLookUp = false;
                     break;
                  }
               }
               else
               {
                  break;
               }
            }
            if(!bNextEventLookUp)
            {
               continue;
            }
            ASSERT(iRelTime >= 0);

            if (iRelTime < iMinRelTime)
            {
               iMinRelTime = iRelTime;
               ptrkFound = ptrack;
               idxTrackFound = idxTrack;
               tkEventFound = tkEventPosition;
            }
         }

         if (ptrkFound == NULL)
         {
            return SEndOfFile;
         }

         ptrack = ptrkFound;

         imedia::position tkPosition = tkEventFound;

         iRelTime = tkPosition - m_tkPosition;
         ASSERT(iRelTime >= 0);
         if(iRelTime < 0)
            iRelTime = 0;



         if(ptrack->get_type() == track_base::TypeLyric)
         {
            ASSERT(true);
         }

         if(bTkMaxInclusive)
         {
            if (m_tkPosition + iMinRelTime > tkMax)
            {
               return SReachedTkMax;
            }
         }
         else
         {
            if (m_tkPosition + iMinRelTime >= tkMax)
            {
               return SReachedTkMax;
            }
         }

         ptrack->WorkGetEvent(pevent, tkMax, bTkMaxInclusive);
         m_iCurrentTrack = idxTrackFound;
         m_tkPosition = ptrack->get_position();
         ptrack->WorkMoveNext();

         if(ptrack->WorkIsEOT())
         {
            DeactivateTrack(ptrack);
         }
         return Success;
      }

      e_file_result tracks::WorkGetNextRawMidiEvent(
         event_v5 *&         pevent,
         imedia::position                   tkMax,
         bool                     bTkMaxInclusive,
         bool                    bOnlyMTrk)
      {
         track *         ptrack;
         track *         ptrkFound;
         int32_t                     idxTrackFound;
         int32_t                     idxTrack;
         imedia::position                   tkEventPosition;
         imedia::position                     tkEventFound;
         imedia::time                     iRelTime;
         imedia::time                     iMinRelTime;

         iMinRelTime = 0x7fffffff;
         ptrkFound = NULL;
         bool bNextEventLookUp;
         for (idxTrack = 0; idxTrack < GetActiveMidiTrackCount(); idxTrack++)
         {
            ptrack = ActiveMidiTrackAt(idxTrack);
            if(ptrack->_WorkIsEOT())
            {
               DeactivateTrack(ptrack);
               continue;
            }
            if(bOnlyMTrk)
            {
               track * ptrackMidi = (track *) ptrack;
               CHUNKHDR FAR * pCh = (CHUNKHDR FAR *) ptrackMidi->GetTrackImage();
               if (pCh->fourccType != FOURCC_MTrk)
                  continue;
            }
            bNextEventLookUp = true;
            while(true)
            {
               tkEventPosition = ptrack->_WorkGetPosition();
               iRelTime = tkEventPosition - m_tkPosition;
               if(iRelTime < 0)
               {
                  if(ptrack->_WorkMoveNext() != VMSR_SUCCESS)
                  {
                     if(ptrack->_WorkIsEOT())
                     {
                        DeactivateTrack(ptrack);
                     }
                     bNextEventLookUp = false;
                     break;
                  }
               }
               else
               {
                  break;
               }
            }
            if(!bNextEventLookUp)
            {
               continue;
            }
            ASSERT(iRelTime >= 0);

            if(iRelTime < iMinRelTime)
            {
               iMinRelTime = iRelTime;
               ptrkFound = ptrack;
               idxTrackFound = idxTrack;
               tkEventFound = tkEventPosition;
               if(iRelTime == 0)
                  break;
            }
         }

         if (ptrkFound == NULL)
         {
            return SEndOfFile;
         }

         ptrack = ptrkFound;

         imedia::position tkPosition = tkEventFound;

         iRelTime = tkPosition - m_tkPosition;
         ASSERT(iRelTime >= 0);
         if(iRelTime < 0)
            iRelTime = 0;



         if(ptrack->get_type() == track_base::TypeLyric)
         {
            ASSERT(true);
         }

         if(bTkMaxInclusive)
         {
            if (m_tkPosition + iMinRelTime > tkMax)
            {
               return SReachedTkMax;
            }
         }
         else
         {
            if (m_tkPosition + iMinRelTime >= tkMax)
            {
               return SReachedTkMax;
            }
         }

         ptrack->_WorkGetEvent(pevent, tkMax, bTkMaxInclusive);

         m_iCurrentTrack = idxTrackFound;
         m_tkPosition = ptrack->get_position();
         ptrack->_WorkMoveNext();

         if(ptrack->_WorkIsEOT())
         {
            DeactivateTrack(ptrack);
         }
         return Success;
      }

      void tracks::WorkSeekBegin()
      {
         m_trackaActive = m_tracka;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track_base * ptrack = (track *) TrackAt(i);
            ptrack->WorkSeekBegin();
         }
         m_tkPosition = 0;
         GetFlags().unsignalize(EndOfTrack);
      }

      void tracks::WorkSeekEnd()
      {
         imedia::position tkPosition = 0;
         for (int32_t i = 0; i < GetTrackCount(); i++)
         {
            track_base * ptrack = (track *) TrackAt(i);
            ptrack->WorkSeekEnd();
            if(ptrack->get_position() > tkPosition)
            {
               tkPosition = ptrack->get_position();
            }
         }
         m_tkPosition = tkPosition;
         GetFlags().signalize(EndOfTrack);
      }


      e_file_result tracks::WorkGetXFTokens(uint32_t uiCodePage, string2a & token2a, imedia::position_2darray *p2DTicks, ::music::xf::lyrics_id_array * pxflh2a)
      {
         ASSERT(p2DTicks != NULL);
         ASSERT(pxflh2a != NULL);
         stringa  tokena;
         imedia::position_array * lptkaTicks = NULL;
         ::music::xf::lyrics_id xflh;
         ASSERT(FALSE);

         imedia::position_array positiona;

         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track *ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->WorkSeekBegin();
            e_file_result smfr;
            smfr = ptrack->WorkSeekXFLyricsHeader();
            if(smfr == Success)
            {
               smfr = ptrack->WorkReadXFLyricsHeader(&xflh);
            }

            tokena.remove_all();

            lptkaTicks = new imedia::position_array();
            ptrack->WorkSeekBegin();
            smfr = ptrack->WorkReadXFTokens(uiCodePage, tokena, lptkaTicks);

            if(smfr == Success
               || smfr == SEndOfTrack)
            {
               token2a.add(canew(stringa(tokena)));
               p2DTicks->add(lptkaTicks);
               pxflh2a->add(canew(::music::xf::lyrics_id(xflh)));
            }
            else
            {
               delete lptkaTicks;
            }
            lptkaTicks = NULL;
         }
         return Success;
      }


      /*e_file_result tracks::WorkGetXFTokens(
      string2a &
      token2a,
      imedia::position_2darray *p2DTicks,
      ::music::xf::lyrics_id_array * pxflh2a)
      {
      ASSERT(p2DTicks != NULL);
      ASSERT(pxflh2a != NULL);
      stringa  tokena;
      imedia::position_array * lptkaTicks = NULL;
      ::music::xf::lyrics_id xflh;
      for(int32_t i = 0; i < GetTrackCount(); i++)
      {
      track *ptrack = (track *)TrackAt(i);
      if(!ptrack->IsType(track_base::TypeMidi))
      continue;
      ptrack->WorkSeekBegin();
      e_file_result smfr;
      smfr = ptrack->WorkSeekXFLyricsHeader();
      if(smfr == Success)
      {
      smfr = ptrack->WorkReadXFLyricsHeader(&xflh);
      }


      tokena.remove_all();
      lptkaTicks = new imedia::position_array();
      ptrack->WorkSeekBegin();
      smfr = ptrack->WorkReadXFTokens(tokena, lptkaTicks);
      if(Success == smfr)
      {
      token2a.add(tokena);
      p2DTicks->add(lptkaTicks);
      pxflh2a->add(xflh);
      }
      else
      {
      delete lptkaTicks;
      }
      lptkaTicks = NULL;
      }
      return Success;
      }*/



      e_file_result tracks::WorkGetXFTokens(uint32_t uiCodePage, string2a & token2a, imedia::position_2darray & position2a, int2a & i2aTokenLine, ::music::xf::lyrics_id_array * pxflh2a, bool bPlain)
      {
         stringa  tokena;
         int_array iaTokenLine;
         imedia::position_array positiona;
         ::music::xf::lyrics_id xflh;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track *ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->WorkSeekBegin();
            e_file_result smfr;
            if(pxflh2a != NULL)
            {

               smfr = ptrack->WorkSeekXFLyricsHeader();
               if(smfr == Success)
               {
                  smfr = ptrack->WorkReadXFLyricsHeader(&xflh);
               }
            }


            tokena.remove_all();
            positiona.remove_all();
            iaTokenLine.remove_all();
            ptrack->WorkSeekBegin();
            smfr = ptrack->WorkReadXFTokens(uiCodePage, tokena, positiona, iaTokenLine, bPlain);
            if(Success == smfr)
            {
               token2a.add(canew(stringa(tokena)));
               position2a.add(canew(::imedia::position_array(positiona)));
               i2aTokenLine.add(canew(int_array(iaTokenLine)));
               if(pxflh2a != NULL)
               {
                  pxflh2a->add(canew(::music::xf::lyrics_id(xflh)));
               }
            }
         }
         return Success;
      }


      e_file_result tracks::WorkGetEmptyXFTokens(int32_t iTrack, string2a & token2a, imedia::position_2darray & position2a, ::music::xf::lyrics_id_array * pxflh2a)
      {
         UNREFERENCED_PARAMETER(pxflh2a);
         stringa  tokena;
         imedia::position_array positiona;
         ::music::xf::lyrics_id xflh;
         e_file_result smfrc;
         string str;
         WorkSeekBegin();
         event_v5 * pevent = NULL;
         int32_t i = 0;
         while(true)
         {
            smfrc   = WorkGetNextEvent(pevent, 0x0fffffff, true, false);
            if(smfrc != Success)
            {
               break;
            }
            if(pevent->GetTrack() == iTrack
               && NoteOn == pevent->GetType_()
               && pevent->GetNoteVelocity() != 0)
            {
               if((i % 3) == 0)
               {
                  str = "<---";
               }
               else
               {
                  str = "---";
               }

               TRACE("%s\n", str);
               tokena.add(str);
               positiona.add(imedia::position(m_tkPosition));
               i++;
            }
         }
         token2a.add(canew(stringa(tokena)));
         position2a.add(canew(::imedia::position_array(positiona)));

         return smfrc;
      }

      e_file_result tracks::WorkGetSoftKaraokeTokens(string2a & token2a, imedia::position_2darray * p2DTicks, ::music::xf::lyrics_id_array * pxflh2a)
      {
         ASSERT(p2DTicks != NULL);
         ASSERT(pxflh2a != NULL);
         stringa  tokena;
         imedia::position_array * lptkaTicks = NULL;
         ::music::xf::lyrics_id xflh;
         stringa stra;
         for(int32_t i = 0; i < GetTrackCount(); i++)
         {
            track *ptrack = (track *)TrackAt(i);
            if(!ptrack->IsType(track_base::TypeMidi))
               continue;
            ptrack->WorkSeekBegin();
            tokena.remove_all();
            lptkaTicks = new imedia::position_array();
            if(Success == ptrack->WorkReadKarTokens(tokena, lptkaTicks, stra))
            {
               token2a.add(canew(stringa(tokena)));
               p2DTicks->add(canew(::imedia::position_array(*lptkaTicks)));
               pxflh2a->add(canew(::music::xf::lyrics_id(xflh)));
            }
            else
            {
               delete lptkaTicks;
            }

            ptrack->WorkSeekBegin();
            tokena.remove_all();
            lptkaTicks = new imedia::position_array();
            if(Success == ptrack->WorkReadTune1000Tokens(
               tokena, lptkaTicks))
            {
               token2a.add(canew(stringa(tokena)));
               p2DTicks->add(lptkaTicks);
               pxflh2a->add(canew(::music::xf::lyrics_id(xflh)));
            }
            else
            {
               delete lptkaTicks;
            }
            lptkaTicks = NULL;
         }
         return Success;
      }


      e_file_result tracks::WorkQuantizeVR()
      {
         for(int32_t i = 0; i < m_tracka.GetTrackCount(); i++)
         {

         }
         return Success;
      }

      track * tracks::CreateTrack(index iIndex, FOURCC fourcc)
      {
         return m_tracka.CreateTrack(iIndex, fourcc);
      }

      void tracks::remove_all()
      {
         m_tracka.remove_all();
         m_trackaActive.remove_all();
      }

      track * tracks::CreateTrack(FOURCC fourcc)
      {
         return m_tracka.CreateTrack(fourcc);
      }




      // remove all track tracks

      void tracks::RemoveAllMidiTracks()
      {
         m_tracka.RemoveAllMidiTracks();
      }

      void tracks::DeactivateTrack(track_base * ptrack)
      {
         m_trackaActive.RemoveTrack(ptrack);
      }
      void tracks::clear()
      {
         remove_all();
         m_tkPosition         = 0;
         m_iLyricDelay        = 0;
         m_bVolumeEventSent   = false;

      }


      void tracks::remove_at(index iIndex)
      {
         m_tracka.remove_at(iIndex);
      }





      void tracks::SetAutoAllocation(bool bSet)
      {
         for (int32_t i = 0; i < GetTrackCount(); i++)
         {
            track_base * ptrack = TrackAt(i);
            if(ptrack->IsType(track_base::TypeMidi))
            {
               track * pmiditrack = (track *) ptrack;
               pmiditrack->SetAutoAllocation(bSet);
            }
         }

      }



      file_flags & tracks::GetFlags()
      {
         return m_flagsFile;
      }



      track * tracks::TrackPtrAllocArray::CreateTrack(FOURCC fourcc)
      {
         return CreateTrack(GetMidiTrackCount(), fourcc);
      }

      track * tracks::TrackPtrAllocArray::CreateTrack(index iIndex, FOURCC fourcc)
      {
         if(iIndex >= m_miditracka.get_size())
         {
            while(iIndex >= m_miditracka.get_size())
            {
               m_miditracka.add(new track(get_app()));
               m_tracka.add(m_miditracka.last_ptr());
            }
         }
         track * ptrack = m_miditracka(iIndex);
         ptrack->WorkClear();
         ptrack->Initialize(m_ptracks);
         ptrack->GetWorkTrack().m_chunkHeader.fourccType = fourcc;
         ptrack->WorkSeekBegin();
         ptrack->seek_begin();
         return ptrack;
      }

      lyric_track * tracks::TrackPtrAllocArray::CreateLyricTrack()
      {
         return CreateLyricTrack(GetLyricTrackCount());
      }

      lyric_track * tracks::TrackPtrAllocArray::CreateLyricTrack(index iIndex)
      {
         lyric_track * ptrack;
         if(iIndex >= m_lyrictracka.get_size())
         {
            while(iIndex >= m_lyrictracka.get_size())
            {
               ptrack = new lyric_track(get_app());
               m_lyrictracka.add(ptrack);
               m_tracka.add(ptrack);
            }
         }
         ptrack = m_lyrictracka(iIndex);
         ptrack->m_pfile = m_ptracks->m_pFile;
         return ptrack;
      }

      void tracks::TrackPtrAllocArray::RemoveTrack(track_base * ptrack)
      {
         m_miditracka      -= ptrack;
         m_lyrictracka     -= ptrack;
         m_tracka          -= ptrack;
      }

      void tracks::TrackPtrArray::RemoveTrack(track_base * ptrack)
      {
         m_miditracka      -= ptrack;
         m_lyrictracka     -= ptrack;
         m_tracka          -= ptrack;
      }

      void tracks::TrackPtrAllocArray::copy(TrackPtrAllocArray & array)
      {
         m_miditracka         = array.m_miditracka;
         m_lyrictracka        = array.m_lyrictracka;
         m_tracka             = array.m_tracka;
      }

      void tracks::TrackPtrArray::copy(TrackPtrAllocArray & array)
      {
         m_miditracka         = array.m_miditracka;
         m_lyrictracka        = array.m_lyrictracka;
         m_tracka             = array.m_tracka;
      }

      tracks::TrackPtrArray & tracks::TrackPtrArray::operator = (TrackPtrAllocArray & array)
      {
         copy(array);
         return *this;
      }

      void tracks::TrackPtrAllocArray::remove_at(index iIndex)
      {
         track_base * ptrack  = m_tracka(iIndex);
         m_miditracka            -= ptrack;
         m_lyrictracka           -= ptrack;
         m_tracka                -= ptrack;
      }

      void tracks::TrackPtrAllocArray::remove_all()
      {
         m_tracka.remove_all();
         m_miditracka.remove_all();
         m_lyrictracka.remove_all();
      }

      void tracks::TrackPtrArray::remove_all()
      {
         m_tracka.remove_all();
         m_miditracka.remove_all();
         m_lyrictracka.remove_all();
      }

      void tracks::TrackPtrAllocArray::RemoveAllMidiTracks()
      {
         while(m_miditracka.get_size() > 0)
         {
            track * ptrack = m_miditracka(0);
            m_tracka -= ptrack;
            m_miditracka.remove_at(0);
         }
      }

      tracks::TrackPtrAllocArray::TrackPtrAllocArray(sp(::ca2::application) papp)
         : ca2(papp)
      {
         m_ptracks = NULL;
      }

      void tracks::TrackPtrAllocArray::Initialize(tracks *ptracks)
      {
         m_ptracks = ptracks;
      }

      tracks::TrackPtrArray::TrackPtrArray()
      {
         m_ptracks = NULL;
      }

      void tracks::TrackPtrArray::Initialize(tracks *ptracks)
      {
         m_ptracks = ptracks;
      }

      imedia::position tracks::CalcTkLength()
      {
         imedia::position tk = 0;
         for(int32_t i = 0; i < GetMidiTrackCount(); i++)
         {
            if(MidiTrackAt(i)->m_smti.m_tkLength > (int32_t) tk)
            {
               tk = MidiTrackAt(i)->m_smti.m_tkLength;
            }
         }
         return tk;
      }



      track_base * tracks::TrackAt(index iIndex)
      {
         return m_tracka.TrackAt(iIndex);
      }

      // tracks::TrackPtrAllocArray
      track_base * tracks::TrackPtrAllocArray::TrackAt(index iIndex)
      {
         return m_tracka(iIndex);
      }

      ::count tracks::TrackPtrAllocArray::GetTrackCount() const
      {
         return m_tracka.get_size();
      }

      ::count tracks::TrackPtrAllocArray::GetMidiTrackCount() const
      {
         return m_miditracka.get_size();
      }

      ::count tracks::TrackPtrAllocArray::GetLyricTrackCount()
      {
         return m_lyrictracka.get_size();
      }

      track * tracks::TrackPtrAllocArray::MidiTrackAt(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < GetMidiTrackCount());
         return m_miditracka(iIndex);
      }

      track * tracks::TrackPtrAllocArray::MidiTrackAt(index iIndex) const
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < GetMidiTrackCount());
         return const_cast < track *> (m_miditracka(iIndex).m_p);
      }

      lyric_track * tracks::TrackPtrAllocArray::LyricTrackAt(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < GetLyricTrackCount());
         return m_lyrictracka(iIndex);

      }

      // tracks::TrackPtrArray
      track_base * tracks::TrackPtrArray::TrackAt(index iIndex)
      {
         return m_tracka.element_at(iIndex);
      }

      ::count tracks::TrackPtrArray::GetTrackCount() const
      {
         return m_tracka.get_size();
      }

      ::count tracks::TrackPtrArray::GetMidiTrackCount() const
      {
         return m_miditracka.get_size();
      }

      ::count tracks::TrackPtrArray::GetLyricTrackCount()
      {
         return m_lyrictracka.get_size();
      }

      track * tracks::TrackPtrArray::MidiTrackAt(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < GetMidiTrackCount());
         return m_miditracka(iIndex);
      }

      lyric_track * tracks::TrackPtrArray::LyricTrackAt(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < GetLyricTrackCount());
         return m_lyrictracka(iIndex);

      }

      ::count tracks::GetTrackCount()
      {
         return m_tracka.GetTrackCount();
      }


      track * tracks::MidiTrackAt(index iIndex)
      {
         return m_tracka.MidiTrackAt(iIndex);
      }

      track * tracks::MidiTrackAt(index iIndex) const
      {
         return m_tracka.MidiTrackAt(iIndex);
      }

      track * tracks::ActiveMidiTrackAt(index iIndex)
      {
         return m_trackaActive.MidiTrackAt(iIndex);
      }

      track_base * tracks::ActiveTrackAt(index iIndex)
      {
         return m_trackaActive.TrackAt(iIndex);
      }

      ::count tracks::GetMidiTrackCount() const
      {
         return m_tracka.GetMidiTrackCount();
      }

      ::count tracks::GetActiveTrackCount() const
      {
         return m_trackaActive.GetTrackCount();
      }

      ::count tracks::GetActiveMidiTrackCount() const
      {
         return m_trackaActive.GetMidiTrackCount();
      }

      imedia::position tracks::get_position()
      {
         return m_tkPosition;
      }


   } // namespace midi_mmsystem


} // namespace music









