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

            m_iKeyShift             = 0;
            m_dTempoShift           = 0;


            m_pFileHeader = NULL;

            keep_pointer((void **) &m_pMThd);
            keep_pointer((void **) &m_pFileHeader);

         }

         buffer::~buffer()
         {

            close_file();

            if(m_estorage == ::music::storage_attach)
            {
               detach();
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

         e_result buffer::InsertPadEvent(imedia_position tkDelta, LPMIDIHDR lpmh)
         {

            LPDWORD     lpdw;
            uint32_t    dwRounded;
            uint32_t    dwLength;


            ASSERT(tkDelta >= 0);
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


         e_result buffer::WorkStreamRender(LPMIDIHDR lpmh, imedia_position tkMax, uint32_t cbPrerollNominalMax)
         {

            e_result                   smfrc;
            imedia_position            tkDelta;
            imedia_position            tkLastPosition;

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

            if (m_flags & file::EndOfFile)
            {

               return SEndOfFile;

            }

            tkLastPosition = GetPosition();

            while(true)
            {

               ASSERT(lpmh->dwBytesRecorded <= lpmh->dwBufferLength);

               if(lpmh->dwBytesRecorded > cbPrerollNominalMax && lpmh->dwBytesRecorded > 0)
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

               if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(uint32_t))
               {

                  break;

               }

               ::music::midi::event *     pevent = NULL;

               smfrc = WorkGetNextEvent(pevent, tkMax, TRUE);

               if (::music::success != smfrc)
               {

                  return smfrc;

               }

               ASSERT(GetPosition() >= tkLastPosition);

               imedia_position tkPosition  = GetPosition();

               imedia_position tkDiv = m_psequence->GetQuarterNote();

               while((m_psequence->TicksToMillisecs(tkLastPosition + tkDiv) - m_psequence->TicksToMillisecs(tkLastPosition)) > 100)
               {

                  tkDiv /= 2;

               }

               while(true)
               {

                  imedia_position tkOp = ((m_tkLastOp + tkDiv) / tkDiv) * tkDiv;

                  if (tkOp > tkPosition)
                  {

                     break;

                  }

                  if (tkOp < tkLastPosition)
                  {

                     tkOp = tkPosition;

                  }

                  tkDelta = tkOp - tkLastPosition;

                  LPDWORD lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

                  int iBytesRecorded = 0;

                  if (m_psequence->m_eeffect == sequence::effect_fade_in || m_psequence->m_eeffect == sequence::effect_fade_out)
                  {

                     double dVolume = m_psequence->get_fade_volume(m_psequence->TicksToMillisecs(tkOp));

                     for (int iTrack = 0; iTrack < 16; iTrack++)
                     {

                        clip(0, 127, m_psequence->m_iaRefVolume[iTrack]);

                        byte bVolume = (byte)(m_psequence->m_iaRefVolume[iTrack] * MAX(0.0, MIN(1.0, dVolume)));

                        if (abs((int)m_keyframe.rbControl[iTrack][ControlChangeVolume] - (int)bVolume) < 3)
                        {

                           TRACE("too few difference!! opt-ed out");

                        }
                        else
                        {

                           uint32_t uiFullType = ControlChange;
                           uiFullType |= iTrack & 0xf;
                           uint32_t uiChB1 = ControlChangeVolume;
                           uint32_t uiChB2 = bVolume;

                           *lpdw++ = (uint32_t)tkDelta;
                           *lpdw++ = 0;
                           *lpdw++ = (((uint32_t)MEVT_SHORTMSG) << 24) |
                                     (uiFullType) |
                                     (uiChB1 << 8) |
                                     (uiChB2 << 16);


                           tkLastPosition += tkDelta;

                           tkDelta = 0;

                           lpmh->dwBytesRecorded += 3 * sizeof(uint32_t);

                           iBytesRecorded += 3 * sizeof(uint32_t);

                        }

                     }

                  }
                  else
                  {

                     for (int iTrack = 0; iTrack < 16; iTrack++)
                     {

                        if(m_keyframe.rbControl[iTrack][ControlChangeVolume] == KF_EMPTY)
                        {

                              m_keyframe.rbControl[iTrack][ControlChangeVolume] = 100;

                        }

                        clip(0, 127, m_keyframe.rbControl[iTrack][ControlChangeVolume]);

                        int iVolume = m_keyframe.rbControl[iTrack][ControlChangeVolume];

                        m_psequence->m_iaRefVolume.set_at_grow(iTrack, iVolume);

                     }

                     *lpdw++ = (uint32_t)tkDelta;
                     *lpdw++ = 0;
                     *lpdw++ = (MEVT_NOP << 24);

                     tkLastPosition += tkDelta;

                     tkDelta = 0;

                     lpmh->dwBytesRecorded += 3 * sizeof(uint32_t);

                     iBytesRecorded += 3 * sizeof(uint32_t);


                  }

                  m_tkLastOp = tkOp;

                  cbPrerollNominalMax += iBytesRecorded;

                  if (cbPrerollNominalMax > lpmh->dwBufferLength / 2)
                  {

                     break;

                  }

               }

               tkDelta = tkPosition - tkLastPosition;

               // The position CB events are grouped in a single position CB
               // event after other type of simultaneous events.
               // The {if block} change the order of simultaneous events.
               // position CB events ( pevent->GetFlags() & 1 )
               // are streamed together in a single position CB event, through
               // StreamEventF1 member function.
               // These position CB events are put after any other kind of event
               // that have the same position.
               // The order of the non PositionCB events are not altered.

               //if(pevent->GetFlags() & 1)
               //{
               //   if(eventptraPositionCB.get_size() <= 0)
               //   {
               //      // This is the first PositionCB event
               //      // at the F1 position.
               //      eventptraPositionCB.add(pevent);
               //      tkPositionF1   = tkPosition;
               //   }
               //   else if(tkPosition == tkPositionF1)
               //   {
               //      // This is another PositionCB event
               //      // at the F1 position.
               //      eventptraPositionCB.add(pevent);
               //   }
               //   else
               //   {
               //      // This is another PositionCB event
               //      // at another position.

               //      ASSERT(tkPositionF1 >= tkLastPosition);

               //      tkDelta = tkPositionF1 - tkLastPosition;
               //      StreamEventF1(tkDelta, eventptraPositionCB, lpmh, tkMax, cbPrerollNominalMax);
               //      tkLastPosition = tkPositionF1;
               //      eventptraPositionCB.remove_all();

               //      eventptraPositionCB.add(pevent);
               //      tkPositionF1   = tkPosition;
               //   }
               //}
               //else
               //{
               //   if(eventptraPositionCB.get_size() > 0)
               //   {
               //      if(pevent->GetPosition() != eventptraPositionCB[0]->GetPosition())
               //      {
               //         ASSERT(tkPositionF1 >= tkLastPosition);

               //         tkDelta = tkPositionF1 - tkLastPosition;
               //         StreamEventF1(tkDelta, eventptraPositionCB, lpmh, tkMax, cbPrerollNominalMax);
               //         tkDelta = tkPosition - tkPositionF1;
               //         eventptraPositionCB.remove_all();
               //      }
               //      else
               //      {
               //         // The event to be streamed in the following StreamEvent function
               //         // has the same position of the eventptraPositionCB events.
               //         // So, when these eventptraPositionCB events are streamed,
               //         // the event delta will be zero.
               //         tkDelta = tkPosition - tkPositionF1;
               //         tkPositionF1   = tkPosition;
               //      }
               //   }
               //   /*         if(pevent->GetType() == NoteOn)
               //   {
               //   TRACE("WorkStreamRender: NoteOn     position %d Delta %d Track %d\n",
               //   m_ptracks->GetPosition(),
               //   tkDelta,
               //   pevent->GetTrack());
               //   }
               //   else
               //   {
               //   TRACE("WorkStreamRender: Type Other position %d Delta %d\n",
               //   m_ptracks->GetPosition(),
               //   tkDelta);
               //   }*/

               StreamEvent(tkDelta, pevent, lpmh, tkMax, cbPrerollNominalMax);
               tkLastPosition = tkPosition;
//               }

            }

            return (m_flags & file::EndOfFile) ? SEndOfFile : ::music::success;
         }


         e_result buffer::WorkSeek(imedia_position tkPosition, LPMIDIHDR lpmh)
         {

            e_result        smfrc;
            ::music::midi::event *           pevent = NULL;
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
               else if((bEvent & 0xF0) == ::music::midi::ProgramChange)
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


      } //namespace mmsystem


   } // namespace midi


} // namespace music










