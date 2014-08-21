#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {

         void PatchChange(HMIDIOUT hmidiout);


         mutex & get_midi_mutex();


         sequence::sequence(sp(::axis::application) papp) :
            element(papp),
            ::ikaraoke::karaoke(papp),
            ::music::midi::object(papp),
            ::music::midi::sequence(papp),
            m_buffera(papp)
         {

            m_hstream = NULL;

            m_cbPreroll = 4 * 1024;

            m_cbPrerollNominalMax = 2 * 1024;

            m_midicallbackdata.m_psequence = this;

            m_buffera.Initialize(16, m_cbPreroll, &m_midicallbackdata);

         }


         sequence::~sequence()
         {

            CloseFile();

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
         *   ::multimedia::result_success If the operation was successful.
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
         ::multimedia::e_result sequence::AllocBuffers()
         {
            return ::multimedia::result_error;

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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If there is already a file open
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

         e_result sequence::OpenFile(::music::midi::sequence & sequence, int32_t openMode)
         {

            e_result                rc = success;
            SMFFILEINFO             sfi;
            e_result                smfrc;
            uint32_t                cbBuffer;

            if (GetState() != status_no_file)
            {

               return EFunctionNotSupported;

            }

            m_iOpenMode = openMode;

            smfrc = file()->OpenFile(*sequence.get_file(), openMode);


            if(::music::success != smfrc)
            {

               goto Seq_Open_File_Cleanup;

            }

            file()->GetFileInfo(&sfi);

            m_dwTimeDivision = sfi.dwTimeDivision;
            m_tkLength       = sfi.tkLength;
            if(m_iOpenMode == file::OpenForPlaying)
            {
               m_msLength      = TicksToMillisecs(m_tkLength);
            }
            /* Track buffers must be big enough to hold the state data returned
            ** by smfSeek()
            */
            cbBuffer = min(m_cbBuffer, ::music::midi::GetStateMaxSize());


Seq_Open_File_Cleanup:
            if (::multimedia::result_success != rc)
               CloseFile();
            else
               SetState(status_opened);

            return rc;
         }


         e_result sequence::OpenFile(const char * lpFileName, int32_t openMode)
         {

            ::file::buffer_sp file;

            try
            {

               file = Application.file().get_file(lpFileName, ::file::mode_read | ::file::share_deny_write | ::file::type_binary);

            }
            catch(...)
            {

            }

            return OpenFile(*file, openMode);

         }


         e_result sequence::OpenFile(primitive::memory * pmemorystorage, int32_t openMode, e_storage estorage)
         {

            SMFFILEINFO                sfi;
            e_result                   smfrc;
            uint32_t                   cbBuffer;

            if (GetState() != status_no_file)
            {

               CloseFile();

            }

            m_iOpenMode = openMode;

            smfrc = file()->OpenFile(pmemorystorage, openMode, estorage);

            if (::music::success != smfrc)
            {

            }
            else
            {
               file()->GetFileInfo(&sfi);

               m_dwTimeDivision = sfi.dwTimeDivision;
               m_tkLength       = sfi.tkLength;
               if(m_iOpenMode == file::OpenForPlaying)
               {
                  m_msLength      = TicksToMillisecs(m_tkLength);
               }
               /* Track buffers must be big enough to hold the state data returned
               ** by smfSeek()
               */
               cbBuffer = min(m_cbBuffer, ::music::midi::GetStateMaxSize());
            }

            if(::music::success != smfrc)
               CloseFile();
            else
               SetState(status_opened);

            return smfrc;

         }

         e_result sequence::OpenFile(::file::stream_buffer & ar, int32_t openMode)
         {

            SMFFILEINFO                sfi;
            e_result                   smfrc = success;
            uint32_t                   cbBuffer;

            if (GetState() != status_no_file)
            {
               
               return EFunctionNotSupported;

            }



            //   m_pstrFile = _tcsdup(lpFileName);
            //   m_strFile = lpFileName;

            //    ASSERT(m_pstrFile != NULL);

            m_iOpenMode = openMode;

            //    sofs.pstrName     = m_pstrFile;

            //PSMF pSmf = new SMF();

            //smfrc = file()->OpenFile(&sofs);
            smfrc = file()->OpenFile(ar, openMode);

            //smfrc = smfOpenFile(&sofs);
            if (::music::success != smfrc)
            {
            }
            else
            {
               //    m_hSmf = sofs.hSmf;
               //    ((PSMF) m_hSmf)->GetFileInfo(&sfi);
               file()->GetFileInfo(&sfi);

               m_dwTimeDivision = sfi.dwTimeDivision;
               m_tkLength       = sfi.tkLength;
               if(m_iOpenMode == file::OpenForPlaying)
               {
                  m_msLength      = TicksToMillisecs(m_tkLength);
               }
               /* Track buffers must be big enough to hold the state data returned
               ** by smfSeek()
               */
               cbBuffer = min(m_cbBuffer, ::music::midi::GetStateMaxSize());
            }

            if(success != smfrc)
               CloseFile();
            else
               SetState(status_opened);

            return smfrc;

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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
         *     stopped.
         *
         * A call to seqCloseFile must be paired with a prior call to
         * seqOpenFile. All buffers associated with the file will be
         * freed and the file will be closed. The sequencer must be
         * stopped before this call will be accepted.
         *
         ***************************************************************************/
         e_result sequence::CloseFile()
         {

            single_lock sl(&m_mutex, true);

            file()->CloseFile();

            if (m_hstream != NULL)
            {

               close_device();

            }

            SetState(status_no_file);

            return success;

         }

         ::multimedia::e_result sequence::mm_start()
         {


            bool bThrow = false;

            UINT uDeviceID;

            uDeviceID = m_uiDeviceID;

            ::multimedia::e_result mmrc = ::multimedia::result_success;

            MIDIPROPTIMEDIV         mptd;

            //
            // We've successfully opened the file and all of the tracks; now
            // open the MIDI device and set the time division.
            //
            // NOTE: seqPreroll is equivalent to seek; device might already be open
            //

            if (m_hstream == NULL)
            {

               

               mmrc = translate_os_result(midiStreamOpen(&m_hstream, &uDeviceID, 1, (DWORD_PTR)m_pthread->get_os_int(), (DWORD_PTR)(sequence *) this, CALLBACK_THREAD), "sequence::mm_start", "midiStreamOpen error");

               if (mmrc != ::multimedia::result_success)
               {
                  
                  m_hstream = NULL;
                  
                  if (bThrow)
                  {
                     SetState(status_opened);
                     throw new exception(get_app(), EMidiPlayerPrerollStreamOpen);
                  }

                  goto mm_start_Cleanup;

               }

               PatchChange((HMIDIOUT)m_hstream);  //from RSDN!! Thank you God!! And Thank you RSDN!!              // ������� ������� �����


               mmrc = translate_os_result(midiOutSetVolume((HMIDIOUT) m_hstream, 0xFFFF));

               if (mmrc != ::multimedia::result_success)
               {

               }


               mptd.cbStruct = sizeof(mptd);

               mptd.dwTimeDiv = m_dwTimeDivision;

               mmrc = translate_os_result(midiStreamProperty(m_hstream, (LPBYTE)&mptd, MIDIPROP_SET | MIDIPROP_TIMEDIV), "mm_start", "midiStreamProperty(timeDiv)");

               if (mmrc != ::multimedia::result_success)
               {

                  TRACE("midiStreamProperty() -> %04X", (WORD)mmrc);

                  close_device();

                  if (bThrow)
                  {

                     SetState(status_opened);

                     throw new exception(get_app(), EMidiPlayerPrerollStreamProperty);

                  }

                  goto mm_start_Cleanup;

               }

            }

            mmrc = ::multimedia::result_success;

            mmrc = m_buffera.midiOutPrepareHeader((HMIDIOUT)m_hstream);
            if (mmrc != ::multimedia::result_success)
            {
               TRACE("sequence::mm_start midiOutPrepareHeader Error!! -> %lu", (uint32_t)mmrc);
               if (bThrow)
               {
                  SetState(status_opened);
                  throw new exception(get_app(), EMidiPlayerPrerollPrepareHeader);
               }
               else
               {
                  goto mm_start_Cleanup;
               }
            }


            if (IsInSpecialModeV001())
            {
               if (m_uiSpecialModeV001Operation == SPMV001GMReset)
               {
                  const uchar gmModeOn[] = {
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                     0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00,
                     0x06, 0x00, 0x00, MEVT_LONGMSG,
                     0xf0, 0x7e, 0x7f, 0x09,
                     0x01, 0xf7, 0x00, 0x00 };
                  char * lpch = m_buffera[0].m_lpmidihdr->lpData + m_buffera[0].m_lpmidihdr->dwBytesRecorded;
                  m_buffera[0].m_lpmidihdr->dwBytesRecorded += sizeof(gmModeOn);
                  memcpy(lpch, gmModeOn, sizeof(gmModeOn));
                  SetSpecialModeV001Flag(false);
               }
            }
            mmrc = m_buffera.midiStreamOut(m_hstream);

            if (mmrc != ::multimedia::result_success)
            {

               TRACE("midiStreamOut(preroll) -> %lu!", (uint32_t)mmrc);

               mmrc = ::multimedia::result_not_ready;

               if (bThrow)
               {

                  SetState(status_opened);

                  throw new exception(get_app(), EMidiPlayerPrerollPrepareHeader2);

               }
               else
               {

                  goto mm_start_Cleanup;

               }

            }
         mm_start_Cleanup:

            if (mmrc != ::multimedia::result_success)
            {

               SetState(status_pre_rolled);

               m_flags.unsignalize(::music::midi::sequence::FlagWaiting);

            }
            else
            {

               mmrc = translate_os_result(midiStreamRestart(m_hstream), "mm_start", "midiStreamRestart");

               if (mmrc != ::multimedia::result_success)
               {
                  SetState(status_pre_rolled);
               }
               else
               {
                  SetState(status_playing);
               }

            }

            return mmrc;


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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
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
         ::multimedia::e_result sequence::Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow)
         {
            
            UNREFERENCED_PARAMETER(pthread);

            synch_lock slMidi(&get_midi_mutex());

            single_lock sl(&m_mutex, TRUE);


            int32_t                 i;
            e_result                smfrc;
            ::multimedia::e_result  mmrc = ::multimedia::result_success;


            ASSERT(m_iOpenMode == file::OpenForPlaying || IsInSpecialModeV001());

            m_flags.unsignalize(FlagEOF);

            m_mmrcLastErr = ::multimedia::result_success;

            if(GetState() != status_opened &&
               GetState() != status_pre_rolled &&
               !IsInSpecialModeV001())
               return ::multimedia::result_unsupported_function;

            m_tkBase = lpPreroll->tkBase;
            m_tkEnd = lpPreroll->tkEnd;

            m_iBuffersInMMSYSTEM = 0;

            SetState(::music::midi::sequence::status_pre_rolling);

            m_buffera.Reset();

            smfrc = file()->WorkSeek(m_tkBase, m_buffera[0].m_lpmidihdr);

            m_tkPrerollBase = get_file()->GetPosition();

            m_flags.unsignalize(FlagEOF);

            file()->GetFlags().unsignalize(file::EndOfFile);

            for (i = 1; i < m_buffera.get_size(); i++)
            {
               
               LPMIDIHDR lpmh = m_buffera[i].GetMidiHdr();

               smfrc = file()->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);

               if (success != smfrc && SEndOfFile != smfrc)
               {
                  
                  TRACE("SFP: smfReadEvents() -> %u", (uint32_t)smfrc);
                  
                  mmrc = translate(smfrc);

                  goto seq_Preroll_Cleanup;

               }

               if (SEndOfFile == smfrc)
               {
                  m_flags.signalize(FlagEOF);
                  break;
               }

            }


seq_Preroll_Cleanup:

            if (mmrc != ::multimedia::result_success)
            {

               SetState(status_opened);

               m_flags.unsignalize(::music::midi::sequence::FlagWaiting);

            }
            else
            {

               SetState(status_pre_rolled);

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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
         *     stopped.
         *
         *   ::multimedia::result_not_ready If the underlying MIDI device could
         *     not be opened or fails any call.
         *
         * The sequencer must be prerolled before seqStart may be called.
         *
         * Just feed everything in the ready queue to the device.
         *
         ***************************************************************************/
         ::multimedia::e_result sequence::Start()
         {
            
            single_lock sl(&m_mutex, TRUE);
            
            if (::music::midi::sequence::status_pre_rolled != GetState())
            {
               
               TRACE( "seqStart(): State is wrong! [%u]", GetState());
               
               return ::multimedia::result_unsupported_function;

            }

            SetState(::music::midi::sequence::status_playing);

            m_evMmsgDone.ResetEvent();

            ::multimedia::e_result mmrc = ::multimedia::result_success;
            sl.unlock();
            if(mmrc == ::multimedia::result_success)
            {
               thread()->PostMidiSequenceEvent(this, ::music::midi::sequence::EventMidiPlaybackStart);

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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
         *     playing.
         *
         * The sequencer must be playing before seqPause may be called.
         * Pausing the sequencer will cause all currently on notes to be turned
         * off. This may cause playback to be slightly inaccurate on restart
         * due to missing notes.
         *
         ***************************************************************************/
         ::multimedia::e_result sequence::Pause()

         {
            single_lock sl(&m_mutex, TRUE);

            //    assert(NULL != pSeq);

            if (status_playing != GetState())
               return ::multimedia::result_unsupported_function;

            SetState(status_paused);

            ::multimedia::e_result mmrc = ::multimedia::result_success;
            //    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream != NULL)
            {
               mmrc = translate_os_result(midiStreamPause(m_hstream), "Pause", "midiStreamPause");
            }
            //slStream.unlock();

            SetLevelMeter(0);

            return ::multimedia::result_success;
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
         *    ::multimedia::result_success If the operation is successful.
         *
         *    ::multimedia::result_unsupported_function If the sequencer instance is not
         *     paused.
         *
         * The sequencer must be paused before seqRestart may be called.
         *
         ***************************************************************************/
         ::multimedia::e_result sequence::Restart()
         {
            //    assert(NULL != pSeq);

            single_lock sl(&m_mutex, TRUE);

            if (status_paused != GetState())
               return ::multimedia::result_unsupported_function;

            SetState(status_playing);
            m_evMmsgDone.ResetEvent();

            //    ::multimedia::e_result mmrc = 0;
            //    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream != NULL)
            {
               midiStreamRestart(m_hstream);
            }
            //slStream.unlock();
            return ::multimedia::result_success;
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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
         *     paused or playing.
         *
         * The sequencer must be paused or playing before seqStop may be called.
         *
         ***************************************************************************/
         ::multimedia::e_result sequence::Stop()
         {

            single_lock sl(&m_mutex, TRUE);

            if(GetState() == status_stopping)
               return ::multimedia::result_success;

            // Automatic success if we're already stopped
            if (GetState() != status_playing
               && GetState() != status_paused)
            {
               m_flags.unsignalize(::music::midi::sequence::FlagWaiting);
               GetPlayerLink().OnFinishCommand(::music::midi::player::command_stop);
               return ::multimedia::result_success;
            }

            SetState(status_stopping);
            m_flags.signalize(::music::midi::sequence::FlagWaiting);

            m_eventMidiPlaybackEnd.ResetEvent();

            if(m_hstream != NULL)
            {
               
               m_mmrcLastErr = translate_os_result(midiStreamStop(m_hstream), "Stop", "midiStreamStop");

               if(::multimedia::result_success != m_mmrcLastErr)
               {

                  TRACE( "::music::midi::sequence::Stop() -> midiOutStop() returned %lu in seqStop()!\n", (uint32_t)m_mmrcLastErr);

                  m_flags.unsignalize(FlagWaiting);

                  return ::multimedia::result_not_ready;

               }

            }

            //m_eventMidiPlaybackEnd.lock();

            SetLevelMeter(0);

            return ::multimedia::result_success;

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
         *   ::multimedia::result_success If the operation is successful.
         *
         *   ::multimedia::result_not_ready If the underlying device fails to report
         *     the position.
         *
         *   ::multimedia::result_unsupported_function If the sequencer instance is not
         *     paused or playing.
         *
         * The sequencer must be paused, playing or prerolled before seqTime
         * may be called.
         *
         ***************************************************************************/
         ::multimedia::e_result sequence::get_ticks(imedia::position &  pTicks)
         {
            single_lock sl(&m_mutex);
            if(!sl.lock(millis(184)))
               return ::multimedia::result_internal;

            ::multimedia::e_result                mmr;
            MMTIME                  mmt;

            if ((::music::midi::sequence::status_playing != GetState() &&
               ::music::midi::sequence::status_paused != GetState() &&
               //::music::midi::sequence::status_pre_rolling != GetState() &&
               //::music::midi::sequence::status_pre_rolled != GetState() &&
               ::music::midi::sequence::status_opened != GetState() &&
               ::music::midi::sequence::status_stopping != GetState())
               || m_hstream == NULL)
            {
               //TRACE( "seqTime(): State wrong! [is %u]", GetState());
               return ::multimedia::result_unsupported_function;
            }

            pTicks = 0;
            if (status_opened != GetState())
            {
               pTicks = m_tkBase;
               if (status_pre_rolled != GetState())
               {
                  mmt.wType = TIME_TICKS;
                  //            single_lock slStream(&m_csStream, false);
                  //          slStream.lock();
                  if(m_hstream == NULL)
                  {
                     TRACE("m_hmidi == NULL!!!!");
                     return ::multimedia::result_not_ready;
                  }
                  else
                  {

                     try
                     {

                        mmr = translate_os_result(midiStreamPosition(m_hstream, &mmt, sizeof(mmt)), "get_ticks", "midiStreamPosition");

                        if (::multimedia::result_success != mmr)
                        {

                           TRACE( "midiStreamPosition() returned %lu", (uint32_t)mmr);

                           return ::multimedia::result_not_ready;

                        }

                     }
                     catch(...)
                     {

                        return ::multimedia::result_not_ready;

                     }

                     pTicks += mmt.u.ticks;

                  }

                  //        slStream.unlock();

               }

            }

            return ::multimedia::result_success;

         }

         void sequence::get_time(imedia::time & time)
         {
            get_millis(time);
         }

         ::multimedia::e_result sequence::get_millis(imedia::time & time)
         {
            single_lock sl(&m_mutex);
            if(!sl.lock(millis(184)))
               return ::multimedia::result_internal;

            ::multimedia::e_result                mmr;
            MMTIME                  mmt;

            if (status_playing != GetState() &&
               status_paused != GetState() &&
               status_pre_rolling != GetState() &&
               status_pre_rolled != GetState() &&
               status_opened != GetState() &&
               status_stopping != GetState())
            {
               TRACE( "seqTime(): State wrong! [is %u]", GetState());
               return ::multimedia::result_unsupported_function;
            }

            time = 0;
            if (status_opened != GetState())
            {
               time = (int_ptr) TicksToMillisecs(m_tkBase);
               if (status_pre_rolled != GetState())
               {
                  mmt.wType = TIME_MS;
                  //            single_lock slStream(&m_csStream, false);
                  //          slStream.lock();
                  if(m_hstream == NULL)
                  {
                     TRACE("m_hmidi == NULL!!!!");
                     return ::multimedia::result_not_ready;
                  }
                  else
                  {
                     try
                     {

                        mmr = translate_os_result(midiStreamPosition(m_hstream, &mmt, sizeof(mmt)), "get_millis", "midiStreamPosition");
                        if (::multimedia::result_success != mmr)
                        {
                           TRACE( "midiStreamPosition() returned %lu", (uint32_t)mmr);
                           return ::multimedia::result_not_ready;
                        }
                     }
                     catch(...)
                     {
                        return ::multimedia::result_not_ready;
                     }
                     time += mmt.u.ms;
                  }
                  //        slStream.unlock();
               }
            }

            return ::multimedia::result_success;
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
            return file()->MillisecsToTicks(msOffset);
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
            return file()->TicksToMillisecs(tkOffset);
         }

         void sequence::OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr)
         {
            
            UNREFERENCED_PARAMETER(hmidistream);
            
            e_result               smfrc;
            
            ASSERT(lpmidihdr != NULL);

            midi_callback_data * pcallbackdata = buffer::get(lpmidihdr)->get_midi_callback_data();

            ASSERT(pcallbackdata != NULL);

            ::music::midi::sequence * psequence = pcallbackdata->m_psequence;

            sequence_thread * pthread = dynamic_cast < sequence_thread * > (psequence->m_pthread);

            ASSERT(NULL != lpmidihdr);

            --m_iBuffersInMMSYSTEM;

            if(m_iBuffersInMMSYSTEM <= 0 )
            {
               m_evBuffersZero.SetEvent();
            }

            if (status_reset == GetState())
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

            bool bStopping = status_stopping == GetState();
            bool bEndOfPlay = m_iBuffersInMMSYSTEM <= 0;
            bool bSpecialModeV001End = m_flags.is_signalized(FlagSpecialModeV001End);

            if (bStopping || bEndOfPlay || bSpecialModeV001End)
            {
               //
               // Reached EOF, just put the buffer back on the free
               // list
               //
               if(bSpecialModeV001End)
               {
                  m_flags.unsignalize(FlagSpecialModeV001End);
                  TRACE("void CALLBACK ::music::midi::sequence::MidiOutProc m_flags.is_signalized(FlagSpecialModeV001End\n");
                  pthread->PostMidiSequenceEvent(
                     this,
                     ::music::midi::sequence::EventSpecialModeV001End,
                     lpmidihdr);
               }
               else if(bStopping)
               {
                  if(m_iBuffersInMMSYSTEM == 0)
                  {
                     TRACE("void CALLBACK ::music::midi::sequence::MidiOutProc status_stopping == pSeq->GetState()\n");
                     pthread->PostMidiSequenceEvent(
                        this,
                        ::music::midi::sequence::EventStopped,
                        lpmidihdr);
                  }
               }
               else
               {
                  if(m_flags.is_signalized(FlagEOF))
                  {
                     TRACE("void CALLBACK ::music::midi::sequence::MidiOutProc m_flags.is_signalized(FlagEOF\n");
                  }
                  if(m_iBuffersInMMSYSTEM <= 0)
                  {
                     pthread->PostMidiSequenceEvent(
                        this,
                        ::music::midi::sequence::EventMidiPlaybackEnd,
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
                     ::music::midi::event event;
                     file()->GetTempoEvent(event);
                     file()->StreamEvent(event.GetDelta(), &event, lpmidihdr, 0x7fffffff, 256);
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
                  smfrc = ::music::success;
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

            return;

            ASSERT(lpmidihdr != NULL);
            LPBYTE lpbData = (LPBYTE) (lpmidihdr->lpData + lpmidihdr->dwOffset);
            MIDIEVENT * lpme = (MIDIEVENT *) lpbData;
            file::buffer::midi_stream_event_header * pheader = (file::buffer::midi_stream_event_header *) &lpme->dwParms[0];
            lpbData = (LPBYTE) pheader;
            LPDWORD lpdwParam;

            int32_t iSize = pheader->m_dwLength;
            switch(pheader->m_dwType)
            {
            case 0:
               {
                  array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> * plyriceventa = NULL;
                  array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lyriceventa;
                  for(int32_t i = sizeof(file::buffer::midi_stream_event_header); i < iSize;)
                  {
                     pheader = (file::buffer::midi_stream_event_header *) &lpbData[i];
                     lpdwParam = (LPDWORD) &lpbData[i + sizeof(file::buffer::midi_stream_event_header)];
                     ASSERT(*lpdwParam == pheader->m_dwType);
                     switch(pheader->m_dwType)
                     {
                     case EVENT_ID_LYRIC_V1:
                        {
                           if(plyriceventa == NULL)
                           {
                              plyriceventa = new array <::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &>;
                           }
                           ::file::memory_buffer memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
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
                           ::file::byte_stream_memory_buffer memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
                           for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
                           {
                              BYTE b;
                              memFile >> b;
                              m_iaLevel.element_at(i) = b;
                           }
                        }
                        break;
                     }
                     i += pheader->m_dwLength + sizeof(file::buffer::midi_stream_event_header);
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

         /*
         void CALLBACK sequence::MidiOutProc(HMIDIOUT hmo, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
         {

            UNREFERENCED_PARAMETER(hmo);
            UNREFERENCED_PARAMETER(dwParam2);

            LPMIDIHDR lpmidihdr     = (LPMIDIHDR) dwParam1;
            sequence * psequence    = (sequence *)dwInstance;

            switch(wMsg)
            {
            case MOM_POSITIONCB:
               psequence->OnPositionCB(lpmidihdr);
               break;

            case MOM_DONE:
               psequence->OnDone(NULL, lpmidihdr);
               break;

            default:
               break;

            }

         }
         */

         bool sequence::IsPlaying()
         {

            return GetState() == status_playing || GetState() == status_stopping;

         }


         e_result sequence::SaveFile()
         {

            return SaveFile(file()->m_strName);

         }


         e_result sequence::SaveFile(const char * lpFileName)
         {

            return file()->SaveFile(lpFileName);

         }


         e_result sequence::SaveFile(::file::buffer_sp &ar)
         {

            return file()->SaveFile(*ar);

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


         void sequence::GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd)
         {

            lpmdd->m_flags = m_flags;

            lpmdd->m_uiSeqPreviousState = m_uiPreviousState;

            lpmdd->m_uiSeqState = m_uiState;

         }


         int32_t sequence::GetKeyShift()
         {

            if(m_pfile == NULL)
               return 0;

            return file()->GetKeyShift();

         }


         int32_t sequence::SetKeyShift(int32_t iKeyShift)
         {

            /*//    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream)
            {
            midiStreamPause(m_hstream);
            if(!file()->SetKeyShift(iKeyShift))
            return false;
            midiStreamRestart(m_hstream);
            }
            //slStream.unlock();
            return true;
            void ::music::midi::sequence::SetKeyShift(int32_t iShift)
            {*/

            bool bPlay = IsPlaying();

            imedia::position ticks = 0;

            if(bPlay)
            {

               ticks = GetPositionTicks();

               Stop();

            }

            get_file()->SetKeyShift(iKeyShift);

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

            return file()->GetTempoShift();

         }



         int32_t sequence::SetTempoShift(int32_t iTempoShift)
         {

            //midiStreamPause(m_hstream);

            if(failed(file()->SetTempoShift(iTempoShift)))
               return false;

            //midiStreamRestart(m_hstream);

            return true;

         }


         bool sequence::SetMidiOutDevice(uint32_t uiDevice)
         {

            m_uiDeviceID = translate_out_device(uiDevice);

            return true;

         }

         ::multimedia::e_result sequence::CloseStream()
         {

            single_lock sl(&m_mutex, TRUE);

            if(IsPlaying())
            {

               Stop();

            }

            /* If we were prerolled, need to clean up -- have an open MIDI handle
            ** and buffers in the ready queue
            */

            m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream);

            if (m_hstream != NULL)
            {
               
               close_device();

            }

            SetState(status_opened);

            return ::multimedia::result_success;
         }

         ::music::e_result sequence::close_device()
         {

            single_lock sl(&m_mutex, true);

            if (m_hstream == NULL)
               return ::music::success;

            ::multimedia::e_result mmrc;

            SetState(::music::midi::sequence::status_reset);

            midiOutReset((HMIDIOUT)m_hstream);

            while (m_iBuffersInMMSYSTEM > 0)
               Sleep(100);
            
            if ((mmrc = m_buffera.midiOutUnprepareHeader((HMIDIOUT)m_hstream)) != ::multimedia::result_success)
            {
               TRACE("midiOutUnprepareHeader failed in seqBufferDone! (%lu)", (uint32_t)mmrc);
            }
            
            midiStreamClose(m_hstream);
   
            m_hstream = NULL;

            SetState(::music::midi::sequence::status_opened);
   
            return ::music::success;

         }



         void sequence::SetLevelMeter(int32_t iLevel)
         {
            for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
            {
               m_iaLevel.element_at(i) = iLevel;
            }
         }

         void sequence::OnMidiPlaybackEnd(::music::midi::sequence::event * pevent)
         {
            UNREFERENCED_PARAMETER(pevent);
            single_lock sl(&m_mutex, TRUE);
            //   LPMIDIHDR lpmh = pevent->m_lpmh;
            //   midi_callback_data * lpData = &m_midicallbackdata;
//            ::multimedia::e_result mmrc;


            if(m_iBuffersInMMSYSTEM <= 0)
            {
               TRACE( "seqBufferDone: normal sequencer shutdown.");

               /* Totally done! Free device and notify.
               */
               close_device();

               m_mmrcLastErr = ::multimedia::result_success;
               m_flags.unsignalize(FlagWaiting);

               m_evMmsgDone.SetEvent();
            }
         }

         void sequence::OnEvent(::music::midi::sequence::event * pevent)
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
                  SetState(status_opened);
               }
               break;
            case EventMidiPlaybackEnd:
               {
                  OnMidiPlaybackEnd(pevent);
                  SetState(status_opened);
               }
               break;
            case EventMidiStreamOut:
               {



                  single_lock sl(&m_mutex, TRUE);

                  ::music::midi::mmsystem::sequence::event * pev = (::music::midi::mmsystem::sequence::event *) pevent;

                  if(m_flags.is_signalized(FlagEOF))
                  {

                     return;

                  }

                  LPMIDIHDR lpmh = pev->m_lpmh;

                  e_result smfrc;

                  if(IsInSpecialModeV001())
                  {
                     TRACE("::music::midi::sequence::OnEvent EventMidiStreamOut IsInSpecialModeV001");
                  }
                  else
                  {
                     smfrc = file()->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);
                  }

                  switch(smfrc)
                  {
                  case ::music::success:

                     break;

                  case SEndOfFile:

                     m_flags.signalize(FlagEOF);

                     smfrc = ::music::success;

                     break;

                  default:


                     TRACE( "smfReadEvents returned %lu in callback!", (uint32_t) smfrc);

                     SetState(status_stopping);

                     break;

                  }

                  if(::music::midi::sequence::status_stopping == GetState())
                  {

                     thread()->PostMidiSequenceEvent(this, EventMidiPlaybackEnd, lpmh);

                     return;

                  }

                  ::multimedia::e_result mmrc;

                  if(m_hstream != NULL)
                  {

                     mmrc = translate_os_result(midiStreamOut(m_hstream, lpmh, sizeof(*lpmh)), "OnEvent", "midiStreamOut");

                     if(mmrc == ::multimedia::result_success)
                     {

                        ++m_iBuffersInMMSYSTEM;

                     }
                     else
                     {

                        TRACE("seqBufferDone(): midiStreamOut() returned %lu!", (uint32_t)mmrc);

                        SetState(::music::midi::sequence::status_stopping);

                     }


                  }

               }
            default:
               break;
            }
         }

         imedia::position sequence::GetPositionTicks()
         {
            single_lock sl(&m_mutex);
            if(!sl.lock(millis(0)))
               return -1;
            MMTIME mmt;
            mmt.wType = TIME_TICKS;
            if(::multimedia::result_success ==
               midiStreamPosition(
               m_hstream,
               &mmt,
               sizeof(mmt)))
               return mmt.u.ticks + m_tkPrerollBase;
            else
               return -1;
         }


         bool sequence::IsChangingTempo()
         {
            return m_flags.is_signalized(::music::midi::sequence::FlagTempoChange);
         }
         void sequence::SetTempoChangeFlag(bool bSet)
         {
            if(bSet)
               m_flags.signalize(::music::midi::sequence::FlagTempoChange);
            else
               m_flags.unsignalize(::music::midi::sequence::FlagTempoChange);
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

         void sequence::GetPosition(imedia::position & position)
         {
            get_ticks(position);
         }

         bool sequence::IsOpened()
         {
            return GetState() != status_no_file;
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

            ::music::midi::file::buffer & file = *this->file();

            ::music::midi::tracks & tracks = file.GetTracks();

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
            ::music::midi::events midiEvents;




            // Note on and off events
            // and maximum and minimum
            // pitch bend peaks.
            ::music::midi::events midiEventsLevel2;

            ::music::midi::events noteOnEvents;
            ::music::midi::events noteOffEvents;

            ::music::midi::events eventsLevel2Beg;
            ::music::midi::events eventsLevel2End;
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
            ::music::midi::events *pMidiEventsV1;

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
                     ((::music::midi::track *)tracks.TrackAt(0))->WorkGetNoteOnOffEventsV1(
                        &midiEvents,
                        (int32_t) pLyricEventsV2->m_iTrack,
                        file.GetFormat() == 1);
                     tracks.TrackAt(0)->WorkSeekBegin();
                     ((::music::midi::track *)tracks.TrackAt(0))->WorkGetLevel2Events(
                        &midiEventsLevel2,
                        (int32_t) pLyricEventsV2->m_iTrack,
                        file.GetFormat() == 1);
                  }
                  else
                  {
                     tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                     ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                        &midiEvents,
                        (int32_t) pLyricEventsV2->m_iTrack,
                        file.GetFormat() == 1);
                     tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                     ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                        &midiEventsLevel2,
                        (int32_t) pLyricEventsV2->m_iTrack,
                        file.GetFormat() == 1);
                  }
               }

               ::music::midi::util miditutil(get_app());

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

            ::music::midi::mmsystem::buffer & file = *this->file();

            ::music::midi::tracks & tracks = file.GetTracks();

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
            ::music::midi::mmsystem::buffer & file = *this->file();
            ::music::midi::tracks & tracks = file.GetTracks();
            string2a & str2a = data.GetStaticData().m_str2aRawTokens;
            imedia::position_2darray position2a;
            int2a i2aTokenLine;

            ::music::xf::info_headers xfihs;
            get_file()->GetTracks().GetXFInfoHeaders(&xfihs);

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


         void sequence::buffer::Initialize(int32_t iSize, buffer_array * pbuffera)
         {

            m_pbuffera = pbuffera;

            m_storage.allocate(iSize);

            Reset();

            m_bPrepared                = false;

         }


         void sequence::buffer_array::Initialize(int32_t iCount, int32_t iSize, midi_callback_data * pcallbackdata)
         {

            m_pcallbackdata = pcallbackdata;

            set_size_create(iCount);

            int32_t i;

            for(i = 0; i < this->get_size(); i++)
            {

               this->element_at(i)->Initialize(iSize, this);

            }

         }


         void sequence::buffer_array::Reset()
         {

            for(int32_t i = 0; i < this->get_size(); i++)
            {

               this->element_at(i)->Reset();

            }

         }

         void sequence::buffer::Reset()
         {
            
            ZEROP(m_lpmidihdr);

            m_lpmidihdr->lpData                 = (char *)m_storage.get_data();
            m_lpmidihdr->dwBufferLength         = (uint32_t)m_storage.get_size();
            m_lpmidihdr->dwUser                 = (DWORD_PTR) this;
            //m_lpmidihdr->dwBytesRecorded        = 0;
            //m_lpmidihdr->dwFlags                = 0;
            
            memset(m_lpmidihdr->lpData, 0, m_lpmidihdr->dwBufferLength);

         
         
         }

         sequence::buffer::buffer(sp(::axis::application) papp) :
            element(papp),
            ::music::midi::object(papp),
            m_storage(papp, true)
         {
            
            m_pbuffera = NULL;
            m_lpmidihdr = (LPMIDIHDR) aligned_memory_alloc(sizeof(MIDIHDR));

         }


         sequence::buffer::~buffer()
         {

            memory_free(m_lpmidihdr);

         }

         sequence::buffer_array::buffer_array(sp(::axis::application) papp) :
            element(papp)
         {

            m_pcallbackdata = NULL;

         }


         sequence::buffer_array::~buffer_array()
         {

         }


         bool sequence::buffer::IsPrepared()
         {
            return m_bPrepared;
         }


         ::multimedia::e_result sequence::buffer::midiOutPrepareHeader(HMIDIOUT hmidiout)
         {
            
            ::multimedia::e_result mmr = ::multimedia::result_success;

            if(hmidiout == NULL)
               return mmr;

            if(m_bPrepared)
               return mmr;

            mmr = translate_os_result(::midiOutPrepareHeader(hmidiout, m_lpmidihdr, sizeof(*m_lpmidihdr)));

            if(mmr == ::multimedia::result_success)
            {

               m_bPrepared = true;

            }

            return mmr;

         }


         midi_callback_data * sequence::buffer::get_midi_callback_data()
         {

            if (m_pbuffera == NULL)
               return NULL;

            return m_pbuffera->m_pcallbackdata;

         }

         
         sequence::buffer * sequence::buffer::get(LPMIDIHDR lpmidihdr)
         {

            return (buffer *) (void *) lpmidihdr->dwUser;

         }


         sequence * sequence::buffer::get_sequence(LPMIDIHDR lpmidihdr)
         {

            return get(lpmidihdr)->get_midi_callback_data()->m_psequence;

         }


         ::multimedia::e_result sequence::buffer::midiOutUnprepareHeader(HMIDIOUT hmidiout)
         {
            
            ::multimedia::e_result mmr = ::multimedia::result_success;

            if(hmidiout == NULL)
               return mmr;

            if(!m_bPrepared)
               return mmr;

            mmr = translate_os_result(::midiOutUnprepareHeader(hmidiout, m_lpmidihdr, sizeof(*m_lpmidihdr)));

            if(mmr == ::multimedia::result_success)
            {

               m_bPrepared = false;

            }

            return mmr;

         }


         ::multimedia::e_result sequence::buffer_array::midiOutUnprepareHeader(HMIDIOUT hmidiout)
         {
            
            ::multimedia::e_result mmr = ::multimedia::result_success;

            for (int32_t i = 0; i < this->get_size(); i++)
            {

               ::multimedia::e_result mmrBuffer = this->element_at(i)->midiOutUnprepareHeader(hmidiout);

               if(mmrBuffer != ::multimedia::result_success)
               {

                  mmr = mmrBuffer;

               }

            }

            return mmr;

         }


         ::multimedia::e_result sequence::buffer_array::midiOutPrepareHeader(HMIDIOUT hmidiout)
         {

            ::multimedia::e_result mmrc = ::multimedia::result_success;

            for(int32_t i = 0; i < this->get_size(); i++)
            {

               mmrc = this->element_at(i)->midiOutPrepareHeader(hmidiout);

               if(mmrc != ::multimedia::result_success)
               {

                  for(; i >= 0; i--)
                  {

                     this->element_at(i)->midiOutUnprepareHeader(hmidiout);

                  }

                  return mmrc;

               }

            }

            return mmrc;

         }


         ::multimedia::e_result sequence::buffer::midiStreamOut(HMIDISTRM hmidiout)
         {

            ASSERT(hmidiout != NULL);

            if (m_lpmidihdr->dwBytesRecorded == 0)
            {

               return ::multimedia::result_error;

            }

            return translate_os_result(::midiStreamOut(hmidiout, m_lpmidihdr, sizeof(*m_lpmidihdr)));

         }


         ::multimedia::e_result sequence::buffer_array::midiStreamOut(HMIDISTRM hmidiout)
         {

            ::multimedia::e_result mmrc = ::multimedia::result_success;

            for(int32_t i = 0; i < this->get_size(); i++)
            {

               mmrc = this->element_at(i)->midiStreamOut(hmidiout);
               
               if (mmrc == ::multimedia::result_success)
               {

                  m_pcallbackdata->m_psequence->m_iBuffersInMMSYSTEM++;

               }
               else
               {

                  if (i <= 0)
                     return mmrc;

                  return ::multimedia::result_success;

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
            get_file()->MuteAll(bMute, iExcludeTrack);
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
            get_file()->MuteTrack(iIndex, bMute);
            if(bPlay)
            {
               m_pthread->PrerollAndWait(ticks);
               Start();
            }
         }

         imedia::position sequence::GetQuarterNote()
         {
            return get_file()->m_pFileHeader->GetQuarterNoteTicks();
         }


         int32_t sequence::GetDefaultCodePage()
         {
            return 1252;
         }


         bool sequence::IsSettingPosition()
         {
            return m_flags.is_signalized(::music::midi::sequence::FlagSettingPos);
         }


         void sequence::SetSettingPositionFlag(bool bSet)
         {
            if(bSet)
               m_flags.signalize(::music::midi::sequence::FlagSettingPos);
            else
               m_flags.unsignalize(::music::midi::sequence::FlagSettingPos);
         }

         uint32_t sequence::GetPreviousState()
         {
            return m_uiPreviousState;
         }

         uint32_t sequence::GetState()
         {
            return m_uiState;
         }

         ::music::midi::sequence::event * sequence::create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr)
         {

            ASSERT(this != NULL);

            event * pevent          = new event();

            pevent->m_eevent        = eevent;
            pevent->m_psequence     = this;
            pevent->m_lpmh          = lpmidihdr;

            return pevent;

         }

         
         ::multimedia::e_result sequence::SendGMReset()
         {

            synch_lock sl(&get_midi_mutex());

            TRACE0("::music::midi::mmsystem::player::SendReset : (0)");

            HMIDIOUT hmidiout = NULL;

            MMRESULT mmr;

            uint32_t uDeviceID = m_uiDeviceID;

            manual_reset_event ev(get_app());

            ev.ResetEvent();

            mmr = midiOutOpen(&hmidiout, uDeviceID, (DWORD_PTR)ev.get_os_data(), 0, CALLBACK_EVENT);

            if (mmr != MMSYSERR_NOERROR)
               return translate_os_result(mmr);

            const uchar gmModeOn[] = {
               0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00,
               0x06, 0x00, 0x00, MEVT_LONGMSG,
               0xf0, 0x7e, 0x7f, 0x09,
               0x01, 0xf7, 0x00, 0x00 };

            MIDIHDR mh;

            LPMIDIHDR lpmh = &mh;

            ZERO(mh);

            lpmh->lpData = (char *)gmModeOn;
            lpmh->dwBufferLength = sizeof(gmModeOn);

            mmr = midiOutPrepareHeader(hmidiout, lpmh, sizeof(MIDIHDR));

            if (mmr != MMSYSERR_NOERROR)
               goto End;

            lpmh->dwBytesRecorded = sizeof(gmModeOn);

            mmr = midiOutLongMsg(hmidiout, lpmh, sizeof(MIDIHDR));

            if (mmr != MMSYSERR_NOERROR)
               goto End;

            while (!(mh.dwFlags & MHDR_DONE))
            {

               ev.wait();

            }

            mmr = midiOutUnprepareHeader(hmidiout, lpmh, sizeof(MIDIHDR));

            if (mmr != MMSYSERR_NOERROR)
               goto End;

         End:

            ::multimedia::e_result mmrc = translate_os_result(mmr);

            mmr = midiOutClose(hmidiout);

            if (mmr != MMSYSERR_NOERROR)
            {
               
               translate_os_result(mmr);

            }

            return mmrc;

         }

         //
         // ������� ��������� � ����� ������
         // ================================
         //

         //void SendPatchChange(void) {

         //   MidiOutChan(0xC0, BYTE(Patch), 0); // ��������� ��������� Cn pp

         //}

         //
         // ��������� ����� ������
         // ======================
         //

         void PatchChange(HMIDIOUT hmidiout)
         {

            if (!hmidiout) {

               return;

            }

            int i;
            PATCHARRAY Patches;           // ������ ���������� �������

            for (i = 0; i < MIDIPATCHSIZE; i++) {

               Patches[i] = 0xFFFF;            // �������� ������ �������

            }

            //Patches[Patch] = 0xFFFF;     // ������� ����� ��� ���� �������

            //MMRESULT mmr = midiOutCachePatches(hmidiout, 0, Patches, MIDI_CACHE_ALL); // �������� ������

            //SendPatchChange();           // ������ ��������� � ����� ������

         }



      } // namespace mmsystem


   } // namespace midi


} // namespace music









