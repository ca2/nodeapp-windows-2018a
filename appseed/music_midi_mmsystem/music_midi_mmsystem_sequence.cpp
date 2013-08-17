#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      //PRIVATE void FAR seqMIDICallback(HMIDISTRM hms, uint32_t uMsg, uint32_t dwUser, uint32_t dw1, uint32_t dw2);
      //::multimedia::result TranslateSMFResult(e_file_result smfrc);

      sequence::sequence(sp(::ca2::application) papp) :
         ca2(papp),
         ::ikaraoke::karaoke(papp),
         m_evMmsgDone(papp, FALSE, TRUE),
         m_memfile(papp),
         m_evBuffersZero(papp),
         m_eventMidiPlaybackEnd(papp),
         m_mutex(papp)
      {

         m_pthread = NULL;
         m_iaLevel.set_size(16);
         SetLevelMeter(0);

         //   m_cBuffer = 0;
         //   m_cbBuffer = 0;
         //m_pstrFile =  NULL;
         m_uiDeviceID = 0;
         //   m_uMCIDeviceID = 0;
         //   m_uMCITimeFormat = 0;
         //   m_uMCITimeDiv = 0;
         //   m_oswindow_ = NULL;

         m_uiPreviousState = StatusNoFile;
         m_uiState = StatusNoFile;
         m_tkLength = 0;
         m_mmrcLastErr = 0;

         m_hMidiStream = NULL;
         m_dwTimeDivision = 0;

         m_lpbAlloc = 0;
         m_tkBase = 0;
         m_tkEnd = 0;

         m_cbPreroll = 8 * 1024;
         m_cbPrerollNominalMax = 8 * 256;

         m_pfile = new file(get_app());

         m_buffera.Initialize(16, m_cbPreroll, (uint32_t) (void *) &m_midicallbackdata);


      };

      sequence::~sequence()
      {

         CloseFile();
         if(m_pfile != NULL)
         {
            delete m_pfile;
            m_pfile = NULL;
         }
      }


      /***************************************************************************
      *
      * seqAllocBuffers
      *
      * allocate buffers for this instance.
      *
      * pSeq                      - The sequencer instance to allocate buffers for.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation was successful.
      *
      *   MCIERR_OUT_OF_MEMORY  If there is insufficient primitive::memory for
      *     the requested number and size of buffers.
      *
      * seqAllocBuffers allocates playback buffers based on the
      * cbBuffer and cBuffer fields of pSeq. cbBuffer specifies the
      * number of bytes in each buffer, and cBuffer specifies the
      * number of buffers to allocate.
      *
      * seqAllocBuffers must be called before any other sequencer call
      * on a newly allocted SEQUENCE structure. It must be paired with
      * a call to seqFreeBuffers, which should be the last call made
      * before the SEQUENCE structure is discarded.
      *
      ***************************************************************************/
      ::multimedia::result sequence::AllocBuffers()
      {
         ASSERT(FALSE);
         /*
         uint32_t                   dwEachBufferSize;
         uint32_t                   dwAlloc;
         uint32_t                    i;
         LPBYTE                  lpbWork;

         //    assert(pSeq != NULL);

         SetState(StatusNoFile);
         m_lpmhFree  = NULL;
         m_lpbAlloc  = NULL;
         //    m_hSmf      = (HSMF)NULL;

         // First make sure we can allocate the buffers they asked for
         //
         dwEachBufferSize = sizeof(MIDIHDR) + (uint32_t)(m_cbBuffer);
         dwAlloc          = dwEachBufferSize * (uint32_t)(m_cBuffer);

         //m_lpbAlloc = (uchar *) GlobalAllocPtr(GMEM_MOVEABLE|GMEM_SHARE, dwAlloc);
         m_lpbAlloc = (uchar *) HeapAlloc(GetProcessHeap(), 0, dwAlloc);
         if (NULL == m_lpbAlloc)
         return MCIERR_OUT_OF_MEMORY;

         // Initialize all MIDIHDR's and throw them into a free list
         //
         m_lpmhFree = NULL;

         lpbWork = m_lpbAlloc;
         for (i=0; i < m_cBuffer; i++)
         {
         ((LPMIDIHDR)lpbWork)->lpNext            = m_lpmhFree;

         ((LPMIDIHDR)lpbWork)->lpData            = (char *) (lpbWork + sizeof(MIDIHDR));
         ((LPMIDIHDR)lpbWork)->dwBufferLength    = m_cbBuffer;
         ((LPMIDIHDR)lpbWork)->dwBytesRecorded   = 0;
         ((LPMIDIHDR)lpbWork)->dwUser            = (uint32_t)(uint32_t)this;
         ((LPMIDIHDR)lpbWork)->dwFlags           = 0;

         m_lpmhFree = (LPMIDIHDR)lpbWork;

         lpbWork += dwEachBufferSize;
         }

         return MMSYSERR_NOERROR;*/
         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqFreeBuffers
      *
      * Free buffers for this instance.
      *
      * pSeq                      - The sequencer instance to free buffers for.
      *
      * seqFreeBuffers frees all allocated primitive::memory belonging to the
      * given sequencer instance pSeq. It must be the last call
      * performed on the instance before it is destroyed.
      *
      ****************************************************************************/
      VOID sequence::FreeBuffers()
      {
         ASSERT(FALSE);
         /*
         LPMIDIHDR               lpmh;

         //    assert(pSeq != NULL);

         if (NULL != m_lpbAlloc)
         {
         lpmh = (LPMIDIHDR)m_lpbAlloc;
         ASSERT(!(lpmh->dwFlags & MHDR_PREPARED));

         //GlobalFreePtr(m_lpbAlloc);
         HeapFree(GetProcessHeap(), 0, m_lpbAlloc);
         }*/
      }

      /***************************************************************************
      *
      * seqOpenFile
      *
      * Associates a MIDI file with the given sequencer instance.
      *
      * pSeq                      - The sequencer instance.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If there is already a file open
      *     on this instance.
      *
      *   MCIERR_OUT_OF_MEMORY If there was insufficient primitive::memory to
      *     allocate internal buffers on the file.
      *
      *   MCIERR_INVALID_FILE If initial attempts to parse the file
      *     failed (such as the file is not a MIDI or RMI file).
      *
      * seqOpenFile may only be called if there is no currently open file
      * on the instance. It must be paired with a call to seqCloseFile
      * when operations on this file are complete.
      *
      * The pstrFile field of pSeq contains the name of the file
      * to open. This name will be passed directly to mmioOpen; it may
      * contain a specifcation for a custom MMIO file handler. The task
      * context used for all I/O will be the task which calls seqOpenFile.
      *
      ***************************************************************************/

      ::multimedia::result sequence::OpenFile(
         sequence &sequence,
         int32_t openMode)
      {
         ::multimedia::result                rc      = MMSYSERR_NOERROR;
         SMFFILEINFO             sfi;
         e_file_result               smfrc;
         uint32_t                   cbBuffer;

         if (GetState() != StatusNoFile)
         {
            return MCIERR_UNSUPPORTED_FUNCTION;
         }

         m_iOpenMode = openMode;

         smfrc = m_pfile->OpenFile(sequence.GetFile(), openMode);

         if (Success != smfrc)
         {
            rc = TranslateSMFResult(smfrc);
            goto Seq_Open_File_Cleanup;
         }

         m_pfile->GetFileInfo(&sfi);

         m_dwTimeDivision = sfi.dwTimeDivision;
         m_tkLength       = sfi.tkLength;
         if(m_iOpenMode == OpenForPlaying)
         {
            m_msLength      = TicksToMillisecs(m_tkLength);
         }
         /* Track buffers must be big enough to hold the state data returned
         ** by smfSeek()
         */
         cbBuffer = min(m_cbBuffer, GetStateMaxSize());


Seq_Open_File_Cleanup:
         if (MMSYSERR_NOERROR != rc)
            CloseFile();
         else
            SetState(StatusOpened);

         return rc;
      }

      ::multimedia::result sequence::OpenFile(
         const char * lpFileName,
         int32_t openMode)
      {
         ::ca2::filesp file(
            get_app());
         file->open(lpFileName,
            ::ca2::file::mode_read |
            ::ca2::file::shareDenyWrite |
            ::ca2::file::type_binary);
         return OpenFile(file, openMode);
      }


      ::multimedia::result sequence::OpenFile(
         primitive::memory * pmemorystorage,
         int32_t openMode,
         e_storage estorage)
      {
         ::multimedia::result                rc = MMSYSERR_NOERROR;
         SMFFILEINFO             sfi;
         e_file_result    smfrc;
         uint32_t                   cbBuffer;

         if (GetState() != StatusNoFile)
         {
            CloseFile();
            //return MCIERR_UNSUPPORTED_FUNCTION;
         }

         m_iOpenMode = openMode;

         smfrc = m_pfile->OpenFile(pmemorystorage, openMode, estorage);

         if (Success != smfrc)
         {
            rc = TranslateSMFResult(smfrc);
         }
         else
         {
            m_pfile->GetFileInfo(&sfi);

            m_dwTimeDivision = sfi.dwTimeDivision;
            m_tkLength       = sfi.tkLength;
            if(m_iOpenMode == OpenForPlaying)
            {
               m_msLength      = TicksToMillisecs(m_tkLength);
            }
            /* Track buffers must be big enough to hold the state data returned
            ** by smfSeek()
            */
            cbBuffer = min(m_cbBuffer, GetStateMaxSize());
         }

         if(MMSYSERR_NOERROR != rc)
            CloseFile();
         else
            SetState(StatusOpened);

         return rc;
      }

      ::multimedia::result sequence::OpenFile(
         ::ca2::file & ar,
         int32_t openMode)
      {
         ::multimedia::result                rc      = MMSYSERR_NOERROR;
         //    SMFOPENFILESTRUCT       sofs;
         SMFFILEINFO             sfi;
         e_file_result               smfrc;
         uint32_t                   cbBuffer;
         //    assert(pSeq != NULL);

         if (GetState() != StatusNoFile)
         {
            return MCIERR_UNSUPPORTED_FUNCTION;
         }



         //   m_pstrFile = _tcsdup(lpFileName);
         //   m_strFile = lpFileName;

         //    ASSERT(m_pstrFile != NULL);

         m_iOpenMode = openMode;

         //    sofs.pstrName     = m_pstrFile;

         //PSMF pSmf = new SMF();

         //smfrc = m_pfile->OpenFile(&sofs);
         smfrc = m_pfile->OpenFile(ar, openMode);

         //smfrc = smfOpenFile(&sofs);
         if (Success != smfrc)
         {
            //      delete pSmf;
            rc = TranslateSMFResult(smfrc);
         }
         else
         {
            //    m_hSmf = sofs.hSmf;
            //    ((PSMF) m_hSmf)->GetFileInfo(&sfi);
            m_pfile->GetFileInfo(&sfi);

            m_dwTimeDivision = sfi.dwTimeDivision;
            m_tkLength       = sfi.tkLength;
            if(m_iOpenMode == OpenForPlaying)
            {
               m_msLength      = TicksToMillisecs(m_tkLength);
            }
            /* Track buffers must be big enough to hold the state data returned
            ** by smfSeek()
            */
            cbBuffer = min(m_cbBuffer, GetStateMaxSize());
         }

         if (MMSYSERR_NOERROR != rc)
            CloseFile();
         else
            SetState(StatusOpened);

         return rc;
      }
      /***************************************************************************
      *
      * seqCloseFile
      *
      * Deassociates a MIDI file with the given sequencer instance.
      *
      * pSeq                      -  The sequencer instance.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     stopped.
      *
      * A call to seqCloseFile must be paired with a prior call to
      * seqOpenFile. All buffers associated with the file will be
      * freed and the file will be closed. The sequencer must be
      * stopped before this call will be accepted.
      *
      ***************************************************************************/
      ::multimedia::result sequence::CloseFile()
      {
         single_lock sl(&m_mutex, true);

         //if (StatusNoFile == GetState())
         //   return MCIERR_UNSUPPORTED_FUNCTION;

         m_pfile->CloseFile();

         /* If we were prerolled, need to clean up -- have an open MIDI handle
         ** and buffers in the ready queue
         */

         //    single_lock slStream(&m_csStream, false);

         //    for (lpmh = m_lpmhFree; lpmh; lpmh = lpmh->lpNext)
         //    for (lpmh = m_buffera[0]; lpmh != NULL; lpmh = lpmh->lpNext)


         /*   m_lpmhFree = NULL;

         if (m_lpmhPreroll != NULL)
         {
         slStream.lock();
         if(m_hMidiStream != NULL)
         {
         midiOutUnprepareHeader((HMIDIOUT) m_hMidiStream, m_lpmhPreroll, sizeof(*m_lpmhPreroll));
         }
         slStream.unlock();
         }
         m_lpmhPreroll = NULL;*/
         //    slStream.lock();
         if (m_hMidiStream != NULL)
         {
            m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hMidiStream);
            midiStreamClose( m_hMidiStream);
            m_hMidiStream = NULL;
         }
         //  slStream.unlock();

         SetState(StatusNoFile);

         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqPreroll
      *
      * Prepares the file for playback at the given position.
      *
      * pSeq                      - The sequencer instance.
      *
      * lpPreroll                 - Specifies the starting and ending tick
      *                             positions to play between.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     opened or prerolled.
      *
      * open the device so we can initialize channels.
      *
      * Loop through the tracks. For each track, seek to the given position and
      * send the init data SMF gives us to the handle.
      *
      * Wait for all init buffers to finish.
      *
      * Unprepare the buffers (they're only ever sent here; the sequencer
      * engine merges them into a single stream during normal playback) and
      * refill them with the first chunk of data from the track.
      *
      *
      ****************************************************************************/
      ::multimedia::result sequence::Preroll(
         ::ca2::thread *         pthread,
         LPPREROLL            lpPreroll,
         bool                  bThrow)
      {
         UNREFERENCED_PARAMETER(pthread);
         single_lock sl(&m_mutex, TRUE);
         int32_t i;
         //   midi_callback_data *      lpData = &m_midicallbackdata;
         e_file_result     smfrc;
         ::multimedia::result                mmrc        = MMSYSERR_NOERROR;
         MIDIPROPTIMEDIV         mptd;
         LPMIDIHDR               lpmh = NULL;
         //   LPMIDIHDR               lpmhPreroll = NULL;
         uint32_t                    uDeviceID;


         ASSERT(m_iOpenMode == OpenForPlaying
            || IsInSpecialModeV001());

         m_flags.unsignalize(FlagEOF);

         m_mmrcLastErr = MMSYSERR_NOERROR;

         if(GetState() != StatusOpened &&
            GetState() != StatusPreRolled &&
            !IsInSpecialModeV001())
            return MCIERR_UNSUPPORTED_FUNCTION;

         m_tkBase = lpPreroll->tkBase;
         m_tkEnd = lpPreroll->tkEnd;
         if(m_hMidiStream != NULL)
         {
            // Recollect buffers from MMSYSTEM back into free queue
            //
            SetState(StatusReset);
            midiOutReset((HMIDIOUT) m_hMidiStream);
            while (m_uBuffersInMMSYSTEM)
               Sleep(0);
         }
         else
         {
         }

         m_uBuffersInMMSYSTEM = 0;
         SetState(StatusPreRolling);

         //
         // We've successfully opened the file and all of the tracks; now
         // open the MIDI device and set the time division.
         //
         // NOTE: seqPreroll is equivalent to seek; device might already be open
         //

         if (m_hMidiStream == NULL)
         {
            uDeviceID = m_uiDeviceID;
            mmrc = midiStreamOpen(&m_hMidiStream,
               &uDeviceID,
               1,
               (uint32_t) MidiOutProc,
               0,
               CALLBACK_FUNCTION);
            if(mmrc != MMSYSERR_NOERROR)
            {
               m_hMidiStream = NULL;
               if(bThrow)
               {
                  SetState(StatusOpened);
                  throw new exception(get_app(), mmrc, MIDIPLAYERPRERROLLSTREAMOPENEXCEPTION);
               }
               TRACE("midiStreamOpenError %d\n", mmrc);
               //goto seq_Preroll_Cleanup;
               if(mmrc == MMSYSERR_BADDEVICEID)
                  goto seq_Preroll_Cleanup;
               else if(mmrc == MMSYSERR_INVALPARAM)
                  goto seq_Preroll_Cleanup;
               else if(mmrc == MMSYSERR_NOMEM)
                  goto seq_Preroll_Cleanup;
               else
                  goto seq_Preroll_Cleanup;
            }

            mptd.cbStruct  = sizeof(mptd);
            mptd.dwTimeDiv = m_dwTimeDivision;
            mmrc = midiStreamProperty(
               m_hMidiStream,
               (LPBYTE)&mptd,
               MIDIPROP_SET
               | MIDIPROP_TIMEDIV);
            if (mmrc != MMSYSERR_NOERROR)
            {
               TRACE( "midiStreamProperty() -> %04X", (WORD)mmrc);
               midiStreamClose(m_hMidiStream);
               m_hMidiStream = NULL;
               mmrc = MCIERR_DEVICE_NOT_READY;
               if(bThrow)
               {
                  SetState(StatusOpened);
                  throw new exception(get_app(), mmrc, MIDIPLAYERPRERROLLSTREAMPROPERTYEXCEPTION);
               }
               goto seq_Preroll_Cleanup;
            }
         }

         mmrc = MMSYSERR_NOERROR;



         m_buffera.Reset();
         lpmh = m_buffera[0].GetMidiHdr();

         if(IsInSpecialModeV001())
         {
            if(m_uiSpecialModeV001Operation == SPMV001GMReset)
            {
               const uchar gmModeOn[] = {
                  //        0x00, 0x00, 0x00, 0x00,
                  //        0x00, 0x00, 0x00, 0x00,
                  //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                  0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00,
                  0x06, 0x00, 0x00, MEVT_LONGMSG,
                  0xf0, 0x7e, 0x7f, 0x09,
                  0x01, 0xf7, 0x00, 0x00};
               char * lpch = lpmh->lpData + lpmh->dwBytesRecorded;
               lpmh->dwBytesRecorded += sizeof(gmModeOn);
               memcpy(lpch, gmModeOn, sizeof(gmModeOn));
               SetSpecialModeV001Flag(false);
            }
         }

         smfrc = m_pfile->WorkSeek(m_tkBase, lpmh);

         m_tkPrerollBase = GetFile().get_position();




         m_flags.unsignalize(FlagEOF);
         m_pfile->GetFlags().unsignalize(EndOfFile);
         for(i = 1; i < m_buffera.get_size(); i++)
         {
            lpmh = m_buffera[i].GetMidiHdr();

            smfrc = m_pfile->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);
            if(Success != smfrc && SEndOfFile != smfrc)
            {
               TRACE( "SFP: smfReadEvents() -> %u", (uint32_t)smfrc);
               mmrc = TranslateSMFResult(smfrc);
               /*if(bThrow)
               {
               SetState(StatusOpened);
               throw new exception(mmrc, MIDIPLAYERPRERROLLREADEVENTSEXCEPTION);
               }*/
               goto seq_Preroll_Cleanup;
            }



            if (SEndOfFile == smfrc)
            {
               m_flags.signalize(FlagEOF);
               break;
            }
         }

         mmrc = m_buffera.midiOutPrepareHeader((HMIDIOUT) m_hMidiStream);
         if (mmrc != MMSYSERR_NOERROR)
         {
            TRACE( "midiOutPrepare(preroll) -> %lu!", (uint32_t)mmrc);
            mmrc = MCIERR_DEVICE_NOT_READY;
            if(bThrow)
            {
               SetState(StatusOpened);
               throw new exception(get_app(), mmrc, MIDIPLAYERPRERROLLPREPAREHEADEREXCEPTION);
            }
            else
            {
               goto seq_Preroll_Cleanup;
            }
         }


         mmrc = m_buffera.midiStreamOut(m_hMidiStream);
         if (mmrc != MMSYSERR_NOERROR)
         {
            TRACE( "midiOutPrepare(preroll) -> %lu!", (uint32_t)mmrc);
            mmrc = MCIERR_DEVICE_NOT_READY;
            if(bThrow)
            {
               SetState(StatusOpened);
               throw new exception(get_app(), mmrc, MIDIPLAYERPRERROLLPREPAREHEADEREXCEPTION);
            }
            else
            {
               goto seq_Preroll_Cleanup;
            }
         }
         m_uBuffersInMMSYSTEM +=(uint32_t)  m_buffera.get_size();

seq_Preroll_Cleanup:

         if (mmrc != MMSYSERR_NOERROR)
         {
            SetState(StatusOpened);
            m_flags.unsignalize(sequence::FlagWaiting);
         }
         else
         {
            SetState(StatusPreRolled);
         }

         return mmrc;
      }

      /***************************************************************************
      *
      * seqStart
      *
      * Starts playback at the current position.
      *
      * pSeq                      - The sequencer instance.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     stopped.
      *
      *   MCIERR_DEVICE_NOT_READY If the underlying MIDI device could
      *     not be opened or fails any call.
      *
      * The sequencer must be prerolled before seqStart may be called.
      *
      * Just feed everything in the ready queue to the device.
      *
      ***************************************************************************/
      ::multimedia::result sequence::Start()
      {
         single_lock sl(&m_mutex, TRUE);
         if (StatusPreRolled != GetState())
         {
            TRACE( "seqStart(): State is wrong! [%u]", GetState());
            return MCIERR_UNSUPPORTED_FUNCTION;
         }

         SetState(StatusPlaying);

         m_evMmsgDone.ResetEvent();

         ::multimedia::result mmrc = 0;
         if(m_hMidiStream != NULL)
         {
            mmrc = midiStreamRestart(m_hMidiStream);
         }
         sl.unlock();
         if(mmrc == MMSYSERR_NOERROR)
         {
            m_pthread->PostMidiSequenceEvent(
               this,
               sequence::EventMidiPlaybackStart,
               NULL);

         }
         return mmrc;
      }

      /***************************************************************************
      *
      * seqPause
      *
      * Pauses playback of the instance.
      *
      * pSeq                      - The sequencer instance.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     playing.
      *
      * The sequencer must be playing before seqPause may be called.
      * Pausing the sequencer will cause all currently on notes to be turned
      * off. This may cause playback to be slightly inaccurate on restart
      * due to missing notes.
      *
      ***************************************************************************/
      ::multimedia::result sequence::Pause()

      {
         single_lock sl(&m_mutex, TRUE);

         //    assert(NULL != pSeq);

         if (StatusPlaying != GetState())
            return MCIERR_UNSUPPORTED_FUNCTION;

         SetState(StatusPaused);

         ::multimedia::result mmrc = 0;
         //    single_lock slStream(&m_csStream, false);
         //  slStream.lock();
         if(m_hMidiStream != NULL)
         {
            mmrc = midiStreamPause(m_hMidiStream);;
         }
         //slStream.unlock();

         SetLevelMeter(0);

         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqRestart
      *
      * Restarts playback of an instance after a pause.
      *
      * pSeq                      - The sequencer instance.
      *
      * Returns
      *    MMSYSERR_NOERROR If the operation is successful.
      *
      *    MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     paused.
      *
      * The sequencer must be paused before seqRestart may be called.
      *
      ***************************************************************************/
      ::multimedia::result sequence::Restart()
      {
         //    assert(NULL != pSeq);

         single_lock sl(&m_mutex, TRUE);

         if (StatusPaused != GetState())
            return MCIERR_UNSUPPORTED_FUNCTION;

         SetState(StatusPlaying);
         m_evMmsgDone.ResetEvent();

         //    ::multimedia::result mmrc = 0;
         //    single_lock slStream(&m_csStream, false);
         //  slStream.lock();
         if(m_hMidiStream != NULL)
         {
            midiStreamRestart(m_hMidiStream);
         }
         //slStream.unlock();
         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqStop
      *
      * Totally stops playback of an instance.
      *
      * pSeq                      - The sequencer instance.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     paused or playing.
      *
      * The sequencer must be paused or playing before seqStop may be called.
      *
      ***************************************************************************/
      ::multimedia::result sequence::Stop()
      {
         single_lock sl(&m_mutex, TRUE);

         // Automatic success if we're already stopped
         if (GetState() != StatusPlaying
            && GetState() != StatusPaused)
         {
            m_flags.unsignalize(sequence::FlagWaiting);
            GetPlayerLink().OnFinishCommand(player::command_stop);
            return MMSYSERR_NOERROR;
         }

         SetState(StatusStopping);
         m_flags.signalize(sequence::FlagWaiting);

         m_eventMidiPlaybackEnd.ResetEvent();

         if(m_hMidiStream != NULL)
         {
            m_mmrcLastErr = midiStreamStop(m_hMidiStream);
            if(MMSYSERR_NOERROR != m_mmrcLastErr)
            {
               TRACE( "sequence::Stop() -> midiOutStop() returned %lu in seqStop()!\n", (uint32_t)m_mmrcLastErr);
               m_flags.unsignalize(FlagWaiting);
               return MCIERR_DEVICE_NOT_READY;
            }
         }

         //m_eventMidiPlaybackEnd.lock();

         SetLevelMeter(0);

         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqTime
      *
      * Determine the current position in playback of an instance.
      *
      * pSeq                      - The sequencer instance.
      *
      * pTicks                    - A pointer to a uint32_t where the current position
      *                             in ticks will be returned.
      *
      * Returns
      *   MMSYSERR_NOERROR If the operation is successful.
      *
      *   MCIERR_DEVICE_NOT_READY If the underlying device fails to report
      *     the position.
      *
      *   MCIERR_UNSUPPORTED_FUNCTION If the sequencer instance is not
      *     paused or playing.
      *
      * The sequencer must be paused, playing or prerolled before seqTime
      * may be called.
      *
      ***************************************************************************/
      ::multimedia::result sequence::get_ticks(imedia::position &  pTicks)
      {
         single_lock sl(&m_mutex);
         if(!sl.lock(millis(184)))
            return MCIERR_INTERNAL;

         ::multimedia::result                mmr;
         MMTIME                  mmt;

         if (StatusPlaying != GetState() &&
            StatusPaused != GetState() &&
            StatusPreRolling != GetState() &&
            StatusPreRolled != GetState() &&
            StatusOpened != GetState() &&
            StatusStopping != GetState())
         {
            TRACE( "seqTime(): State wrong! [is %u]", GetState());
            return MCIERR_UNSUPPORTED_FUNCTION;
         }

         pTicks = 0;
         if (StatusOpened != GetState())
         {
            pTicks = m_tkBase;
            if (StatusPreRolled != GetState())
            {
               mmt.wType = TIME_TICKS;
               //            single_lock slStream(&m_csStream, false);
               //          slStream.lock();
               if(m_hMidiStream == NULL)
               {
                  TRACE("m_hmidi == NULL!!!!");
                  return MCIERR_DEVICE_NOT_READY;
               }
               else
               {
                  try
                  {

                     mmr = midiStreamPosition(m_hMidiStream, &mmt, sizeof(mmt));
                     if (MMSYSERR_NOERROR != mmr)
                     {
                        TRACE( "midiStreamPosition() returned %lu", (uint32_t)mmr);
                        return MCIERR_DEVICE_NOT_READY;
                     }
                  }
                  catch(...)
                  {
                     return MCIERR_DEVICE_NOT_READY;
                  }
                  pTicks += mmt.u.ticks;
               }
               //        slStream.unlock();
            }
         }

         return MMSYSERR_NOERROR;
      }

      void sequence::get_time(imedia::time & time)
      {
         get_millis(time);
      }

      ::multimedia::result sequence::get_millis(imedia::time & time)
      {
         single_lock sl(&m_mutex);
         if(!sl.lock(millis(184)))
            return MCIERR_INTERNAL;

         ::multimedia::result                mmr;
         MMTIME                  mmt;

         if (StatusPlaying != GetState() &&
            StatusPaused != GetState() &&
            StatusPreRolling != GetState() &&
            StatusPreRolled != GetState() &&
            StatusOpened != GetState() &&
            StatusStopping != GetState())
         {
            TRACE( "seqTime(): State wrong! [is %u]", GetState());
            return MCIERR_UNSUPPORTED_FUNCTION;
         }

         time = 0;
         if (StatusOpened != GetState())
         {
            time = (int_ptr) TicksToMillisecs(m_tkBase);
            if (StatusPreRolled != GetState())
            {
               mmt.wType = TIME_MS;
               //            single_lock slStream(&m_csStream, false);
               //          slStream.lock();
               if(m_hMidiStream == NULL)
               {
                  TRACE("m_hmidi == NULL!!!!");
                  return MCIERR_DEVICE_NOT_READY;
               }
               else
               {
                  try
                  {

                     mmr = midiStreamPosition(m_hMidiStream, &mmt, sizeof(mmt));
                     if (MMSYSERR_NOERROR != mmr)
                     {
                        TRACE( "midiStreamPosition() returned %lu", (uint32_t)mmr);
                        return MCIERR_DEVICE_NOT_READY;
                     }
                  }
                  catch(...)
                  {
                     return MCIERR_DEVICE_NOT_READY;
                  }
                  time += mmt.u.ms;
               }
               //        slStream.unlock();
            }
         }

         return MMSYSERR_NOERROR;
      }

      /***************************************************************************
      *
      * seqMillisecsToTicks
      *
      * Given a millisecond offset in the output stream, returns the associated
      * tick position.
      *
      * pSeq                      - The sequencer instance.
      *
      * msOffset                  - The millisecond offset into the stream.
      *
      * Returns the number of ticks into the stream.
      *
      ***************************************************************************/
      imedia::position sequence::MillisecsToTicks(imedia::time msOffset)
      {
         return m_pfile->MillisecsToTicks(msOffset);
      }

      /***************************************************************************
      *
      * seqTicksToMillisecs
      *
      * Given a tick offset in the output stream, returns the associated
      * millisecond position.
      *
      * pSeq                      - The sequencer instance.
      *
      * tkOffset                  - The tick offset into the stream.
      *
      * Returns the number of milliseconds into the stream.
      *
      ***************************************************************************/
      imedia::time sequence::TicksToMillisecs(imedia::position tkOffset)
      {
         return m_pfile->TicksToMillisecs(tkOffset);
      }

      void sequence::OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr)
      {
         UNREFERENCED_PARAMETER(hmidistream);
         e_file_result               smfrc;
         midi_callback_data *      lpData;
         ASSERT(lpmidihdr != NULL);
         lpData = (midi_callback_data *) lpmidihdr->dwUser;
         ASSERT(lpData != NULL);

         sequence * psequence = lpData->m_psequence;

         sequence_thread * pthread = psequence->m_pthread;
         ASSERT(NULL != lpmidihdr);

         --m_uBuffersInMMSYSTEM;

         if(m_uBuffersInMMSYSTEM == 0)
         {
            m_evBuffersZero.SetEvent();
         }

         if (StatusReset == GetState())
         {
            // We're recollecting buffers from MMSYSTEM
            //
            //      if (lpmidihdr != m_lpmhPreroll)
            //    {
            //     lpmidihdr->lpNext    = m_lpmhFree;
            //   m_lpmhFree           = lpmidihdr;
            //      }
            return;
         }

         bool bStopping = StatusStopping == GetState();
         bool bEof = m_flags.is_signalized(FlagEOF);
         bool bSpecialModeV001End = m_flags.is_signalized(FlagSpecialModeV001End);

         if (bStopping || bEof || bSpecialModeV001End)
         {
            //
            // Reached EOF, just put the buffer back on the free
            // list
            //
            if(bSpecialModeV001End)
            {
               m_flags.unsignalize(FlagSpecialModeV001End);
               TRACE("void CALLBACK sequence::MidiOutProc m_flags.is_signalized(FlagSpecialModeV001End\n");
               pthread->PostMidiSequenceEvent(
                  this,
                  sequence::EventSpecialModeV001End,
                  lpmidihdr);
            }
            else if(bStopping)
            {
               if(m_uBuffersInMMSYSTEM == 0)
               {
                  TRACE("void CALLBACK sequence::MidiOutProc StatusStopping == pSeq->GetState()\n");
                  pthread->PostMidiSequenceEvent(
                     this,
                     sequence::EventStopped,
                     lpmidihdr);
               }
            }
            else
            {
               if(m_flags.is_signalized(FlagEOF))
               {
                  TRACE("void CALLBACK sequence::MidiOutProc m_flags.is_signalized(FlagEOF\n");
               }
               //       if (lpmidihdr != m_lpmhPreroll)
               //     {
               //      lpmidihdr->lpNext = m_lpmhFree;
               //    m_lpmhFree        = lpmidihdr;
               //}
               if(m_uBuffersInMMSYSTEM <= 0)
               {
                  pthread->PostMidiSequenceEvent(
                     this,
                     sequence::EventMidiPlaybackEnd,
                     lpmidihdr);
               }
            }
         }
         else
         {
            if(IsInSpecialModeV001())
            {
               if(m_uiSpecialModeV001Operation == SPMV001GMReset)
               {
                  const uchar gmModeOn[] = {
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                     0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00,
                     0x06, 0x00, 0x00, MEVT_LONGMSG,
                     0xf0, 0x7e, 0x7f, 0x09,
                     0x01, 0xf7, 0x00, 0x00};
                  lpmidihdr->dwBytesRecorded = sizeof(gmModeOn);
                  memcpy(lpmidihdr->lpData, gmModeOn, sizeof(gmModeOn));
               }
               else if(m_uiSpecialModeV001Operation == SPMV001TempoChange)
               {
                  event_v7 event;
                  m_pfile->GetTempoEvent(event);
                  m_pfile->StreamEvent(event.GetDelta(), &event, lpmidihdr, 0x7fffffff, 256);
                  // lpmidihdr->dwBytesRecorded = sizeof(gmModeOn);
                  // memcpy(lpmidihdr->lpData, gmModeOn, sizeof(gmModeOn));
               }
               else
               {
                  ASSERT(FALSE);
               }
               //post_thread_message(lpData->dwThreadID, MIDIPLAYERMESSAGE_STREAMOUT, (WPARAM) pSeq, (LPARAM) lpmidihdr);
               pthread->PostMidiSequenceEvent(
                  this,
                  EventMidiStreamOut,
                  lpmidihdr);
               m_flags.signalize(FlagSpecialModeV001End);
               smfrc = Success;
               return;
            }
            //
            // Not EOF yet; attempt to fill another buffer
            //
            pthread->PostMidiSequenceEvent(
               this,
               EventMidiStreamOut,
               lpmidihdr);
         }

      }

      void sequence::OnPositionCB(LPMIDIHDR lpmidihdr)
      {
         ASSERT(lpmidihdr != NULL);
         LPBYTE lpbData = (LPBYTE) (lpmidihdr->lpData + lpmidihdr->dwOffset);
         MIDIEVENT * lpme = (MIDIEVENT *) lpbData;
         file::midi_stream_event_header * pheader = (file::midi_stream_event_header *) &lpme->dwParms[0];
         lpbData = (LPBYTE) pheader;
         LPDWORD lpdwParam;

         int32_t iSize = pheader->m_dwLength;
         switch(pheader->m_dwType)
         {
         case 0:
            {
               array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> * plyriceventa = NULL;
               array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lyriceventa;
               for(int32_t i = sizeof(file::midi_stream_event_header); i < iSize;)
               {
                  pheader = (file::midi_stream_event_header *) &lpbData[i];
                  lpdwParam = (LPDWORD) &lpbData[i + sizeof(file::midi_stream_event_header)];
                  ASSERT(*lpdwParam == pheader->m_dwType);
                  switch(pheader->m_dwType)
                  {
                  case EVENT_ID_LYRIC_V1:
                     {
                        if(plyriceventa == NULL)
                        {
                           plyriceventa = new array <::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &>;
                        }
                        ::primitive::memory_file memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
                        /* x2x                  CArchive ar(&memFile, CArchive::load);
                        lyriceventa.Serialize(ar);
                        plyriceventa->append(lyriceventa); */
                     }
                     break;
                  case EVENT_ID_PAD:
                     {
                     }
                     break;
                  case EVENT_ID_NOTE_ON:
                     {
                        ::ca2::byte_stream_memory_file memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
                        for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
                        {
                           BYTE b;
                           memFile >> b;
                           m_iaLevel.element_at(i) = b;
                        }
                     }
                     break;
                  }
                  i += pheader->m_dwLength + sizeof(file::midi_stream_event_header);
               }
               /*         if(plyriceventa != NULL)
               {
               ::PostMessage(m_midicallbackdata.oswindow, WM_APP + 3388, 3388, (LPARAM) plyriceventa);
               }*/
            }
            break;
         case EVENT_ID_PAD:
            break;
         default:
            ASSERT(FALSE);
            break;
         }

      }

      void CALLBACK sequence::MidiOutProc(
         HMIDIOUT hmo,
         uint32_t wMsg,
         uint32_t dwInstance,
         uint32_t dwParam1,
         uint32_t dwParam2)
      {
         UNREFERENCED_PARAMETER(hmo);
         UNREFERENCED_PARAMETER(dwInstance);
         UNREFERENCED_PARAMETER(dwParam2);

         LPMIDIHDR               lpmh         = (LPMIDIHDR) dwParam1;

         switch(wMsg)
         {
         case MOM_POSITIONCB:
            ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnPositionCB(lpmh);
            break;
         case MOM_DONE:
            ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnDone(NULL,lpmh);
            return;
         }
      }

      /***************************************************************************
      *
      * seqMIDICallback
      *
      * Called by the system when a buffer is done
      *
      * dw1                       - The buffer that has completed playback.
      *
      ***************************************************************************/
      /*void sequence
      ::MidiOutCallbackDeprecated(HMIDISTRM hms, uint32_t uMsg, uint32_t dwUser, uint32_t dw1, uint32_t dw2_, player::player * lpMidiPlayer, CXfplayerView * pview)
      {
      //TRACE("sequence::MIDICallback current thread %X\n", GetCurrentThreadId());
      LPMIDIHDR               lpmh      = (LPMIDIHDR)dw1;
      //LPMIDIHDR               lpmh2      = (LPMIDIHDR)dw2;
      sequence *         pSeq;
      //    ::multimedia::result                mmrc;
      e_file_result               smfrc;
      LPMIDICALLBACKDATA      lpData = (LPMIDICALLBACKDATA) dwUser;
      //EVENT      *pEvent;
      //   char *      str;
      //   LPDWORD      lpdw;
      //   uint32_t      dwSize;




      if (uMsg == MM_MOM_POSITIONCB)
      {
      //CMidiEventV001 *pEvent = (CMidiEventV001 *) lpmh->lpData;
      LPDWORD lpdw = (uint32_t long *) (lpmh->lpData + lpmh->dwOffset);
      TRACE("MM_MOM_POSITIONCB lpmh->dwOffset = %d\n", lpmh->dwOffset);
      lpdw++;
      lpdw++;
      lpdw++;
      if(*lpdw == EVENT_ID_LYRIC_V1)
      {
      ::ikaraoke::lyric_event_v1 *pLyricEvent = new ::ikaraoke::lyric_event_v1();
      CMemFile memFile((LPBYTE) lpdw, lpmh->dwBytesRecorded - lpmh->dwOffset);
      CArchive ar(&memFile, CArchive::load);
      pLyricEvent->Serialize(ar);
      //::ca2::window * pWnd = (::ca2::window *) dwUser;
      //   oswindow oswindow = dwUser;
      //pWnd->PostMessage(WM_USER, 3377, (LPARAM) pLyricEvent);
      //pview->OnUserMessage(3377, (LPARAM) pLyricEvent);
      //lpMidiPlayer->OnUserMessage(3377, (LPARAM) pLyricEvent);
      //post_thread_message(lpData->dwThreadID, WM_USER, 3377, (LPARAM) pLyricEvent);
      PostMessage(lpData->oswindow, WM_USER, 3377, (LPARAM) pLyricEvent);
      }
      else if(*lpdw == EVENT_ID_PAD)
      {
      //TRACE("MIDICallback pad event\n");
      }
      //   pEvent = lpmh->lpData;
      //      lpdw = (uint32_t long *) (lpmh->lpData + lpmh->dwOffset);
      //      lpdw++;
      //      lpdw++;
      //      dwSize = *lpdw & 0x00FFFFFFL;
      //      lpdw++;
      //      str = (char *) malloc(dwSize  + 1);
      //      strncpy(str, (const char *) lpdw, dwSize);
      //      str[dwSize] = 0;
      //      TRACE0("Lyric:");
      //      TRACE1("%s", str);
      //      TRACE0("\n");
      //      printf("MOM_POSITIONCB");
      return;
      }

      if (uMsg != MM_MOM_DONE)
      return;

      player::player * pthread = lpData->m_pthread;
      ASSERT(NULL != lpmh);

      pSeq = (sequence *)(lpmh->dwUser);

      ASSERT(pSeq != NULL);

      --pSeq->m_uBuffersInMMSYSTEM;

      if (StatusReset == pSeq->GetState())
      {
      // We're recollecting buffers from MMSYSTEM
      //
      //      if (lpmh != pSeq->m_lpmhPreroll)
      //      {
      //          lpmh->lpNext   = pSeq->m_lpmhFree;
      //        pSeq->m_lpmhFree = lpmh;
      //  }

      return;
      }


      if ((StatusStopping == pSeq->GetState()) || (m_flags.is_signalized(FlagEOF)))
      {
      /*
      ** Reached EOF, just put the buffer back on the free
      ** list
      */
      //      if (lpmh != pSeq->m_lpmhPreroll)
      //      {
      //         lpmh->lpNext   = pSeq->m_lpmhFree;
      //       pSeq->m_lpmhFree = lpmh;
      //}
      //      post_thread_message(lpData->dwThreadID, MIDIPLAYERMESSAGE_PLAYBACKEND, (WPARAM) pSeq, (LPARAM) lpmh);
      /*      pplayer->PostMidiSequenceEvent(
      pSeq,
      EventMidiPlaybackEnd,
      lpmh);

      }
      else
      {
      /*
      ** Not EOF yet; attempt to fill another buffer
      */
      /*        smfrc = pSeq->m_pfile->WorkStreamRender(lpmh, pSeq->m_tkEnd, m_cbPrerollNominalMax);

      switch(smfrc)
      {
      case Success:
      break;

      case SEndOfFile:
      pSeq->m_flags.signalize(FlagEOF);
      smfrc = Success;
      break;

      default:
      TRACE( "smfReadEvents returned %lu in callback!", (uint32_t)smfrc);
      pSeq->SetState(StatusStopping);
      break;
      }

      if (Success == smfrc)
      {
      //         post_thread_message(lpData->dwThreadID, MIDIPLAYERMESSAGE_STREAMOUT, (WPARAM) pSeq, (LPARAM) lpmh);
      pplayer->PostMidiSequenceEvent(
      pSeq,
      EventMidiStreamOut,
      lpmh);
      }
      }
      }*/

      /***************************************************************************
      *
      * TranslateSMFResult
      *
      * Translates an error from the SMF layer into an appropriate MCI error.
      *
      * smfrc                     - The return code from any SMF function.
      *
      * Returns
      *   A parallel error from the MCI error codes.
      *
      ***************************************************************************/




      bool sequence::IsPlaying()
      {
         return GetState() == StatusPlaying ||
            GetState() == StatusStopping;
      }




      ::multimedia::result sequence::SaveFile()
      {
         return SaveFile(m_pfile->m_strName);
      }

      ::multimedia::result sequence::SaveFile(const char * lpFileName)
      {
         return TranslateSMFResult(m_pfile->SaveFile(lpFileName));

      }

      ::multimedia::result sequence::SaveFile(::ca2::filesp &ar)
      {
         return m_pfile->SaveFile(ar);
      }



      uint32_t sequence::SetState(uint32_t uiState)
      {
         m_uiPreviousState = m_uiState;
         m_uiState = uiState;
         return m_uiPreviousState;
      }

      void sequence::SetSpecialModeV001Flag(bool bSet)
      {
         if(m_flags.is_signalized(FlagIsInSpecialModeV001))
            m_flags.signalize(FlagWasInSpecialModeV001);
         else
            m_flags.unsignalize(FlagWasInSpecialModeV001);

         if(bSet)
            m_flags.signalize(FlagIsInSpecialModeV001);
         else
            m_flags.unsignalize(FlagIsInSpecialModeV001);
      }

      void sequence::SetSpecialModeV001Operation(uint32_t uiOperation)
      {
         m_uiPreSpecialModeV001State = m_uiState;
         SetSpecialModeV001Flag();
         m_uiSpecialModeV001Operation = uiOperation;
      }

      bool sequence::IsInSpecialModeV001()
      {
         return m_flags.is_signalized(FlagIsInSpecialModeV001);
      }

      bool sequence::WasInSpecialModeV001()
      {
         return m_flags.is_signalized(FlagWasInSpecialModeV001);
      }


      void sequence::GetMidiDoneData(LPMIDIDONEDATA lpmdd)
      {
         lpmdd->m_flags = m_flags;
         lpmdd->m_uiSeqPreviousState = m_uiPreviousState;
         lpmdd->m_uiSeqState = m_uiState;
      }

      int32_t sequence::GetKeyShift()
      {

         if(m_pfile == NULL)
            return 0;

         return m_pfile->GetKeyShift();

      }

      int32_t sequence::SetKeyShift(int32_t iKeyShift)
      {
         /*//    single_lock slStream(&m_csStream, false);
         //  slStream.lock();
         if(m_hMidiStream)
         {
         midiStreamPause(m_hMidiStream);
         if(!m_pfile->SetKeyShift(iKeyShift))
         return false;
         midiStreamRestart(m_hMidiStream);
         }
         //slStream.unlock();
         return true;
         void sequence::SetKeyShift(int32_t iShift)
         {*/
         bool bPlay = IsPlaying();
         imedia::position ticks = 0;
         if(bPlay)
         {
            ticks = GetPositionTicks();
            Stop();
         }
         GetFile().SetKeyShift(iKeyShift);
         if(bPlay)
         {
            m_pthread->PrerollAndWait(ticks);
            Start();
         }
         //}
         return true;
      }

      int32_t sequence::GetTempoShift()
      {
         return m_pfile->GetTempoShift();
      }



      int32_t sequence::SetTempoShift(int32_t iTempoShift)
      {

         //midiStreamPause(m_hMidiStream);
         if(VMS_FAILED(m_pfile->SetTempoShift(iTempoShift)))
            return false;
         //midiStreamRestart(m_hMidiStream);
         return true;
      }


      bool sequence::SetMidiOutDevice(uint32_t uiDevice)
      {
         m_uiDeviceID = uiDevice;
         return true;
      }

      ::multimedia::result sequence::CloseStream()
      {
         single_lock sl(&m_mutex, TRUE);
         if(IsPlaying())
         {
            Stop();
         }

         /* If we were prerolled, need to clean up -- have an open MIDI handle
         ** and buffers in the ready queue
         */

         m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hMidiStream);

         if (m_hMidiStream != NULL)
         {
            midiStreamClose( m_hMidiStream);
            m_hMidiStream = NULL;
         }

         SetState(StatusOpened);

         return MMSYSERR_NOERROR;
      }

      void sequence::SetLevelMeter(int32_t iLevel)
      {
         for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
         {
            m_iaLevel.element_at(i) = iLevel;
         }
      }

      void sequence::OnMidiPlaybackEnd(sequence::Event * pevent)
      {
         UNREFERENCED_PARAMETER(pevent);
         single_lock sl(&m_mutex, TRUE);
         //   LPMIDIHDR lpmh = pevent->m_lpmh;
         //   midi_callback_data * lpData = &m_midicallbackdata;
         ::multimedia::result mmrc;


         if (0 == m_uBuffersInMMSYSTEM)
         {
            TRACE( "seqBufferDone: normal sequencer shutdown.");

            /* Totally done! Free device and notify.
            */
            if(m_hMidiStream)
            {
               if((mmrc = m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hMidiStream))
                  != MMSYSERR_NOERROR)
               {
                  TRACE( "midiOutUnprepareHeader failed in seqBufferDone! (%lu)", (uint32_t)mmrc);
               }
               midiStreamClose(m_hMidiStream);
               m_hMidiStream = NULL;
            }

            m_mmrcLastErr = MMSYSERR_NOERROR;
            m_flags.unsignalize(FlagWaiting);

            m_evMmsgDone.SetEvent();
         }
      }

      void sequence::OnEvent(sequence::Event * pevent)
      {
         switch(pevent->m_eevent)
         {
         case EventSpecialModeV001End:
            {
               SetSpecialModeV001Flag(false);
               OnMidiPlaybackEnd(pevent);
               SetState(m_uiPreSpecialModeV001State);
            }
            break;
         case EventStopped:
            {
               OnMidiPlaybackEnd(pevent);
               SetState(StatusOpened);
            }
            break;
         case EventMidiPlaybackEnd:
            {
               OnMidiPlaybackEnd(pevent);
               SetState(StatusOpened);
            }
            break;
         case EventMidiStreamOut:
            {
               single_lock sl(&m_mutex, TRUE);

               LPMIDIHDR lpmh = pevent->m_lpmh;
               //         midi_callback_data * lpData = &m_midicallbackdata;
               ::multimedia::result mmrc;

               if(IsInSpecialModeV001())
               {
                  TRACE("sequence::OnEvent EventMidiStreamOut IsInSpecialModeV001");
               }
               else
               {
                  mmrc = m_pfile->WorkStreamRender(
                     lpmh,
                     m_tkEnd,
                     m_cbPrerollNominalMax);
               }

               switch(mmrc)
               {
               case Success:
                  break;
               case SEndOfFile:
                  m_flags.signalize(FlagEOF);
                  mmrc = Success;
                  break;
               default:
                  TRACE( "smfReadEvents returned %lu in callback!", (uint32_t)mmrc);
                  SetState(StatusStopping);
                  break;
               }


               if(sequence::StatusStopping == GetState())
               {
                  m_pthread->PostMidiSequenceEvent(
                     this,
                     EventMidiPlaybackEnd,
                     lpmh);
                  return;
               }
               ++m_uBuffersInMMSYSTEM;

               //         single_lock slStream(&m_csStream, false);
               //         slStream.lock();
               if(m_hMidiStream != NULL)
               {
                  mmrc = midiStreamOut(m_hMidiStream, lpmh, sizeof(*lpmh));
               }
               //         slStream.unlock();

               if (MMSYSERR_NOERROR != mmrc)
               {
                  TRACE("seqBufferDone(): midiStreamOut() returned %lu!", (uint32_t)mmrc);

                  --m_uBuffersInMMSYSTEM;
                  SetState(sequence::StatusStopping);
               }
            }
         default:
            break;
         }
      }

      void sequence::OnEvent(e_event eevent)
      {
         Event ev;
         ev.m_psequence = this;
         ev.m_eevent = eevent;
         ev.m_lpmh = NULL;
         OnEvent(&ev);
      }

      imedia::position sequence::GetPositionTicks()
      {
         single_lock sl(&m_mutex);
         if(!sl.lock(millis(0)))
            return -1;
         MMTIME mmt;
         mmt.wType = TIME_TICKS;
         if(MMSYSERR_NOERROR ==
            midiStreamPosition(
            m_hMidiStream,
            &mmt,
            sizeof(mmt)))
            return mmt.u.ticks + m_tkPrerollBase;
         else
            return -1;
      }


      bool sequence::IsChangingTempo()
      {
         return m_flags.is_signalized(sequence::FlagTempoChange);
      }
      void sequence::SetTempoChangeFlag(bool bSet)
      {
         if(bSet)
            m_flags.signalize(sequence::FlagTempoChange);
         else
            m_flags.unsignalize(sequence::FlagTempoChange);
      }

      void sequence::PlayerLink::SetCommand(player::command *pcommand)
      {
         m_spcommand = pcommand;
      }

      void sequence::PlayerLink::OnFinishCommand(player::e_command ecommand)
      {
         if(m_spcommand.is_set())
         {
            if(ecommand == m_spcommand->GetCommand())
            {
               m_spcommand->OnFinish();
            }
            ::ca::release(m_spcommand.m_p);
         }
      }

      sequence::e_flag sequence::PlayerLink::GetFlag()
      {
         return m_eflag;
      }

      void sequence::PlayerLink::SetFlag(e_flag eflag)
      {
         m_eflag = eflag;
      }

      bool sequence::PlayerLink::TestFlag(e_flag eflag)
      {
         return (m_eflag & eflag) > 0;
      }

      void sequence::PlayerLink::ModifyFlag(e_flag eflagAdd, e_flag eflagRemove)
      {
         ASSERT(!(eflagAdd & eflagRemove));
         m_eflag = (e_flag) (m_eflag | eflagAdd);
         m_eflag = (e_flag) (m_eflag & ~eflagRemove);
      }

      sequence::PlayerLink & sequence::GetPlayerLink()
      {
         return m_playerlink;
      }


      sequence::PlayerLink::PlayerLink()
      {
         m_eflag = FlagNull;
      }

      bool sequence::IsNull()
      {
         if(this == NULL)
            return true;
         return false;
      }

      imedia::position sequence::TimeToPosition(imedia::time millis)
      {
         return imedia::position(MillisecsToTicks((int_ptr) millis));
      }

      imedia::time sequence::PositionToTime(imedia::position tk)
      {
         return imedia::time(TicksToMillisecs((imedia::position) (int_ptr) tk));
      }

      void sequence::get_position(imedia::position & position)
      {
         get_ticks(position);
      }

      bool sequence::IsOpened()
      {
         return GetState() != StatusNoFile;
      }

      void sequence::GetTimeLength(imedia::time & time)
      {
         time = m_msLength;
      }



      void sequence::Prepare(
         string2a & str2a,
         imedia::position_2darray & tka2DTokensTicks,
         int32_t iMelodyTrack,
         int2a & ia2TokenLine,
         ::ikaraoke::data & data)
      {
         UNREFERENCED_PARAMETER(str2a);
         file & file = GetFile();
         tracks & tracks = file.GetTracks();

         ASSERT(!file.IsNull());
         file.GetTracks().seek_begin();
         imedia::position               tkMax = file.m_tkLength;
         imedia::position               tkLastPosition = 0;


         ::ikaraoke::static_data & staticdata = data.GetStaticData();

         if(staticdata.m_LyricsDisplayOffset < 480)
         {
            staticdata.m_LyricsDisplayOffset = 480;
         }
         if(staticdata.m_LyricsDisplayOffset > 720)
         {
            staticdata.m_LyricsDisplayOffset = 720;
         }
         staticdata.m_LyricsDisplay = 30;

         imedia::position_2darray tk2DNoteOnPositions(get_app());
         imedia::position_2darray tk2DNoteOffPositions(get_app());
         imedia::position_2darray tk2DBegPositions(get_app());
         imedia::position_2darray tk2DEndPositions(get_app());
         imedia::time_2darray ms2DTokensMillis(get_app());
         imedia::time_2darray ms2DNoteOnMillis(get_app());
         imedia::time_2darray ms2DNoteOffMillis(get_app());
         imedia::time_2darray ms2DBegMillis(get_app());
         imedia::time_2darray ms2DEndMillis(get_app());
         events_v1 midiEvents;




         // Note on and off events
         // and maximum and minimum
         // pitch bend peaks.
         events_v1 midiEventsLevel2;

         events_v1 noteOnEvents;
         events_v1 noteOffEvents;

         events_v1 eventsLevel2Beg;
         events_v1 eventsLevel2End;
         ::ikaraoke::events_tracks_v1 lyricEventsForPositionCB;
         ::ikaraoke::events_tracks_v1 lyricEventsForBouncingBall;
         ::ikaraoke::events_tracks_v1 lyricEventsForScoring;
         ::ikaraoke::events_tracks_v1 lyricEvents;

         //   tracks.seek_begin();
         // tracks.GetXFInfoHeaders(
         //  &m_xfInfoHeaders);



         file.PositionToTime(
            ms2DTokensMillis,
            tka2DTokensTicks,
            0);

         ::ikaraoke::lyric_events_v2 *pLyricEventsV2;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2_;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2B;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2C;
         events_v1 *pMidiEventsV1;

         tk2DNoteOnPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DNoteOffPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DBegPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DEndPositions.set_size_create(tka2DTokensTicks.get_size());
         int32_t i;
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2B = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2C = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2_ = new ::ikaraoke::lyric_events_v2();
            staticdata.m_eventsv1.add(pLyricEventsV2);
            staticdata.m_eventsv1.add(pLyricEventsV2B);
            staticdata.m_eventsv1.add(pLyricEventsV2C);
            staticdata.m_eventsv1.add(pLyricEventsV2_);
            lyricEvents.add(pLyricEventsV2_);
            lyricEventsForScoring.add(pLyricEventsV2);
            lyricEventsForPositionCB.add(pLyricEventsV2B);
            lyricEventsForBouncingBall.add(pLyricEventsV2C);
            pLyricEventsV2->m_iType = 0;
            pLyricEventsV2->m_iOrder = i;
            pLyricEventsV2->m_iType = 2;
            pLyricEventsV2->m_iOrder = i;
            pLyricEventsV2B->m_iType = ikaraoke::EventRunningElement;
            pLyricEventsV2B->m_iOrder = i;
            pLyricEventsV2C->m_iType = 4;
            pLyricEventsV2C->m_iOrder = i;
            pMidiEventsV1 = NULL;
            if(iMelodyTrack < 0)
            {
               pLyricEventsV2->m_iTrack =
                  file.WorkCalcMelodyTrack(
                  &pMidiEventsV1,
                  tka2DTokensTicks.operator[](i),
                  ia2TokenLine[i]);
            }
            else
            {
               pLyricEventsV2->m_iTrack = iMelodyTrack;
            }
            pLyricEventsV2B->m_iTrack = pLyricEventsV2->m_iTrack;
            pLyricEventsV2C->m_iTrack = pLyricEventsV2->m_iTrack;
            pLyricEventsV2_->m_iTrack = pLyricEventsV2->m_iTrack;
            if(pLyricEventsV2->m_iTrack < 0)
            {
               pLyricEventsV2->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2B->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2C->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2_->m_iTrack = tracks.m_iMelodyTrackTipA;
            }
            staticdata.SetGuessMelodyTrack(pLyricEventsV2->m_iTrack);
            if(pLyricEventsV2->m_iTrack >= 0)
            {
               if(file.GetFormat() == 0)
               {
                  tracks.TrackAt(0)->WorkSeekBegin();
                  ((track *)tracks.TrackAt(0))->WorkGetNoteOnOffEventsV1(
                     &midiEvents,
                     (int32_t) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                  tracks.TrackAt(0)->WorkSeekBegin();
                  ((track *)tracks.TrackAt(0))->WorkGetLevel2Events(
                     &midiEventsLevel2,
                     (int32_t) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
               }
               else
               {
                  tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                  ((track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                     &midiEvents,
                     (int32_t) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                  tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                  ((track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                     &midiEventsLevel2,
                     (int32_t) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
               }
            }

            util miditutil(get_app());

            miditutil.PrepareNoteOnOffEvents(
               &noteOnEvents,
               &noteOffEvents,
               (int32_t) pLyricEventsV2->m_iTrack,
               file.GetFormat(),
               &midiEvents,
               tka2DTokensTicks.operator[](i));

            miditutil.PrepareLevel2Events(
               &eventsLevel2Beg,
               &eventsLevel2End,
               (int32_t) pLyricEventsV2->m_iTrack,
               file.GetFormat(),
               &midiEventsLevel2,
               tka2DTokensTicks.operator[](i));


            tk2DNoteOnPositions[i]     = noteOnEvents.m_tkaEventsPosition;
            tk2DNoteOffPositions[i]    = noteOffEvents.m_tkaEventsPosition;
            tk2DBegPositions[i]        = eventsLevel2Beg.m_tkaEventsPosition;
            tk2DEndPositions[i]        = eventsLevel2End.m_tkaEventsPosition;
            pLyricEventsV2->m_dwaNotesData.copy(noteOnEvents.m_dwaEventsData);
            pLyricEventsV2B->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            pLyricEventsV2C->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            pLyricEventsV2_->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            midiEvents.remove_all();
            noteOnEvents.remove_all();
            noteOffEvents.remove_all();
            midiEventsLevel2.remove_all();
            eventsLevel2Beg.remove_all();
            eventsLevel2End.remove_all();
            delete pMidiEventsV1;
         }

         file.PositionToTime(
            ms2DNoteOnMillis,
            tk2DNoteOnPositions,
            0);

         file.PositionToTime(
            ms2DNoteOffMillis,
            tk2DNoteOffPositions,
            0);

         file.PositionToTime(
            ms2DBegMillis,
            tk2DBegPositions,
            0);

         file.PositionToTime(
            ms2DEndMillis,
            tk2DEndPositions,
            0);


         ::ikaraoke::lyric_events_v1 *pLyricEventsV1;


         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEvents.get_at(i);
            staticdata.m_eventstracks.add(pLyricEventsV2);
            file.TimeToPosition(
               pLyricEventsV2->m_tkaTokensPosition,
               ms2DTokensMillis[i],
               0);
            file.TimeToPosition(
               pLyricEventsV2->m_tkaNotesPosition,
               ms2DNoteOnMillis[i],
               0);

            imedia::time time1(0);
            imedia::time time2(0);

            pLyricEventsV2->m_msaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

            pLyricEventsV2->m_msaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

            imedia::time time3(0);
            imedia::time time4(0);

            pLyricEventsV2->m_msaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

            imedia::time time5(0x7fffffff);

            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

         }


         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV1 = new ::ikaraoke::lyric_events_v1();
            pLyricEventsV1->m_iType = ikaraoke::EventAdvanceShow;
            pLyricEventsV1->m_iOrder = i;
            //staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV1);
            file.TimeToPosition(
               pLyricEventsV1->m_tkaTokensPosition,
               ms2DTokensMillis[i],
               -1000);
            //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
            //pLyricTrk->Prepare(*pLyricEventsV1);
         }


         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForScoring.get_at(i);
            staticdata.m_eventsTracksForScoring.add(pLyricEventsV2);
            file.TimeToPosition(
               pLyricEventsV2->m_tkaTokensPosition,
               ms2DTokensMillis[i],
               0);
            file.TimeToPosition(
               pLyricEventsV2->m_tkaNotesPosition,
               ms2DNoteOnMillis[i],
               0);

            imedia::time time1(-100);
            imedia::time time2(0);

            pLyricEventsV2->m_msaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

            pLyricEventsV2->m_msaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

            imedia::time time3(-100);
            imedia::time time4(0);

            pLyricEventsV2->m_msaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

            imedia::time time5(0x7fffffff);

            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

            pLyricEventsV2->PrepareForScoring(this);
         }







         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForPositionCB.get_at(i);
            staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV2);

            staticdata.m_eventstracksV002.add(pLyricEventsV2);

            file.TimeToPosition(
               pLyricEventsV2->m_tkaTokensPosition,
               ms2DTokensMillis[i],
               -100);

            file.TimeToPosition(
               pLyricEventsV2->m_tkaNotesPosition,
               ms2DNoteOnMillis[i],
               -100);

            imedia::time time1(-100);
            imedia::time time2(0);

            pLyricEventsV2->m_msaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

            pLyricEventsV2->m_msaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

            /*         
            pLyricEventsV2->m_msaNotesDuration.Diff(
            ms2DNoteOffMillis[i],
            ms2DNoteOnMillis[i]);
            */


            imedia::time time3(-100);
            imedia::time time4(0);

            pLyricEventsV2->m_msaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

            imedia::time time5(0x7fffffff);

            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

            pLyricEventsV2->PrepareForLyricsDisplay(this);
            //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
            //pLyricTrk->Prepare(*pLyricEventsV2);
         }










         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForBouncingBall.get_at(i);
            staticdata.m_eventsTracksForBouncingBall.add(pLyricEventsV2);

            file.TimeToPosition(
               pLyricEventsV2->m_tkaTokensPosition,
               ms2DTokensMillis[i],
               -100);

            file.TimeToPosition(
               pLyricEventsV2->m_tkaNotesPosition,
               ms2DNoteOnMillis[i],
               -100);

            imedia::time time1(-100);
            imedia::time time2(0);

            pLyricEventsV2->m_msaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

            pLyricEventsV2->m_msaNotesDuration.Diff(
               ms2DEndMillis[i],
               ms2DBegMillis[i]);

            /*
            pLyricEventsV2->m_msaNotesDuration.Diff(
            ms2DNoteOffMillis[i],
            ms2DNoteOnMillis[i]);
            */

            imedia::time time3(-100);
            imedia::time time4(0);

            pLyricEventsV2->m_msaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

            imedia::time time5(0x7fffffff);

            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

            pLyricEventsV2->PrepareForBouncingBall(this);
         }

      }

      void sequence::Prepare(int32_t iTrack, ::ikaraoke::data & data)
      {
         file & file = GetFile();
         tracks & tracks = file.GetTracks();
         string2a & str2a = data.GetStaticData().m_str2aRawTokens;
         imedia::position_2darray position2a;
         int2a ia2TokenLine;


         ia2TokenLine.add_new();

         tracks.WorkSeekBegin();
         tracks.WorkGetEmptyXFTokens(
            iTrack,
            str2a,
            position2a,
            NULL);

         Prepare(
            str2a,
            position2a,
            iTrack,
            ia2TokenLine,
            data);

      }

      void sequence::Prepare(::ikaraoke::data & data)
      {
         file & file = GetFile();
         tracks & tracks = file.GetTracks();
         string2a & str2a = data.GetStaticData().m_str2aRawTokens;
         imedia::position_2darray position2a;
         int2a i2aTokenLine;

         ::music::xf::info_headers xfihs;
         GetFile().GetTracks().GetXFInfoHeaders(&xfihs);

         ::ikaraoke::static_data & staticdata = data.GetStaticData();
         string str;

         // add Title
         staticdata.m_straTitleFormat.add("%0");
         staticdata.m_str2aTitle.set_app(get_app());
         staticdata.m_str2aTitle.add_new();
         staticdata.m_str2aTitle[0].add(xfihs.m_strSongName);

         if(xfihs.m_xfInfoHeader.m_straComposer.get_size() > 0)
         {
            // add Performer
            staticdata.m_straTitleFormat.add("Performer: %0");
            xfihs.m_xfInfoHeader.m_straPerformer.get_format_string(str, ", ");
            staticdata.m_str2aTitle.add_new();
            staticdata.m_str2aTitle[1].add(str);

            // add Composer
            staticdata.m_straTitleFormat.add("Composer: %0");
            xfihs.m_xfInfoHeader.m_straComposer.get_format_string(str, ", ");
            staticdata.m_str2aTitle.add_new();
            staticdata.m_str2aTitle[1].add(str);


            //      staticdata.m_straPerformer = xfihs.m_xfInfoHeader.m_straPerformer;
            //    staticdata.m_straArtist = xfihs.m_xfInfoHeader.m_straPerformer;
         }

         staticdata.m_dwDefaultCodePage = 1252; // Latin (Default of All Default Code Pages)

         if(xfihs.m_xfaInfoHeaderLS.get_count())
         {
            staticdata.m_dwDefaultCodePage = ::music::xf::SymbolCharCodeToCodePage(xfihs.m_xfaInfoHeaderLS[0].m_strLanguage);
         }

         tracks.WorkSeekBegin();
         //tracks.WorkGetXFTokens(staticdata.m_dwDefaultCodePage, str2a, position2a, i2aTokenLine, NULL);
         tracks.WorkGetXFTokens((uint32_t) -1, str2a, position2a, i2aTokenLine, NULL, false);

         Prepare(
            str2a,
            position2a,
            -1,
            i2aTokenLine,
            data);

         tracks.WorkSeekBegin();
         tracks.WorkGetLongestXFLyrics(staticdata.m_strLyrics, false);
         tracks.WorkGetLongestXFLyrics(staticdata.m_strLyricsEx1, true);



      }


      void sequence::GetPositionLength(imedia::position &position)
      {
         position = m_tkLength;
      }

      void sequence::buffer::Initialize(int32_t iSize, uint32_t dwUser)
      {
         m_storage.allocate(iSize);
         m_midihdr.lpData           = (char *) m_storage.get_data();
         m_midihdr.dwBufferLength   = (uint32_t) m_storage.get_size();
         m_midihdr.dwUser           = dwUser;
         m_bPrepared                = false;

      }

      void sequence::BufferArray::Initialize(int32_t iCount, int32_t iSize, uint32_t dwUser)
      {
         set_size(iCount);

         int32_t i;

         /*   for(int32_t i = 0; i < this->get_size() - 1; i++)
         {
         this->element_at(i).SetNextMidiHdr(this->element_at(i + 1).GetMidiHdr());
         }

         if(this->get_size() > 1)
         {
         this->element_at(get_upper_bound()).SetNextMidiHdr(this->element_at(0).GetMidiHdr());
         }
         else
         {
         this->element_at(0).SetNextMidiHdr(NULL);
         }*/

         for(i = 0; i < this->get_size(); i++)
         {
            this->element_at(i).Initialize(iSize, dwUser);
         }
      }

      void sequence::BufferArray::Reset()
      {
         for(int32_t i = 0; i < this->get_size(); i++)
         {
            this->element_at(i).Reset();
         }
      }

      void sequence::buffer::Reset()
      {
         m_midihdr.dwBytesRecorded  = 0;
         m_midihdr.dwFlags          = 0;
      }

      bool sequence::buffer::IsPrepared()
      {
         return m_bPrepared;
      }


      ::multimedia::result sequence::buffer::midiOutPrepareHeader(HMIDIOUT hmidiout)
      {
         ::multimedia::result mmr = 0;
         if(hmidiout == NULL)
            return mmr;
         if(m_bPrepared)
            return mmr;
         mmr = ::midiOutPrepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));
         if(mmr == MMSYSERR_NOERROR)
         {
            m_bPrepared = true;
         }
         return mmr;
      }

      ::multimedia::result sequence::buffer::midiOutUnprepareHeader(HMIDIOUT hmidiout)
      {
         ::multimedia::result mmr = 0;
         if(hmidiout == NULL)
            return mmr;
         if(!m_bPrepared)
            return mmr;
         mmr = ::midiOutUnprepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));
         if(mmr == MMSYSERR_NOERROR)
         {
            m_bPrepared = false;
         }
         return mmr;
      }

      ::multimedia::result sequence::BufferArray::midiOutUnprepareHeader(HMIDIOUT hmidiout)
      {
         ::multimedia::result mmr = MMSYSERR_NOERROR;

         for (int32_t i = 0; i < this->get_size(); i++)
         {
            ::multimedia::result mmrBuffer = this->element_at(i).midiOutUnprepareHeader(hmidiout);
            if(mmrBuffer != MMSYSERR_NOERROR)
            {
               mmr = mmrBuffer;
            }
         }
         return mmr;
      }

      ::multimedia::result sequence::BufferArray::midiOutPrepareHeader(HMIDIOUT hmidiout)
      {
         ::multimedia::result mmrc = MMSYSERR_NOERROR;
         for(int32_t i = 0; i < this->get_size(); i++)
         {
            mmrc = this->element_at(i).midiOutPrepareHeader(
               hmidiout);
            if(mmrc != MMSYSERR_NOERROR)
            {
               for(; i >= 0; i--)
               {
                  this->element_at(i).midiOutUnprepareHeader(hmidiout);
               }
               return mmrc;
            }
         }
         return mmrc;
      }

      LPMIDIHDR sequence::buffer::GetMidiHdr()
      {
         return &m_midihdr;
      }

      void sequence::buffer::SetNextMidiHdr(LPMIDIHDR lpNext)
      {
         m_midihdr.lpNext = lpNext;
      }


      ::multimedia::result sequence::buffer::midiStreamOut(HMIDISTRM hmidiout)
      {
         ASSERT(hmidiout != NULL);
         return ::midiStreamOut(hmidiout, &m_midihdr, sizeof(m_midihdr));
      }

      ::multimedia::result sequence::BufferArray::midiStreamOut(HMIDISTRM hmidiout)
      {
         ::multimedia::result mmrc = MMSYSERR_NOERROR;
         for(int32_t i = 0; i < this->get_size(); i++)
         {
            mmrc = this->element_at(i).midiStreamOut(
               hmidiout);
            if(mmrc != MMSYSERR_NOERROR)
            {
               //         for(; i >= 0; i--)
               //       {
               //        this->element_at(i).midiOutUnprepareHeader(hmidiout);
               //   }
               return mmrc;
            }
         }
         return mmrc;
      }





      void sequence::MuteAll(bool bMute, int32_t iExcludeTrack)
      {
         bool bPlay = IsPlaying();
         imedia::position ticks = 0;
         if(bPlay)
         {
            ticks = GetPositionTicks();
            Stop();
         }
         GetFile().MuteAll(bMute, iExcludeTrack);
         if(bPlay)
         {
            m_pthread->PrerollAndWait(ticks);
            Start();
         }
      }

      void sequence::MuteTrack(int32_t iIndex, bool bMute)
      {
         bool bPlay = IsPlaying();
         imedia::position ticks = 0;
         if(bPlay)
         {
            ticks = GetPositionTicks();
            Stop();
         }
         GetFile().MuteTrack(iIndex, bMute);
         if(bPlay)
         {
            m_pthread->PrerollAndWait(ticks);
            Start();
         }
      }

      imedia::position sequence::GetQuarterNote()
      {
         return GetFile().m_pFileHeader->GetQuarterNoteTicks();
      }


      int32_t sequence::GetDefaultCodePage()
      {
         return 1252;
      }


      bool sequence::IsSettingPosition()
      {
         return m_flags.is_signalized(sequence::FlagSettingPos);
      }


      void sequence::SetSettingPositionFlag(bool bSet)
      {
         if(bSet)
            m_flags.signalize(sequence::FlagSettingPos);
         else
            m_flags.unsignalize(sequence::FlagSettingPos);
      }

      file & sequence::GetFile()
      {
         return * m_pfile;
      }

      uint32_t sequence::GetPreviousState()
      {
         return m_uiPreviousState;
      }

      uint32_t sequence::GetState()
      {
         return m_uiState;
      }


   } // namespace midi_mmsystem


} // namespace music









