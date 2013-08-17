#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      track::track(sp(::ca2::application) papp) :
         ca2(papp),
         track_base(papp),
         primitive::memory_container (papp)
      {
         m_bWorkTrackEnd = false;
         m_ptracks = NULL;

         m_idxTrack = 0;

         m_tkPosition = 0;
         m_cbLeft = 0;
         m_hpbImage = NULL;


         m_smti.m_tkLength = 0;
         m_smti.m_cbLength = 0;

         m_estate = StateOnDelta;

         m_bAutoAllocation = false;

         m_tkDelta = 0;

         keep_pointer((void **) &m_hpbImage);
         keep_pointer((void **) &m_hpbEventImage);

      }

      track::track(const track & track) :
         ca2(track.get_app()),
         track_base(track.get_app()),
         primitive::memory_container (track)
      {
         operator =(track);
      }

      track::~track()
      {
         m_hpbImage = NULL;
      }

      void track::Initialize(tracks * pTracks)
      {
         m_ptracks = pTracks;
      }

      void track::dump(dump_context & dumpcontext) const
      {
         dumpcontext << "\n";
         dumpcontext << "track";
         dumpcontext << "\n";
         imedia::position tk = 0;
         for(int32_t i = 0; i < m_trackWorkStorage.GetEventCount(); i++)
         {
            event_v5 & event = const_cast < track_v8 & > (m_trackWorkStorage).EventAt(i);
            dumpcontext << "delta " << event._GetDelta();
            dumpcontext << "type " << event.GetFullType();

            tk = tk + imedia::position(event._GetDelta());

            dumpcontext << "\n";
         }

      }


      ////////////////////////////////////////////////////////////////////////////
      //
      // smfGetVDword
      //
      // Reads a var length uint32_t from the given file.
      //
      // hpbImage                  - Pointer to the first byte of the VDWORD.
      //
      // dwLeft                    - Bytes left in image
      //
      // pDw                       - Pointer to a uint32_t to store the result in.
      //                             track.
      //
      // Returns the number of bytes consumed from the stream.
      //
      // A var length uint32_t stored in a MIDI file contains one or more
      // bytes. Each byte except the last has the high bit set; only the
      // low 7 bits are significant.
      //
      ////////////////////////////////////////////////////////////////////////////
      uint32_t track::GetVDWord(
         uint32_t * pDw)
      {
         return GetVDWord(m_cbLeft, pDw);
      }

      uint32_t track::GetVDWord(
         uint32_t dwLeft,
         uint32_t * pDw)
      {
         BYTE                    b;
         uint32_t                   dwUsed  = 0;
         byte *               hpbImage = m_hpbImage;

         ASSERT(hpbImage != NULL);
         ASSERT(pDw != NULL);

         *pDw = 0;

         do
         {
            if (!dwLeft)
            {
               return 0;
            }

            b = *hpbImage++;
            dwLeft--;
            dwUsed++;

            *pDw = (*pDw << 7) | (b & 0x7F);
         } while (b&0x80);

         return dwUsed;
      }

      uint32_t track::SetVDWord(
         uint32_t dwLeft,
         uint32_t dw)
      {
         uint32_t       dwUsed  = 0;
         byte *      hpbImage = m_hpbImage;
         uint32_t         dwBuffer;

         ASSERT(!m_bAutoAllocation);
         ASSERT(hpbImage != NULL);
         ASSERT(dw <= 0x0fffffff);

         if(!dwLeft)
            return 0;
         dwBuffer = dw & 0x7f;
         dwLeft--;
         dwUsed++;
         while ((dw >>= 7) > 0)
         {
            if(!dwLeft)
               return 0;
            dwBuffer <<= 8;
            dwBuffer |= 0x80;
            dwBuffer |= (dw & 0x7f);
            dwLeft--;
            dwUsed++;
         }
         while (TRUE)
         {
            *hpbImage++ = (BYTE) dwBuffer;
            if (dwBuffer & 0x80)
               dwBuffer >>= 8;
            else
               break;
         }

         return dwUsed;
      }


      uint32_t track::SetVDWord(
         uint32_t dw)
      {
         uint32_t                   dwUsed  = 0;
         uint32_t               dwBuffer;

         ASSERT(m_bAutoAllocation);
         ASSERT(dw <= 0x0fffffff);


         dwBuffer = dw & 0x7f;
         dwUsed++;
         while ((dw >>= 7) > 0)
         {
            dwBuffer <<= 8;
            dwBuffer |= 0x80;
            dwBuffer |= (dw & 0x7f);
            dwUsed++;
         }
         try
         {
            allocate_add_up(dwUsed);
         }
         catch(memory_exception * pe)
         {
            pe->Delete();
            return 0;
         }

         byte *   hpbImage = m_hpbImage;
         while (TRUE)
         {
            *hpbImage++ = (BYTE) (dwBuffer & 0xff);
            if (dwBuffer & 0x80)
               dwBuffer >>= 8;
            else
               break;
         }

         return dwUsed;
      }

      //#ifdef _UNICODE
      /*string *pStr;
      int32_t result;
      int32_t bufferSize = 1;
      //LPTSTR wideCharStr = (LPTSTR) malloc(1 * sizeof(char));
      LPTSTR wideCharStr = new char;
      result = MultiByteToWideChar(
      CP_ACP,         // code page
      MB_COMPOSITE,         // character-type options
      (const char *) m_event.hpbParm, // address of string to map
      m_event.cbParm,      // number of bytes in string
      wideCharStr,  // address of wide-character buffer
      bufferSize       // size of buffer
      );
      if(result)
      {
      pStr = new string(wideCharStr, result);
      delete wideCharStr;
      }
      else
      {
      bufferSize = MultiByteToWideChar(
      CP_ACP,         // code page
      MB_COMPOSITE,         // character-type options
      (const char *) m_event.hpbParm, // address of string to map
      m_event.cbParm,      // number of bytes in string
      wideCharStr,  // address of wide-character buffer
      0       // size of buffer
      );
      //wideCharStr = (LPTSTR) malloc(bufferSize * sizeof(char));

      wideCharStr = new char[bufferSize];
      result = MultiByteToWideChar(
      CP_ACP,         // code page
      MB_COMPOSITE,         // character-type options
      (const char *) m_event.hpbParm, // address of string to map
      m_event.cbParm,      // number of bytes in string
      wideCharStr,  // address of wide-character buffer
      bufferSize       // size of buffer
      );
      pStr = new string(wideCharStr, result);
      delete wideCharStr;
      }*/




      e_file_result track::ReadDelta()
      {
         imedia::position tkDelta;

         switch(m_estate)
         {
         case StateOnDelta:
            {

               m_hpbEventImage = m_hpbImage;

               if (!(m_dwUsed = GetVDWord((uint32_t *) &tkDelta)))
               {
                  TRACE("Hit end of track w/o end marker!\n");
                  return EInvalidFile;
               }

               if(tkDelta > 1000000)
               {
                  m_tkDelta = tkDelta;
               }
               else
               {
                  m_tkDelta = tkDelta;
               }
               m_hpbImage += m_dwUsed;
               m_cbLeft -= m_dwUsed;
               m_estate = StateOnEvent;

               return Success;
            }
         case StateOnEvent:
            {
               return Success;
            }
         case StateOnEventRead:
            {
               return Success;
            }
         }
         ASSERT(FALSE);
         return EFail;



      }

      e_file_result track::GetEvent(
         event_base *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         e_file_result  mfr;
         if(Success != (mfr = ReadEvent(tkMax, bTkMaxInclusive)))
         {
            return mfr;
         }
         pevent = &m_event;
         return Success;
      }

      e_file_result track::GetEvent(
         event_v1 *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         e_file_result  mfr;
         if(Success != (mfr = ReadEvent(tkMax, bTkMaxInclusive)))
         {
            return mfr;
         }
         pevent = &m_event;
         return Success;
      }



      /******************************************************************************
      *
      * smfGetNextEvent
      *
      * read the next m_event from the given file.
      *
      * pSmf                      - File to read the m_event from.
      *
      * pEvent                    - Pointer to an m_event structure which will receive
      *                             basic information about the m_event.
      *
      * tkMax                     - Tick destination. An attempt to read past this
      *                             position in the file will fail.
      *
      * Returns
      *   Success The events were successfully read.
      *   SEndOfFile There are no more events to read in this track.
      *   SReachedTkMax No m_event was read because <p tkMax> was reached.
      *   EInvalidFile A disk or parse error occured on the file.
      *
      * This is the lowest level of parsing for a raw MIDI stream. The basic
      * information about one m_event in the file will be returned in pEvent.
      *
      * Merging data from all tracks into one stream is performed here.
      *
      * m_event.tkDelta will contain the tick delta for the m_event.
      *
      * m_event.abEvent will contain a description of the m_event.
      *  m_event.abEvent[0] will contain
      *    F0 or F7 for a System Exclusive message.
      *    FF for a MIDI file meta m_event.
      *    The status byte of any other MIDI message. (Running status will
      *    be tracked and expanded).
      *
      * m_event.cbParm will contain the number of bytes of paramter data
      *   which is still in the file behind the m_event header already read.
      *   This data may be read with <f smfGetTrackEventData>. Any unread
      *   data will be skipped on the next call to <f smfGetNextTrackEvent>.
      *
      * Channel messages (0x8? - 0xE?) will always be returned fully in
      *   m_event.abEvent.
      *
      *  Meta events will contain the meta type in m_event.abEvent[1].
      *
      *  System exclusive events will contain only an 0xF0 or 0xF7 in
      *    m_event.abEvent[0].
      *
      *  The following fields in pTrk are used to maintain state and must
      *  be updated if a seek-in-track is performed:
      *
      *  bRunningStatus contains the last running status message or 0 if
      *   there is no valid running status.
      *
      *  hpbImage is a pointer into the file image of the first byte of
      *   the m_event to follow the m_event just read.
      *
      *  dwLeft contains the number of bytes from hpbImage to the end
      *   of the track.
      *
      *
      * get the next due m_event from all (in-use?) tracks
      *
      * For all tracks
      *  If not end-of-track
      *   decode m_event delta time without advancing through buffer
      *   event_absolute_time = track_tick_time + track_event_delta_time
      *   relative_time = event_absolute_time - last_stream_time
      *   if relative_time is lowest so far
      *    save this track as the next to pull from, along with times
      *
      * If we found a track with a due m_event
      *  Advance track pointer past m_event, saving ptr to parm data if needed
      *  track_tick_time += track_event_delta_time
      *  last_stream_time = track_tick_time
      * Else
      *  Mark and return end_of_file
      *
      *****************************************************************************/
      e_file_result track::ReadEvent(
         imedia::position   tkMax,
         bool   bTkMaxInclusive)
      {
         BYTE                    bEvent;
         uint32_t                   dwGotTotal;
         uint32_t                   dwGot;
         uint32_t                   cbEvent;
         e_file_result      mfr;
         byte *                  hpbImage;

         if(m_estate == StateOnDelta)
         {
            mfr = ReadDelta();
            if(Success != mfr)
            {
               _GetFlags().signalize(EndOfTrack);
               return SEndOfTrack;
            }
         }


         // Probably you have forgotten to call MoveNext
         ASSERT(m_estate == StateOnEvent);

         m_event.clear();

         // We MUST have at least three bytes here (cause we haven't hit
         // the end-of-track meta yet, which is three bytes long). Checking
         // against three means we don't have to check how much is left
         // in the track again for any int16_t m_event, which is most cases.

         if(m_cbLeft < 3)
         {
            //Attention
            return EInvalidFile;
         }


         m_tkPosition += m_tkDelta;

         m_event.SetPosition(m_tkPosition);
         m_event.SetDelta(m_tkDelta);

         if(bTkMaxInclusive)
         {
            if(m_tkPosition > tkMax)
            {
               return SReachedTkMax;
            }
         }
         else
         {
            if(m_tkPosition >= tkMax)
            {
               return SReachedTkMax;
            }
         }

         hpbImage = m_hpbImage;

         bEvent = *hpbImage++;

         if (bEvent < Msg)
         {
            if(0 == m_ptracks->m_uchRunningStatus)
            {
               //Attention
               return EInvalidFile;
            }

            dwGotTotal = 1;
            m_event.SetFullType(m_ptracks->m_uchRunningStatus);
            m_event.SetChB1(bEvent);
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               m_event.SetChB2(*hpbImage++);
               dwGotTotal++;
            }
         }
         else if (SysEx > bEvent)
         {
            m_ptracks->m_uchRunningStatus = bEvent;

            dwGotTotal = 2;
            m_event.SetFullType(bEvent);
            m_event.SetChB1(*hpbImage++);
            if(3 == GetMessageLen(bEvent))
            {
               m_event.SetChB2(*hpbImage++);
               dwGotTotal++;
            }
         }
         else
         {
            if(Meta == bEvent)
            {
               m_event.SetFullType(Meta);
               m_event.SetChB1(*hpbImage++);
               if(MetaEOT == m_event.GetChB1())
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
               dwGotTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               m_event.SetFullType(bEvent);
               dwGotTotal = 1;
            }
            else
            {
               //Attention
               ASSERT(FALSE);
               return EInvalidFile;
            }
            if(0 == (dwGot = ::music::midi::GetVDWord(hpbImage, m_cbLeft - 2, cbEvent)))
            {
               //Attention
               ASSERT(FALSE);
               return EInvalidFile;
            }

            //hpbImage  += dwGot;
            dwGotTotal  += dwGot;

            if(dwGotTotal + cbEvent > m_cbLeft)
            {
               //Attention
               ASSERT(FALSE);
               return EInvalidFile;
            }

            m_event.SetParam(hpbImage, cbEvent);

            hpbImage += cbEvent;
            dwGotTotal += cbEvent;
         }

         ASSERT(m_cbLeft >= dwGotTotal);

         m_hpbImage = hpbImage ;
         m_cbLeft -= dwGotTotal;
         m_dwUsed = dwGotTotal;
         m_estate = StateOnEventRead;

         //m_event.m_cbImage = hpbImage - m_hpbEventImage;


         return Success;
      }

      VMSRESULT track::ReadEvent(event_v5 &midiEvent)
      {
         BYTE                       bEvent;
         int32_t                        iGotTotal;
         uint32_t                      dwGot;
         uint32_t                      cbEvent;
         e_file_result        smfrc;

         if(m_estate == StateOnDelta)
         {
            smfrc = ReadDelta();
            if(Success != smfrc)
            {
               _GetFlags().signalize(EndOfTrack);
               return SEndOfTrack;
            }
         }
         else if(m_estate == StateOnEventRead)
         {
            return Success;
         }

         // Probably you have forgotten to call MoveNext
         ASSERT(m_estate == StateOnEvent);

         midiEvent.clear();
         midiEvent._SetDelta(m_tkDelta);
         m_tkPosition += m_tkDelta;
         midiEvent.SetPosition(m_tkPosition);
         if (m_cbLeft < 3)
         {
            return VMSR_E_INVALIDFILE;
         }
         bEvent = *m_hpbImage++;
         if (Msg > bEvent)
         {
            iGotTotal = 1;
            midiEvent._SetFullType(m_ptracks->m_uchRunningStatus);
            midiEvent._SetChB1(bEvent);
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               midiEvent._SetChB2(*m_hpbImage++);
               iGotTotal++;
            }
         }
         else if (SysEx > bEvent)
         {
            m_ptracks->m_uchRunningStatus = bEvent;
            iGotTotal = 2;
            midiEvent._SetFullType(bEvent);
            midiEvent._SetChB1(*m_hpbImage++);
            if (3 == GetMessageLen(bEvent))
            {
               midiEvent._SetChB2(*m_hpbImage++);
               iGotTotal++;
            }
         }
         else
         {
            if (Meta == bEvent)
            {
               midiEvent._SetFullType(Meta);
               midiEvent.SetMetaType(*m_hpbImage++);
               if (MetaEOT == midiEvent.GetMetaType())
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
               iGotTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               midiEvent._SetFullType(bEvent);
               iGotTotal = 1;
            }
            else
            {
               return VMSR_E_INVALIDFILE;
            }

            if (0 == (dwGot = event_base::GetVDWord(m_hpbImage, m_cbLeft - iGotTotal, &cbEvent)))
            {
               return VMSR_E_INVALIDFILE;
            }

            m_hpbImage  += dwGot;
            iGotTotal  += dwGot;

            if (iGotTotal + (int32_t) cbEvent > (int32_t) m_cbLeft)
            {
               return VMSR_E_INVALIDFILE;
            }

            midiEvent.WriteParam(m_hpbImage, cbEvent);

            m_hpbImage += cbEvent;
            iGotTotal += cbEvent;
         }

         ASSERT(m_cbLeft >= (uint32_t) iGotTotal);

         m_cbLeft -= iGotTotal;
         m_dwUsed = iGotTotal;
         m_estate = StateOnEventRead;
         m_iCurrentEvent++;

         return VMSR_SUCCESS;
      }

      VMSRESULT track::ReadEvent(event_v1 &midiEvent)
      {
         BYTE                       bEvent;
         int32_t                        iGotTotal;
         uint32_t                      dwGot;
         uint32_t                      cbEvent;
         e_file_result        smfrc;

         if(m_estate == StateOnDelta)
         {
            smfrc = ReadDelta();
            if(Success != smfrc)
            {
               _GetFlags().signalize(EndOfTrack);
               return SEndOfTrack;
            }
         }
         else if(m_estate == StateOnEventRead)
         {
            return Success;
         }

         // Probably you have forgotten to call MoveNext
         ASSERT(m_estate == StateOnEvent);


         midiEvent.clear();
         midiEvent._SetDelta(m_tkDelta);
         m_tkPosition += m_tkDelta;
         midiEvent.SetPosition(m_tkPosition);
         if (m_cbLeft < 1)
         {
            return VMSR_E_INVALIDFILE;
         }
         midiEvent._SetImage(m_hpbImage);
         bEvent = *m_hpbImage++;
         if (bEvent < Msg)
         {
            //ASSERT(m_ptracks->m_uchRunningStatus >= Msg);
            iGotTotal = 1;
            midiEvent._SetFullType(m_ptracks->m_uchRunningStatus);
            midiEvent._SetChB1(bEvent);
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               if (m_cbLeft < 2)
               {
                  return VMSR_E_INVALIDFILE;
               }
               midiEvent._SetChB2(*m_hpbImage++);
               iGotTotal++;
            }
         }
         else if (SysEx > bEvent)
         {
            if (m_cbLeft < 2)
            {
               return VMSR_E_INVALIDFILE;
            }
            m_ptracks->m_uchRunningStatus = bEvent;
            iGotTotal = 2;
            midiEvent._SetFullType(bEvent);
            midiEvent._SetChB1(*m_hpbImage++);
            if (3 == GetMessageLen(bEvent))
            {
               if (m_cbLeft < 3)
               {
                  return VMSR_E_INVALIDFILE;
               }
               midiEvent._SetChB2(*m_hpbImage++);
               iGotTotal++;
            }
         }
         else
         {
            if (Meta == bEvent)
            {
               if (m_cbLeft < 2)
               {
                  return VMSR_E_INVALIDFILE;
               }
               midiEvent._SetFullType(Meta);
               midiEvent._SetMetaType(*m_hpbImage++);
               if (MetaEOT == midiEvent.GetMetaType())
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
               iGotTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               midiEvent._SetFullType(bEvent);
               iGotTotal = 1;
            }
            else
            {
               return VMSR_E_INVALIDFILE;
            }

            dwGot = event_base::GetVDWord(m_hpbImage, m_cbLeft - iGotTotal, &cbEvent);

            m_hpbImage  += dwGot;
            iGotTotal  += dwGot;

            if (iGotTotal + (int32_t) cbEvent > (int32_t) m_cbLeft)
            {
               return VMSR_E_INVALIDFILE;
            }

            midiEvent._SetParam(m_hpbImage, cbEvent);

            m_hpbImage += cbEvent;
            iGotTotal += cbEvent;
         }

         ASSERT(m_cbLeft >= (uint32_t) iGotTotal);

         m_cbLeft -= iGotTotal;
         m_dwUsed = iGotTotal;
         m_estate = StateOnEventRead;

         midiEvent._SetImageSize(iGotTotal);

         /*TRACE("%04d ", midiEvent.GetDelta());

         switch(midiEvent.get_type())
         {
         case NoteOff:
         TRACE("%02d ", midiEvent.GetTrack());
         TRACE("NoteOff %d %d", midiEvent.GetChB1(), midiEvent.GetChB2());
         break;
         case NoteOn:
         TRACE("%02d ", midiEvent.GetTrack());
         TRACE("NoteOn %d %d", midiEvent.GetChB1(), midiEvent.GetChB2());
         break;
         }
         TRACE("\n");
         */

         return VMSR_SUCCESS;
      }




      e_file_result track::GetNextXFInfoHeader(::music::xf::info_header * pXfih)
      {
         ASSERT(pXfih != NULL);
         e_file_result smfrc;
         if(Success == (smfrc = seek(SeekXFInfoHeader)))
         {
            string_tokenizer strtokenizer;
            ::ca2::international::MultiByteToMultiByte(CP_UTF8, strtokenizer, ::ca2::international::CodePageLatin1, (const char *) m_event.GetParam(), m_event.GetParamSize());
            pXfih->FromData(strtokenizer);
            string str;
            TRACE("****GetXFInfoHeader");
            TRACE("Date: %s\n", pXfih->m_strDate);
            TRACE("Country: %s\n", pXfih->m_strCountry);
            TRACE("Category: %s\n", pXfih->m_strCategory);
            TRACE("Beat: %s\n", pXfih->m_strBeat);

            pXfih->m_straComposer.get_format_string(str, "/");
            TRACE("Composer: %s\n", str);

            pXfih->m_straLyricist.get_format_string(str, "/");
            TRACE("Lyricist: %s\n", str);

            pXfih->m_straArranger.get_format_string(str, "/");
            TRACE("Arranger: %s\n", str);

            pXfih->m_straPerformer.get_format_string(str,"/");
            TRACE("Performer: %s\n", str);

            TRACE("ID: %s\n", pXfih->m_id);
            return Success;
         }
         return smfrc;
      }

      e_file_result track::GetNextXFInfoHeaderLS(::music::xf::info_header_ls * pXfihls)
      {
         ASSERT(pXfihls != NULL);
         e_file_result smfrc;
         if(Success == (smfrc = seek(SeekXFInfoHeaderLS)))
         {
            string_tokenizer str;
            ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
            pXfihls->FromData(str);
            return Success;
         }
         return smfrc;
      }

      e_file_result track::GetNextXFInfoHeaderLS(::music::xf::info_header_ls * pXfihls, const string &strLanguage)
      {
         ::music::xf::info_header_ls xfihls;
         while(Success == GetNextXFInfoHeaderLS(&xfihls))
         {
            if(xfihls.m_strLanguage.CompareNoCase(strLanguage) == 0)
            {
               *pXfihls = xfihls;
               return Success;
            }
         }
         return SEndOfFile;
      }

      e_file_result track::GetNextXFSongName(string &str)
      {
         //LPTSTR pSongName = NULL;
         e_file_result smfrc;
         if(Success == (smfrc = seek(SeekXFSongName)))
         {
            //   string str;

            ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
            //   pSongName = _tcsdup(str);
            //   TRACE("Song Name: %s\n", pSongName);
            return Success;
         }
         return smfrc;
      }



      e_file_result track::ReadXFInfoHeader_(
         LPTSTR *ppSongName,
         ::music::xf::info_header **ppXfih,
         ::music::xf::info_header_ls **ppXfihls)
      {
         ASSERT(*ppSongName == NULL);
         ASSERT(*ppXfih == NULL);
         ASSERT(*ppXfihls == NULL);

         //    PSMF                    pSmf = (PSMF)hSmf;
         e_file_result               smfrc;
         //    CMidiEvent              m_event;
         imedia::position               tkLastDelta = 0 ;
         bool               okSongName = FALSE;
         bool               okInfoHeader = FALSE;
         bool               okInfoHeaderLS = FALSE;

         *ppXfih = NULL;
         *ppXfihls = NULL;
         *ppSongName = NULL;

         if(m_tkPosition != 0)
         {
            return   EInvalidTkPosition;
         }

         /*
         ** read events from the track and pack them into the buffer in polymsg
         ** format.
         **
         ** If a SysEx or meta would go over a buffer boundry, split it.
         */
         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         while(TRUE)
         {
            if(okSongName &&
               okInfoHeader &&
               okInfoHeaderLS)
               break;
            smfrc = ReadEvent(0, TRUE);
            if (Success != smfrc)
            {
               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-file.
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               if (SReachedTkMax == smfrc)
               {
                  //                m_fdwSMF |= EndOfFile;
                  _GetFlags().signalize(EndOfFile);
               }

               TRACE( "smfReadEvents: ReadXFInfoHeader_() -> %u", (uint32_t)smfrc);
               break;
            }
            if(m_tkPosition > 0)
               break;

            if ((Meta == m_event.GetFullType()) &&
               (::music::xf::MetaSongName == m_event.GetMetaType()))
            {
               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str,
                  (const char *) m_event.GetParam(), m_event.GetParamSize());
               *ppSongName = _tcsdup(str);
               TRACE("Song Name: %s\n", *ppSongName);
               okSongName = TRUE;
            }
            else if ((Meta == m_event.GetFullType()) &&
               (::music::xf::MetaXFInfoHdr == m_event.GetMetaType()))
            {
               string_tokenizer wstrtokenizer;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  wstrtokenizer,
                  (const char *) m_event.GetParam(),
                  m_event.GetParamSize());
               if(wstrtokenizer.Mid(0, 4).Compare("XFhd") == 0)
               {
                  *ppXfih = new ::music::xf::info_header();
                  (*ppXfih)->FromData(wstrtokenizer);
                  //if(iHi < 0)
                  //   goto XF_Lyrics_Parse_End;
                  //m_LyricsDisplayOffset = _ttoi(str.Mid(iLo + 1, iHi - iLo - 1));
                  //         XF_IH_Parse_End:;
                  string str;
                  TRACE("****GetXFInfoHeader");
                  TRACE("Date: %s\n", (*ppXfih)->m_strDate);
                  TRACE("Country: %s\n", (*ppXfih)->m_strCountry);
                  TRACE("Category: %s\n", (*ppXfih)->m_strCategory);
                  TRACE("Beat: %s\n", (*ppXfih)->m_strBeat);

                  (*ppXfih)->m_straComposer.get_format_string(str, "/");
                  TRACE("Composer: %s\n", str);

                  (*ppXfih)->m_straLyricist.get_format_string(str, "/");
                  TRACE("Lyricist: %s\n", str);

                  (*ppXfih)->m_straArranger.get_format_string(str, "/");
                  TRACE("Arranger: %s\n", str);

                  (*ppXfih)->m_straPerformer.get_format_string(str, "/");
                  TRACE("Performer: %s\n", str);

                  TRACE("ID: %s\n", (*ppXfih)->m_id);
               }
               else if(wstrtokenizer.Mid(0, 4).Compare("XFln") == 0)
               {
                  *ppXfihls = new ::music::xf::info_header_ls();
                  (*ppXfihls)->FromData(wstrtokenizer);
                  //if(iHi < 0)
                  //   goto XF_Lyrics_Parse_End;
                  //m_LyricsDisplayOffset = _ttoi(str.Mid(iLo + 1, iHi - iLo - 1));
                  //XF_IHLS_Parse_End:;
                  okInfoHeaderLS = TRUE;
               }
            }
         }
         //   if(!okSongName &&
         //      !okInfoHeader)
         //   {
         //      delete *ppXfih;
         //      pXfih = NULL;
         //   }
         //   if(!okInfoHeaderLS)
         //   {
         //      delete *ppXfihls;
         //      pXfihls= NULL;
         //   }
         //    return (m_fdwSMF & EndOfFile) ? SEndOfFile : Success;
         return (_GetFlags().is_signalized(EndOfFile)) ? SEndOfFile : Success;
      }

      e_file_result track::WriteXFInfoHeader(
         const char * pszSongName,
         ::music::xf::info_header *pXfih,
         ::music::xf::info_header_ls *pXfihls)
      {
         //    PSMF                    pSmf = (PSMF)hSmf;
         e_file_result               smfrc;
         //    CMidiEvent              m_event;
         imedia::position               tkLastDelta = 0 ;
         primitive::memory   memstorage;

         if(m_tkPosition != 0)
         {
            return   EInvalidTkPosition;
         }

         /*
         ** read events from the track and pack them into the buffer in polymsg
         ** format.
         **
         ** If a SysEx or meta would go over a buffer boundry, split it.
         */
         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         m_event.SetDelta(0);
         if(pszSongName != NULL)
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaSongName);
            memstorage.allocate(::ca2::international::Utf8ToMultiByteCount(::ca2::international::CodePageLatin1, pszSongName));
            m_event.SetParam((byte *) memstorage.get_data(), (int32_t) (memstorage.get_size() - 1));
            ::ca2::international::utf8_to_multibyte(
               ::ca2::international::CodePageLatin1,
               (char *) m_event.GetParam(),
               m_event.GetParamSize(),
               pszSongName);
            //      m_event.GetParam() = (byte *)
            //         International
            //         ::UnicodeToAnsiDup(pSongName);
            //m_event.m_cbParm = strlen((const char *)m_event.GetParam());
            if((smfrc = WriteCompleteEvent(0x7fffffff, false)) != Success)
               return smfrc;
         }
         if(pXfih != NULL)
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaXFInfoHdr);
            string str;
            pXfih->ToData(str);

            ::ca2::international::utf8_to_multibyte(::ca2::international::CodePageLatin1, memstorage, str);
            //      memstorage.allocate(
            //         International
            //         ::StringToLatin1Count(str)@);
            m_event.SetParam((byte *) memstorage.get_data(), (int32_t) (memstorage.get_size() - 1));
            //      ::ca2::international::StringToLatin1(
            //         (char *) m_event.GetParam() ,
            //         m_event.GetParamSize(),
            //         str);
            //      m_event.GetParam() = (byte *)
            //         International
            //         ::UnicodeToAnsiDup(str);
            //      m_event.GetParamSize() = strlen((const char *) m_event.GetParam());
            if((smfrc = WriteCompleteEvent(0x7fffffff, false)) != Success)
               return smfrc;

            TRACE("****WriteXFInfoHeader");
            TRACE("Date: %s\n", (char *) m_event.GetParam());
            //      free(m_event.GetParam());
         }
         if(pXfihls != NULL)
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaXFInfoHdr);
            string str;
            pXfihls->ToData(str);
            memstorage.allocate(
               ::ca2::international::Utf8ToMultiByteCount(::ca2::international::CodePageLatin1, str));
            m_event.SetParam((byte *) memstorage.get_data(), (int32_t) (memstorage.get_size() - 1));
            ::ca2::international::utf8_to_multibyte(
               ::ca2::international::CodePageLatin1,
               (char *) m_event.GetParam() ,
               m_event.GetParamSize(),
               str);
            //      m_event.GetParam() = (byte *)
            //         International
            //         ::UnicodeToAnsiDup(str);
            //      m_event.GetParamSize() = strlen((const char *) m_event.GetParam());
            if((smfrc = WriteCompleteEvent(0x7fffffff, false)) != Success)
               return smfrc;
            //      delete m_event.GetParam();
         }
         return Success;
      }


      e_file_result track::ReadKarTokens(
         stringa &  tokena,
         imedia::position_array *   lptkaTicks)
      {
         string str;
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         ASSERT(lptkaTicks != NULL);
         tokena.remove_all();
         lptkaTicks->remove_all();


         while(m_tkPosition == 0)
         {
            smfrc = ReadEvent(tkMax, TRUE);
            if(Success != smfrc)
            {
               break;
            }
            if((Meta == m_event.GetFullType()) &&
               (MetaKarLyric == m_event.GetMetaType()))
            {
               LPTSTR lpsz = str.GetBufferSetLength(m_event.GetParamSize() + 1);
               memcpy(lpsz, m_event.GetParam(), m_event.GetParamSize());
               lpsz[m_event.GetParamSize()] = '\0';
               str.ReleaseBuffer();
               //::ca2::international::MultiByteToUnicode(
               // CodePageLatin1,
               //            str, (char *) m_event.GetParam(), m_event.GetParamSize());
               if(!str.is_empty())
               {
                  if(str[0] != '@')
                  {
                     tokena.add(str);
                     lptkaTicks->add(m_tkPosition);
                  }
               }
            }
            smfrc = MoveNext();
            if (Success != smfrc)
            {
               break;
            }
         }
         if (Success != smfrc)
         {
            return smfrc;
         }
         while(TRUE)
         {
            smfrc = ReadEvent(tkMax, TRUE);
            if (Success != smfrc)
            {
               break;
            }


            if ((Meta == m_event.GetFullType()) &&
               (MetaKarLyric == m_event.GetMetaType()))
            {
               /*         ::ca2::international::MultiByteToUnicode(
               CodePageLatin1,
               str,
               (char *) m_event.GetParam(),
               m_event.GetParamSize());*/
               LPTSTR lpsz = str.GetBufferSetLength(m_event.GetParamSize() + 1);
               memcpy(lpsz, m_event.GetParam(), m_event.GetParamSize());
               lpsz[m_event.GetParamSize()] = '\0';
               str.ReleaseBuffer();
               tokena.add(str);
               lptkaTicks->add(m_tkPosition);
            }
            smfrc = MoveNext();
            if (Success != smfrc)
            {
               break;
            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      e_file_result track::ReadAnsiXFTokens(
         stringa &  tokena,
         imedia::position_array *   lptkaTicks)
      {
         ASSERT(lptkaTicks != NULL);
         string str;
         tokena.remove_all();
         lptkaTicks->remove_all();
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         //m_ptracks->seek_begin();

         //event_v5 event;

         /*    while(Success == ReadEvent(tkMax, TRUE))
         {
         if ((Meta == m_event.GetFullType()) &&
         (::music::xf::MetaLyric == m_event.GetMetaType()))
         {
         string str;
         International
         ::Latin1ToString(
         str,
         (const char *) m_event.GetParam(),
         m_event.GetParamSize());
         TRACE("%s\n", str);
         lpstraTokens->add(str);
         lptkaTicks->add(m_tkPosition);
         }
         }*/

         while(TRUE)
         {
            smfrc = ReadEvent(tkMax, TRUE);
            if (Success != smfrc)
            {
               break;
            }


            if ((Meta == m_event.GetFullType()) &&
               (::music::xf::MetaLyric == m_event.GetMetaType()))
            {
               ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
               TRACE("%s\n", str);
               tokena.add(str);
               lptkaTicks->add(m_tkPosition);
            }


            smfrc = MoveNext();
            if (Success != smfrc)
            {
               break;
            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      e_file_result track::ReadShiftJisXFTokens(
         stringa &  tokena,
         imedia::position_array *   lptkaTicks)
      {
         ASSERT(lptkaTicks != NULL);
         tokena.remove_all();
         lptkaTicks->remove_all();
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         string str;

         while(TRUE)
         {
            smfrc = ReadEvent(tkMax, TRUE);
            if (Success != smfrc)
            {
               break;
            }


            if ((Meta == m_event.GetFullType()) &&
               (::music::xf::MetaLyric == m_event.GetMetaType()))
            {
               ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageShiftJIS, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
               TRACE("%s\n", str);
               tokena.add(str);
               lptkaTicks->add(m_tkPosition);
            }


            smfrc = MoveNext();
            if (Success != smfrc)
            {
               break;
            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }


      e_file_result   track::seek_begin()
      {
         if(m_bAutoAllocation)
         {
            m_tkPosition      = 0;
            m_cbLeft            = m_smti.m_cbLength;
            m_hpbImage         = GetAllocationImage() + sizeof(CHUNKHDR);
            m_ptracks->m_uchRunningStatus   = 0;
            m_estate            = StateOnDelta;
            m_tkDelta         = 0;
            m_iCurrentEvent   = -1;
            _GetFlags().unsignalize_all();
         }
         else
         {
            ASSERT(m_ptracks != NULL);
            sp(::music::midi::file) pfile  = m_ptracks->m_pFile;
            if(pfile == NULL)
            {
               m_hpbImage        = NULL;
            }
            else
            {
               m_hpbImage        = pfile->GetImage() + m_idxTrack;
            }
            m_tkPosition         = 0;
            m_cbLeft             = m_smti.m_cbLength;
            m_ptracks->m_uchRunningStatus     = 0;
            m_estate               = StateOnDelta;
            m_tkDelta            = 0;
            m_iCurrentEvent      = -1;
            _GetFlags().unsignalize_all();
         }
         return Success;
      }


      /*bool track::allocate(uint32_t dwNewLength)
      {
      ASSERT(m_bAutoAllocation);
      //   if(m_bMirrorAllocation)
      //      if(!AllocateMirror(dwNewLength))
      //         return FALSE;
      if(m_hpbAllocation == NULL)
      {
      m_hpbAllocation = (byte *) malloc(dwNewLength + sizeof(CHUNKHDR));
      if(m_hpbAllocation!= NULL)
      {
      //         m_dwAllocation = HeapSize(theApp.m_hHeap, 0 , m_hpbAllocation);
      m_dwAllocation = dwNewLength + sizeof(CHUNKHDR);
      m_hpbImage = m_hpbAllocation + sizeof(CHUNKHDR);
      m_smti.m_cbLength = dwNewLength;
      //         m_dwAllocation = dwNewLength + sizeof(CHUNKHDR);
      return TRUE;
      }
      else
      {
      return FALSE;
      }
      }
      else
      {
      if(dwNewLength  + sizeof(CHUNKHDR) > m_dwAllocation)
      {
      uint32_t dwAllocation = dwNewLength + sizeof(CHUNKHDR)+ m_dwAllocationAddUp;
      LPVOID lpVoid = realloc(m_hpbAllocation, dwAllocation);
      //LPVOID lpVoid =HeapReAlloc(theApp.m_hHeap, 0, m_hpbAllocation, dwAllocation);
      if(lpVoid == NULL)
      {
      uint32_t dw = GetLastError();
      //            TRACELASTERROR();
      return FALSE;
      }
      else
      {
      //m_dwAllocation = HeapSize(theApp.m_hHeap, 0 , m_hpbAllocation);
      m_dwAllocation = dwAllocation;
      m_hpbImage = (byte *) lpVoid + (m_hpbImage - m_hpbAllocation);
      m_hpbAllocation = (byte *) lpVoid;
      m_smti.m_cbLength = dwNewLength;
      return TRUE;
      }
      }
      else
      {
      m_smti.m_cbLength = dwNewLength;
      return TRUE;
      }

      }
      }*/

      /*bool track::AllocateMirror(uint32_t dwNewLength)
      {
      ASSERT(m_bMirrorAllocation);
      if(m_hpbMirrorAllocation == NULL)
      {
      m_hpbMirrorAllocation = (byte *) malloc(dwNewLength + sizeof(CHUNKHDR));
      if(m_hpbMirrorAllocation!= NULL)
      {
      m_hpbMirrorImage = m_hpbMirrorAllocation + sizeof(CHUNKHDR);
      return TRUE;
      }
      else
      {
      return FALSE;
      }
      }
      else
      {
      if(dwNewLength  + sizeof(CHUNKHDR) > m_dwAllocation)
      {
      uint32_t dwAllocation = dwNewLength + sizeof(CHUNKHDR)+ m_dwAllocationAddUp;
      LPVOID lpVoid = realloc(m_hpbAllocation, dwAllocation);
      //LPVOID lpVoid =HeapReAlloc(theApp.m_hHeap, 0, m_hpbAllocation, dwAllocation);
      if(lpVoid == NULL)
      {
      uint32_t dw = GetLastError();
      TRACELASTERROR();
      return FALSE;
      }
      else
      {
      //m_dwAllocation = HeapSize(theApp.m_hHeap, 0 , m_hpbAllocation);
      m_hpbMirrorImage = (byte *) lpVoid + (m_hpbMirrorImage - m_hpbMirrorAllocation);
      m_hpbMirrorAllocation = (byte *) lpVoid;
      return TRUE;
      }
      }
      else
      {
      return TRUE;
      }

      }
      }*/

      e_file_result track::WriteDelta()
      {
         ASSERT(m_estate == StateOnDelta);
         ASSERT(m_bAutoAllocation);
         if (!(m_dwUsed = SetVDWord((uint32_t) m_tkDelta)))
         {
            TRACE("No Memory Available.");
            return ENoMemory;
         }

         m_hpbImage += m_dwUsed;
         m_cbLeft -= m_dwUsed;
         //m_smti.m_cbLength += m_dwUsed;
         m_estate = StateOnEvent;

         return Success;
      }

      void track::SetAutoAllocation(bool bValue)
      {
         if(bValue)
         {
            if(!m_bAutoAllocation)
            {
               ASSERT(!m_bAutoAllocation);
               byte * hpb = GetTrackImage();
               uint32_t cbSize = GetTrackImageLength();
               m_bAutoAllocation = true;
               if(hpb != NULL)
               {
                  allocate(cbSize);
                  memcpy(GetAllocationImage(), hpb, cbSize);
               }
               else
               {
                  allocate(sizeof(CHUNKHDR));
               }
               m_hpbImage = get_data();
            }
         }
         else
         {
            if(m_bAutoAllocation)
            {
               /*         if(m_hpbAllocation != NULL)
               {
               free(m_hpbAllocation);
               m_hpbAllocation = NULL;
               m_dwAllocation = 0;
               }*/
            }
            m_bAutoAllocation = false;
         }

      }

      e_file_result track::WriteCompleteEvent(imedia::position tkMax, bool bUseEventPosition)
      {

         ASSERT(m_estate == StateOnDelta);
         ASSERT(m_bAutoAllocation);

         BYTE                    bEvent;
         uint32_t                   dwUsedTotal;
         uint32_t                   dwUsed;
         uint32_t                   cbEvent;
         e_file_result            smfrc;

         /* We MUST have at least three bytes here (cause we haven't hit
         ** the end-of-track meta yet, which is three bytes long). Checking
         ** against three means we don't have to check how much is left
         ** in the track again for any int16_t m_event, which is most cases.
         */

         if(bUseEventPosition)
         {
            ASSERT(m_event.get_position() >= m_tkPosition);
            m_tkDelta = m_event.get_position() - m_tkPosition;
         }
         else
         {
            m_tkDelta = m_event._GetDelta();
         }


         if((smfrc = WriteDelta()) != Success)
         {
            return smfrc;
         }

         m_tkPosition += m_tkDelta;

         if (m_tkPosition > tkMax)
         {
            return SReachedTkMax;
         }

         bEvent = m_event.GetFullType();

         if(bEvent < Msg)
         {
            try
            {
               allocate_add_up(1);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }


            dwUsedTotal = 1;
            *m_hpbImage++ = m_event.GetChB1();
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = m_event.GetChB2();
               dwUsedTotal++;
            }
         }
         else if(bEvent < SysEx)
         {
            m_ptracks->m_uchRunningStatus = bEvent;
            try
            {
               allocate_add_up(2);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }
            dwUsedTotal = 2;
            *m_hpbImage++ = bEvent;
            *m_hpbImage++ = m_event.GetChB1();
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = m_event.GetChB2();
               dwUsedTotal++;
            }
         }
         else
         {
            if (Meta == bEvent)
            {
               try
               {
                  allocate_add_up(2);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = Meta;
               if (MetaEOT == (*m_hpbImage++ = m_event.GetMetaType()))
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
               dwUsedTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = bEvent;
               dwUsedTotal = 1;
            }
            else
            {
               ASSERT(FALSE);
            }

            cbEvent = (uint32_t) m_event.GetParamSize();

            if(!(dwUsed = SetVDWord(cbEvent)))
            {
               return ENoMemory;
            }

            m_hpbImage  += dwUsed;
            dwUsedTotal  += dwUsed;


            try
            {
               allocate_add_up(cbEvent);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }

            memcpy(m_hpbImage, m_event.GetParam(), cbEvent);

            m_hpbImage += cbEvent;
            dwUsedTotal += cbEvent;
         }

         //   m_smti.m_cbLength += dwUsedTotal;
         m_estate = StateOnDelta;

         return Success;
      }


      e_file_result track::WriteCompleteEvent(event_base & event, imedia::position tkMax, bool bUseEventPosition)
      {

         ASSERT(m_estate == StateOnDelta);
         ASSERT(m_bAutoAllocation);

         BYTE                    bEvent;
         uint32_t                   dwUsedTotal;
         uint32_t                   dwUsed;
         uint32_t                   cbEvent;
         e_file_result            smfrc;

         /* We MUST have at least three bytes here (cause we haven't hit
         ** the end-of-track meta yet, which is three bytes long). Checking
         ** against three means we don't have to check how much is left
         ** in the track again for any int16_t event, which is most cases.
         */

         if(bUseEventPosition)
         {
            ASSERT(event.get_position() >= m_tkPosition);
            m_tkDelta = event.get_position() - m_tkPosition;
         }
         else
         {
            m_tkDelta = event.GetDelta();
         }


         if((smfrc = WriteDelta()) != Success)
         {
            return smfrc;
         }

         m_tkPosition += m_tkDelta;

         if (m_tkPosition > tkMax)
         {
            return SReachedTkMax;
         }

         bEvent = event.GetFullType();

         if(bEvent < Msg)
         {
            ASSERT(FALSE);
         }
         else if(bEvent == m_ptracks->m_uchRunningStatus)
         {
            try
            {
               allocate_add_up(1);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }


            dwUsedTotal = 1;
            *m_hpbImage++ = event.GetChB1();
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = event.GetChB2();
               dwUsedTotal++;
            }
         }
         else if(bEvent < SysEx)
         {
            m_ptracks->m_uchRunningStatus = bEvent;
            try
            {
               allocate_add_up(2);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }
            dwUsedTotal = 2;
            *m_hpbImage++ = bEvent;
            *m_hpbImage++ = event.GetChB1();
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = event.GetChB2();
               dwUsedTotal++;
            }
         }
         else
         {
            if (Meta == bEvent)
            {
               try
               {
                  allocate_add_up(2);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = Meta;
               if (MetaEOT == (*m_hpbImage++ = event.GetMetaType()))
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
               dwUsedTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception *)
               {
                  return ENoMemory;
               }
               *m_hpbImage++ = bEvent;
               dwUsedTotal = 1;
            }
            else
            {
               ASSERT(FALSE);
            }

            cbEvent = (uint32_t) event.GetParamSize();

            if(!(dwUsed = SetVDWord(cbEvent)))
            {
               return ENoMemory;
            }

            m_hpbImage  += dwUsed;
            dwUsedTotal  += dwUsed;


            try
            {
               allocate_add_up(cbEvent);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }

            memcpy(m_hpbImage, event.GetParam(), cbEvent);

            m_hpbImage += cbEvent;
            dwUsedTotal += cbEvent;
         }

         //   m_smti.m_cbLength += dwUsedTotal;
         m_estate = StateOnDelta;

         return Success;
      }

      /*e_file_result track::copy(track *pTrk)
      {
      if(m_bAutoAllocation)
      {
      if(!allocate(pTrk->m_smti.m_cbLength))
      return ENoMemory;
      byte * hpbImage = pTrk->m_ptracks->m_pFile->GetImage() + pTrk->m_idxTrack - sizeof(CHUNKHDR);
      memcpy(m_hpbAllocation, hpbImage, m_smti.m_cbLength + sizeof(CHUNKHDR));
      return Success;
      }
      else
      {
      ASSERT(FALSE);
      return FunctionNotSupported;
      }
      }*/

      e_file_result track::WriteHeader(CHUNKHDR *pHdr)
      {
         ASSERT(m_bAutoAllocation);
         byte * hpbAllocation = GetAllocationImage();
         ASSERT(hpbAllocation != NULL);
         memcpy(
            hpbAllocation,
            &pHdr->fourccType,
            sizeof(pHdr->fourccType));
         uint32_t dw = pHdr->dwLength;
         dw = DWORDSWAP(dw);
         memcpy(
            hpbAllocation + sizeof(pHdr->fourccType),
            &dw,
            sizeof(pHdr->dwLength));
         return Success;
      }

      e_file_result track::WriteHeaderLength()
      {
         ASSERT(!m_bAutoAllocation);
         //byte * hpbAllocation = GetAllocationImage();
         //ASSERT(hpbAllocation == NULL);
         byte * hpbImage = GetTrackImage();
         uint32_t dw = m_smti.m_cbLength;
         dw = DWORDSWAP(dw);
         memcpy(
            hpbImage + sizeof(FOURCC),
            &dw,
            sizeof(uint32_t));
         return Success;
      }


      e_file_result track::WriteXFLyricEvent(string &str, imedia::position tkPosition)
      {
         e_file_result smfrc;
         if(tkPosition < m_tkPosition)
            m_event.SetDelta(0);
         else
            m_event.SetDelta(tkPosition - m_tkPosition);
         m_event.SetFullType(Meta);
         m_event.SetMetaType(::music::xf::MetaLyric);

         ::ca2::international::utf8_to_multibyte(::ca2::international::CodePageLatin1, m_memstorageHelper, str);
         m_event.SetParam((byte *) m_memstorageHelper.get_data(), (int32_t) (m_memstorageHelper.get_size() - 1));
         smfrc = WriteCompleteEvent(0x7fffffff, false);
         return smfrc;
      }


      byte * track::GetAllocationImage()
      {
         return get_data();
      }

      e_file_result track::copy(track *pTrk, int32_t iMode)
      {
         event_v1 * pevent;
         imedia::position tkDelta;

         SetAutoAllocation();
         seek_begin();
         allocate(sizeof(CHUNKHDR));
         memcpy(GetTrackImage(), pTrk->GetTrackImage(), sizeof(CHUNKHDR));
         pTrk->seek_begin();
         pevent = &pTrk->m_event;
         tkDelta = 0;
         register bool bCopy;
         while(TRUE)
         {
            if(pTrk->_GetFlags().is_signalized(EndOfTrack))
               break;
            if(pTrk->ReadEvent(0x7fffffff, TRUE) != VMSR_SUCCESS)
               break;
            bCopy = true;
            if(pevent->GetFlags() != 0)
            {
               bCopy = false;
            }
            if(bCopy)
            {
               if((iMode & CopyExcludeXFMetaLyrics) > 0)
               {
                  if(pevent->_GetFullType() == Meta &&
                     pevent->_GetMetaType() == ::music::xf::MetaLyric)
                  {
                     bCopy = false;
                  }
               }
               if(bCopy)
               {
                  if((iMode & CopyExcludeKarMetaLyrics) > 0)
                  {
                     if(pevent->_GetFullType() == Meta &&
                        pevent->_GetMetaType() == MetaKarLyric)
                     {
                        bCopy = false;
                     }
                  }
               }
            }
            if(bCopy)
            {
               m_event = *pevent;
               m_event.SetDelta(m_event._GetDelta() + tkDelta);
               tkDelta = 0;
               WriteCompleteEvent(0x7fffffff, false);
            }
            else
            {
               tkDelta += pevent->_GetDelta();
            }
            if(pTrk->MoveNext() != VMSR_SUCCESS)
               break;

         }
         CHUNKHDR chHdr;
         chHdr.fourccType = FOURCC_MTrk;
         chHdr.dwLength = m_smti.m_cbLength;
         WriteHeader(&chHdr);
         return Success;
      }

      e_file_result track::CopyWork(track *pTrk, int32_t iMode)
      {
         imedia::position tkDelta;

         SetAutoAllocation();
         seek_begin();
         allocate(sizeof(CHUNKHDR));
         memcpy(GetTrackImage(), pTrk->GetTrackImage(), sizeof(CHUNKHDR));
         pTrk->seek_begin();
         tkDelta = 0;
         register bool bCopy;

         for(int32_t i = 0; i < pTrk->m_trackWorkStorage.GetEventCount(); i++)
         {
            event_v5 & event = pTrk->m_trackWorkStorage.EventAt(i);
            bCopy = true;
            if(event.GetFlags() != 0)
            {
               bCopy = false;
            }
            if(bCopy)
            {
               if((iMode & CopyExcludeXFMetaLyrics) > 0)
               {
                  if(event._GetFullType() == Meta &&
                     event._GetMetaType() == ::music::xf::MetaLyric)
                  {
                     bCopy = false;
                  }
               }
               if(bCopy)
               {
                  if((iMode & CopyExcludeKarMetaLyrics) > 0)
                  {
                     if(event._GetFullType() == Meta &&
                        event._GetMetaType() == MetaKarLyric)
                     {
                        bCopy = false;
                     }
                  }
               }
            }
            if(bCopy)
            {
               m_event = event;
               m_event.SetDelta(m_event._GetDelta() + tkDelta);
               tkDelta = 0;
               WriteCompleteEvent(0x7fffffff, false);
            }
            else
            {
               tkDelta += event._GetDelta();
            }
         }
         CHUNKHDR chHdr;
         chHdr.fourccType = FOURCC_MTrk;
         chHdr.dwLength = m_smti.m_cbLength;
         WriteHeader(&chHdr);
         return Success;
      }

      e_file_result track::WorkCopyWork(track *pTrk, int32_t iMode)
      {
         imedia::position tkDelta;
         bool bCopy;

         m_trackWorkStorage.remove_all();

         memcpy(&m_trackWorkStorage.m_chunkHeader, &pTrk->m_trackWorkStorage.m_chunkHeader, sizeof(CHUNKHDR));

         tkDelta = 0;
         for(int32_t i = 0; i < pTrk->m_trackWorkStorage.GetEventCount(); i++)
         {
            event_v5 & event = pTrk->m_trackWorkStorage.EventAt(i);
            //      ASSERT(event.GetFullType() >= Msg);
            bCopy = true;
            if(event.GetFlags() != 0)
            {
               bCopy = false;
            }
            if(bCopy)
            {
               if((iMode & CopyExcludeXFMetaLyrics) > 0)
               {
                  if(event.GetFullType() == Meta &&
                     event.GetMetaType() == ::music::xf::MetaLyric)
                  {
                     bCopy = false;
                  }
               }
               if(bCopy)
               {
                  if((iMode & CopyExcludeKarMetaLyrics) > 0)
                  {
                     if(event.GetFullType() == Meta &&
                        event.GetMetaType() == MetaKarLyric)
                     {
                        bCopy = false;
                     }
                  }
               }
            }
            if(bCopy)
            {
               m_trackWorkStorage.add(event);
               m_trackWorkStorage.LastEvent().SetDelta(event._GetDelta() + tkDelta);
               tkDelta = 0;
            }
            else
            {
               tkDelta += event._GetDelta();
            }
         }
         return Success;
      }

      e_file_result track::GetNextKarInfo(
         ::music::softkaraoke::info *pKarI)
      {
         ASSERT(pKarI != NULL);

         //    PSMF                    pSmf = (PSMF)hSmf;
         e_file_result               smfrc;
         //    CMidiEvent              m_event;
         imedia::position               tkLastDelta = 0 ;
         bool               okSongName = false;
         bool               okAuthor = false;
         bool               okCopyright = false;
         int32_t                  nTCount = 0;



         if(m_tkPosition != 0)
         {
            return   EInvalidTkPosition;
         }

         /*
         ** read events from the track and pack them into the buffer in polymsg
         ** format.
         **
         ** If a SysEx or meta would go over a buffer boundry, split it.
         */
         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         while(true)
         {
            //   if(nTCount >= 2)
            //      break;
            smfrc = ReadEvent(0, TRUE);
            if (Success != smfrc)
            {
               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-file.
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               if (SReachedTkMax == smfrc)
               {
                  //                m_fdwSMF |= EndOfFile;
                  _GetFlags().signalize(EndOfFile);
               }

               TRACE( "smfReadEvents: GetNextKarInfo() -> %u", (uint32_t)smfrc);
               break;
            }
            if(m_tkPosition > 0)
               break;

            if ((Meta == m_event.GetFullType()) &&
               (MetaKarHeader == m_event.GetMetaType()))
            {

               string str;
               ::ca2::international::MultiByteToMultiByte(
                  ::ca2::international::CodePageLatin1,
                  str,
                  ::GetOEMCP(),
                  (const char *) m_event.GetParam(), m_event.GetParamSize());
               if(str.Mid(0, 2).Compare("@T") == 0)
               {
                  nTCount++;
                  //            if(*ppKarI == NULL)
                  //            {
                  //               TRACE("****GetKarInfoHeader");
                  //               *ppKarI = new ::music::softkaraoke::info();
                  //            }
                  if(nTCount == 1)
                  {
                     pKarI->m_strSongName = str.Mid(2);
                     TRACE("Date: %s\n", pKarI->m_strSongName);
                     okSongName = true;
                     //               (*ppKarI)->m_strSongName = str.Mid(2);
                     //               TRACE("Date: %s\n", (*ppKarI)->m_strSongName);
                  }
                  if(nTCount == 2)
                  {
                     pKarI->m_strAuthor = str.Mid(2);
                     TRACE("Date: %s\n", str.Mid(2));
                     okAuthor = true;
                     //               (*ppKarI)->m_strOther = str.Mid(2);
                     //               TRACE("Date: %s\n", (*ppKarI)->m_strOther);
                  }
                  if(nTCount >= 3)
                  {
                     pKarI->m_strCopyright = str.Mid(2);
                     TRACE("Date: %s\n", str.Mid(2));
                     okCopyright = true;
                     //               (*ppKarI)->m_strOther = str.Mid(2);
                     //               TRACE("Date: %s\n", (*ppKarI)->m_strOther);
                  }
               }
            }
            smfrc = MoveNext();
            if (Success != smfrc)
            {
               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-spfile->
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               if (SReachedTkMax == smfrc)
               {
                  //                m_fdwSMF |= EndOfFile;
                  _GetFlags().signalize(EndOfFile);
               }

               TRACE( "smfReadEvents: GetNextKarInfo() -> %u", (uint32_t)smfrc);
               break;
            }
            if(okSongName && okAuthor && okCopyright)
               break;
         }
         //   if(!okSongName &&
         //      !okInfoHeader)
         //   {
         //      delete *ppXfih;
         //      pXfih = NULL;
         //   }
         //   if(!okInfoHeaderLS)
         //   {
         //      delete *ppXfihls;
         //      pXfihls= NULL;
         //   }
         //    return (m_fdwSMF & EndOfFile) ? SEndOfFile : Success;
         return okSongName || okAuthor || okCopyright ? Success : SEndOfFile;
      }


      e_file_result track::GetNextTune1000Info(
         ::music::tune1000::info *pTune1000I)
      {
         ASSERT(pTune1000I != NULL);

         //    PSMF                    pSmf = (PSMF)hSmf;
         e_file_result               smfrc;
         //    CMidiEvent              m_event;
         imedia::position               tkLastDelta = 0 ;
         bool               okSongName = FALSE;
         bool               okOther = FALSE;
         bool               okTSongName = FALSE;
         bool               okTOther = FALSE;
         int32_t               nTCount = 0;
         int32_t               nCount = 0;


         if(m_tkPosition != 0)
         {
            return   EInvalidTkPosition;
         }

         /*
         ** read events from the track and pack them into the buffer in polymsg
         ** format.
         **
         ** If a SysEx or meta would go over a buffer boundry, split it.
         */
         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         while(true)
         {
            //      if(nTCount >= 2)
            //         break;
            smfrc = ReadEvent(0, TRUE);
            if (Success != smfrc)
            {
               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-spfile->
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               if (SReachedTkMax == smfrc)
               {
                  //                m_fdwSMF |= EndOfFile;
                  _GetFlags().signalize(EndOfFile);
               }

               TRACE( "smfReadEvents: GetNextTune1000Info() -> %u", (uint32_t)smfrc);
               break;
            }
            if(m_tkPosition > 0)
               break;

            if ((Meta == m_event.GetFullType()) &&
               (MetaTrackName == m_event.GetMetaType()
               || MetaKarLyric == m_event.GetMetaType()  ))
            {

               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str,
                  (const char *) m_event.GetParam(), m_event.GetParamSize());
               if(str.Mid(0, 2).Compare("@T") == 0)
               {
                  nTCount++;
                  if(nTCount == 1)
                  {
                     pTune1000I->m_strSongName = str.Mid(2);
                     TRACE("Date: %s\n", pTune1000I->m_strSongName);
                     okTSongName = true;
                  }
                  if(nTCount >= 2)
                  {
                     pTune1000I->m_straOther.add_unique(str.Mid(2));
                     TRACE("Date: %s\n", str);
                     okTOther = true;
                  }
               }
               else if(!okTSongName && str != "@LENGL")
               {
                  nCount++;
                  if(nCount == 1)
                  {
                     pTune1000I->m_strSongName = str;
                     TRACE("Date: %s\n", pTune1000I->m_strSongName);
                     okSongName = true;
                  }
                  if(nCount == 2)
                  {
                     pTune1000I->m_straOther.add_unique(str);
                     TRACE("Date: %s\n", str);
                     okOther = true;
                  }
               }
            }
            smfrc = MoveNext();
            if (Success != smfrc)
            {
               /* smfGetNextEvent doesn't set this because smfSeek uses it
               ** as well and needs to distinguish between reaching the
               ** seek point and reaching end-of-file.
               **
               ** To the ::fontopus::user, however, we present the selection between
               ** their given tkBase and tkEnd as the entire file, therefore
               ** we want to translate this into EOF.
               */
               if (SReachedTkMax == smfrc)
               {
                  //                m_fdwSMF |= EndOfFile;
                  _GetFlags().signalize(EndOfFile);
               }

               TRACE( "smfReadEvents: GetNextTune1000Info() -> %u", (uint32_t)smfrc);
               break;
            }
         }
         //   if(!okSongName &&
         //      !okInfoHeader)
         //   {
         //      delete *ppXfih;
         //      pXfih = NULL;
         //   }
         //   if(!okInfoHeaderLS)
         //   {
         //      delete *ppXfihls;
         //      pXfihls= NULL;
         //   }
         //    return (m_fdwSMF & EndOfFile) ? SEndOfFile : Success;
         return okSongName || okOther ? Success : SEndOfFile;
      }

      e_file_result track::GetStandardEventsV1(events_v1 *pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack)
      {

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         if(bAnyTrack)
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if ((iFilter == m_event._GetType()))
               {
                  pEvents->AddEvent(&m_event, m_tkPosition);
               }
               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         else
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if ((iFilter == m_event._GetType()) &&
                  (iTrack == m_event._GetTrack() ))
               {
                  pEvents->AddEvent(&m_event, m_tkPosition);
               }
               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         return smfrc;

      }

      e_file_result track::GetNoteOnOffEventsV1(events_v1 *pEvents, int32_t iTrack, bool bAnyTrack)
      {

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         if(bAnyTrack)
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if ((NoteOn == m_event._GetType()) ||
                  (NoteOff == m_event._GetType() ))
               {
                  pEvents->AddEvent(&m_event, m_tkPosition);
               }
            }
         }
         else
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if (((NoteOn == m_event._GetType()) ||
                  (NoteOff == m_event._GetType())) &&
                  (iTrack == m_event._GetTrack()))
               {
                  pEvents->AddEvent(&m_event, m_tkPosition);
               }
               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         return smfrc;

      }


      // it returns in pevents, note on, note off,
      // and maximum and minimum peaks of pitch bend

      e_file_result track::GetLevel2Events(events_v1 *pevents, int32_t iTrack, bool bAnyTrack)
      {

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         int32_t iPitchBend;
         int32_t iLastPitchBend = 0;
         enum EDirection
         {
            Up,
            Down,
         } edirection = Up;
         event_v1 eventLastPitchBend;
         if(bAnyTrack)
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if ((NoteOn == m_event._GetType()) ||
                  (NoteOff == m_event._GetType() ))
               {
                  pevents->AddEvent(&m_event, m_tkPosition);
               }
               else if (PitchBend == m_event._GetType())
               {
                  iPitchBend = m_event.GetPitchBendLevel();
                  if(edirection == Up)
                  {
                     if(iPitchBend < iLastPitchBend)
                     {
                        edirection = Down;
                        pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                     }
                     else if(iPitchBend == 0
                        && iLastPitchBend != 0)
                     {
                        pevents->AddEvent(&m_event, m_tkPosition);
                     }

                  }
                  else if(edirection == Down)
                  {
                     if(iPitchBend > iLastPitchBend)
                     {
                        edirection = Up;
                        pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                     }
                     else if(iPitchBend == 0
                        && iLastPitchBend != 0)
                     {
                        pevents->AddEvent(&m_event, m_tkPosition);
                     }
                  }
                  eventLastPitchBend = m_event;
                  iLastPitchBend = iPitchBend;
               }

               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         else
         {
            while(TRUE)
            {
               smfrc = ReadEvent(tkMax, TRUE);
               if (Success != smfrc)
               {
                  break;
               }



               if (iTrack == m_event._GetTrack())
               {
                  if(((NoteOn == m_event._GetType()) ||
                     (NoteOff == m_event._GetType())))
                  {
                     pevents->AddEvent(&m_event, m_tkPosition);
                  }
                  else if (PitchBend == m_event._GetType())
                  {
                     iPitchBend = m_event.GetPitchBendLevel();
                     if(edirection == Up)
                     {
                        if(iPitchBend < iLastPitchBend)
                        {
                           edirection = Down;
                           pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                        }
                        else if(iPitchBend == 0
                           && iLastPitchBend != 0)
                        {
                           pevents->AddEvent(&m_event, m_tkPosition);
                        }

                     }
                     else if(edirection == Down)
                     {
                        if(iPitchBend > iLastPitchBend)
                        {
                           edirection = Up;
                           pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                        }
                        else if(iPitchBend == 0
                           && iLastPitchBend != 0)
                        {
                           pevents->AddEvent(&m_event, m_tkPosition);
                        }
                     }
                     eventLastPitchBend = m_event;
                     iLastPitchBend = iPitchBend;
                  }
               }
               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         return smfrc;

      }

      bool track::IsTune1000File()
      {
         seek_begin();
         stringa strArray;
         imedia::position_array tkArray;
         ReadTune1000Tokens(strArray, &tkArray);
         if(strArray.get_size() > 0)
            return true;
         seek_begin();
         ReadKarTokens(strArray, &tkArray);
         if(strArray.get_size() > 0)
            return true;


         return false;

      }

      bool track::IsSoftKaraokeFile(bool bWork, stringa * pstra)
      {
         event_v5 * pevent;
         e_file_result               smfrc;
         imedia::position               tkLastDelta = 0 ;
         bool               bIsSoftKaraokeFile = false;
         bool                    bSoftKaraoke = false;

         smfrc = _SeekBegin(bWork);
         if (Success != smfrc)
         {
            TRACE( "smfReadEvents: IsXFFile() -> %u", (uint32_t)smfrc);
            return false;
         }

         ASSERT(!_GetFlags().is_signalized(EndOfFile));
         int32_t nArrobaCount = 0;
         while(true)
         {
            _GetEvent(bWork, pevent, 0, TRUE);

            if(m_tkPosition > 0)
               break;

            if((Meta == pevent->GetFullType()) &&
               (MetaTrackName == pevent->GetMetaType()))
            {
               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str,
                  (const char *) pevent->GetParam(),
                  pevent->GetParamSize());
               if(str.CollateNoCase("Soft Karaoke") == 0)
               {
                  bSoftKaraoke = true;
                  if(pstra != NULL)
                  {
                     pstra->add(str);
                  }
               }
            }
            else if ((Meta == pevent->GetFullType()) &&
               (MetaKarHeader == pevent->GetMetaType()))
            {
               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str,
                  (const char *) pevent->GetParam(), pevent->GetParamSize());
               //         string strMid =  str.Mid(0, 19);
               //if(strMid.Compare("@T") == 0 ||
               //                strMid.Compare("@I") == 0 ||
               //              strMid.Compare("@L") == 0 ||
               //            strMid.Compare("@K") == 0 ||
               //          strMid.Compare("@V") == 0)
               //    {
               if(::ca2::str::begins_ci(str, "@C"))
               {
                  nArrobaCount++;
                  if(pstra != NULL)
                  {
                     pstra->add(str);
                  }
               }
               else if(::ca2::str::begins_ci(str, "@KMIDI KARAOKE FILE"))
               {
                  nArrobaCount++;
                  if(pstra != NULL)
                  {
                     pstra->add(str);
                  }
               }
            }
            //if(nArrobaCount > 0 && bSoftKaraoke)
            // break;

            smfrc = _MoveNext(bWork);

            if (Success != smfrc)
            {
               break;
            }
         }

         bIsSoftKaraokeFile = nArrobaCount > 0 || bSoftKaraoke;
         return bIsSoftKaraokeFile;
      }


      /*bool track::Mirror()
      {
      memcpy(m_hpbMirrorAllocation, m_hpbAllocation, m_dwAllocation);
      return TRUE;
      }

      e_file_result track::InsertCompleteEvent(imedia::position tkMax)
      {

      }*/

      /*bool track::ContainsEvent(event_base *pEvent)
      {
      byte * hpbImageStart = GetImage();
      byte * hpbImageEnd = hpbImageStart + m_smti.m_cbLength + sizeof(CHUNKHDR);
      return pEvent->GetImage() >= hpbImageStart &&
      pEvent->GetImage() < hpbImageEnd;
      }*/

      uint32_t track::GetTrackImageLength()
      {
         return m_smti.m_cbLength + sizeof(CHUNKHDR);
      }

      byte * track::GetTrackImage()
      {
         if(m_bAutoAllocation)
         {
            return GetAllocationImage();
         }
         else
         {
            if(m_ptracks == NULL)
               return NULL;
            ASSERT(m_ptracks != NULL);
            sp(::music::midi::file) pFile = m_ptracks->m_pFile;
            if(pFile == NULL)
               return NULL;
            else
            {
               ASSERT(pFile != NULL);
               if(pFile->GetImage() == NULL)
                  return NULL;
               else
                  return pFile->GetImage() + m_idxTrack - sizeof(CHUNKHDR);
            }
         }
      }

      uint32_t track::GetTrackBodyLength()
      {
         return m_smti.m_cbLength + sizeof(CHUNKHDR);
      }

      byte * track::GetTrackBody()
      {
         if(m_bAutoAllocation)
         {
            if(GetAllocationImage() == NULL)
               return NULL;
            else
               return GetAllocationImage() + sizeof(CHUNKHDR);
         }
         else
         {
            if(m_ptracks == NULL)
               return NULL;
            ASSERT(m_ptracks != NULL);
            sp(::music::midi::file) pFile = m_ptracks->m_pFile;
            if(pFile == NULL)
               return NULL;
            else
            {
               ASSERT(pFile != NULL);
               if(pFile->GetImage() == NULL)
                  return NULL;
               else
                  return pFile->GetImage() + m_idxTrack;
            }
         }
      }

      e_file_result track::seek(uint32_t dwSeekWhat)
      {
         e_file_result               smfrc;
         imedia::position               tkLastDelta = 0 ;
         //   bool               okSongName = FALSE;
         //   bool               okInfoHeader = FALSE;
         //   bool               okInfoHeaderLS = FALSE;

         //   if(m_tkPosition != 0)
         //   {
         //      return   EInvalidTkPosition;
         //   }

         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         while(true)
         {
            smfrc = ReadEvent(0, TRUE);
            if (Success != smfrc)
               break;
            if(m_tkPosition > 0)
               break;
            switch(dwSeekWhat)
            {
            case SeekXFSongName:
               {
                  if ((Meta == m_event.GetFullType()) &&
                     (::music::xf::MetaSongName == m_event.GetMetaType()))
                     return Success;
                  break;
               }
            case SeekXFInfoHeader:
               {
                  if ((Meta == m_event.GetFullType()) &&
                     (::music::xf::MetaXFInfoHdr == m_event.GetMetaType()))
                  {
                     string_tokenizer str;
                     ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
                     if(::ca2::str::begins_ci(str, "XFhd"))
                        return Success;
                  }
                  break;
               }
            case SeekXFInfoHeaderLS:
               {
                  if ((Meta == m_event.GetFullType()) &&
                     (::music::xf::MetaXFInfoHdr == m_event.GetMetaType()))
                  {
                     string_tokenizer str;
                     ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
                     if(::ca2::str::begins_ci(str, "XFln"))
                        return Success;
                  }
                  break;
            case SeekKarID:
               if ((Meta == m_event.GetFullType()) &&
                  (MetaTrackName == m_event.GetMetaType()))
               {
                  string str;
                  ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) m_event.GetParam(), m_event.GetParamSize());
                  if(str.CollateNoCase("Soft Karaoke") == 0)
                  {
                     return Success;
                  }
               }
               break;
               }
            default:;
            }

            smfrc = MoveNext();
            if (Success != smfrc)
               break;
         }
         return smfrc;
      }

      VMSRESULT track::WorkSeek(uint32_t dwSeekWhat)
      {
         e_file_result     smfrc = SNotFound;
         imedia::position               tkLastDelta = 0 ;
         //   bool               okSongName = FALSE;
         //   bool               okInfoHeader = FALSE;
         //   bool               okInfoHeaderLS = FALSE;

         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         ::count iSize = m_trackWorkStorage.GetEventCount() - 1;
         while(m_iWorkCurrentEvent < iSize)
         {
            event_v5 & event = m_trackWorkStorage.EventAt(m_iWorkCurrentEvent + 1);
            m_iWorkCurrentEvent++;
            m_tkPosition += event._GetDelta();
            switch(dwSeekWhat)
            {
            case SeekXFSongName:
               {
                  if((Meta == event.GetFullType()) &&
                     (::music::xf::MetaSongName == event.GetMetaType()))
                     return Success;
                  if(m_tkPosition > 0)
                     return SNotFound;
                  break;
               }
            case SeekXFInfoHeader:
               {
                  if ((Meta == event.GetFullType()) &&
                     (::music::xf::MetaXFInfoHdr == event.GetMetaType()))
                  {
                     string_tokenizer str;
                     ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) event.GetParam(), event.GetParamSize());
                     if(::ca2::str::begins_ci(str, "XFhd"))
                        return Success;
                  }
                  if(m_tkPosition > 0)
                     return SNotFound;
                  break;
               }
            case SeekXFInfoHeaderLS:
               {
                  if ((Meta == event.GetFullType()) &&
                     (::music::xf::MetaXFInfoHdr == event.GetMetaType()))
                  {
                     string_tokenizer str;
                     ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) event.GetParam(), event.GetParamSize());
                     if(::ca2::str::begins_ci(str, "XFln"))
                        return Success;
                  }
                  if(m_tkPosition > 0)
                     return SNotFound;
               }
               break;
            case SeekKarID:
               {
                  if ((Meta == event.GetFullType()) &&
                     (MetaTrackName == event.GetMetaType()))
                  {
                     if(event.GetParam() != NULL)
                     {
                        string str;
                        ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) event.GetParam(), event.GetParamSize());
                        if(str.CollateNoCase("Soft Karaoke") == 0)
                        {
                           return Success;
                        }
                     }
                  }
                  if(m_tkPosition > 0)
                     return SNotFound;
               }
               break;
            default:;
            }
         }
         return smfrc;

      }

      bool track::IsXFFile()
      {
         if(m_trackWorkStorage.GetEventCount() > 0)
         {
            event_v5 * pevent;
            e_file_result               smfrc;
            imedia::position               tkLastDelta = 0 ;
            bool               bIsXFFile = false;

            smfrc = WorkSeekBegin();
            if (Success != smfrc)
            {
               TRACE( "smfReadEvents: IsXFFile() -> %u", (uint32_t)smfrc);
               return bIsXFFile;
            }


            while(TRUE)
            {
               WorkGetEvent(pevent, 0x7fffffff, TRUE);

               if ((Meta == pevent->GetFullType()) &&
                  (MetaSeqSpecific == pevent->GetMetaType()))
               {
                  if(pevent->GetParam()[0] == 0x43 && // YAMAHA ID
                     pevent->GetParam()[1] == 0x7B && //
                     pevent->GetParam()[2] == 0x00 && //
                     pevent->GetParam()[3] == 0x58 && // X
                     pevent->GetParam()[4] == 0x46 && // F
                     pevent->GetParam()[5] == 0x30) // 0
                  {
                     bIsXFFile = pevent->GetParam()[6] == 0x32 && m_tkPosition == 0;
                     bIsXFFile |= (pevent->GetParam()[6] == 0x31);
                     if(bIsXFFile)
                        break;
                  }
               }
               smfrc = WorkMoveNext();
               if (Success != smfrc)
               {
                  TRACE( "smfReadEvents: IsXFFile() -> %u", (uint32_t)smfrc);
                  break;
               }
            }
            return bIsXFFile;
         }
         else
         {
            e_file_result               smfrc;
            imedia::position               tkLastDelta = 0 ;
            bool               bIsXFFile = false;

            smfrc = seek_begin();
            if (Success != smfrc)
            {
               TRACE( "smfReadEvents: IsXFFile() -> %u", (uint32_t)smfrc);
               return bIsXFFile;
            }


            while(TRUE)
            {
               smfrc = ReadEvent(0, true);
               if(smfrc != Success)
                  return false;

               if ((Meta == m_event.GetFullType()) &&
                  (MetaSeqSpecific == m_event.GetMetaType()))
               {
                  if(m_event.GetParam()[0] == 0x43 && // YAMAHA ID
                     m_event.GetParam()[1] == 0x7B && //
                     m_event.GetParam()[2] == 0x00 && //
                     m_event.GetParam()[3] == 0x58 && // X
                     m_event.GetParam()[4] == 0x46 && // F
                     m_event.GetParam()[5] == 0x30) // 0
                  {
                     bIsXFFile = m_event.GetParam()[6] == 0x32 && m_tkPosition == 0;
                     bIsXFFile |= (m_event.GetParam()[6] == 0x31);
                     if(bIsXFFile)
                        break;
                  }
               }
               smfrc = MoveNext();
               if (Success != smfrc)
               {
                  TRACE( "smfReadEvents: IsXFFile() -> %u", (uint32_t)smfrc);
                  break;
               }
            }
            return bIsXFFile;
         }

      }

      e_file_result track::ReadTune1000Tokens(
         stringa &  tokena,
         imedia::position_array *lptkaTicks)
      {
         ASSERT(lptkaTicks != NULL);
         string str;
         tokena.remove_all();
         lptkaTicks->remove_all();

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         while(true)
         {
            smfrc = ReadEvent(tkMax, TRUE);
            if (Success != smfrc)
            {
               break;
            }


            if ((Meta == m_event.GetFullType()) &&
               (MetaLyric == m_event.GetMetaType()))
            {
               /*         ::ca2::international::MultiByteToUnicode(
               CodePageLatin1,
               str,
               (char *) m_event.GetParam(),
               m_event.GetParamSize());*/
               LPTSTR lpsz = str.GetBufferSetLength(m_event.GetParamSize() + 1);
               memcpy(lpsz, m_event.GetParam(), m_event.GetParamSize());
               lpsz[m_event.GetParamSize()] = '\0';
               str.ReleaseBuffer();
               tokena.add(str);
               lptkaTicks->add(m_tkPosition);
            }
            smfrc = MoveNext();
            if (Success != smfrc)
            {
               break;
            }
         }

         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }


      VMSRESULT track::FromWorkTrack(bool b)
      {
         UNREFERENCED_PARAMETER(b);
         return FromWorkTrack(m_trackWorkStorage);
      }

      VMSRESULT track::FromWorkTrack(track_v8 & track)
      {

         SetAutoAllocation();
         allocate(sizeof(CHUNKHDR));
         seek_begin();

         m_smti.m_cbLength = 0;
         ::count iCount = track.GetEventCount();
         for(::index i = 0; i < iCount; i++)
         {
            WriteEvent(track.EventAt(i));
         }

         m_smti.m_cbLength = (uint32_t) (this->get_size() - sizeof(CHUNKHDR));
         m_trackWorkStorage.m_chunkHeader.dwLength = (uint32_t) (this->get_size() - sizeof(CHUNKHDR));
         WriteHeader(&track.m_chunkHeader);

         return VMSR_SUCCESS;
      }


      VMSRESULT track::ToWorkStorage()
      {
         event_v1 eventWork;

         int32_t iCurrentEvent = m_iCurrentEvent;

         ASSERT(GetTrackImage());

         memcpy(&m_trackWorkStorage.m_chunkHeader, GetTrackImage(), sizeof(CHUNKHDR));


         seek_begin();
         m_trackWorkStorage.remove_all();
         while(ReadEvent(eventWork) == VMSR_SUCCESS)
         {
            //      ASSERT(eventWork.GetFullType() >= Msg);
            //      if(m_tkPosition >= 31450)
            //       __debug_break();
            m_trackWorkStorage.add(eventWork);
            if(MoveNext() != Success)
               break;
         }
         m_smti.m_tkLength = m_tkPosition;
         m_iCurrentEvent = iCurrentEvent;
         return VMSR_SUCCESS;
      }

      track_v8 & track::GetWorkTrack()
      {
         return m_trackWorkStorage;
      }

      e_file_result track::WriteEvent(event_v5 & eventMidi)
      {
         ASSERT(m_estate == StateOnDelta);
         ASSERT(m_bAutoAllocation);

         BYTE                       bEvent;
         uint32_t                      cbEvent;
         e_file_result         smfrc;
         uint32_t                      dwUsed;

         m_tkDelta = eventMidi._GetDelta();

         if(m_tkDelta < 0)
            m_tkDelta = 0;

         if((smfrc = WriteDelta()) != Success)
         {
            return smfrc;
         }

         m_tkPosition += eventMidi._GetDelta();

         bEvent = eventMidi.GetFullType();

         //if(bEvent == m_ptracks->m_uchRunningStatus)
         if(false) // do not optimize with Running Status
         {
            try
            {
               allocate_add_up(1);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }

            *m_hpbImage++ = eventMidi.GetChB1();

            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = eventMidi.GetChB2();
            }
         }
         else if(bEvent < SysEx)
         {
            m_ptracks->m_uchRunningStatus = bEvent;
            try
            {
               allocate_add_up(2);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }
            *m_hpbImage++ = bEvent;
            *m_hpbImage++ = eventMidi.GetChB1();
            if (3 == GetMessageLen(m_ptracks->m_uchRunningStatus))
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = eventMidi.GetChB2();
            }
         }
         else
         {
            if (Meta == bEvent)
            {
               try
               {
                  allocate_add_up(2);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = Meta;
               if (MetaEOT == (*m_hpbImage++ = eventMidi.GetMetaType()))
               {
                  _GetFlags().signalize(EndOfTrack);
                  m_smti.m_tkLength = m_tkPosition;
               }
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               try
               {
                  allocate_add_up(1);
               }
               catch(memory_exception * pe)
               {
                  pe->Delete();
                  return ENoMemory;
               }
               *m_hpbImage++ = bEvent;
            }
            else
            {
               ASSERT(FALSE);
            }

            cbEvent = (uint32_t) eventMidi.GetParamSize();

            if(!(dwUsed = SetVDWord(cbEvent)))
            {
               return ENoMemory;
            }

            m_hpbImage     += dwUsed;

            try
            {
               allocate_add_up(cbEvent);
            }
            catch(memory_exception * pe)
            {
               pe->Delete();
               return ENoMemory;
            }

            memcpy(m_hpbImage, eventMidi.GetParam(), cbEvent);

            m_hpbImage     += cbEvent;
         }

         m_estate = StateOnDelta;

         return Success;
      }

      int32_t track::GetCurrentEvent()
      {
         return m_iCurrentEvent;
      }

      void track::WorkDeleteEvent()
      {
         m_trackWorkStorage.remove_at(m_iWorkCurrentEvent);
         ///    m_iCurrentEvent--;
      }


      /*track & track::operator =(track &eventSrc)
      {
      m_bAutoAllocation = eventSrc.m_bAutoAllocation;
      m_ptracks->m_uchRunningStatus = eventSrc.m_ptracks->m_uchRunningStatus;
      m_cbLeft = eventSrc.m_cbLeft;
      //m_dwAllocation = eventSrc.m_dwAllocation;
      //    m_dwAllocationAddUp = eventSrc.m_dwAllocationAddUp;
      m_dwUsed = eventSrc.m_dwUsed;
      m_event = eventSrc.m_event;
      m_fdwTrack = eventSrc.m_fdwTrack;
      if(eventSrc.m_hpbAllocation != NULL &&
      eventSrc.m_bAutoAllocation)
      {
      allocate(eventSrc.m_dwAllocation);
      memcpy(m_hpbAllocation, eventSrc.m_hpbAllocation, m_dwAllocation);
      }
      m_hpbEventImage = m_hpbAllocation + (eventSrc.m_hpbEventImage - eventSrc.m_hpbAllocation);
      m_hpbImage = m_hpbAllocation + (eventSrc.m_hpbImage - eventSrc.m_hpbAllocation);
      m_iCurrentEvent = eventSrc.m_iCurrentEvent;
      m_idxTrack = eventSrc.m_idxTrack;
      m_estate = eventSrc.m_estate;
      m_ptracks = eventSrc.m_ptracks;
      m_smti = eventSrc.m_smti;
      m_tkDelta = eventSrc.m_tkDelta;
      m_tkPosition = eventSrc.m_tkPosition;
      m_trackWorkStorage = eventSrc.m_trackWorkStorage;

      return *this;

      }*/

      e_file_result track::get_position(imedia::position & tkPosition, imedia::position tkMax)
      {
         UNREFERENCED_PARAMETER(tkMax);
         ASSERT(m_estate == StateOnDelta);

         imedia::position tkDelta;
         VMSRESULT vmsr;
         vmsr = GetDelta(tkDelta);
         if (Success != vmsr)
         {
            _GetFlags().signalize(EndOfTrack);
            return SEndOfTrack;
         }
         if(_GetFlags().is_signalized(EndOfTrack))
         {
            return SEndOfTrack;
         }
         tkPosition = m_tkPosition + tkDelta;
         return Success;

      }

      /*int32_t track::GetFlags()
      {
      return m_fdwTrack;
      }*/

      imedia::position track::get_position()
      {
         if(m_estate == StateOnDelta || m_estate == StateOnEventRead)
         {
            return m_tkPosition;
         }
         else
         {
            return m_tkPosition + m_tkDelta;
         }
      }

      int32_t track::GetMidiTrackIndex()
      {
         seek_begin();
         int32_t iFirstTrack = -1;
         int32_t iTrack = -1;
         while(Success == ReadEvent(0x7fffffff, true))
         {
            event_v1 & event = GetEvent();
            if(event.GetFullType() < SysEx)
            {
               iTrack = event._GetTrack();
               if(iFirstTrack < 0)
               {
                  iFirstTrack = iTrack;
               }
               else
               {
                  if(iTrack != iFirstTrack)
                  {
                     return -2;
                  }
               }
            }
         }
         return iFirstTrack;

      }

      e_file_result track::MoveNext()
      {
         if(m_estate != StateOnEventRead)
         {
            return EFail;
         }

         if(_GetFlags().is_signalized(EndOfTrack))
         {
            return SEndOfTrack;
         }

         m_iCurrentEvent++;
         m_estate = StateOnDelta;

         return Success;

      }

      e_file_result track::ReadEvent()
      {
         return ReadEvent(0x7fffffff, true);
      }

      e_file_result track::GetDelta(imedia::position &tkDeltaParam)
      {
         imedia::position tkDelta;

         switch(m_estate)
         {
         case StateOnDelta:
            {
               if (!(m_dwUsed = GetVDWord((uint32_t *) &tkDelta)))
               {
                  TRACE("Hit end of track w/o end marker!\n");
                  return EInvalidFile;
               }
               tkDeltaParam = tkDelta;
               return Success;
            }
         case StateOnEvent:
            {
               tkDeltaParam = m_tkDelta;
               return Success;
            }
         case StateOnEventRead:
            {
               tkDeltaParam = m_tkDelta;
               return Success;
            }
         }
         ASSERT(FALSE);
         return EFail;



      }

      e_file_result track::SeekXFLyricsHeader()
      {
         e_file_result               smfrc;
         imedia::position               tkLastDelta = 0 ;


         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         while(true)
         {
            smfrc = ReadEvent(0, TRUE);
            if(Success != smfrc)
               break;
            if(m_tkPosition > 0)
            {
               smfrc = SNotFound;
               break;
            }
            if ((Meta == m_event.GetFullType()) &&
               (::music::xf::MetaLyricsHeader == m_event.GetMetaType()))
            {
               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str, (const char *) m_event.GetParam(), 6);
               if(str.CollateNoCase("$Lyrc:") == 0)
               {
                  return Success;
               }
            }

            smfrc = MoveNext();
            if (Success != smfrc)
               break;
         }
         return smfrc;

      }

      e_file_result track::ReadXFLyricsHeader(::music::xf::lyrics_id *pxflh)
      {
         if (Meta != m_event.GetFullType())
            return EFail;
         if(::music::xf::MetaLyricsHeader != m_event.GetMetaType())
            return EFail;
         string_tokenizer tokenizer;
         string strToken;
         ::ca2::international::multibyte_to_utf8(
            ::ca2::international::CodePageLatin1,
            tokenizer, (const char *) m_event.GetParam(), m_event.GetParamSize());
         if(!tokenizer.GetNextToken(strToken, ":"))
            return EFail;
         if(strToken.CollateNoCase("$Lyrc") != 0)
            return EFail;

         string strMidiChannel;
         if(!tokenizer.GetNextToken(strMidiChannel, ":"))
            return EFail;

         string strOffsetValue;
         if(!tokenizer.GetNextToken(strOffsetValue, ":"))
            return EFail;

         string strLanguage;
         tokenizer.GetNextToken(strLanguage, ":");

         pxflh->m_strLanguage = strLanguage;


         tokenizer.Restart(strMidiChannel);
         string strChannel;
         while(tokenizer.GetNextToken(strChannel, ","))
         {
            pxflh->m_MelodyTracks.add(_tcstoul(strChannel, NULL, 10));
         }

         pxflh->m_tkDisplayOffset.from(strOffsetValue);


         return Success;


      }

      e_file_result track::WorkReadXFTokens(uint32_t uiCodePage, stringa &  tokena, imedia::position_array & positiona, int_array & iaTokenLine, bool bPlain)
      {

         string str;
         tokena.remove_all();
         positiona.remove_all();
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         event_v5 * pevent;
         int32_t iLine = 0;

         if(!_WorkIsEOT())
         {
            while(TRUE)
            {

               _WorkGetEvent(pevent, tkMax, TRUE);

               if ((Meta == pevent->GetFullType()) &&
                  (::music::xf::MetaLyric == pevent->GetMetaType()))
               {
                  if(uiCodePage == (uint32_t) -1)
                  {
                     str = string((const char * ) pevent->GetParam(), pevent->GetParamSize());
                  }
                  else 
                  {
                     ::ca2::international::multibyte_to_utf8(uiCodePage, str, (const char * ) pevent->GetParam(), pevent->GetParamSize());
                  }
                  if(str[0] == '<')
                  {
                     iLine++;
                     if(bPlain)
                     {
                        str = str.Mid(1);
                     }
                  }
                  TRACE("%s\n", str);
                  tokena.add(str);
                  iaTokenLine.add(iLine);
                  positiona.add(imedia::position(m_tkPosition));
               }

               smfrc = _WorkMoveNext();

               if (Success != smfrc)
               {
                  break;
               }

            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      /*e_file_result track::WorkReadXFTokens(
      stringa &  tokena,
      imedia::position_array & positiona,
      int_array & iaTokenLine)
      {
      string str;
      string str;
      tokena.remove_all();
      positiona.remove_all();
      imedia::position tkMax = m_smti.m_tkLength;
      e_file_result smfrc;
      imedia::position tkLastPosition = 0;

      event_v5 * pevent;
      int32_t iLine = 0;

      if(!_WorkIsEOT())
      {
      while(TRUE)
      {
      _WorkGetEvent(pevent, tkMax, TRUE);

      if ((Meta == pevent->GetFullType()) &&
      (::music::xf::MetaLyric == pevent->GetMetaType()))
      {
      LPTSTR lpsz = str.GetBuffer(pevent->GetParamSize() + 1);
      memcpy(lpsz, pevent->GetParam(), pevent->GetParamSize());
      lpsz[pevent->GetParamSize()] = '\0';
      if(lpsz[0] == '<')
      {
      iLine++;
      }
      str.ReleaseBuffer();
      TRACE("%s\n", str);
      ::ca2::international::ACPToUnicode(str, str);
      tokena.add(str);
      iaTokenLine.add(iLine);
      positiona.add(imedia::position(m_tkPosition));
      }


      smfrc = _WorkMoveNext();
      if (Success != smfrc)
      {
      break;
      }
      }
      }


      return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }*/

      e_file_result track::WriteXFLyricTrack(
         stringa &  tokena,
         imedia::position_array & positiona,
         const char * psz)
      {

         string XFLYRICSHEADER = "$Lyrc:1:270:" + string(psz);
         BYTE CUEPOINTSOLO[] = "&s";
         string str;
         imedia::position tkPosition;

         event_v5 event;



         WorkSeekBegin();

         event.SetFullType(Meta);
         event.SetMetaType(MetaXFLyricsHeader);
         event.SetParam((void *) (const char *) XFLYRICSHEADER, XFLYRICSHEADER.length());
         event.SetDelta(0);
         WorkWriteEvent(event);

         event.SetFullType(Meta);
         event.SetMetaType(MetaCuePoint);
         event.SetParam(CUEPOINTSOLO, sizeof(CUEPOINTSOLO) - 1);
         WorkWriteEvent(event);

         string strToken;
         for(int32_t j = 0; j < tokena.get_size(); j++)
         {
            strToken = tokena.element_at(j);
            tkPosition = positiona.get_at(j);
            if(strToken.Mid(0, 1).Compare("\\") == 0)
            {
               str = "<";
               str += strToken.Mid(1);
            }
            else if(strToken.Mid(0, 1).Compare("/") == 0)
            {
               str = "/";
               WorkWriteXFLyricEvent(str, tkPosition);
               str = strToken.Mid(1);
            }
            else
            {
               str = strToken;
            }
            str.replace(" ", "^");
            WorkWriteXFLyricEvent(str, tkPosition);
         }
         event.SetFullType(Meta);
         event.SetMetaType(MetaEOT);
         event.SetParam(NULL, 0);
         WorkWriteEvent(event);
         m_trackWorkStorage.m_chunkHeader.fourccType = FOURCC_XFKM;

         return Success;

      }

      e_file_result track::WorkReadEmptyXFTokens(
         stringa &  tokena,
         imedia::position_array & positiona)
      {
         string str;
         tokena.remove_all();
         positiona.remove_all();
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         event_v5 * pevent;

         if(!_WorkIsEOT())
         {
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);

               if (NoteOn == pevent->GetType_() &&
                  pevent->GetNoteVelocity() != 0)
               {
                  //LPTSTR lpsz = str.GetBuffer(pevent->GetParamSize() + 1);
                  // LPTSTR lpsz = "---";
                  //memcpy(lpsz, pevent->GetParam(), pevent->GetParamSize());
                  //lpsz[pevent->GetParamSize()] = '\0';
                  //str.ReleaseBuffer();
                  str = "---";
                  TRACE("%s\n", str);
                  tokena.add(str);
                  positiona.add(imedia::position(m_tkPosition));
               }


               smfrc = _WorkMoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      e_file_result track::WorkReadXFTokens(uint32_t uiCodePage, stringa &  tokena, imedia::position_array * lptkaTicks)
      {
         ASSERT(lptkaTicks != NULL);
         string str;
         tokena.remove_all();
         lptkaTicks->remove_all();
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         event_v5 * pevent;

         if(!_WorkIsEOT())
         {
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);

               if((Meta == pevent->GetFullType()) &&
                  (::music::xf::MetaLyric == pevent->GetMetaType()))
               {
                  ::ca2::international::multibyte_to_utf8(uiCodePage, str, (const char * ) pevent->GetParam(), pevent->GetParamSize());
                  tokena.add(str);
                  lptkaTicks->add(m_tkPosition);
               }


               smfrc = _WorkMoveNext();

               if (Success != smfrc)
               {
                  break;
               }

            }
         }


         return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      /*e_file_result track::WorkReadAnsiXFTokens(stringa &  tokena, imedia::position_array *   lptkaTicks)
      {
      ASSERT(lptkaTicks != NULL);
      string str;
      tokena.remove_all();
      lptkaTicks->remove_all();
      imedia::position tkMax = m_smti.m_tkLength;
      e_file_result smfrc;
      imedia::position tkLastPosition = 0;

      event_v5 * pevent;

      if(!_WorkIsEOT())
      {
      while(TRUE)
      {

      _WorkGetEvent(pevent, tkMax, TRUE);

      if ((Meta == pevent->GetFullType()) &&
      (::music::xf::MetaLyric == pevent->GetMetaType()))
      {
      ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, str, (const char *) pevent->GetParam(), pevent->GetParamSize());
      TRACE("%s\n", str);
      tokena.add(str);
      lptkaTicks->add(m_tkPosition);
      }

      smfrc =  WorkMoveNext();

      if (Success != smfrc)
      {
      break;
      }

      }
      }

      return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }

      e_file_result track::WorkReadShiftJisXFTokens(stringa &  tokena, imedia::position_array *   lptkaTicks)
      {
      ASSERT(lptkaTicks != NULL);
      tokena.remove_all();
      lptkaTicks->remove_all();
      imedia::position tkMax = m_smti.m_tkLength;
      e_file_result smfrc;
      imedia::position tkLastPosition = 0;
      string str;
      event_v5 * pevent;

      if(!_WorkIsEOT())
      {
      while(TRUE)
      {
      _WorkGetEvent(pevent, tkMax, TRUE);
      if ((Meta == pevent->GetFullType()) &&
      (::music::xf::MetaLyric == pevent->GetMetaType()))
      {
      ::ca2::international::MultiByteToUnicode(
      ::ca2::international::CodePageShiftJIS,
      str,
      (const char *) pevent->GetParam(),
      pevent->GetParamSize());
      TRACE("%s\n", str);
      tokena.add(str);
      lptkaTicks->add(m_tkPosition);
      }
      smfrc = _WorkMoveNext();
      if (Success != smfrc)
      {
      break;
      }
      }
      }

      return tokena.get_size() == 0 ? SEndOfTrack : Success;
      }*/

      e_file_result track::WorkReadKarTokens(stringa & tokena, imedia::position_array * lptkaTicks, stringa & stra)
      {

         bool bUtf16le = false;
         for(int32_t i = 0; i < stra.get_count(); i++)
         {
            if(::ca2::str::begins_ci(stra[i], "@C"))
            {
               if(stra[i].find_ci("utf-16le"))
               {
                  bUtf16le = true;
               }
               else if(stra[i].find_ci("utf-16"))
               {
                  bUtf16le = true;
               }
            }
         }

         ASSERT(lptkaTicks != NULL);
         string str;
         tokena.remove_all();
         lptkaTicks->remove_all();
         // CMidiEvent m_event;
         imedia::position tkMax = m_smti.m_tkLength;
         imedia::position tkLastPosition = 0;
         imedia::position tkPosition = 0;
         int32_t i;
         wstring wstr;
         for(i = 0; i < m_trackWorkStorage.GetEventCount(); i++)
         {
            if(tkPosition > 0)
               break;
            event_v5 & event = m_trackWorkStorage.EventAt(i);
            tkPosition = event._GetPosition();
            if((Meta == event.GetFullType()) &&
               (MetaKarLyric == event.GetMetaType()))
            {
               if(bUtf16le)
               {
                  LPWSTR lpwsz = wstr.alloc(event.GetParamSize() / 2);
                  memcpy(lpwsz, event.GetParam(), event.GetParamSize());
                  lpwsz[event.GetParamSize() / 2] = '\0';
                  wstr.set_length(event.GetParamSize() / 2);
                  str = ::ca2::international::unicode_to_utf8(wstr);
               }
               else
               {
                  LPTSTR lpsz = str.GetBufferSetLength(event.GetParamSize() + 1);
                  memcpy(lpsz, event.GetParam(), event.GetParamSize());
                  lpsz[event.GetParamSize()] = '\0';
                  str.ReleaseBuffer();
               }
               /*         ::ca2::international::MultiByteToUnicode(
               CodePageLatin1,
               str,
               (char *) event.GetParam(), event.GetParamSize());*/
               if(!str.is_empty())
               {
                  if(str.operator[](0) != '@')
                  {
                     tokena.add(str);
                     lptkaTicks->add(tkPosition);
                  }
               }
            }
         }

         for(; i < m_trackWorkStorage.GetEventCount(); i++)
         {

            event_v5 & event = m_trackWorkStorage.EventAt(i);

            tkPosition = event._GetPosition();

            if ((Meta == event.GetFullType()) &&
               (MetaKarLyric == event.GetMetaType()))
            {
               if(bUtf16le)
               {
                  LPWSTR lpwsz = wstr.alloc(event.GetParamSize() / 2);
                  memcpy(lpwsz, event.GetParam(), event.GetParamSize());
                  lpwsz[event.GetParamSize() / 2] = '\0';
                  wstr.set_length(event.GetParamSize() / 2);
                  str = ::ca2::international::unicode_to_utf8(wstr);
               }
               else
               {
                  LPTSTR lpsz = str.GetBufferSetLength(event.GetParamSize() + 1);
                  memcpy(lpsz, event.GetParam(), event.GetParamSize());
                  lpsz[event.GetParamSize()] = '\0';
                  str.ReleaseBuffer();
               }
               /*
               ::ca2::international::MultiByteToUnicode(
               CodePageLatin1,
               str, (char *) event.GetParam(), event.GetParamSize());*/
               tokena.add(str);
               lptkaTicks->add(tkPosition);
            }
         }

         return tokena.get_size() == 0 ? SEndOfTrack : Success;

      }


      e_file_result track::WorkReadTune1000Tokens(
         stringa &  tokena,
         imedia::position_array *lptkaTicks)
      {
         ASSERT(lptkaTicks != NULL);
         string str;
         tokena.remove_all();
         lptkaTicks->remove_all();

         imedia::position tkMax = m_smti.m_tkLength;
         imedia::position tkLastPosition = 0;

         imedia::position tkPosition = 0;

         for(int32_t i = 0; i < m_trackWorkStorage.GetEventCount(); i++)
         {
            event_v5 & event = m_trackWorkStorage.EventAt(i);
            tkPosition = event._GetPosition();
            if ((Meta == event.GetFullType()) &&
               (MetaLyric == event.GetMetaType()))
            {
               LPTSTR lpsz = str.GetBufferSetLength(event.GetParamSize() + 1);
               memcpy(lpsz, event.GetParam(), event.GetParamSize());
               lpsz[event.GetParamSize()] = '\0';
               str.ReleaseBuffer();
               /*         ::ca2::international::MultiByteToUnicode(
               CodePageLatin1,
               str, (char *) event.GetParam(), event.GetParamSize());*/
               tokena.add(str);
               lptkaTicks->add(tkPosition);
            }
         }

         return tokena.get_size() == 0 ? SEndOfTrack : Success;

      }



      e_file_result track::WorkMoveNext()
      {
         m_estate = StateOnEventRead;

         m_iWorkCurrentEvent++;
         if(m_iWorkCurrentEvent >= m_trackWorkStorage.GetEventCount())
         {
            m_bWorkTrackEnd = true;
            _GetFlags().signalize(EndOfFile);
            m_smti.m_tkLength = m_tkPosition;
            return SEndOfTrack;
         }

         m_tkPosition  += m_trackWorkStorage.EventAt(m_iWorkCurrentEvent).GetDelta();

         return Success;

      }


      void track::WorkGetEvent(
         event_base *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(tkMax);
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         ASSERT(pevent != NULL);
         ASSERT(m_estate == StateOnEventRead);
         ASSERT(!_WorkIsEOT());
         pevent = &m_trackWorkStorage.EventAt(m_iWorkCurrentEvent);
      }

      void track::WorkGetEvent(
         event_v5 *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(tkMax);
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         ASSERT(m_estate == StateOnEventRead);
         ASSERT(!_WorkIsEOT());
         pevent = &m_trackWorkStorage.EventAt(m_iWorkCurrentEvent);
      }


      e_file_result   track::WorkSeekBegin()
      {
         m_iWorkCurrentEvent  = -1;
         m_tkPosition         = 0;
         m_bWorkTrackEnd      = false;
         return _WorkMoveNext();
      }

      e_file_result   track::WorkSeekEnd()
      {
         m_iWorkCurrentEvent = m_trackWorkStorage.GetEventCount();
         m_bWorkTrackEnd = true;
         m_estate = StateOnEventRead;
         _GetFlags().signalize(EndOfFile);
         m_tkPosition = m_smti.m_tkLength;
         if(m_trackWorkStorage.GetEventCount() <= 0)
         {
            m_tkPosition = 0;
         }
         else
         {
            m_tkPosition = m_trackWorkStorage.EventAt(m_trackWorkStorage.GetEventCount() - 1).get_position();
         }
         return SEndOfTrack;
      }

      e_file_result track::WorkGetNoteOnOffEventsV1(events_v1 *pEvents, int32_t iTrack, bool bAnyTrack)
      {
         event_v5 * pevent;
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         if(!_WorkIsEOT())
         {
            if(bAnyTrack)
            {
               while(TRUE)
               {
                  _WorkGetEvent(pevent, tkMax, TRUE);

                  if ((NoteOn == pevent->_GetType()) ||
                     (NoteOff == pevent->_GetType() ))
                  {

                     pEvents->AddEvent(pevent, m_tkPosition);
                  }
                  smfrc = _WorkMoveNext();
                  if (Success != smfrc)
                  {
                     break;
                  }
               }
            }
            else
            {
               while(TRUE)
               {
                  _WorkGetEvent(pevent, tkMax, TRUE);

                  if (((NoteOn == pevent->_GetType()) ||
                     (NoteOff == pevent->_GetType())) &&
                     (iTrack == pevent->_GetTrack()))
                  {
                     pEvents->AddEvent(pevent, m_tkPosition);
                  }
                  smfrc = _WorkMoveNext();
                  if (Success != smfrc)
                  {
                     break;
                  }
               }
            }
         }
         return smfrc;

      }


      // it returns in pevents, note on, note off,
      // and maximum and minimum peaks of pitch bend

      e_file_result track::WorkGetLevel2Events(events_v1 *pevents, int32_t iTrack, bool bAnyTrack)
      {
         event_v5 * pevent;
         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         int32_t iPitchBend;
         int32_t iLastPitchBend = 0;
         enum EDirection
         {
            Up,
            Down,
         } edirection = Up;
         event_v1 eventLastPitchBend;
         if(!_WorkIsEOT())
         {
            if(bAnyTrack)
            {
               while(TRUE)
               {
                  _WorkGetEvent(pevent, tkMax, TRUE);

                  if ((NoteOn == pevent->_GetType()) ||
                     (NoteOff == pevent->_GetType() ))
                  {

                     pevents->AddEvent(pevent, m_tkPosition);
                  }
                  else if (PitchBend == pevent->_GetType())
                  {
                     iPitchBend = pevent->GetPitchBendLevel();
                     if(edirection == Up)
                     {
                        if(iPitchBend < iLastPitchBend)
                        {
                           edirection = Down;
                           //                     pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                           pevents->AddEvent(pevent, m_tkPosition);
                        }
                        else if(iPitchBend == 0
                           && iLastPitchBend != 0)
                        {
                           pevents->AddEvent(pevent, m_tkPosition);
                        }

                     }
                     else if(edirection == Down)
                     {
                        if(iPitchBend > iLastPitchBend)
                        {
                           edirection = Up;
                           //                     pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                           pevents->AddEvent(pevent, m_tkPosition);
                        }
                        else if(iPitchBend == 0
                           && iLastPitchBend != 0)
                        {
                           pevents->AddEvent(pevent, m_tkPosition);
                        }
                     }
                     eventLastPitchBend = *pevent;
                     iLastPitchBend = iPitchBend;
                  }

                  smfrc = _WorkMoveNext();
                  if (Success != smfrc)
                  {
                     break;
                  }
               }
            }
            else
            {
               while(TRUE)
               {
                  _WorkGetEvent(pevent, tkMax, TRUE);


                  if (iTrack == pevent->_GetTrack())
                  {
                     if(((NoteOn == pevent->_GetType()) ||
                        (NoteOff == pevent->_GetType())))
                     {
                        pevents->AddEvent(pevent, m_tkPosition);
                     }
                     else if (PitchBend == pevent->_GetType())
                     {
                        iPitchBend = pevent->GetPitchBendLevel();
                        if(edirection == Up)
                        {
                           if(iPitchBend < iLastPitchBend)
                           {
                              edirection = Down;
                              //pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                              pevents->AddEvent(pevent, m_tkPosition);
                           }
                           else if(iPitchBend == 0
                              && iLastPitchBend != 0)
                           {
                              pevents->AddEvent(pevent, m_tkPosition);
                           }

                        }
                        else if(edirection == Down)
                        {
                           if(iPitchBend > iLastPitchBend)
                           {
                              edirection = Up;
                              //pevents->AddEvent(&eventLastPitchBend, tkLastPosition);
                              pevents->AddEvent(pevent, m_tkPosition);
                           }
                           else if(iPitchBend == 0
                              && iLastPitchBend != 0)
                           {
                              pevents->AddEvent(pevent, m_tkPosition);
                           }
                        }
                        eventLastPitchBend = *pevent;
                        iLastPitchBend = iPitchBend;
                     }
                  }
                  smfrc = _WorkMoveNext();
                  if (Success != smfrc)
                  {
                     break;
                  }
               }
            }
         }
         return smfrc;

      }

      e_file_result track::WorkGetStandardEventsV1(events_v1 *pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack)
      {
         event_v5 * pevent;

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;
         if(bAnyTrack)
         {
            if(_WorkIsEOT())
               SEndOfTrack;
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);

               if(iFilter == NoteOn)
               {
                  if(NoteOn == pevent->_GetType()
                     && pevent->GetNoteVelocity() != 0)
                  {
                     pEvents->AddEvent(pevent);
                  }
               }
               else
               {
                  if(iFilter == pevent->_GetType())
                  {
                     pEvents->AddEvent(pevent);
                  }
               }

               smfrc = _WorkMoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         else
         {
            if(_WorkIsEOT())
               SEndOfTrack;
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);

               if(iTrack == pevent->_GetTrack())
               {
                  if(iFilter == NoteOn)
                  {
                     if(NoteOn == pevent->_GetType()
                        && pevent->GetNoteVelocity() != 0)
                     {
                        pEvents->AddEvent(pevent);
                     }
                  }
                  else
                  {
                     if (iFilter == pevent->_GetType())
                     {

                        pEvents->AddEvent(pevent);
                     }
                  }
               }

               smfrc = _WorkMoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         return smfrc;

      }

      e_file_result track::WorkGetStandardEventsV1OnlyMelodic(events_v1 *pEvents, int32_t iFilter, int32_t iTrack, bool bAnyTrack)
      {
         event_v5 * pevent;

         imedia::position tkMax = m_smti.m_tkLength;
         e_file_result smfrc;
         imedia::position tkLastPosition = 0;

         if(bAnyTrack)
         {
            if(_WorkIsEOT())
               SEndOfTrack;
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);


               if ((iFilter == pevent->_GetType()))
               {
                  pEvents->AddEvent(pevent);
               }

               smfrc = _WorkMoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         else
         {
            if(_WorkIsEOT())
               SEndOfTrack;
            while(TRUE)
            {
               _WorkGetEvent(pevent, tkMax, TRUE);

               if ((iFilter == pevent->_GetType()) &&
                  (iTrack == pevent->_GetTrack() ))
               {

                  pEvents->AddEvent(pevent);
               }

               smfrc = _WorkMoveNext();
               if (Success != smfrc)
               {
                  break;
               }
            }
         }
         return smfrc;

      }

      track & track::operator =(const track &track)
      {
         m_bAutoAllocation = track.m_bAutoAllocation;
         m_cbLeft = track.m_cbLeft;
         m_dwUsed = track.m_dwUsed;
         m_estate = track.m_estate;
         m_event = track.m_event;
         m_flags = track.m_flags;
         m_hpbEventImage = track.m_hpbEventImage;
         m_hpbImage = track.m_hpbImage;
         m_iCurrentEvent = track.m_iCurrentEvent;
         m_idxTrack = track.m_idxTrack;
         m_iIndex = track.m_iIndex;
         m_iWorkCurrentEvent = track.m_iWorkCurrentEvent;
         m_memstorageHelper.copy_from(&track.m_memstorageHelper);
         m_ptracks = track.m_ptracks;
         m_smti = track.m_smti;
         m_tkDelta = track.m_tkDelta;
         m_tkPosition = track.m_tkPosition;
         m_trackWorkStorage = track.m_trackWorkStorage;
         primitive::memory_container::operator =(track);
         keep_pointer((void **) &m_hpbImage);
         keep_pointer((void **) &m_hpbEventImage);
         return *this;
      }

      void track::allocate(uint32_t dwNewLength)
      {
         ASSERT(m_bAutoAllocation);
         if(m_hpbImage == NULL)
         {
            m_hpbImage = (byte *) sizeof(CHUNKHDR);
         }
         primitive::memory_container::allocate(dwNewLength);
         m_smti.m_cbLength = (uint32_t) (primitive::memory_container::get_size() - sizeof(CHUNKHDR));
      }

      e_file_result track::WorkWriteXFInfoHeader(const char * pSongName, ::music::xf::info_header * pXfih, ::music::xf::info_header_ls * pXfihls)
      {

         //   e_file_result               smfrc;
         imedia::position               tkLastDelta = 0 ;
         primitive::memory   memstorage;

         if(m_tkPosition != 0)
         {
            return   EInvalidTkPosition;
         }

         m_event.SetDelta(0);
         if(pSongName != NULL && *pSongName != '\0')
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaSongName);
            memstorage.allocate(::ca2::international::Utf8ToMultiByteCount(::ca2::international::CodePageLatin1, pSongName));
            m_event.SetParam((byte *) memstorage.get_data(), (int32_t) (memstorage.get_size() - 1));
            ::ca2::international::utf8_to_multibyte(::ca2::international::CodePageLatin1, (char *) m_event.GetParam(), m_event.GetParamSize(), pSongName);
            WorkWriteEvent();
         }
         if(pXfih != NULL)
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaXFInfoHdr);
            string str;
            pXfih->ToData(str);
            ::ca2::international::utf8_to_multibyte(::ca2::international::CodePageLatin1, memstorage, str);
            m_event.SetParam((byte *) memstorage.get_data(), memstorage.get_size() - 1);
            WorkWriteEvent();
         }
         if(pXfihls != NULL)
         {
            m_event.SetFullType(Meta);
            m_event.SetMetaType(::music::xf::MetaXFInfoHdr);
            string str;
            pXfihls->ToData(str);
            memstorage.allocate(::ca2::international::Utf8ToMultiByteCount(::ca2::international::CodePageLatin1, str));
            m_event.SetParam((byte *) memstorage.get_data(), memstorage.get_size() - 1);
            ::ca2::international::utf8_to_multibyte(::ca2::international::CodePageLatin1, (char *) m_event.GetParam(), m_event.GetParamSize(), str);
            WorkWriteEvent();
         }
         return Success;
      }

      // event position is set to the current track position
      // track position is updated accordingly to the event delta
      // so the delta must have been filled correctly.
      e_file_result track::WorkWriteEvent()
      {
         m_tkPosition += m_event.GetDelta();
         m_event.SetPosition(get_position());
         m_trackWorkStorage.add(m_event);
         return Success;
      }

      // event position is set to the current track position
      // track position is updated accordingly to the event delta
      // so the delta must have been filled correctly.
      e_file_result track::WorkWriteEvent(event_v5 & event)
      {
         m_tkPosition += event.GetDelta();
         m_trackWorkStorage.add(event);
         return Success;
      }

      /*
      e_file_result track::WorkWriteXFLyricEvent(string &str, imedia::position tkPosition)
      {
      e_file_result smfrc;
      if(tkPosition < m_tkPosition)
      m_event.SetDelta(0);
      else
      m_event.SetDelta(tkPosition - m_tkPosition);
      m_event.SetFullType(Meta);
      m_event.SetMetaType(::music::xf::MetaLyric);

      ::ca2::international::UnicodeToMultiByte(::ca2::international::CodePageLatin1, m_memstorageHelper, str);
      m_event.SetParam((byte *) m_memstorageHelper.get_data(),
      m_memstorageHelper.get_size() - 1);
      smfrc = WorkWriteEvent();
      return smfrc;
      }
      */

      e_file_result track::WorkWriteXFLyricEvent(string &str, imedia::position tkPosition)
      {
         e_file_result smfrc;
         if(tkPosition < m_tkPosition)
            m_event.SetDelta(0);
         else
            m_event.SetDelta(tkPosition - m_tkPosition);
         m_event.SetFullType(Meta);
         m_event.SetMetaType(::music::xf::MetaLyric);
         m_memstorageHelper.allocate(str.get_length());
         m_event.SetParam((byte *) m_memstorageHelper.get_data(),
            m_memstorageHelper.get_size());
         memcpy(
            (char *) m_event.GetParam() ,
            (const char *) str,
            m_event.GetParamSize());
         smfrc = WorkWriteEvent();
         return smfrc;
      }

      void track::WorkClear()
      {
         m_trackWorkStorage.clear();
      }

      e_file_result track::WorkSeekXFLyricsHeader()
      {
         e_file_result               smfrc;
         imedia::position               tkLastDelta = 0 ;
         event_v5 * pevent;

         if (_GetFlags().is_signalized(EndOfFile))
         {
            return SEndOfFile;
         }

         if(_WorkIsEOT())
         {
            return SEndOfFile;
         }

         while(TRUE)
         {
            _WorkGetEvent(pevent, 0, TRUE);
            if(m_tkPosition > 0)
            {
               smfrc = SNotFound;
               break;
            }
            if ((Meta == pevent->GetFullType()) &&
               (::music::xf::MetaLyricsHeader == pevent->GetMetaType()))
            {
               string str;
               ::ca2::international::multibyte_to_utf8(
                  ::ca2::international::CodePageLatin1,
                  str, (const char *) pevent->GetParam(), 6);
               if(str.CollateNoCase("$Lyrc:") == 0)
               {
                  return Success;
               }
            }

            smfrc = _WorkMoveNext();
            if (Success != smfrc)
               break;
         }
         return smfrc;

      }


      e_file_result track::WorkReadXFLyricsHeader(::music::xf::lyrics_id *pxflh)
      {
         event_v5 * pevent;
         WorkGetEvent(pevent, 0, TRUE);
         if (Meta != pevent->GetFullType())
            return EFail;
         if(::music::xf::MetaLyricsHeader != pevent->GetMetaType())
            return EFail;
         string_tokenizer tokenizer;
         string strToken;
         ::ca2::international::multibyte_to_utf8(::ca2::international::CodePageLatin1, tokenizer, (const char *) pevent->GetParam(), pevent->GetParamSize());
         if(!tokenizer.GetNextToken(strToken, ":"))
            return EFail;
         if(strToken.CollateNoCase("$Lyrc") != 0)
            return EFail;

         string strMidiChannel;
         if(!tokenizer.GetNextToken(strMidiChannel, ":"))
            return EFail;

         string strOffsetValue;
         if(!tokenizer.GetNextToken(strOffsetValue, ":"))
            return EFail;

         string strLanguage;
         tokenizer.GetNextToken(strLanguage, ":");

         pxflh->m_strLanguage = strLanguage;


         tokenizer.Restart(strMidiChannel);
         string strChannel;
         while(tokenizer.GetNextToken(strChannel, ","))
         {
            pxflh->m_MelodyTracks.add(strtoul(strChannel, NULL, 10));
         }

         pxflh->m_tkDisplayOffset.from(strOffsetValue);


         return Success;


      }

      file_flags & track::GetFlags()
      {
         return m_flags;
      }

      track_base::e_type track::get_type()
      {
         return TypeMidi;
      }

      bool track::WorkIsEOT()
      {
         return m_bWorkTrackEnd;

      }

      e_file_result track::WorkSetEOT()
      {
         m_bWorkTrackEnd = true;
         _GetFlags().signalize(EndOfFile);
         m_smti.m_tkLength = m_tkPosition;
         return SEndOfTrack;
      }

      e_file_result track::_SeekBegin(bool bWork)
      {
         if(bWork)
         {
            return WorkSeekBegin();
         }
         else
         {
            return seek_begin();
         }
      }

      void track::_GetEvent(
         bool bWork,
         event_v5 *&pevent,
         imedia::position tkMax,
         bool bTkMaxInclusive
         )
      {
         if(bWork)
         {
            _WorkGetEvent(
               pevent,
               tkMax,
               bTkMaxInclusive);
         }
         else
         {
            if(m_estate == StateOnDelta
               || m_estate == StateOnEvent)
            {
               ReadEvent();
            }
            m_event8 = m_event;
            pevent = &m_event8;
         }
      }

      e_file_result track::_MoveNext(bool bWork)
      {
         if(bWork)
         {
            return WorkMoveNext();
         }
         else
         {
            e_file_result smfr = MoveNext();
            if(smfr != Success)
               return smfr;
            return ReadEvent();
         }

      }

      track::State track::GetState()
      {
         return m_estate;
      }


      event_v1 & track::GetEvent()
      {
         return m_event;
      }


      void track::_WorkGetEvent(
         event_v5 *&    pevent,
         imedia::position                  tkMax,
         bool                  bTkMaxInclusive)
      {
         UNREFERENCED_PARAMETER(tkMax);
         UNREFERENCED_PARAMETER(bTkMaxInclusive);
         ASSERT(m_estate == StateOnEventRead);
         ASSERT(!_WorkIsEOT());
         pevent = &m_trackWorkStorage.EventAt(m_iWorkCurrentEvent);
      }

      imedia::position track::_WorkGetPosition()
      {
         ASSERT(m_estate == StateOnEventRead);
         ASSERT(!_WorkIsEOT());
         return m_tkPosition;
      }


      e_file_result track::_WorkMoveNext()
      {
         m_estate = StateOnEventRead;
         m_iWorkCurrentEvent++;
         if(m_iWorkCurrentEvent >= m_trackWorkStorage.GetEventCount())
         {
            return WorkSetEOT();
         }
         m_tkPosition = m_tkPosition + m_trackWorkStorage.EventAt(m_iWorkCurrentEvent)._GetDelta();
         return Success;
      }

      bool track::_WorkIsEOT()
      {
         return m_bWorkTrackEnd;
      }

      file_flags & track::_GetFlags()
      {
         return m_flags;
      }

      imedia::position track::WorkGetPosition()
      {
         return m_tkPosition;
      }


   } // namespace midi_mmsystem


} // namespace music









