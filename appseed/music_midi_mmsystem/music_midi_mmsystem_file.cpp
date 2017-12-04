#include "framework.h"
#include <math.h>

namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         buffer::buffer(::aura::application * papp) :
            ::object(papp),
            primitive::memory_container (papp),
            ::music::midi::file::buffer(papp)
         {


            m_estorage              = ::music::storage_none;

            m_tkLength              = 0;
            m_dwFormat              = 0;
            m_dwTracks              = 0;
            m_dwTimeDivision        = 0;

            m_dwPendingUserEvent    = 0;
            m_cbPendingUserEvent    = 0;
            m_hpbPendingUserEvent   = 0;

            m_tempomap.set_size(0, C_TEMPO_MAP_CHK);

            m_iKeyShift             = 0;
            m_dTempoShift           = 0;


            m_pFileHeader = NULL;

            keep_pointer((void **) &m_pMThd);
            keep_pointer((void **) &m_pFileHeader);

         }

         buffer::~buffer()
         {

            CloseFile();

            if(m_estorage == ::music::storage_attach)
            {
               detach();
            }

         }


//         e_result buffer::OpenFile(::music::midi::file::buffer &file, int32_t openMode)
//         {
//
//            e_result               smfrc = ::music::success;
//
//            m_ptracks->m_iMelodyTrackTipA = -1;
//
//            m_iOpenMode = openMode;
//
//            m_flags = 0;
//
//            if(openMode == file::OpenForParsingLevel3)
//            {
//
//               m_flags |= file::DisablePlayLevel1Operations;
//
//            }
//
//            m_ptracks->m_flags = m_flags;
//
//            uint32_t cbImage = file.GetImageSize();
//            try
//            {
//               allocate(cbImage);
//            }
//            catch(memory_exception * pe)
//            {
//               TRACE( "smfOpenFile: No memory for image! [%08lX]", cbImage);
//               smfrc = ENoMemory;
//               delete pe;
//               goto smf_Open_File_Cleanup;
//            }
//
//            LPBYTE lpbImage;
//
//            lpbImage = GetImage();
//
//            memcpy(lpbImage, file.GetImage(), cbImage);
//
//            /* If the file exists, parse it just enough to pull out the header and
//            ** build a track index.
//            */
//            smfrc = Build();
//
//smf_Open_File_Cleanup:
//
//            if (::music::success != smfrc)
//            {
//            }
//            else
//            {
//               SetOpened();
//            }
//
//            return smfrc;
//         }
//
//         e_result buffer::OpenFile(memory * pmemorystorage, int32_t openMode, e_storage estorage)
//         {
//            ASSERT(pmemorystorage != NULL);
//
//            e_result smfrc = ::music::success;
//
//            m_ptracks->m_iMelodyTrackTipA = -1;
//
//            m_iOpenMode = openMode;
//
//            m_flags = 0;
//
//            if(openMode == file::OpenForParsingLevel3)
//            {
//
//               m_flags |= file::DisablePlayLevel1Operations;
//
//            }
//
//            m_ptracks->m_flags = m_flags;
//
//            if(estorage == ::music::storage_copy)
//            {
//
//               this->get_primitive_memory()->operator=(*pmemorystorage);
//
//            }
//            else if(estorage == ::music::storage_attach)
//            {
//
//               if(m_estorage == ::music::storage_attach)
//               {
//
//                  detach();
//
//               }
//               else if(m_estorage == ::music::storage_own)
//               {
//
//                  delete detach();
//
//               }
//
//               attach(pmemorystorage);
//
//            }
//            else if(estorage == ::music::storage_own)
//            {
//
//               if(m_estorage == ::music::storage_attach)
//               {
//
//                  detach();
//
//               }
//
//               this->get_primitive_memory()->operator=(*pmemorystorage);
//
//            }
//
//            m_estorage = estorage;
//
//
//            /* If the file exists, parse it just enough to pull out the header and
//            ** build a track index.
//            */
//
//            smfrc = Build();
//
//            if (::music::success == smfrc)
//            {
//
//               SetOpened();
//
//            }
//
//            return smfrc;
//
//         }
//
//
//         e_result buffer::OpenFile(memory * pmemorystorage, file::e_open eopenmode, e_storage estorage)
//         {
//
//            return OpenFile(pmemorystorage, (int32_t) eopenmode, estorage);
//
//         }
//
//
         /*****************************************************************************
         *
         * smfOpenFile
         *
         * This function opens a MIDI file for access.
         *
         * psofs                     - Specifies the file to open and associated
         *                             parameters. Contains a valid HSMF handle
         *                             on success.
         *
         * Returns
         *   ::music::success The specified file was opened.
         *
         *   SMF_OPEN_FAILED The specified file could not be opened because it
         *     did not exist or could not be created on the disk.
         *
         *   EInvalidFile The specified file was corrupt or not a MIDI file.
         *
         *   ENoMemory There was insufficient memory to open the file.
         *
         *   SMF_INVALID_PARM The given flags or time division in the
         *     SMFOPENFILESTRUCT were invalid.
         *
         *****************************************************************************/
         //e_result buffer::OpenFile(::file::file & ar, int32_t openMode)
         //{
         //   e_result               smfrc = ::music::success;

         //   m_ptracks->m_iMelodyTrackTipA = -1;

         //   m_iOpenMode = openMode;

         //   GetFlags().unsignalize_all();

         //   if(openMode == file::OpenForParsingLevel3)
         //   {
         //      GetFlags().signalize(file::DisablePlayLevel1Operations);
         //   }

         //   m_ptracks->GetFlags() = GetFlags();


         //   get_memory()->transfer_from_begin(ar);

         //   /* If the file exists, parse it just enough to pull out the header and
         //   ** build a track index.
         //   */
         //   smfrc = Build();

         //   if (::music::success != smfrc)
         //   {
         //   }
         //   else
         //   {
         //      SetOpened();
         //   }

         //   return smfrc;
         //}

         /*****************************************************************************
         *
         * smfCloseFile
         *
         * This function closes an open MIDI file.
         *
         * hSmf                      - The handle of the open file to close.
         *
         * Returns
         *   ::music::success The specified file was closed.
         *   SMF_INVALID_PARM The given handle was not valid.
         *
         * Any track handles opened from this file handle are invalid after this
         * call.
         *
         *****************************************************************************/
         e_result buffer::CloseFile()
         {

            synch_lock sl(m_pmutex);

            SetOpened(false);

            delete_contents();

            return ::music::success;

         }


         void buffer::delete_contents()
         {

            synch_lock sl(m_pmutex);

            SetOpened(false);

            m_ptracks->clear();

            m_tempomap.remove_all();

            m_bPendingLyric = false;

            for(int32_t i = 0; i < m_mepaOnQuarterNote.get_size(); i++)
            {

               delete m_mepaOnQuarterNote.element_at(i);

            }

            m_mepaOnQuarterNote.remove_all();

            for(int32_t i = 0; i < m_mepaImmediate.get_size(); i++)
            {

               delete m_mepaImmediate.element_at(i);

            }

            m_mepaImmediate.remove_all();

            m_tkLength              = 0;
            m_dwFormat              = 0;
            m_dwTracks              = 0;
            m_dwTimeDivision        = 0;

            m_dwPendingUserEvent    = 0;
            m_cbPendingUserEvent    = 0;
            m_hpbPendingUserEvent   = 0;

            m_iKeyShift             = 0;
            m_dTempoShift           = 0;

            m_pFileHeader           = NULL;

         }


         /******************************************************************************
         *
         * smfGetFileInfo This function gets information about the MIDI file.
         *
         * hSmf                      - Specifies the open MIDI file to inquire about.
         *
         * psfi                      - A structure which will be filled in with
         *                             information about the file.
         *
         * Returns
         *   ::music::success Information was gotten about the file.
         *   SMF_INVALID_PARM The given handle was invalid.
         *
         *****************************************************************************/
         e_result buffer::GetFileInfo(
            PSMFFILEINFO            psfi)
         {
            //    PSMF                    pSmf = (PSMF)hSmf;
            //    assert(pSmf != NULL);
            ASSERT(psfi != NULL);

            /*
            ** Just fill in the structure with useful information.
            */
            psfi->dwTracks      = m_dwTracks;
            //psfi->dwTracks      = m_ptracks->get_size();
            psfi->dwFormat      = m_dwFormat;
            psfi->dwTimeDivision= m_dwTimeDivision;
            psfi->tkLength      = m_tkLength;

            return ::music::success;
         }

         /******************************************************************************
         *
         * smfTicksToMillisecs
         *
         * This function returns the millisecond offset into the file given the
         * tick offset.
         *
         * hSmf                      - Specifies the open MIDI file to perform
         *                             the conversion on.
         *
         * tkOffset                  - Specifies the tick offset into the stream
         *                             to convert.
         *
         * Returns the number of milliseconds from the start of the stream.
         *
         * The conversion is performed taking into ac::count the file's time division and
         * tempo map from the first track. Note that the same millisecond value
         * might not be valid at a later time if the tempo track is rewritten.
         *
         *****************************************************************************/
         int_ptr buffer::TicksToMillisecs(imedia_position tkOffset)
         {

            synch_lock sl(m_pmutex);

            if (!IsOpened())
            {

               return ::numeric_info < int_ptr >::allset_value();

            }

            ::music::midi::tempo_map_entry *  ptempo;
            int32_t                  idx;
            uint32_t                 uSMPTE;
            uint32_t                dwTicksPerSec;

            if (tkOffset > m_tkLength)
            {
               //TRACE( "sTTM: Clipping ticks to file length!");
               tkOffset = m_tkLength;
            }

            /* SMPTE time is easy -- no tempo map, just linear conversion
            ** Note that 30-Drop means nothing to us here since we're not
            ** converting to a colonized format, which is where dropping
            ** happens.
            */
            if (m_dwTimeDivision & 0x8000)
            {
               uSMPTE = -(int32_t)(char)((m_dwTimeDivision >> 8)&0xFF);
               if (29 == uSMPTE)
                  uSMPTE = 30;

               dwTicksPerSec = (uint32_t)uSMPTE *
                               (uint32_t)(BYTE)(m_dwTimeDivision & 0xFF);

               return (uint32_t)muldiv32((int32_t) tkOffset, 1000L, dwTicksPerSec);
            }

            /* Walk the tempo map and find the nearest tick position. Linearly
            ** calculate the rest (using MATH.ASM)
            */

            for (idx = 0; idx < m_tempomap.get_size(); idx++)
            {
               ptempo = &m_tempomap.element_at(idx);
               if (tkOffset < ptempo->tkTempo)
                  break;
            }

            if(idx <= 0)
               idx = 0;
            else
               idx--;

            if(idx >= m_tempomap.get_size())
               return 0;

            if(idx <= 0)
               ptempo = &m_tempomap.element_at(0);
            else
               ptempo = &m_tempomap.element_at(idx);

            /* ptempo is the tempo map entry preceding the requested tick offset.
            */

            return (int_ptr) (
                      ptempo->mcBase / 1000 +
                      muldiv32(
                         (int32_t)(tkOffset - ptempo->tkTempo),
                         ptempo->dwTempo,
                         1000L * m_dwTimeDivision)
                   );

         }


         /******************************************************************************
         *
         * smfMillisecsToTicks
         *
         * This function returns the nearest tick offset into the file given the
         * millisecond offset.
         *
         * hSmf                      - Specifies the open MIDI file to perform the
         *                             conversion on.
         *
         * msOffset                  - Specifies the millisecond offset into the stream
         *                             to convert.
         *
         * Returns the number of ticks from the start of the stream.
         *
         * The conversion is performed taking into ac::count the file's time division and
         * tempo map from the first track. Note that the same tick value
         * might not be valid at a later time if the tempo track is rewritten.
         * If the millisecond value does not exactly map to a tick value, then
         * the tick value will be rounded down.
         *
         *****************************************************************************/
         imedia_position buffer::MillisecsToTicks(imedia_time msOffset)
         {

            synch_lock sl(m_pmutex);

            if (!IsOpened())
            {

               return ::numeric_info < imedia_position >::allset_value();

            }

            if (m_tempomap.get_size() <= 0)
            {

               return ::numeric_info < imedia_position >::minimum_value();

            }

            ::music::midi::tempo_map_entry *    ptempo;
            int32_t                      idx;
            uint32_t                    uSMPTE;
            uint32_t                   dwTicksPerSec;
            imedia_position                   tkOffset;

            /* SMPTE time is easy -- no tempo map, just linear conversion
            ** Note that 30-Drop means nothing to us here since we're not
            ** converting to a colonized format, which is where dropping
            ** happens.
            */
            if (m_dwTimeDivision & 0x8000)
            {
               uSMPTE = -(int32_t)(char)((m_dwTimeDivision >> 8)&0xFF);
               if (29 == uSMPTE)
                  uSMPTE = 30;

               dwTicksPerSec = (uint32_t)uSMPTE * (uint32_t)(BYTE)(m_dwTimeDivision & 0xFF);

               return (int64_t) muldiv32((int32_t) msOffset, dwTicksPerSec, 1000L);
            }

            /* Walk the tempo map and find the nearest millisecond position. Linearly
            ** calculate the rest (using MATH.ASM)
            */
            //ptempo = m_pTempoMap;
            ptempo = &m_tempomap.element_at(0);
            ASSERT(ptempo != NULL);

            //for (idx = 0; idx < m_cTempoMap; idx++, ptempo++)
            for (idx = 0; idx < m_tempomap.get_size(); idx++)
            {

               ptempo = &m_tempomap.element_at(idx);

               if (::comparison::lt(msOffset, ptempo->mcBase / 1000))
               {

                  break;

               }

            }
            if (idx == 0)
            {

               return 0;

            }
            ptempo = &m_tempomap.element_at(--idx);

            /* ptempo is the tempo map entry preceding the requested tick offset.
            */

            tkOffset =  (imedia_position)
                        (
                           ptempo->tkTempo +
                           muldiv32(
                              (int32_t) (msOffset-ptempo->mcBase / 1000),
                              (int32_t)(1000L * m_dwTimeDivision),
                              (int32_t)(ptempo->dwTempo)
                           )
                        );

            if (tkOffset > m_tkLength)
            {
               //TRACE( "sMTT: Clipping ticks to file length!");
               tkOffset = m_tkLength;
            }

            return tkOffset;
         }

         imedia_time  buffer::PositionToTime(
            imedia_position position)
         {
            return TicksToMillisecs(position);
         }

         imedia_position  buffer::TimeToPosition(
            imedia_time time)
         {
            return MillisecsToTicks(time);
         }

         void  buffer::PositionToTime(imedia::time_array & timea, imedia::position_array  & positiona, int_ptr tkOffset)
         {
            if(tkOffset < 0)
            {
               for(int32_t i = 0; i < positiona.get_size(); i++)
               {
                  imedia_position tk = positiona[i];
                  imedia_position tkResult =  tk + tkOffset;
                  if(tkResult > tk)
                     tkResult = 0;

                  timea.add(
                     PositionToTime(tkResult));
               }
            }
            else
            {
               for(int32_t i = 0; i < positiona.get_size(); i++)
               {
                  imedia_position tk = positiona[i];
                  imedia_position tkResult =  tk + tkOffset;
                  if(tkResult < tk)
                     tkResult = 0xFFFFFFFF;
                  timea.add(
                     PositionToTime(
                        tkResult));
               }
            }
         }

         void buffer::TicksToMillisecs(imedia::time_2darray * p2DMillis, imedia::position_2darray *p2DTicks, int_ptr tkOffset)
         {
            ASSERT(p2DMillis->get_size() == 0);
            p2DMillis->set_app(get_app());
            p2DMillis->set_size_create(p2DTicks->get_size());
            int32_t i;
            for(i = 0; i < p2DTicks->get_size(); i++)
            {
               TicksToMillisecs(
                  p2DMillis->element_at(i),
                  p2DTicks->element_at(i),
                  tkOffset);
            }

         }

         void buffer::PositionToTime(imedia::time_2darray  & timea, imedia::position_2darray  & positiona, int_ptr tkOffset)
         {
            ASSERT(timea.get_size() == 0);
            timea.set_app(get_app());
            timea.set_size_create(positiona.get_size());
            int32_t i;
            for(i = 0; i < positiona.get_size(); i++)
            {
               PositionToTime(
                  timea(i),
                  positiona(i),
                  tkOffset);
            }

         }

         void  buffer::TicksToMillisecs(imedia::time_array *pMillisArray, imedia::position_array *pTickArray, int_ptr tkOffset)
         {
            if(tkOffset < 0)
            {
               for(int32_t i = 0; i < pTickArray->get_size(); i++)
               {
                  imedia_position tk = pTickArray->operator [](i);
                  imedia_position tkResult =  tk + tkOffset;
                  if(tkResult > tk)
                     tkResult = 0;

                  pMillisArray->add(
                     (imedia_time)
                     TicksToMillisecs(
                        tkResult));
               }
            }
            else
            {
               for(int32_t i = 0; i < pTickArray->get_size(); i++)
               {
                  imedia_position tk = pTickArray->operator [](i);
                  imedia_position tkResult =  tk + tkOffset;
                  if(tkResult < tk)
                     tkResult = 0xFFFFFFFF;
                  pMillisArray->add(
                     (imedia_time)
                     TicksToMillisecs(
                        tkResult));
               }
            }
         }


         void  buffer::MillisecsToTicks(
            imedia::position_array *pTickArray,
            imedia::time_array *pMillisArray,
            imedia_time msOffset)
         {
            if(msOffset < 0)
            {
               for(int32_t i = 0; i < pMillisArray->get_size(); i++)
               {
                  imedia_time ms = pMillisArray->operator [](i);
                  imedia_time msResult = ms + msOffset;
                  if(msResult > ms)
                     msResult = 0;
                  pTickArray->add(
                     (imedia_position)
                     MillisecsToTicks(
                        msResult));
               }
            }
            else
            {
               for(int32_t i = 0; i < pMillisArray->get_size(); i++)
               {
                  imedia_time ms = pMillisArray->operator [](i);
                  imedia_time msResult = ms + msOffset;
                  if(msResult < ms)
                     msResult = 0xffffffff;
                  pTickArray->add(
                     (imedia_position)
                     MillisecsToTicks(
                        msResult));
               }
            }

         }

         void  buffer::TimeToPosition(
            imedia::position_array & positiona,
            imedia::time_array  & timea,
            imedia_time msOffset)
         {
            if(msOffset < 0)
            {
               for(int32_t i = 0; i < timea.get_size(); i++)
               {
                  imedia_time ms = timea[i];
                  imedia_time msResult = ms + msOffset;
                  if(msResult < 0)
                     msResult = 0;
                  positiona.add(
                     TimeToPosition(
                        msResult));
               }
            }
            else
            {
               for(int32_t i = 0; i < timea.get_size(); i++)
               {
                  imedia_time ms = timea[i];
                  imedia_time msResult = ms + msOffset;
                  if(msResult < ms)
                     msResult = 0xffffffff;
                  positiona.add(
                     TimeToPosition(
                        msResult));
               }
            }

         }

         ::music::e_result buffer::StreamEventF1(imedia_position tkDelta,
                                                 array < ::music::midi::event *, ::music::midi::event * > & eventptra,
                                                 LPMIDIHDR lpmh,
                                                 imedia_position tkMax,
                                                 uint32_t cbPrerollNomimalMax
                                                )
         {
            UNREFERENCED_PARAMETER(tkMax);
            UNREFERENCED_PARAMETER(cbPrerollNomimalMax);
            e_result               smfrc;

            if(eventptra.get_size() <= 0)
            {
               return success;
            }

            ::music::midi::event * pevent;
            int32_t iSize = sizeof(midi_stream_event_header);
            int32_t i;
            for(i = 0; i < eventptra.get_size(); i++)
            {
               pevent = eventptra[i];
               ASSERT(pevent->GetFlags() & 1);
               iSize += (int32_t) pevent->GetDataSize();
               iSize += sizeof(midi_stream_event_header);
            }

            m_memstorageF1.allocate(iSize);
            LPBYTE lpbParam;
            LPDWORD lpdwType;
            midi_stream_event_header * pheader;
            pheader = (midi_stream_event_header *) &m_memstorageF1.get_data()[0];
            pheader->m_dwLength = iSize - sizeof(midi_stream_event_header);
            pheader->m_dwType = 0;
            iSize = sizeof(midi_stream_event_header);
            for(i = 0; i < eventptra.get_size(); i++)
            {
               pevent = eventptra[i];
               lpbParam = pevent->GetData();
               lpdwType = (LPDWORD) lpbParam;
               pheader = (midi_stream_event_header *) &m_memstorageF1.get_data()[iSize];
               pheader->m_dwLength = (uint32_t) pevent->GetDataSize();
               pheader->m_dwType = *lpdwType;
               memcpy(
                  &m_memstorageF1.get_data()[iSize + sizeof(midi_stream_event_header)],
                  lpbParam,
                  pheader->m_dwLength);
               iSize += pheader->m_dwLength + sizeof(midi_stream_event_header);
            }

            m_cbPendingUserEvent = (uint32_t) m_memstorageF1.get_size();
            m_hpbPendingUserEvent = m_memstorageF1.get_data();
            ASSERT(m_hpbPendingUserEvent);
            m_flags &= ~file::InsertSysEx;
            m_dwPendingUserEvent = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((uint32_t)MEVT_COMMENT)<<24)) & 0xFF000000L);

            smfrc = InsertParmData(tkDelta, lpmh);
            if (::music::success != smfrc)
            {
               TRACE( "smfInsertParmData[2] %u", (uint32_t)smfrc);
               return smfrc;
            }
            return smfrc;
         }

         ::music::e_result buffer::StreamEvent(
            imedia_position                   tkDelta,
            ::music::midi::event *        pEvent,
            LPMIDIHDR               lpmh,
            imedia_position                   tkMax,
            uint32_t                   cbPrerollNominalMax)
         {
            UNREFERENCED_PARAMETER(tkMax);
            UNREFERENCED_PARAMETER(cbPrerollNominalMax);
            //   TRACE("buffer::StreamEvent tkDelta %d\n", tkDelta);

            uint32_t                   dwTempo;
            LPDWORD                 lpdw;
            e_result    smfrc;
            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
            ASSERT(!(pEvent->GetFlags() & 1));
            if(pEvent->GetFlags() & 1)
            {
               ASSERT(FALSE);
            }
            else if (pEvent->GetFullType() < ::music::midi::SysEx)
            {
               if(pEvent->GetType() == ::music::midi::ProgramChange)
               {
                  int32_t iTrack = pEvent->GetTrack();
                  int32_t iProgramChange = pEvent->GetChB1();
                  m_keyframe.rbProgram[iTrack] = (byte) iProgramChange;
                  //if(iProgramChange == 54)
                  //{
                  // pEvent->SetChB1(62);
                  //}
               }
               if(pEvent->GetTrack() == 9 ||
                     pEvent->GetTrack() == 15)
               {
                  //         TRACE("ReadEvents Track %d Program %d", pEvent->GetTrack(), m_keyframe.rbProgram[pEvent->GetTrack()]);
               }
               if((pEvent->GetType() == ::music::midi::NoteOn ||
                     pEvent->GetType() == ::music::midi::NoteOff)
                     && !((m_keyframe.rbProgram[pEvent->GetTrack()] == 0)
                         ))
                  //&& (pEvent->GetTrack() == 9 ||
                  //pEvent->GetTrack() == 15)))
               {
                  int32_t iNotePitch = pEvent->GetNotePitch();
                  iNotePitch += m_iKeyShift;
                  while(iNotePitch >= 0x80)
                  {
                     iNotePitch -= 12;
                  }
                  while(iNotePitch < 0)
                  {
                     iNotePitch += 12;
                  }
                  pEvent->SetNotePitch((byte) iNotePitch);
               }

               *lpdw++ = (uint32_t)tkDelta;
               *lpdw++ = 0;
               *lpdw++ = (((uint32_t)MEVT_SHORTMSG)<<24) |
                         ((uint32_t)pEvent->GetFullType()) |
                         (((uint32_t)pEvent->GetChB1()) << 8) |
                         (((uint32_t)pEvent->GetChB2()) << 16);

               lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
            }
            else if (pEvent->GetFullType() == ::music::midi::Meta &&
                     pEvent->GetMetaType() ==  ::music::midi::MetaEOT)
            {
               /* These are ignoreable since smfReadNextEvent()
               ** takes care of track merging
               */
            }
            else if (::music::midi::Meta == pEvent->GetFullType() && ::music::midi::MetaTempo == pEvent->GetMetaType())
            {

               if (pEvent->GetDataSize() != 3)
               {

                  TRACE( "smfReadEvents: Corrupt tempo event");

                  return EInvalidFile;

               }

               dwTempo = (((uint32_t)pEvent->GetData()[0])<<16)|
                         (((uint32_t)pEvent->GetData()[1])<<8)|
                         ((uint32_t)pEvent->GetData()[2]);
               dwTempo = (uint32_t) ((double) dwTempo / GetTempoShiftRate());
               uint32_t dw = (((uint32_t)MEVT_TEMPO)<<24)| dwTempo;


               *lpdw++ = (uint32_t)tkDelta;
               *lpdw++ = 0;
               *lpdw++ = dw;

               lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
            }
            //       else if ((Meta == pEvent->GetType()) &&
            //          (05 == pEvent->GetType()))
            ///      {
            /**lpdw++ = (uint32_t)pEvent->tkDelta;
            *lpdw++ = 0;
            *lpdw++ = MEVT_F_SHORT | MEVT_F_CALLBACK |
            (((uint32_t)MEVT_SHORTMSG )<<24) |
            ((uint32_t)7) |
            (((uint32_t)13) << 8) |
            (((uint32_t)31) << 16);
            lpmh->dwBytesRecorded += 3*sizeof(uint32_t);*/
            //m_cbPendingUserEvent = ((uint32_t)MEVT_SHORTMSG )<<24;
            //         str = (char *) malloc(pEvent->cbParm + 1);
            //         strncpy(str, (const char *) pEvent->hpbParm,  pEvent->cbParm);
            //         str[pEvent->cbParm] = 0;
            //         printf("%s", str);

            //         m_cbPendingUserEvent = pEvent->cbParm;
            //            m_hpbPendingUserEvent = pEvent->hpbParm;
            //            m_fdwSMF &= ~FlagInsertSysEx;
            //         m_dwPendingUserEvent = MEVT_F_CALLBACK |(((uint32_t)MEVT_LONGMSG )<<24);
            //m_dwPendingUserEvent = (((uint32_t)MEVT_LONGMSG )<<24);
            //         smfrc = smfInsertParmData(pSmf, pEvent->tkDelta, lpmh);
            //            if (::music::success != smfrc)
            //            {
            //                TRACE( "smfInsertParmData[2] %u", (uint32_t)smfrc);
            //                return smfrc;
            //            }

            //            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);


            //smfrc = smfInsertParmData(pSmf, pEvent->tkDelta, lpmh);
            //if (::music::success != smfrc)
            //{
            //    TRACE( "smfInsertParmData[2] %u", (uint32_t)smfrc);
            //    return smfrc;
            //}
            //         _CrtSetReportFile(_CRT_WARN, debugFile);
            //         _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

            //_RPT0(_CRT_WARN, "Lyric:");
            //_RPT1(_CRT_WARN, "%s", lpmh->lpData);
            //_RPT0(_CRT_WARN, "\n");

            //    }
            else if (pEvent->GetFullType() != ::music::midi::Meta)
               //   else
            {
               //            if(pEvent->GetFullType() == SysEx)
               //         {
               //                break;
               //            }
               /* Must be F0 or F7 system exclusive or FF meta
               ** that we didn't recognize
               */

               m_cbPendingUserEvent = (uint32_t) pEvent->GetDataSize();
               m_hpbPendingUserEvent = pEvent->GetData();
               m_flags &= ~file::InsertSysEx;

               if(pEvent->GetFullType() == ::music::midi::SysExEnd)
               {
                  m_dwPendingUserEvent = ((uint32_t)MEVT_LONGMSG) << 24;
               }
               else if(pEvent->GetFullType() == ::music::midi::SysEx)
               {
                  m_flags |= file::InsertSysEx;
                  ++m_cbPendingUserEvent;

                  /* Falling through...
                  */

                  m_dwPendingUserEvent = ((uint32_t)MEVT_LONGMSG) << 24;
               }

               smfrc = InsertParmData(tkDelta, lpmh);
               if (::music::success != smfrc)
               {
                  TRACE( "smfInsertParmData[2] %u", (uint32_t)smfrc);
                  return smfrc;
               }

               lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
            }
            else // Meta
            {
               // se o meta event possuir tkDelta > 0,
               // insere o evento no stream para que n縊 haja perda de sincronismo
               if(tkDelta > 0)
               {
                  InsertPadEvent(tkDelta, lpmh);
                  lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
               }
            }
            return ::music::success;
         }

         /******************************************************************************
         *
         * smfReadEvents
         *
         * This function reads events from a track.
         *
         * hSmf                      - Specifies the file to read data from.
         *
         * lpmh                      - Contains information about the buffer to fill.
         *
         * tkMax                     - Specifies a cutoff point in the stream
         *                             beyond which events will not be read.
         *
         * Return@rdes
         *   ::music::success The events were successfully read.
         *   SMF_END_OF_TRACK There are no more events to read in this track.
         *   EInvalidFile A disk error occured on the file.
         *
         * @xref <f smfWriteEvents>
         *****************************************************************************/
         e_result buffer::StreamRender(LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNominalMax)
         {

            //    PSMF                    pSmf = (PSMF)hSmf;
            e_result               smfrc;
            ::music::midi::event * pevent;
            LPDWORD                 lpdw;
            //    uint32_t                   dwTempo;
            imedia_position               tkDelta = 0;
            //   imedia_position               tkPosition;
            //   imedia_position               tkMin;
            //   imedia_position               tk;
            int32_t                  iTrackFound;
            //   int32_t                  i;
            imedia_position               tkLastDelta = 0 ;
            imedia_position                   tkOnQuarterNoteDelta = 0;
            //   imedia_position               tkLastPositionCB = 0x7fffffff;
            //CXfplayerApp *pApp = (CXfplayerApp *) &System;
            //ASSERT(pApp != NULL);
            //CXfplayerDoc *pDoc = (CXfplayerDoc *) pApp->GetPlayingDocument();
            //ASSERT(pDoc != NULL);
            //   char * str;
            //assert(pSmf != NULL);
            ASSERT(lpmh != NULL);

            /*
            ** read events from the track and pack them into the buffer in polymsg
            ** format.
            **
            ** If a SysEx or meta would go over a buffer boundry, split it.
            */
            lpmh->dwBytesRecorded = 0;
            if (m_dwPendingUserEvent)
            {
               smfrc = InsertParmData((imedia_position)0, lpmh);
               if (::music::success != smfrc)
               {
                  TRACE( "smfInsertParmData() -> %u", (uint32_t)smfrc);
                  return smfrc;
               }
            }
            //   if(m_pendingLyricEventArray.get_size() > 0)
            //   {
            //      smfrc = InsertLyricEvents(NULL, lpmh);
            //        if (::music::success != smfrc)
            //      {
            //        TRACE( "smfInsertParmData() -> %u", (uint32_t)smfrc);
            //      return smfrc;
            //}
            //}

            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

            if (m_flags & file::EndOfFile)
            {

               return SEndOfFile;

            }

            /*if(pDoc->m_iBufferingOffsetTokenIndex >= pDoc->m_TokensOffsetTicks.get_size())
            {
            tkNextOffsetLyricEvent = -1;
            }
            else
            {
            tkNextOffsetLyricEvent = pDoc->m_TokensOffsetTicks[pDoc->m_iBufferingOffsetTokenIndex ];
            }
            if(pDoc->m_iBufferingTokenIndex >= pDoc->m_TokensTicks.get_size())
            {
            tkNextLyricEvent = -1;
            }
            else
            {
            tkNextLyricEvent = pDoc->m_TokensTicks[pDoc->m_iBufferingTokenIndex ];
            }*/
            //   ::ikaraoke::lyric_events_v1 *pEventsV1;
            while(TRUE)
            {
               ASSERT(lpmh->dwBytesRecorded <= lpmh->dwBufferLength);
               if(lpmh->dwBytesRecorded > cbPrerollNominalMax && lpmh->dwBytesRecorded > 0)
               {
                  break;
               }

               /* If we know ahead of time we won't have room for the
               ** event, just break out now. We need 2 uint32_t's for the
               ** terminator event and at least 2 uint32_t's for any
               ** event we might store - this will allow us a full
               ** int16_t event or the delta time and stub for a long
               ** event to be split.
               */
               /*
               now 16 DWORDS
               */
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
                  break;
               }




               //smfrc = GetNextEventTkPosition(&tkPosition, tkMax);


               /*        if (::music::success != smfrc)
               {
               smfrc = GetNextEvent((event *)&event, tkMax, TRUE);
               ASSERT(::music::success != smfrc); */

               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-file.
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               /*if (SReachedTkMax == smfrc)
               {
               m_fdwSMF |= FlagEof;
               }

               TRACE( "smfReadEvents: ReadEvents() -> %u", (uint32_t)smfrc);
               break;
               }*/
               iTrackFound = -1;
               tkLastDelta = 0;
               /*      while(TRUE)
               {
               for(i = 0; i < m_eventsTracksForPositionCB.get_size(); i++)
               {
               pEventsV1 = m_eventsTracksForPositionCB.get_at(i);
               if(pEventsV1->GetClassOrder() == 1)
               {
               while(m_ptracks->m_tkPosition >
               (tk = pEventsV1->GetCurrentTkTokenPosition()))
               {
               pEventsV1->m_iCurrentToken++;
               }
               }
               else
               {
               while(m_ptracks->m_tkPosition >
               (tk = ((::ikaraoke::lyric_events_v2 *)pEventsV1)->GetCurrentTkNotePosition()))
               {
               pEventsV1->m_iCurrentNote++;
               }
               }
               if(iTrackFound < 0)
               {
               if(tk >= 0)
               if(tkPosition >= tk)
               {
               tkMin = tk;
               iTrackFound = i;
               }
               }
               else
               {
               if(tk >= 0)
               if(tkPosition >= tk)
               {
               if(tk < tkMin)
               {
               tkMin = tk;
               iTrackFound = i;
               }
               }

               }
               }
               if(iTrackFound < 0)
               break;
               else
               {
               pEventsV1 = m_eventsTracksForPositionCB.get_at(iTrackFound);
               #ifdef DEBUG
               //         if(pEventsV1->m_iType == 1)
               //         {
               //            TRACE("ReadEvents pEventsV1->m_iCurrentToken = %d\n", pEventsV1->m_iCurrentToken);
               //            TRACE("ReadEvents pEventsV1->m_iType = %d\n", pEventsV1->m_iType);
               //            TRACE("ReadEvents m_ptracks->m_tkPosition = %d\n", m_ptracks->m_tkPosition);
               //         }
               #endif
               tkDelta = tkMin - m_ptracks->m_tkPosition;
               if((tkDelta - tkLastDelta) == 0)
               {
               pEventsV1->m_tkDelta = 0;
               InsertLyricEvents(pEventsV1, lpmh);
               }
               else
               {
               if(m_pendingLyricEventArray.get_size() > 0)
               {
               smfrc = InsertLyricEvents(NULL, lpmh);
               if(MetaPending == smfrc)
               {
               break;
               }
               else if (::music::success != smfrc)
               {

               TRACE( "smfInsertLyricEvent %u", (uint32_t)smfrc);
               return smfrc;
               }
               lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
               }
               pEventsV1->m_tkDelta = tkDelta - tkLastDelta;
               InsertLyricEvents(pEventsV1, lpmh);
               }

               iTrackFound = -1;
               tkLastDelta = tkDelta;
               }
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
               break;
               }
               }*/
               /*if(m_pendingLyricEventArray.get_size() > 0)
               {
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
               break;
               }
               smfrc = InsertLyricEvents(NULL, lpmh);
               if(MetaPending == smfrc)
               {
               break;
               }
               else if (::music::success != smfrc)
               {
               TRACE( "smfInsertLyricEvent %u", (uint32_t)smfrc);
               return smfrc;
               }
               lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
               }*/
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
                  m_ptracks->m_tkPosition += tkDelta;
                  break;
               }

               smfrc = GetNextEvent(pevent, tkMax, TRUE);
               if (::music::success != smfrc)
               {
                  /* smfGetNextEvent doesn't set this because smfSeek uses it
                  ** as well and needs to distinguish between reaching the
                  ** seek point and reaching end-of-file.
                  **
                  ** To the ::fontopus::user, however, we present the selection between
                  ** their given tkBase and tkEnd as the entire file, therefore
                  ** we want to translate this into EOF.
                  */
                  if(SReachedTkMax == smfrc)
                  {
                     m_flags |= file::EndOfFile;
                  }

                  TRACE( "smfReadEvents: ReadEvents() -> %u", (uint32_t)smfrc);
                  break;
               }
               ::music::e_result vmsr;
               vmsr = StreamEvent(tkDelta, pevent, lpmh, tkMax, cbPrerollNominalMax);

            }

            return (m_flags & file::EndOfFile) ? SEndOfFile : ::music::success;
         }

         /******************************************************************************
         *
         * smfReadEvents
         *
         * This function reads events from a track.
         *
         * hSmf                      - Specifies the file to read data from.
         *
         * lpmh                      - Contains information about the buffer to fill.
         *
         * tkMax                     - Specifies a cutoff point in the stream
         *                             beyond which events will not be read.
         *
         * Return@rdes
         *   ::music::success The events were successfully read.
         *   SMF_END_OF_TRACK There are no more events to read in this track.
         *   EInvalidFile A disk error occured on the file.
         *
         * @xref <f smfWriteEvents>
         *****************************************************************************/

         ::music::e_result buffer::InsertLyricEvents(
            ::ikaraoke::lyric_event_v1 *      pLyricEvent,
            LPMIDIHDR         lpmh)
         {
            UNREFERENCED_PARAMETER(pLyricEvent);
            UNREFERENCED_PARAMETER(lpmh);
            /*    LPDWORD                 lpdw;
            uint32_t                   dwRounded;
            uint32_t               dwLength;
            uint32_t               dwSize;
            imedia_position               tkDelta;

            if(pLyricEvent != NULL)
            {
            m_pendingLyricEventArray.add(*pLyricEvent);
            if(pLyricEvent->GetClassOrder() == 1)
            pLyricEvent->m_iCurrentToken++;
            else
            {
            ::ikaraoke::lyric_events_v2 * pPendingLyricEventV2 =
            (::ikaraoke::lyric_events_v2 *) pLyricEvent;
            pPendingLyricEventV2->m_iCurrentNote++;
            if(pPendingLyricEventV2->m_iCurrentNote <
            pPendingLyricEventV2->m_msaNotesPosition.get_size())
            {
            uint_array dwArray;
            //            VERIFY(1 == pPendingLyricEventV2->GetNoteTokens(m_pPendingLyricEventV1->m_iCurrentNote, dwArray));
            pPendingLyricEventV2->GetNoteTokens(pPendingLyricEventV2->m_iCurrentNote, dwArray);
            if(dwArray.get_size() > 0)
            pPendingLyricEventV2->m_iCurrentToken = dwArray.get_at(0);
            else
            pPendingLyricEventV2->m_iCurrentToken = -1;
            }
            }
            return ::music::success;
            }

            ASSERT(lpmh != NULL);

            if(m_pendingLyricEventArray.get_size() <= 0)
            {
            return ::music::success;
            }

            tkDelta = m_pendingLyricEventArray[0].m_tkDelta;

            ASSERT(tkDelta >= 0);

            /* Can't fit 4 uint32_t's? (tkDelta + stream-id + event + some data)
            ** Can't do anything.
            */
            /*ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);
            //   m_dwPendingUserEvent = MEVT_F_CALLBACK |
            //             (((uint32_t)MEVT_LONGMSG)<<24);
            //   m_cbPendingUserEvent = 2 * sizeof(uint32_t);

            if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4 * sizeof(uint32_t))
            {
            if (0 == tkDelta)
            return ::music::success;

            /* If we got here with a real delta, that means smfReadEvents screwed
            ** up calculating left space and we should flag it somehow.
            */
            /*        TRACE( "Can't fit initial piece of SysEx into buffer!");
            return EInvalidFile;
            }

            CMemFile memFile;
            CArchive ar(&memFile, CArchive::store);
            m_pendingLyricEventArray.Serialize(ar);
            ar.Flush();
            dwSize = (memFile.get_length() + sizeof(uint32_t) + 3) & (~3L);


            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
            dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(uint32_t);
            if(dwSize > dwLength)
            return MetaPending;
            dwLength = dwSize;

            //   uint32_t dwa = MEVT_F_CALLBACK;
            //   uint32_t dwb = MEVT_LONGMSG;
            //   uint32_t dwc = MEVT_F_LONG;
            //   uint32_t dwd = MEVT_COMMENT;

            ASSERT((dwSize & 3L) == 0);
            dwRounded = dwSize;
            //dwRounded = (dwLength + 3) & (~3L);

            //tkOffset = tkDelta;
            *lpdw++ = (uint32_t) tkDelta;
            *lpdw++ = 0;
            //*lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((uint32_t)MEVT_LONGMSG)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
            *lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((uint32_t)MEVT_COMMENT)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
            *lpdw++ = EVENT_ID_LYRIC_V1;

            //   if(m_cbPendingLyricEventV1 >= (3 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nType;
            //   }
            //   if(m_cbPendingLyricEventV1 >= (2 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nTrack;
            //   }
            //   if(m_cbPendingLyricEventV1 >= (1 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nCurrentIndex;
            //      m_pPendingLyricEventV1->m_Union.m_pevent->m_nCurrentIndex++;
            //   }
            memFile.seek_to_begin();
            memFile.read(lpdw, dwLength);
            while(0 < m_pendingLyricEventArray.get_size())
            {
            m_pendingLyricEventArray.remove_at(
            m_pendingLyricEventArray.get_size() - 1);
            }
            lpmh->dwBytesRecorded += 3*sizeof(uint32_t) + dwRounded;
            */
            return ::music::success;
         }

         e_result buffer::InsertPadEvent(
            imedia_position            tkDelta,
            LPMIDIHDR         lpmh)

         {
            LPDWORD                 lpdw;
            uint32_t                   dwRounded;
            uint32_t               dwLength;


            ASSERT(tkDelta >= 0);
            //    assert(pSmf != NULL);
            ASSERT(lpmh != NULL);

            /* Can't fit 4 uint32_t's? (tkDelta + stream-id + event + some data)
            ** Can't do anything.
            */
            ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);
            //   m_dwPendingUserEvent = MEVT_F_CALLBACK |
            //             (((uint32_t)MEVT_LONGMSG)<<24);
            //   m_cbPendingUserEvent = 2 * sizeof(uint32_t);
            //   if(!m_cbPendingLyricEventV1)
            //   if(!1)
            //   {
            //      return ::music::success;
            //   }

            if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4 * sizeof(uint32_t))
            {
               if (0 == tkDelta)
                  return ::music::success;

               /* If we got here with a real delta, that means smfReadEvents screwed
               ** up calculating left space and we should flag it somehow.
               */
               TRACE( "Can't fit initial piece of SysEx into buffer!");
               return EInvalidFile;

            }

            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
            dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(uint32_t);
            dwLength = MIN(dwLength, 8);

            //uint32_t dwa = MEVT_F_CALLBACK;
            //   uint32_t dwb = MEVT_LONGMSG;
            //   uint32_t dwc = MEVT_F_LONG;
            //   uint32_t dwd = MEVT_COMMENT;

            dwRounded = (dwLength + 3) & (~3L);

            //tkOffset = tkDelta;
            *lpdw++ = (uint32_t) tkDelta;
            *lpdw++ = 0;
            //*lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((uint32_t)MEVT_LONGMSG)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
            //   *lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((uint32_t)MEVT_COMMENT)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
            *lpdw++ = ((MEVT_F_LONG |(((uint32_t)MEVT_COMMENT)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
            *lpdw++ = 0;
            *lpdw++ = EVENT_ID_PAD;

            //   if(m_cbPendingLyricEventV1 >= (3 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nType;
            //   }
            //   if(m_cbPendingLyricEventV1 >= (2 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nTrack;
            //   }
            //   if(m_cbPendingLyricEventV1 >= (1 * sizeof(uint32_t)))
            //   {
            //      // offset Lyric CallBack Event Code
            //      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_pevent->m_nCurrentIndex;
            //      m_pPendingLyricEventV1->m_Union.m_pevent->m_nCurrentIndex++;
            //   }
            //   m_pPendingLyricEventV1->ToData(lpdw, m_cbPendingLyricEventV1);
            //   if (0 == (m_cbPendingLyricEventV1 -= dwLength))
            //   {
            //   }
            //   else
            //   {
            //      int32_t i = 5;
            //   }

            lpmh->dwBytesRecorded += 3*sizeof(uint32_t) + dwRounded;
            return ::music::success;
         }
         /******************************************************************************
         *
         * smfInsertParmData
         *
         * Inserts pending long data from a track into the given buffer.
         *
         * pSmf                      - Specifies the file to read data from.
         *
         * tkDelta                   - Specfices the tick delta for the data.
         *
         * lpmh                      - Contains information about the buffer to fill.
         *
         * Returns
         *   ::music::success The events were successfully read.
         *   EInvalidFile A disk error occured on the file.
         *
         * Fills as much data as will fit while leaving room for the buffer
         * terminator.
         *
         * If the long data is depleted, resets m_dwPendingUserEvent so
         * that the next event may be read.
         *
         *****************************************************************************/
         e_result buffer::InsertParmData(
            imedia_position                   tkDelta,
            LPMIDIHDR               lpmh)
         {
            uint32_t                   dwLength;
            uint32_t                   dwRounded;
            LPDWORD                 lpdw;

            //    assert(pSmf != NULL);
            ASSERT(lpmh != NULL);

            /* Can't fit 4 uint32_t's? (tkDelta + stream-id + event + some data)
            ** Can't do anything.
            */
            ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);

            if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
            {
               if (0 == tkDelta)
                  return ::music::success;

               /* If we got here with a real delta, that means smfReadEvents screwed
               ** up calculating left space and we should flag it somehow.
               */
               TRACE( "Can't fit initial piece of SysEx into buffer!");
               return EInvalidFile;
            }

            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

            dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(uint32_t);
            dwLength = MIN(dwLength, m_cbPendingUserEvent);

            *lpdw++ = (uint32_t)tkDelta;
            *lpdw++ = 0L;
            *lpdw++ = (m_dwPendingUserEvent & 0xFF000000L) | (dwLength & 0x00FFFFFFL);

            dwRounded = (dwLength + 3) & (~3L);

            if (m_flags & file::InsertSysEx)
            {
               LPBYTE lpb = (LPBYTE) lpdw;
               *lpb++ = ::music::midi::SysEx;
               m_flags &= ~file::InsertSysEx;
               --dwLength;
               --m_cbPendingUserEvent;
               lpdw = (LPDWORD) lpb;
            }

            if (dwLength & 0x80000000L)
            {
               TRACE( "dwLength %08lX  dwBytesRecorded %08lX  dwBufferLength %08lX", dwLength, lpmh->dwBytesRecorded, lpmh->dwBufferLength);
               TRACE( "cbPendingUserEvent %08lX  dwPendingUserEvent %08lX dwRounded %08lX", m_cbPendingUserEvent, m_dwPendingUserEvent, dwRounded);
               TRACE( "offset into MIDI image %08lX", (uint32_t)(m_hpbPendingUserEvent - GetImage()));
               TRACE( "!hmemcpy is about to fault");
            }

            memcpy(lpdw, m_hpbPendingUserEvent, dwLength);
            if (0 == (m_cbPendingUserEvent -= dwLength))
               m_dwPendingUserEvent = 0;

            lpmh->dwBytesRecorded += 3*sizeof(uint32_t) + dwRounded;

            return ::music::success;
         }

         /******************************************************************************
         *
         * smfSeek
         *
         * This function moves the file pointer within a track
         * and gets the state of the track at the new position. It returns a buffer of
         * state information which can be used to set up to play from the new position.
         *
         * hSmf                      - Handle of file to seek within
         *
         * tkPosition                - The position to seek to in the track.
         *
         * lpmh                      - A buffer to contain the state information.
         *
         * Returns
         *   ::music::success | The state was successfully read.
         *   SMF_END_OF_TRACK | The pointer was moved to end of track and no state
         *     information was returned.
         *   SMF_INVALID_PARM | The given handle or buffer was invalid.
         *   ENoMemory | There was insufficient memory in the given buffer to
         *     contain all of the state data.
         *
         * The state information in the buffer includes patch changes, tempo changes,
         * time signature, key signature,
         * and controller information. Only the most recent of these paramters before
         * the current position will be stored. The state buffer will be returned
         * in polymsg format so that it may be directly transmitted over the MIDI
         * bus to bring the state up to date.
         *
         * The buffer is mean to be sent as a streaming buffer; i.e. immediately
         * followed by the first data buffer. If the requested tick position
         * does not exist in the file, the last event in the buffer
         * will be a MEVT_NOP with a delta time calculated to make sure that
         * the next stream event plays at the proper time.
         *
         * The meta events (tempo, time signature, key signature) will be the
         * first events in the buffer if they exist.
         *
         * Use smfGetStateMaxSize to determine the maximum size of the state
         * information buffer. State information that will not fit into the given
         * buffer will be lost.
         *
         * On return, the dwBytesRecorded field of lpmh will contain the
         * actual number of bytes stored in the buffer.
         *
         *****************************************************************************/


         e_result buffer::seek(
            imedia_position                   tkPosition,
            LPMIDIHDR               lpmh)
         {
            e_result     smfrc;
            ::music::midi::event *        pevent;
            LPDWORD                 lpdw;
            BYTE                    bEvent;
            uint32_t                    idx;
            uint32_t                    idxChannel;
            uint32_t                    idxController;

            _SyncSeek(tkPosition);

            memset(&m_keyframe, 0, sizeof(m_keyframe));



            memset(&m_keyframe, 0xFF, sizeof(m_keyframe));
            memset(&m_keyframe.rbProgram, 0x00, sizeof(m_keyframe.rbProgram));

            m_ptracks->m_tkPosition = 0;
            m_flags &= ~file::EndOfFile;
            m_ptracks->m_flags &= ~file::EndOfFile;

            m_ptracks->seek_begin();
            //for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)

            while (::music::success == (smfrc = GetNextEvent(pevent, tkPosition, FALSE)))
            {

               if (::music::midi::Meta == (bEvent = pevent->GetFullType()))
               {
                  if (pevent->GetMetaType() == ::music::midi::MetaTempo)
                  {
                     if (pevent->GetDataSize() != sizeof(m_keyframe.rbTempo))
                        return EInvalidFile;

                     memcpy((byte *)m_keyframe.rbTempo, pevent->GetData(), pevent->GetDataSize());
                  }
               }
               if((bEvent & 0xF0) == ::music::midi::ProgramChange)
               {
                  m_keyframe.rbProgram[bEvent & 0x0F] = pevent->GetChB1();
               }
               else if((bEvent & 0xF0) == ::music::midi::ControlChange)
               {
                  m_keyframe.rbControl[bEvent & 0x0F][pevent->GetChB1()] = pevent->GetChB2();
               }
            }

            if(SReachedTkMax != smfrc)
            {
               return smfrc;
            }

            /* Build lpmh from keyframe
            */
            lpmh->dwBytesRecorded = 0;
            lpdw = (LPDWORD)lpmh->lpData;

            /* Tempo change event?
            */
            if (KF_EMPTY != m_keyframe.rbTempo[0] ||
                  KF_EMPTY != m_keyframe.rbTempo[1] ||
                  KF_EMPTY != m_keyframe.rbTempo[2])
            {
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                  return ENoMemory;

               *lpdw++ = 0;
               *lpdw++ = 0;
               //      *lpdw++ = (((uint32_t)m_keyframe.rbTempo[2])<<16)|
               //                (((uint32_t)m_keyframe.rbTempo[1])<<8)|
               //            ((uint32_t)m_keyframe.rbTempo[0])|
               //            (((uint32_t)MEVT_SHORTMSG) << 24);
               //
               //      *lpdw++ = (((uint32_t)m_keyframe.rbTempo[0])<<16)|
               //                (((uint32_t)m_keyframe.rbTempo[1])<<8)|
               //             ((uint32_t)m_keyframe.rbTempo[2])|
               //            (((uint32_t)MEVT_SHORTMSG) << 24);

               uint32_t dwTempo =   (((uint32_t)m_keyframe.rbTempo[0])<<16)|
                                    (((uint32_t)m_keyframe.rbTempo[1])<<8)|
                                    ((uint32_t)m_keyframe.rbTempo[2]);
               dwTempo = (uint32_t) ((double) dwTempo / GetTempoShiftRate());
               uint32_t dw = (((uint32_t)MEVT_TEMPO)<<24)| dwTempo;


               //*lpdw++ = (uint32_t)pEvent->_GetDelta();
               //*lpdw++ = 0;
               //*lpdw++ = dw;
               *lpdw++ = dw;

               //*lpdw++ = (((uint32_t)m_keyframe.rbTempo[0])<<16)|
               //        (((uint32_t)m_keyframe.rbTempo[1])<<8)|
               //      ((uint32_t)m_keyframe.rbTempo[2])|
               //    (((uint32_t)MEVT_TEMPO) << 24);
               //        *lpdw++ = (((uint32_t)m_keyframe.rbTempo[2])<<16)|
               //                (((uint32_t)m_keyframe.rbTempo[1])<<8)|
               //              ((uint32_t)m_keyframe.rbTempo[0])|
               ///            (((uint32_t)MEVT_TEMPO) << 24);

               lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
            }

            /* Program change events?
            */
            for (idx = 0; idx < 16; idx++)
            {
               if (KF_EMPTY != m_keyframe.rbProgram[idx])
               {
                  if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                     return ENoMemory;

                  *lpdw++ = 0;
                  *lpdw++ = 0;
                  *lpdw++ = (((uint32_t)MEVT_SHORTMSG) << 24)      |
                            ((uint32_t)::music::midi::ProgramChange)         |
                            ((uint32_t)idx)                        |
                            (((uint32_t)m_keyframe.rbProgram[idx]) << 8);

                  lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
               }
            }

            /* Controller events?
            */
            for (idxChannel = 0; idxChannel < 16; idxChannel++)
            {
               for (idxController = 0; idxController < 120; idxController++)
               {
                  if (KF_EMPTY != m_keyframe.rbControl[idxChannel][idxController])
                  {
                     if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                        return ENoMemory;

                     *lpdw++ = 0;
                     *lpdw++ = 0;
                     *lpdw++ = (((uint32_t)MEVT_SHORTMSG << 24)     |
                                ((uint32_t)::music::midi::ControlChange)       |
                                ((uint32_t)idxChannel)               |
                                (((uint32_t)idxController) << 8)     |
                                (((uint32_t)m_keyframe.rbControl[idxChannel][idxController]) << 16));


                     lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
                  }

               }
            }

            /* Force all tracks to be at tkPosition. We are guaranteed that
            ** all tracks will be past the event immediately preceding tkPosition;
            ** this will force correct delta-ticks to be generated so that events
            ** on all tracks will line up properly on a seek into the middle of the
            ** file.
            */
            // ^^^^ This cause problems... tracks are already positioned...
            // changing track position make all the song envents be shifted
            // irregularly...
            //    for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)
            //   for (idxTrack = m_dwTracks; idxTrack--;)
            //  {
            //   ptrk = m_tracks[idxTrack];
            //  ptrk->tkPosition        = tkPosition;
            //}
            //return seek(tkPosition);

            return ::music::success;
         }

         e_result buffer::seek(
            imedia_position                   tkPosition)
         {
            e_result  smfrc;
            ::music::midi::event *     pevent;


            m_ptracks->m_tkPosition = 0;
            m_flags &= ~file::EndOfFile;
            m_ptracks->m_flags &= ~file::EndOfFile;

            m_ptracks->seek_begin();

            while(::music::success == (smfrc = GetNextEvent(pevent, tkPosition, FALSE)));

            if (SReachedTkMax != smfrc)
            {
               return smfrc;
            }

            return ::music::success;
         }


         //e_result buffer::GetNextEvent(::music::midi::event * & pevent, imedia_position tkMax, bool bTkMaxInclusive)
         //{

         //   if (m_flags & file::EndOfFile)
         //   {

         //      return SEndOfFile;

         //   }

         //   if(!(m_flags & file::DisablePlayLevel1Operations))
         //   {
         //      if(m_mepaImmediate.get_size() > 0)
         //      {
         //         ::music::midi::event * peventImmediate = m_mepaImmediate.element_at(0);
         //         *pevent = *peventImmediate;
         //         delete peventImmediate;
         //         m_mepaImmediate.remove_at(0);
         //         return success;
         //      }
         //      if(m_mepaOnQuarterNote.get_size() > 0)
         //      {
         //         imedia_position tkMod = m_ptracks->m_tkPosition  % WORDSWAP(m_pFileHeader->wDivision);
         //         imedia_position tkQuarterNote;
         //         if(tkMod == 0)
         //         {
         //            tkQuarterNote = m_ptracks->m_tkPosition;
         //         }
         //         else
         //         {
         //            tkQuarterNote = m_ptracks->m_tkPosition + WORDSWAP(m_pFileHeader->wDivision) - tkMod;
         //         }
         //         imedia_position tkPosition;
         //         e_result smfrc = GetNextEventTkPosition(&tkPosition, tkMax);
         //         if(smfrc == SEndOfFile)
         //            m_flags |= file::EndOfFile;
         //         if(tkPosition > tkQuarterNote)
         //         {
         //            m_ptracks->m_tkPosition  = tkQuarterNote;
         //            *pevent = *m_mepaOnQuarterNote.element_at(0);
         //            delete m_mepaOnQuarterNote.element_at(0);
         //            m_mepaOnQuarterNote.remove_at(0);
         //            return ::music::success;
         //         }
         //      }
         //   }

         //   e_result smfrc   = m_ptracks->GetNextEvent(pevent, tkMax, bTkMaxInclusive, false, false);

         //   if(!(m_flags & file::DisablePlayLevel1Operations))
         //   {
         //      if(pevent->GetType() == ::music::midi::NoteOn &&
         //         pevent->GetNoteVelocity() != 0)
         //      {
         //         m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack())++;
         //         m_ptracks->m_iaLevel.element_at(pevent->GetTrack())
         //            = pevent->GetNoteVelocity();
         //      }
         //      else if((pevent->GetType() == ::music::midi::NoteOn &&
         //         pevent->GetNoteVelocity() == 0) ||
         //         pevent->GetType() == ::music::midi::NoteOff)
         //      {
         //         int32_t iCount = m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack());
         //         if(iCount > 0)
         //            iCount--;
         //         m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack()) =
         //            iCount;
         //         if(iCount == 0)
         //            m_ptracks->m_iaLevel.element_at(pevent->GetTrack())
         //            = 0;
         //      }
         //   }
         //   if(smfrc == SEndOfFile)
         //      GetFlags().signalize(file::EndOfFile);
         //   return smfrc;



         //}

         //e_result buffer::GetNextEventTkPosition(imedia_position * pTkPosition, imedia_position tkMax)
         //{

         //   if (GetFlags().is_signalized(file::EndOfFile))
         //   {

         //      return SEndOfFile;

         //   }

         //   return m_ptracks->GetNextEventTkPosition(pTkPosition, tkMax);

         //}




         /*
         *   mysmfGetTrackEventData
         *
         *
         *
         *
         ****/
         uint32_t mysmfGetTrackEventData(sp(::music::midi::file::buffer) pSmf, ::music::midi::event * pEvent, BYTE * pData[])
         {

            byte *                  hpbImage;

            uint32_t                index;

            ASSERT(pSmf != NULL);

            ASSERT(pEvent != NULL);

            auto len = pEvent->GetDataSize();

            *pData = (uchar *) LocalAlloc(LPTR,len);

            hpbImage = (byte *) pEvent->GetData();

            for(index = 0; index < len; index++)
            {

               *pData[index] = *hpbImage++;

            }

            return ( uint32_t ) (len);

         }


         e_result buffer::MergeTracks()
         {
            int32_t i;
            uint32_t dwAllocation;
            ::music::midi::track * ptrack;


            dwAllocation = sizeof(CHUNKHDR) + sizeof(MIDIFILEHDR);

            for(i = 0; i < m_ptracks->GetMidiTrackCount(); i++)
            {
               ptrack = m_ptracks->MidiTrackAt(i);
               dwAllocation += ptrack->GetTrackImageLength();
//               ptrack->SetAutoAllocation();
            }

            try
            {
               allocate(dwAllocation);
            }
            catch(memory_exception * pe)
            {
               delete pe;
               return ENoMemory;
            }

            byte * hpbDest, * hpbSrc;

            hpbDest = GetImage();

            hpbDest += sizeof(CHUNKHDR);

            hpbDest += sizeof(MIDIFILEHDR);

            for(i = 0; i < m_ptracks->GetMidiTrackCount(); i++)
            {

               ptrack = m_ptracks->MidiTrackAt(i);
               hpbSrc = ptrack->GetTrackImage();
               memcpy(hpbDest, hpbSrc, ptrack->GetTrackImageLength());
               ptrack->Initialize(m_ptracks);
               ptrack->WriteHeaderLength();
               hpbDest += ptrack->GetTrackImageLength();

            }

            hpbDest = GetImage();

            CHUNKHDR * lpch = (CHUNKHDR *) hpbDest;
            lpch->fourccType = FOURCC_MThd;
            //lpch->dwLength = DWORDSWAP(this->get_size());
            lpch->dwLength = DWORDSWAP(sizeof(MIDIFILEHDR));

            //   m_ptracks->remove_all();
            SetOpened();
            return ::music::success;

         }

         e_result buffer::WorkMergeTracks()
         {
            ::music::midi::track * ptrack;
            for(int32_t i = 0; i < m_ptracks->GetMidiTrackCount(); i++)
            {
               ptrack = m_ptracks->MidiTrackAt(i);
               ptrack->FromWorkTrack(false);
            }
            return MergeTracks();
         }



         //int32_t buffer::CalcMelodyTrack(::music::midi::events **ppEvents, imedia::position_array *pTicks)
         //{
         //    return -1;
         // return m_ptracks->CalcMelodyTrack(ppEvents, pTicks, m_pFileHeader->wFormat);
         //}

         /*int32_t buffer::WorkCalcMelodyTrack(
            ::music::midi::events ** ppEvents,
            imedia::position_array & positiona,
            int_array & iaTokenLine)
         {
            return m_ptracks->WorkCalcMelodyTrack(
               ppEvents,
               positiona,
               iaTokenLine,
               m_pFileHeader->wFormat);
         }*/

         e_result buffer::SaveFile(const char * lpcFilePath)
         {
            HMMIO                   hmmio = (HMMIO)NULL;
            //    e_result               smfrc = ::music::success;
            MMIOINFO                mmioinfo;

            ASSERT(lpcFilePath != NULL);

            LPTSTR lpFilePath = _tcsdup(lpcFilePath);

            /* Verify that the file can be opened or created
            */
            memset(&mmioinfo, 0, sizeof(mmioinfo));

            hmmio = mmioOpen(lpFilePath, &mmioinfo, MMIO_WRITE |MMIO_ALLOCBUF);
            if ((HMMIO)NULL == hmmio)
            {
               TRACE(_T( "smfOpenFile: mmioOpen failed!"));
               return ESaveFailed;
            }

            if(mmioWrite(hmmio, (char _huge* ) GetImage(), GetImageSize()) != (int32_t) GetImageSize())
            {
               TRACE(_T( "smfOpenFile: mmioOpen failed!"));
               return ESaveFailed;
            }

            return ::music::success;
         }

         e_result buffer::SaveFile(::file::file &ar)
         {
            ar.write(GetImage(), GetImageSize());
            return ::music::success;
         }

         bool buffer::IsTune1000File()
         {
            return m_ptracks->IsTune1000File();
         }

         bool buffer::IsSoftKaraokeFile(bool bWork, stringa * pstra)
         {
            return m_ptracks->IsSoftKaraokeFile(bWork, pstra);
         }

         /*buffer::e_result buffer::DeleteEvent(event_base *pEvent)
         {
         buffer::e_result   smfrc;
         if(pEvent->GetImage() > 0)
         {
         if(pEvent->_GetDelta() != 0)
         {
         // if this event being deleted has non-zero delta,
         // the previous event must have it delta corrected;
         event   previousEvent;
         if(buffer::::music::success != (smfrc = GetPreviousEvent(
         &previousEvent,
         pEvent)))
         {
         return smfrc;
         }
         if(buffer::::music::success != (smfrc = ChangeEventDelta(
         &previousEvent,
         previouspevent->m_tkDelta + pEvent->_GetDelta())))
         {
         return smfrc;
         }
         }
         track * pTrack = m_ptracks->GetEventTrack(pEvent);
         if(pTrack == NULL)
         return TrackNotFound;
         pTrack->m_smti.m_cbLength -= pEvent->GetImageSize();
         pTrack->WriteHeaderLength();
         m_cbImage -= pEvent->GetImageSize();
         memmove(
         pEvent->GetImage(),
         pEvent->GetImage() + pEvent->GetImageSize(),
         m_cbImage - (pEvent->GetImage() - m_hpbImage) - pEvent->GetImageSize());
         if(buffer::::music::success != (smfrc = BuildIndex()))
         {
         return smfrc;
         }

         return buffer::::music::success;
         }
         else
         {
         return buffer::::music::success;
         }

         } */

         //e_result buffer::GetPreviousEvent(
         //   ::music::midi::event  * pPreviousEvent,
         //   ::music::midi::event  * pEvent)
         //{
         //   UNREFERENCED_PARAMETER(pPreviousEvent);

         //   e_result               smfrc;
         //   ::music::midi::event *           pevent = NULL;
         //   ::music::midi::event *           peventPrevious = NULL;

         //   m_ptracks->m_tkPosition = 0;
         //   GetFlags().unsignalize(file::EndOfFile);
         //   m_ptracks->GetFlags().unsignalize(file::EndOfFile);

         //   m_ptracks->seek_begin();
         //   //for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)

         //   while(::music::success == (smfrc = GetNextEvent(pevent, pEvent->GetPosition(), TRUE)))
         //   {
         //      if(pevent->m_pImage == pEvent->m_pImage)
         //      {
         //         pEvent->operator =(*peventPrevious);
         //         return ::music::success;
         //      }
         //      peventPrevious = pevent;
         //   }

         //   if (SReachedTkMax != smfrc)
         //   {
         //      return smfrc;
         //   }

         //   return ::music::success;

         //}

         e_result buffer::ReplaceSameDeltaEvent(::music::midi::event &eventNew)
         {
            int32_t iCurrentTrack = m_ptracks->m_iCurrentTrack;
            if(iCurrentTrack < 0)
               return EError;
            ::music::midi::track * pTrkMidi = (::music::midi::track *) m_ptracks->TrackAt(iCurrentTrack);
            ASSERT(pTrkMidi->GetType() == ::music::midi::track_base::TypeMidi);
            int32_t iCurrentEvent = pTrkMidi->GetCurrentEvent();
            if(iCurrentEvent < 0)
               return EError;

            ::music::midi::track_v8 & trackWork = pTrkMidi->GetWorkTrack();

            ::music::midi::event & event = trackWork.EventAt(iCurrentEvent);

            event = eventNew;
            return ::music::success;
         }
         /*e_result ReplaceSameDeltaEvent(::music::midi::event *pEventOld, CMidiEventV007 *pEventNew)
         {
         ASSERT(pEventOld->_GetDeltaTicks() == pEventNew->_GetDeltaTicks());
         ASSERT(pEventNew->IsAutoAllocated());
         track * pTrack = m_ptracks->GetEventTrack(pEventOld);
         if(pTrack == NULL)
         return TrackNotFound;
         byte * hpbImage = pEventOld->GetImage();
         byte * hpbFileImage = m_hpbImage;
         if(pEventOld->GetImageSize() != pEventNew->m_cbImage)
         {
         int32_t iAddUp = pEventNew->m_cbImage - pEventOld->GetImageSize();
         pEventNew->m_pTrack = pTrack;
         allocate_add_up(iAddUp);
         hpbImage += hpbFileImage - m_hpbImage;
         pTrack->m_smti.m_cbLength += iAddUp;
         pTrack->WriteHeaderLength();
         memmove(
         hpbImage + iAddUp,
         hpbImage,
         m_cbImage - (hpbImage - m_hpbImage));
         m_cbImage += iAddUp;
         }
         memcpy(hpbImage, pEventNew->GetImage(), pEventNew->m_cbImage);

         WriteHeader();

         return ::music::success;*/

         /*   ASSERT(pEventOld->_GetDeltaTicks() == pEventNew->_GetDeltaTicks());
         ASSERT(pEventNew->IsAutoAllocated());
         track * pTrack = m_ptracks->GetEventTrack(pEventOld);
         if(pTrack == NULL)
         return TrackNotFound;
         byte * hpbImage = pEventOld->GetImage();
         byte * hpbFileImage = m_hpbImage;
         if(pEventOld->GetImageSize() != pEventNew->m_cbImage)
         {
         int32_t iAddUp = pEventNew->m_cbImage - pEventOld->GetImageSize();
         pEventNew->m_pTrack = pTrack;
         allocate_add_up(iAddUp);
         hpbImage += hpbFileImage - m_hpbImage;
         pTrack->m_smti.m_cbLength += iAddUp;
         pTrack->WriteHeaderLength();
         memmove(
         hpbImage + iAddUp,
         hpbImage,
         m_cbImage - (hpbImage - m_hpbImage));
         m_cbImage += iAddUp;
         }
         memcpy(hpbImage, pEventNew->GetImage(), pEventNew->m_cbImage);

         WriteHeader();

         return ::music::success;

         }*/

         /*e_result ChangeEventDelta(::music::midi::event *pEvent, imedia_position tkNewDelta)
         {
         e_result smfrc;
         if(pEvent->_GetDelta() != tkNewDelta)
         {
         track * pTrack = m_ptracks->GetEventTrack(pEvent);
         if(pTrack == NULL)
         return TrackNotFound;
         BYTE buffer[256];
         uint32_t dwSetUsed = ::music::midi::event::SetVDWord(buffer, 256, tkNewDelta);
         imedia_position tkOldDelta;
         uint32_t dwGetUsed = ::music::midi::event::GetVDWord(pEvent->GetImage(), 256, &tkOldDelta);
         ASSERT(tkOldDelta == pEvent->_GetDelta());
         byte * hpbImage = pEvent->GetImage();
         if(dwSetUsed != dwGetUsed)
         {
         int32_t iAddUp = dwSetUsed - dwGetUsed;
         if(iAddUp > 0)
         {
         if(!allocate(m_cbImage + iAddUp))
         {
         TRACE("DeleteEvent No Memory for deleting\n");
         return ::music::midi::ENoMemory;
         }
         }
         pTrack->m_smti.m_cbLength += iAddUp;
         pTrack->WriteHeaderLength();
         memmove(
         hpbImage + iAddUp,
         hpbImage,
         m_cbImage - (hpbImage - m_hpbImage));
         m_cbImage += iAddUp;
         if(::music::success != (smfrc = BuildIndex()))
         {
         return smfrc;
         }
         }
         ::music::midi::event::SetVDWord(hpbImage, dwSetUsed, tkNewDelta);
         return ::music::success;
         }
         else
         return ::music::success;

         }*/

         e_result buffer::WriteHeader(MIDIFILEHDR * lpmfh)
         {
            m_pMThd = (CHUNKHDR *) GetImage();
            //   uint32_t dwLength = m_cbImage - sizeof(CHUNKHDR);
            //  m_pMThd->dwLength = DWORDSWAP(dwLength);
            m_pMThd->fourccType = FOURCC_MThd;
            m_pMThd->dwLength = DWORDSWAP(sizeof(MIDIFILEHDR));
            m_pFileHeader = (LPMIDIFILEHDR) (GetImage() + sizeof(CHUNKHDR));
            memcpy(m_pFileHeader, lpmfh, sizeof(MIDIFILEHDR));
            return ::music::success;
         }



         bool buffer::IsXFFile()
         {
            m_ptracks->seek_begin();
            return m_ptracks->IsXFFile();
         }

         int32_t buffer::SetKeyShift(int32_t iKeyShift)
         {
            m_iKeyShift = iKeyShift;
            return true;
         }

         int32_t buffer::GetKeyShift()
         {
            return m_iKeyShift;
         }


         void buffer::FromWorkStorage()
         {
            m_ptracks->FromWorkStorage();

         }
         void buffer::ToWorkStorage()
         {
            m_ptracks->ToWorkStorage();
         }


         ::music::e_result buffer::SetTempoShift(int32_t dTempoShift)
         {

            return ::music::midi::file::buffer::SetTempoShift(dTempoShift);

         }


         double buffer::GetTempoShift()
         {

            return ::music::midi::file::buffer::GetTempoShift();

         }


         double buffer::GetTempoShiftRate()
         {

            return ::music::midi::file::buffer::GetTempoShiftRate();

         }


         ::music::midi::tempo_map_entry * buffer::GetTempoMapEntry(const imedia_position & tk)
         {
            int32_t idx;
            for (idx = 0; idx < m_tempomap.get_size(); idx++)
            {
               ::music::midi::tempo_map_entry & tempo = m_tempomap.element_at(idx);
               if((int32_t) tk < (int32_t) tempo.tkTempo)
                  break;
            }
            if(idx <= 0)
               return NULL;
            else
               return &m_tempomap.element_at(idx - 1);
         }

         // event must have space to put event data
         ::music::e_result buffer::GetTempoEvent(::music::midi::event &event)
         {
            ::music::midi::tempo_map_entry * pmtme = GetTempoMapEntry(m_ptracks->m_tkPosition);
            if(pmtme != NULL)
            {
               double dTempo;
               dTempo = pmtme->dwTempo;
               //dTempo = dTempo / GetTempoShiftRate();
               union
               {
                  uint32_t   dw;
                  BYTE    ba[4];
               } uA, uB;
               uA.dw = (uint32_t) dTempo;
               uB.ba[0] = uA.ba[2];
               uB.ba[1] = uA.ba[1];
               uB.ba[2] = uA.ba[0];

               event.SetFullType(::music::midi::Meta);
               event.SetMetaType(::music::midi::MetaTempo);
               event.SetData(&uB.dw, 3);
            }
            return ::music::success;

         }

         ::music::e_result buffer::InsertTempoEvent()
         {
            ::music::midi::tempo_map_entry * pmtme = GetTempoMapEntry(m_ptracks->m_tkPosition);
            if(pmtme != NULL)
            {
               ::music::midi::event * pevent = new ::music::midi::event();
               GetTempoEvent(*pevent);
               m_mepaOnQuarterNote.add(pevent);
            }
            return ::music::success;
         }

         ::music::e_result buffer::Format0()
         {
            MIDIFILEHDR fileHdr;

            fileHdr.wFormat = WORDSWAP(0);
            fileHdr.wDivision = m_pFileHeader->wDivision ;
            fileHdr.wTracks = WORDSWAP(1) ;

            ::music::midi::track * pTrackUnion = new ::music::midi::track(get_app());
            pTrackUnion->Initialize(m_ptracks);
            m_ptracks->CompactTracks(*pTrackUnion, true, true);

            m_ptracks->RemoveAllMidiTracks();
            ::music::midi::track * ptrack = m_ptracks->CreateTrack();
            *ptrack = *pTrackUnion;


            e_result smfrc;
            if(MMSYSERR_NOERROR != (smfrc = MergeTracks()))
               return smfrc;

            WriteHeader(&fileHdr);

            if(MMSYSERR_NOERROR != (smfrc = Build()))
               return smfrc;

            return ::music::success;
         }

         LPBYTE buffer::GetImage()
         {
            return get_data();
         }

         uint32_t buffer::GetImageSize()
         {
            return (uint32_t) this->get_size();
         }

         void buffer::OnStop()
         {
            m_ptracks->OnStop();
         }


         ::multimedia::e_result buffer::ImmediatePutTempoChange()
         {

            ::music::midi::event * pevent = new ::music::midi::event;

            GetTempoEvent(*pevent);

            m_mepaImmediate.add(pevent);

            return ::multimedia::result_success;

         }



         e_result buffer::WorkStreamRender(LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNominalMax)
         {
            e_result       smfrc;
            ::music::midi::event *           pevent;
            array < ::music::midi::event *, ::music::midi::event * > eventptraPositionCB;
            LPDWORD                    lpdw;
            imedia_position                        tkDelta;
            imedia_position                        tkLastDelta = 0 ;
            imedia_position                      tkOnQuarterNoteDelta = 0;
            //   imedia_position                        tkLastPosition;
            imedia_position      tkPosition;
            imedia_position      tkPositionF1;
            imedia_position &    tkLastPosition = m_positionLastWorkRender;
            ASSERT(lpmh != NULL);

            /////////////////////////////////////////////////////////////////////////////
            // read events from the track and pack them into the buffer in polymsg
            // format.
            //
            // If a SysEx or meta would go over a buffer boundry, split it.
            /////////////////////////////////////////////////////////////////////////////

            lpmh->dwBytesRecorded = 0;
            if(m_dwPendingUserEvent)
            {
               smfrc = InsertParmData((imedia_position)0, lpmh);
               if(::music::success != smfrc)
               {
                  TRACE( "smfInsertParmData() -> %u", (uint32_t)smfrc);
                  return smfrc;
               }
            }

            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

            if (m_flags & file::EndOfFile)
            {
               return SEndOfFile;
            }

            if(tkLastPosition > GetPosition())
            {
               tkLastPosition = GetPosition();
            }
            while(true)
            {
               ASSERT(lpmh->dwBytesRecorded <= lpmh->dwBufferLength);
               if(lpmh->dwBytesRecorded > cbPrerollNominalMax && lpmh->dwBytesRecorded > 0
                     && eventptraPositionCB.get_size() <= 0)
               {
                  break;
               }

               // If we know ahead of time we won't have room for the
               // event, just break out now. We need 2 uint32_t's for the
               // terminator event and at least 2 uint32_t's for any
               // event we might store - this will allow us a full
               // int16_t event or the delta time and stub for a long
               // event to be split.
               //
               //


               // now 16 DWORDS
               //
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
                  break;
               }

               tkLastDelta = 0;

               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {
                  //         m_ptracks->m_tkPosition += tkDelta;
                  break;
               }

               smfrc = WorkGetNextEvent(pevent, tkMax, TRUE);
               if (::music::success != smfrc)
               {
                  // smfGetNextEvent doesn't set this because smfSeek uses it
                  // as well and needs to distinguish between reaching the
                  // seek point and reaching end-of-file.
                  //
                  // To the ::fontopus::user, however, we present the selection between
                  // their given tkBase and tkEnd as the entire file, therefore
                  // we want to translate this into EOF.
                  //
                  return smfrc;

                  TRACE( "smfReadEvents: ReadEvents() -> %u", (uint32_t)smfrc);
                  break;
               }

               ASSERT(GetPosition() >= tkLastPosition);

               tkPosition  = GetPosition();
               tkDelta     = tkPosition - tkLastPosition;

               // The position CB events are grouped in a single position CB
               // event after other type of simultaneous events.
               // The {if block} change the order of simultaneous events.
               // position CB events ( pevent->GetFlags() & 1 )
               // are streamed together in a single position CB event, through
               // StreamEventF1 member function.
               // These position CB events are put after any other kind of event
               // that have the same position.
               // The order of the non PositionCB events are not altered.

               if(pevent->GetFlags() & 1)
               {
                  if(eventptraPositionCB.get_size() <= 0)
                  {
                     // This is the first PositionCB event
                     // at the F1 position.
                     eventptraPositionCB.add(pevent);
                     tkPositionF1   = tkPosition;
                  }
                  else if(tkPosition == tkPositionF1)
                  {
                     // This is another PositionCB event
                     // at the F1 position.
                     eventptraPositionCB.add(pevent);
                  }
                  else
                  {
                     // This is another PositionCB event
                     // at another position.

                     ASSERT(tkPositionF1 >= tkLastPosition);

                     tkDelta = tkPositionF1 - tkLastPosition;
                     StreamEventF1(tkDelta, eventptraPositionCB, lpmh, tkMax, cbPrerollNominalMax);
                     tkLastPosition = tkPositionF1;
                     eventptraPositionCB.remove_all();

                     eventptraPositionCB.add(pevent);
                     tkPositionF1   = tkPosition;
                  }
               }
               else
               {
                  if(eventptraPositionCB.get_size() > 0)
                  {
                     if(pevent->GetPosition() != eventptraPositionCB[0]->GetPosition())
                     {
                        ASSERT(tkPositionF1 >= tkLastPosition);

                        tkDelta = tkPositionF1 - tkLastPosition;
                        StreamEventF1(tkDelta, eventptraPositionCB, lpmh, tkMax, cbPrerollNominalMax);
                        tkDelta = tkPosition - tkPositionF1;
                        eventptraPositionCB.remove_all();
                     }
                     else
                     {
                        // The event to be streamed in the following StreamEvent function
                        // has the same position of the eventptraPositionCB events.
                        // So, when these eventptraPositionCB events are streamed,
                        // the event delta will be zero.
                        tkDelta = tkPosition - tkPositionF1;
                        tkPositionF1   = tkPosition;
                     }
                  }
                  /*         if(pevent->GetType() == NoteOn)
                  {
                  TRACE("WorkStreamRender: NoteOn     position %d Delta %d Track %d\n",
                  m_ptracks->GetPosition(),
                  tkDelta,
                  pevent->GetTrack());
                  }
                  else
                  {
                  TRACE("WorkStreamRender: Type Other position %d Delta %d\n",
                  m_ptracks->GetPosition(),
                  tkDelta);
                  }*/

                  StreamEvent(tkDelta, pevent, lpmh, tkMax, cbPrerollNominalMax);
                  tkLastPosition = tkPosition;
               }

            }

            return (m_flags & file::EndOfFile) ? SEndOfFile : ::music::success;
         }

         //e_result buffer::WorkGetNextRawEvent(
         //   ::music::midi::event *&      pevent,
         //   imedia_position                tkMax,
         //   bool                  bTkMaxInclusive)
         //{
         //   if(GetFlags().is_signalized(file::EndOfFile))
         //   {
         //      return SEndOfFile;
         //   }
         //   e_result smfrc = m_ptracks->WorkGetNextRawEvent(
         //      pevent,
         //      tkMax,
         //      bTkMaxInclusive,
         //      false);
         //   if(smfrc == SEndOfFile)
         //   {
         //      GetFlags().signalize(file::EndOfFile);
         //   }
         //   return smfrc;
         //}

         //e_result buffer::WorkGetNextRawMidiEvent(::music::midi::event * & pevent)
         //{
         //   if(GetFlags().is_signalized(file::EndOfFile))
         //   {
         //      return SEndOfFile;
         //   }
         //   e_result smfrc = m_ptracks->WorkGetNextRawMidiEvent(pevent);
         //   if(smfrc == SEndOfFile)
         //   {
         //      GetFlags().signalize(file::EndOfFile);
         //   }
         //   return smfrc;
         //}

         ////e_result buffer::WorkGetNextRawMidiEvent(
         ////   ::music::midi::event *&      pevent,
         ////   imedia_position                tkMax,
         ////   bool                  bTkMaxInclusive)
         ////{
         ////   if (GetFlags().is_signalized(file::EndOfFile))
         ////   {
         ////      return SEndOfFile;
         ////   }
         ////   e_result smfrc = m_ptracks->WorkGetNextRawMidiEvent(
         ////      pevent,
         ////      tkMax,
         ////      bTkMaxInclusive);
         ////   if (smfrc == SEndOfFile)
         ////   {
         ////      GetFlags().signalize(file::EndOfFile);
         ////   }
         ////   return smfrc;
         ////}

         //e_result buffer::WorkGetNextEvent(
         //   ::music::midi::event *&      pevent,
         //   imedia_position                tkMax,
         //   bool                  bTkMaxInclusive)
         //{
         //   if (GetFlags().is_signalized(file::EndOfFile))
         //   {
         //      return SEndOfFile;
         //   }
         //   // XXX
         //   /*   if(!GetFlags().is_signalized(DisablePlayLevel1Operations))
         //   {
         //   if(m_mepaImmediate.get_size() > 0)
         //   {
         //   event * peventImmediate = m_mepaImmediate.element_at(0);
         //   pevent = peventImmediate;
         //   delete peventImmediate;
         //   m_mepaImmediate.remove_at(0);
         //   return ::music::success;
         //   }
         //   if(m_mepaOnQuarterNote.get_size() > 0)
         //   {
         //   imedia_position tkMod = m_ptracks->m_tkPosition  % WORDSWAP(m_pFileHeader->wDivision);
         //   imedia_position tkQuarterNote;
         //   if(tkMod == 0)
         //   {
         //   tkQuarterNote = m_ptracks->m_tkPosition;
         //   }
         //   else
         //   {
         //   tkQuarterNote = m_ptracks->m_tkPosition + WORDSWAP(m_pFileHeader->wDivision) - tkMod;
         //   }
         //   imedia_position tkPosition;
         //   e_result smfrc = GetNextEventTkPosition(&tkPosition, tkMax);
         //   if(smfrc == SEndOfFile)
         //   GetFlags().signalize(file::EndOfFile);
         //   if(tkPosition > tkQuarterNote)
         //   {
         //   m_ptracks->m_tkPosition  = tkQuarterNote;
         //   pevent = m_mepaOnQuarterNote.element_at(0);
         //   delete m_mepaOnQuarterNote.element_at(0);
         //   m_mepaOnQuarterNote.remove_at(0);
         //   return ::music::success;
         //   }
         //   }
         //   }*/

         //   e_result smfrc;
         //   while(true)
         //   {
         //      smfrc   = m_ptracks->WorkGetNextEvent(
         //         pevent,
         //         tkMax,
         //         bTkMaxInclusive,
         //         false);
         //      if(smfrc != ::music::success)
         //      {
         //         break;
         //      }
         //      //      if(GetFlags().is_signalized(DisablePlayLevel1Operations))
         //      //    {
         //      //     break;
         //      //      }
         //      if(GetFlags().is_signalized(file::DisableMute))
         //      {
         //         break;
         //      }
         //      else
         //      {
         //         if(pevent->GetType() == ::music::midi::NoteOn
         //            && pevent->GetNoteVelocity() > 0
         //            && m_iaMuteTrack.contains(pevent->GetTrack()))
         //         {
         //            ASSERT(TRUE);
         //         }
         //         else
         //         {
         //            break;
         //         }
         //      }
         //   }

         //   if(!GetFlags().is_signalized(file::DisablePlayLevel1Operations))
         //   {
         //      if(pevent->GetType() == ::music::midi::NoteOn &&
         //         pevent->GetNoteVelocity() != 0)
         //      {
         //         m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack())++;
         //         m_ptracks->m_iaLevel.element_at(pevent->GetTrack())
         //            = pevent->GetNoteVelocity();
         //      }
         //      else if((pevent->GetType() == ::music::midi::NoteOn &&
         //         pevent->GetNoteVelocity() == 0) ||
         //         pevent->GetType() == ::music::midi::NoteOff)
         //      {
         //         int32_t iCount = m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack());
         //         if(iCount > 0)
         //            iCount--;
         //         m_ptracks->m_iaNoteOn.element_at(pevent->GetTrack()) =
         //            iCount;
         //         if(iCount == 0)
         //            m_ptracks->m_iaLevel.element_at(pevent->GetTrack())
         //            = 0;
         //      }
         //   }
         //   if(smfrc == SEndOfFile)
         //   {
         //      GetFlags().signalize(file::EndOfFile);
         //   }
         //   return smfrc;



         //}


         e_result buffer::WorkSeek(
            imedia_position                   tkPosition,
            LPMIDIHDR               lpmh)
         {

            e_result        smfrc;
            ::music::midi::event *           pevent;
            LPDWORD                    lpdw;
            BYTE                       bEvent;
            uint32_t                       idx;
            uint32_t                       idxChannel;
            uint32_t                       idxController;

            _SyncSeek(tkPosition);

            ::lemon::set_maximum(m_positionLastWorkRender);

            memset(&m_keyframe, 0xFF, sizeof(m_keyframe));
            memset(&m_keyframe.rbProgram, 0x00, sizeof(m_keyframe.rbProgram));

            m_ptracks->m_tkPosition = 0;
            m_flags &= ~file::EndOfFile;
            m_ptracks->m_flags &= ~file::EndOfFile;

            m_ptracks->WorkSeekBegin();

            while (::music::success == (smfrc = WorkGetNextEvent(pevent, tkPosition, FALSE)))
            {
               if (::music::midi::Meta == (bEvent = pevent->GetFullType()))
               {
                  if (pevent->GetMetaType() == ::music::midi::MetaTempo)
                  {
                     if (pevent->GetDataSize() != sizeof(m_keyframe.rbTempo))
                        return EInvalidFile;

                     memcpy((byte *)m_keyframe.rbTempo, pevent->GetData(), pevent->GetDataSize());
                  }
               }
               if((bEvent & 0xF0) == ::music::midi::ProgramChange)
               {
                  m_keyframe.rbProgram[bEvent & 0x0F] = pevent->GetChB1();
               }
               else if((bEvent & 0xF0) == ::music::midi::ControlChange)
               {
                  m_keyframe.rbControl[bEvent & 0x0F][pevent->GetChB1()] = pevent->GetChB2();
               }
            }

            if (SReachedTkMax != smfrc)
            {
               return smfrc;
            }

            /* Build lpmh from keyframe
            */
            //lpmh->dwBytesRecorded = 0;
            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

            /* Tempo change event?
            */
            if (KF_EMPTY != m_keyframe.rbTempo[0] ||
                  KF_EMPTY != m_keyframe.rbTempo[1] ||
                  KF_EMPTY != m_keyframe.rbTempo[2])
            {
               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                  return ENoMemory;

               *lpdw++ = 0;
               *lpdw++ = 0;
               uint32_t dwTempo =   (((uint32_t)m_keyframe.rbTempo[0])<<16)|
                                    (((uint32_t)m_keyframe.rbTempo[1])<<8)|
                                    ((uint32_t)m_keyframe.rbTempo[2]);
               double dTempoShiftRate = GetTempoShiftRate();
               dwTempo = (uint32_t) ((double) dwTempo / dTempoShiftRate);
               uint32_t dw = (((uint32_t)MEVT_TEMPO)<<24)| dwTempo;

               *lpdw++ = dw;

               lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
            }

            /* Program change events?
            */
            for (idx = 0; idx < 16; idx++)
            {
               if (KF_EMPTY != m_keyframe.rbProgram[idx])
               {
                  if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                     return ENoMemory;

                  *lpdw++ = 0;
                  *lpdw++ = 0;
                  *lpdw++ = (((uint32_t)MEVT_SHORTMSG) << 24)      |
                            ((uint32_t)::music::midi::ProgramChange)         |
                            ((uint32_t)idx)                        |
                            (((uint32_t)m_keyframe.rbProgram[idx]) << 8);

                  lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
               }
            }

            /* Controller events?
            */
            for (idxChannel = 0; idxChannel < 16; idxChannel++)
            {
               for (idxController = 0; idxController < 120; idxController++)
               {
                  if (KF_EMPTY != m_keyframe.rbControl[idxChannel][idxController])
                  {
                     if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(uint32_t))
                        return ENoMemory;

                     *lpdw++ = 0;
                     *lpdw++ = 0;
                     *lpdw++ = (((uint32_t)MEVT_SHORTMSG << 24)     |
                                ((uint32_t)ControlChange)       |
                                ((uint32_t)idxChannel)               |
                                (((uint32_t)idxController) << 8)     |
                                (((uint32_t)m_keyframe.rbControl[idxChannel][idxController]) << 16));


                     lpmh->dwBytesRecorded += 3*sizeof(uint32_t);
                  }

               }
            }

            /* Force all tracks to be at tkPosition. We are guaranteed that
            ** all tracks will be past the event immediately preceding tkPosition;
            ** this will force correct delta-ticks to be generated so that events
            ** on all tracks will line up properly on a seek into the middle of the
            ** file.
            */
            // ^^^^ This cause problems... tracks are already positioned...
            // changing track position make all the song envents be shifted
            // irregularly...
            //    for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)
            //   for (idxTrack = m_dwTracks; idxTrack--;)
            //  {
            //   ptrk = m_tracks[idxTrack];
            //  ptrk->tkPosition        = tkPosition;
            //}
            //return seek(tkPosition);

            return ::music::success;
         }

         /*         e_result buffer::CreateTempoMap()
                  {
                     bool bFirst = true;
                     ::music::midi::tempo_map_entry tempo;
                     e_result smfrc;
                     ::music::midi::event * pevent;
                     m_ptracks->WorkSeekBegin();
                     while (::music::success
                        == (smfrc = WorkGetNextRawMidiEvent(pevent, MAX_TICKS, TRUE)))
                     {
                        if (::music::midi::Meta == pevent->GetFullType() &&
                           ::music::midi::MetaTempo == pevent->GetMetaType() &&
                           (pevent->GetFlags() != 1))
                        {
                           if (3 != pevent->GetParamSize())
                           {
                              return EInvalidFile;
                           }

                           if (bFirst && m_ptracks->m_tkPosition != 0)
                           {
                              /* Inserting first event and the absolute time is zero.
                              ** Use defaults of 500,000 uSec/qn from MIDI spec
                              */

         /*tempo.tkTempo = 0;
         tempo.msBase  = 0;
         tempo.dwTempo = ::music::midi::DefaultTempo;
         m_tempomap.add(tempo);


         bFirst = false;
         }

         tempo.tkTempo = m_ptracks->m_tkPosition;

         if (bFirst)
         tempo.msBase = 0;
         else
         {
         ::music::midi::tempo_map_entry * pLastTempo = &m_tempomap.element_at(m_tempomap.get_size() - 1) ;
         /* NOTE: Better not be here unless we're q/n format!
         */
         //                ptempo->msBase = (ptempo-1)->msBase +
         //                               muldiv32(ptempo->tkTempo-((ptempo-1)->tkTempo),
         //                                      (ptempo-1)->dwTempo,
         //                                    1000L*m_dwTimeDivision);
         /*tempo.msBase = (pLastTempo)->msBase +
            muldiv32((int32_t) (tempo.tkTempo-((pLastTempo)->tkTempo)),
            (pLastTempo)->dwTempo,
            1000L * m_dwTimeDivision);
         }

         tempo.dwTempo = (((uint32_t)pevent->GetParam()[0])<<16) |
         (((uint32_t)pevent->GetParam()[1])<<8) |
         ((uint32_t)pevent->GetParam()[2]);

         m_tempomap.add(tempo);
         }
         }
         if (0 == m_tempomap.get_size())
         {
         TRACE("File contains no tempo map! Insert default tempo.");

         ::music::midi::tempo_map_entry * ptempo = &m_tempomap.add_new();
         ptempo->tkTempo = 0;
         ptempo->msBase  = 0;
         //      ptempo->dwTempo = DefaultTempo;
         ptempo->dwTempo = ::music::midi::DefaultTempo;
         }

         return ::music::success;

         }*/

         //e_result buffer::CalcTkLength()
         //{
         //   m_tkLength = m_ptracks->CalcTkLength();
         //   return ::music::success;

         //}

         //e_result buffer::WorkSeekBegin()
         //{
         //   m_ptracks->WorkSeekBegin();
         //   GetFlags().unsignalize(file::EndOfFile);
         //   m_tkLastDelta = 0;
         //   ::lemon::set_maximum(m_positionLastWorkRender);
         //   return ::music::success;
         //}

         void buffer::MuteTrack(int32_t iIndex, bool bMute)
         {
            if(bMute)
            {
               m_iaMuteTrack.add_unique(iIndex);
            }
            else
            {
               m_iaMuteTrack.remove(iIndex);
            }
         }

         void buffer::MuteAll(bool bMute, int32_t iExcludeTrack)
         {
            for(int32_t i = 0; i < 16; i++)
            {
               if(i == iExcludeTrack)
               {
                  MuteTrack(i, !bMute);
               }
               else
               {
                  MuteTrack(i, bMute);
               }
            }
         }

         bool buffer::IsTrackMute(int32_t iIndex)
         {
            return m_iaMuteTrack.contains(iIndex);
         }


         bool buffer::IsNull()
         {
            if(this == NULL)
               return true;
            return false;
         }

         void buffer::WorkSeekEnd()
         {
            GetTracks().WorkSeekEnd();
         }

         void buffer::AttachSeekSync(::ikaraoke::events_tracks_v1 *ptracksv1)
         {
            m_evtrkv1ptraSeek.add(ptracksv1);
         }

         void buffer::_SyncSeek(imedia_position tkPosition)
         {
            for(int32_t i = 0; i < m_evtrkv1ptraSeek.get_size(); i++)
            {
               _SyncSeek(tkPosition, m_evtrkv1ptraSeek.element_at(i));
            }
         }

         void buffer::_SyncSeek(
            imedia_position tkPosition,
            ::ikaraoke::events_tracks_v1 *ptracksv1)
         {
            ASSERT(ptracksv1 != NULL);
            ::ikaraoke::lyric_events_v1 * pEventsV1;
            ::ikaraoke::lyric_events_v2 * pEventsV2 = NULL;

            ::index iTokenIndex = -1;
            ::index iNoteIndex = -1;
            ::index i;
            for(i = 0; i < ptracksv1->get_size(); i++)
            {
               pEventsV1 =   ptracksv1->get_at(i);
               if(pEventsV1->GetClassOrder() == 2)
               {
                  pEventsV2 = (::ikaraoke::lyric_events_v2 *) pEventsV1;
                  iNoteIndex = pEventsV2->GetPositionNote(tkPosition);
                  if(iNoteIndex >= 0)
                  {
                     break;
                  }
                  iTokenIndex = pEventsV2->GetPositionToken(tkPosition);
                  if(iTokenIndex >= 0)
                  {
                     break;
                  }
               }
            }
            if(iTokenIndex < 0 && iNoteIndex < 0)
            {
               iTokenIndex = 0;
               iNoteIndex = 0;
            }
            else if(iNoteIndex < 0)
            {
               iNoteIndex = pEventsV2->GetTokenFirstNote(iTokenIndex);
               if(iNoteIndex < 0)
                  iNoteIndex = 0;
            }
            else if(iTokenIndex < 0)
            {
               iTokenIndex = pEventsV2->GetNoteFirstToken(iNoteIndex);
               if(iTokenIndex < 0)
                  iTokenIndex = 0;
            }
            for(i = 0; i < ptracksv1->get_size(); i++)
            {
               ptracksv1->get_at(i)->m_iCurrentToken = iTokenIndex;
               ptracksv1->get_at(i)->m_iCurrentNote = iNoteIndex;
            }
         }



         imedia_position buffer::GetPosition()
         {
            return m_ptracks->GetPosition();
         }

         int32_t buffer::GetDivision()
         {
            return WORDSWAP(m_pFileHeader->wDivision);
         }

         int32_t buffer::GetFormat()
         {
            return WORDSWAP(m_pFileHeader->wFormat);
         }


         //e_result buffer::WorkWriteXFTracks(
         //   stringa &  tokena,
         //   imedia::position_array & positiona,
         //   ::music::xf::info_header & xfinfoheader)
         //{
         //   ::music::midi::tracks & tracka = GetTracks();

         //   while(tracka.GetTrackCount() > 1)
         //   {
         //      tracka.remove_at(1);
         //   }



         //   ::music::midi::track & track = *(class ::music::midi::track *) tracka.TrackAt(0);
         //   if(!track.IsXFFile())
         //   {
         //      BYTE XFVERSIONID[] = {
         //         0x43,  // YAMAHA ID
         //         0x7B,  //
         //         0x00,  //
         //         0x58,  // X
         //         0x46,  // F
         //         0x30,  // 0
         //         0x32,   // 2
         //         0x00,
         //         0x11};
         //      track.seek_begin();
         //      ::music::midi::event eventV008;
         //      eventV008.SetPosition(0);
         //      eventV008.SetFullType(::music::midi::Meta);
         //      eventV008.SetMetaType(::music::midi::MetaSeqSpecific);
         //      eventV008.SetData(XFVERSIONID, sizeof(XFVERSIONID));
         //      track.GetWorkTrack().insert_at(0, eventV008);
         //   }

         //   ::music::xf::info_header xfInfoHeader;

         //   class ::music::midi::track * pmiditrack = tracka.CreateTrack(FOURCC_XFIH);

         //   pmiditrack->WorkWriteXFInfoHeader(
         //      NULL,
         //      &xfinfoheader,
         //      NULL);
         //   pmiditrack->GetEvent().SetFullType(::music::midi::Meta);
         //   pmiditrack->GetEvent().SetMetaType(::music::midi::MetaEOT);
         //   pmiditrack->GetEvent().SetData(NULL, 0);
         //   pmiditrack->GetEvent().SetDelta(0);
         //   pmiditrack->WorkWriteEvent();

         //   pmiditrack = tracka.CreateTrack(FOURCC_XFIH);

         //   pmiditrack->WriteXFLyricTrack(tokena, positiona, "L1");

         //   return ::music::success;
         //}


         //bool buffer::IsOpened()
         //{
         //   return GetFlags().is_signalized(file::Opened);
         //}

         //void buffer::SetOpened(bool bOpened)
         //{
         //   if(bOpened)
         //   {
         //      GetFlags().signalize(file::Opened);
         //   }
         //   else
         //   {
         //      GetFlags().unsignalize(file::Opened);
         //   }
         //}


      } //namespace mmsystem


   } // namespace midi


} // namespace music









