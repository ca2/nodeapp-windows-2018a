#include "framework.h"


namespace audio
{


   wave_out::wave_out(sp(::ca2::application) papp) :
      ca2(papp),
      thread(papp),
      m_eventStopped(papp)
   {
      m_pwavebuffer = new wave_buffer(papp);
      if(m_pwavebuffer == NULL)
         throw new memory_exception(papp);
      m_estate             = state_initial;
      m_pthreadCallback    = NULL;
      m_hwaveout           = NULL;
      m_iBufferInMMSys     = 0;
      m_mmr                = MMSYSERR_NOERROR;
      m_peffect            = NULL;
      m_dwLostSampleCount  = 0;
   }

   wave_out::~wave_out()
   {

   }

   void wave_out::install_message_handling(::ca2::message::dispatch * pinterface)
   {
#ifdef WINDOWS
      IGUI_WIN_MSG_LINK(MM_WOM_OPEN, pinterface, this, &wave_out::OnMultimediaOpen);
      IGUI_WIN_MSG_LINK(MM_WOM_DONE, pinterface, this, &wave_out::OnMultimediaDone);
      IGUI_WIN_MSG_LINK(MM_WOM_CLOSE, pinterface, this, &wave_out::OnMultimediaClose);
#endif
      IGUI_WIN_MSG_LINK(MessageBufferReady, pinterface, this, &wave_out::OnBufferReady);
      IGUI_WIN_MSG_LINK(MessageClose, pinterface, this, &wave_out::OnClose);
   }


   bool wave_out::initialize_instance()
   {
      m_pprebuffer = (wave_pre_buffer *) new wave_pre_buffer(get_app());
      return true;
   }

   int32_t wave_out::exit_instance()
   {
      if(GetState() == StatePlaying)
      {
         Stop();
         m_eventStopped.wait(seconds(60));
      }
      if(GetSafeHandle() != NULL)
      {
         close();
      }
      for(int32_t i = 0; i < m_evptraEnd.get_size(); i++)
      {
         m_evptraEnd[i]->SetEvent();
      }

      if(m_pwavebuffer != NULL)
      {
         delete m_pwavebuffer;
         m_pwavebuffer = NULL;
      }


      return thread::exit_instance();
   }

   MMRESULT wave_out::open(
      thread * pthreadCallback,
      int32_t iBufferCount,
      int32_t iBufferSampleCount)
   {
      single_lock sLock(&m_csHandle, TRUE);
      if(m_hwaveout != NULL &&
         m_estate != state_initial)
         return MMSYSERR_NOERROR;
      m_pthreadCallback = pthreadCallback;
      #ifdef WINDOWS
      MMRESULT mmr;
      ASSERT(m_hwaveout == NULL);
      ASSERT(m_estate == state_initial);

      m_waveformatex.wFormatTag = WAVE_FORMAT_PCM;
      m_waveformatex.nChannels = 2;
      m_waveformatex.nSamplesPerSec = 44100;
      m_waveformatex.wBitsPerSample = sizeof(WAVEBUFFERDATA) * 8;
      m_waveformatex.nBlockAlign = m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
      m_waveformatex.nAvgBytesPerSec = m_waveformatex.nSamplesPerSec * m_waveformatex.nBlockAlign;
      m_waveformatex.cbSize = 0;
      sp(::audio::wave) audiowave = Application.audiowave();

      if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
         &m_hwaveout,
         audiowave->m_uiWaveInDevice,
         &m_waveformatex,
         get_os_int(),
         (uint32_t) 0,
         CALLBACK_THREAD)))
         goto Opened;
      m_waveformatex.nSamplesPerSec = 22050;
      m_waveformatex.nAvgBytesPerSec = m_waveformatex.nSamplesPerSec * m_waveformatex.nBlockAlign;
      if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
         &m_hwaveout,
         WAVE_MAPPER,
         &m_waveformatex,
         (uint32_t) get_os_int(),
         (uint32_t) 0,
         CALLBACK_THREAD)))
         goto Opened;
      m_waveformatex.nSamplesPerSec = 11025;
      m_waveformatex.nAvgBytesPerSec = m_waveformatex.nSamplesPerSec * m_waveformatex.nBlockAlign;
      if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
         &m_hwaveout,
         WAVE_MAPPER,
         &m_waveformatex,
         (uint32_t) get_os_int(),
         (uint32_t) 0,
         CALLBACK_THREAD)))
         goto Opened;

      if(mmr !=MMSYSERR_NOERROR)
      {
         if(mmr == MMSYSERR_ALLOCATED)
         {
            TRACE("Specified resource is already allocated.");
         }
         else if(mmr == MMSYSERR_BADDEVICEID)
         {
            TRACE("Specified device identifier is out of range.");
         }
         else if(mmr == WAVERR_BADFORMAT)
         {
            TRACE("Attempted to open with an unsupported waveform-audio format.");
         }
         TRACE("ERROR OPENING WAVE INPUT DEVICE");
         return mmr;
      }

Opened:
      uint32_t uiBufferSizeLog2;
      uint32_t uiBufferSize;
      uint32_t uiAnalysisSize;
      uint32_t uiAllocationSize;
      uint32_t uiInterestSize;
      uint32_t uiSkippedSamplesCount;
      uint32_t uiBufferCount = iBufferCount;

      if(m_waveformatex.nSamplesPerSec == 44100)
      {
         uiBufferSizeLog2 = 16;
         uiBufferSize = m_waveformatex.nChannels * 2 * iBufferSampleCount; // 512 kbytes
         uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
         if(iBufferCount > 0)
         {
            uiAllocationSize = iBufferCount * uiAnalysisSize;
         }
         else
         {
            uiAllocationSize = 8 * uiAnalysisSize;
         }
         uiInterestSize = 200;
         uiSkippedSamplesCount = 2;
      }
      else if(m_waveformatex.nSamplesPerSec == 22050)
      {
         uiBufferSizeLog2 = 10;
         uiBufferSize = 4 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 200;
         uiSkippedSamplesCount = 1;
      }
      else if(m_waveformatex.nSamplesPerSec == 11025)
      {
         uiBufferSizeLog2 = 10;
         uiBufferSize = 2 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 200;
         uiSkippedSamplesCount = 1;
      }
      GetBuffer().PCMOutOpen(
         uiBufferSize,
         uiBufferCount,
         &m_waveformatex,
         &m_waveformatex);

      m_pprebuffer->open(
         this, // callback thread (thread)
         m_waveformatex.nChannels, // channel count
         uiBufferCount, // group count
         iBufferSampleCount); // group sample count

      int32_t i, iSize;
      iSize = GetBuffer().GetBufferCount();
      for(i = 0; i < iSize; i++)
      {
         if(MMSYSERR_NOERROR != (mmr =  waveOutPrepareHeader(
            m_hwaveout,
            GetBuffer().GetHdr(i),
            sizeof(WAVEHDR))))
         {
            TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
            return mmr;
         }
      }
      m_estate = StateOpened;
      return MMSYSERR_NOERROR;
   }

   MMRESULT wave_out::OpenEx(
      thread * pthreadCallback,
      int32_t iBufferCount,
      int32_t iBufferSampleCount,
      uint32_t uiSamplesPerSec,
      uint32_t uiChannelCount,
      uint32_t uiBitsPerSample)
   {
      single_lock sLock(&m_csHandle, TRUE);
      if(m_hwaveout != NULL &&
         m_estate != state_initial)
         return MMSYSERR_NOERROR;
      m_pthreadCallback = pthreadCallback;
      MMRESULT mmr;
      ASSERT(m_hwaveout == NULL);
      ASSERT(m_estate == state_initial);

      m_waveformatex.wFormatTag        = WAVE_FORMAT_PCM;
      m_waveformatex.nChannels         = (WORD) uiChannelCount;
      m_waveformatex.nSamplesPerSec    = uiSamplesPerSec;
      m_waveformatex.wBitsPerSample    = (WORD) uiBitsPerSample;
      m_waveformatex.nBlockAlign       = m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
      m_waveformatex.nAvgBytesPerSec   = m_waveformatex.nSamplesPerSec * m_waveformatex.nBlockAlign;
      m_waveformatex.cbSize            = sizeof(m_waveformatex);

      sp(::audio::wave) audiowave = Application.audiowave();

      try
      {
         if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
            &m_hwaveout,
            audiowave->m_uiWaveInDevice,
            &m_waveformatex,
            get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;
      }
      catch(const ::ca2::exception &)
      {
         return MMSYSERR_ERROR;
      }
      catch(...)
      {
         return MMSYSERR_ERROR;
      }

      if(mmr != MMSYSERR_NOERROR)
      {
         if(mmr == MMSYSERR_ALLOCATED)
         {
            TRACE("Specified resource is already allocated.");
         }
         else if(mmr == MMSYSERR_BADDEVICEID)
         {
            TRACE("Specified device identifier is out of range.");
         }
         else if(mmr == WAVERR_BADFORMAT)
         {
            TRACE("Attempted to open with an unsupported waveform-audio format.");
         }
         TRACE("ERROR OPENING WAVE INPUT DEVICE");
         return mmr;
      }

Opened:
      uint32_t uiBufferSizeLog2;
      uint32_t uiBufferSize;
      uint32_t uiAnalysisSize;
      uint32_t uiAllocationSize;
      uint32_t uiInterestSize;
      uint32_t uiSkippedSamplesCount;
      uint32_t uiBufferCount = iBufferCount;

      //   if(m_waveformatex.nSamplesPerSec == 44100)
      if(true)
      {
         uiBufferSizeLog2 = 16;
         uiBufferSize = m_waveformatex.nChannels * 2 * iBufferSampleCount; // 512 kbytes
         uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
         if(iBufferCount > 0)
         {
            uiAllocationSize = iBufferCount * uiAnalysisSize;
         }
         else
         {
            uiAllocationSize = 8 * uiAnalysisSize;
         }
         uiInterestSize = 200;
         uiSkippedSamplesCount = 2;
      }
      else if(m_waveformatex.nSamplesPerSec == 22050)
      {
         uiBufferSizeLog2 = 10;
         uiBufferSize = 4 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 200;
         uiSkippedSamplesCount = 1;
      }
      else if(m_waveformatex.nSamplesPerSec == 11025)
      {
         uiBufferSizeLog2 = 10;
         uiBufferSize = 2 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 200;
         uiSkippedSamplesCount = 1;
      }
      GetBuffer().PCMOutOpen(
         uiBufferSize,
         uiBufferCount,
         &m_waveformatex,
         &m_waveformatex);

      m_pprebuffer->open(
         this, // callback thread (thread)
         m_waveformatex.nChannels, // channel count
         uiBufferCount, // group count
         iBufferSampleCount); // group sample count

      int32_t i, iSize;
      iSize = GetBuffer().GetBufferCount();
      for(i = 0; i < iSize; i++)
      {
         if(MMSYSERR_NOERROR != (mmr =  waveOutPrepareHeader(
            m_hwaveout,
            GetBuffer().GetHdr(i),
            sizeof(WAVEHDR))))
         {
            TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
            return mmr;
         }
      }
      m_pprebuffer->SetMinL1BufferCount(GetBuffer().GetBufferCount() + 4);
      #endif
      m_estate = StateOpened;
      return MMSYSERR_NOERROR;
   }

   MMRESULT wave_out::Start(const imedia::position & position)
   {
      single_lock sLock(&m_csHandle, TRUE);
      if(m_estate == StatePlaying)
         return MMSYSERR_NOERROR;
      ASSERT(m_estate == StateOpened || m_estate == StateStopped);
      m_estate = StatePlaying;
      m_pprebuffer->Reset();
      m_pprebuffer->Start(position);
      for(int32_t dw = 0; dw < GetBuffer().GetBufferCount(); dw++)
      {
         _Free(&GetBuffer().GetBuffer(dw).m_wavehdr);
      }
      return MMSYSERR_NOERROR;
   }


   MMRESULT wave_out::close()
   {
      single_lock sLock(&m_csHandle, TRUE);
      if(m_estate == StatePlaying)
      {
         Stop();
      }
      if(m_estate != StateOpened)
         return MMSYSERR_NOERROR;
#ifdef WINDOWS
      MMRESULT mmr;
      int32_t i, iSize;
      iSize =  GetBuffer().GetBufferCount();
      for(i = 0; i < iSize; i++)
      {
         if(MMSYSERR_NOERROR != (mmr = waveOutUnprepareHeader(
            m_hwaveout,
            GetBuffer().GetHdr(i),
            sizeof(WAVEHDR))))
         {
            TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", mmr);
         }
      }

      mmr = waveOutClose(m_hwaveout);
      m_hwaveout = NULL;
      #endif
      m_pprebuffer->Reset();
      m_estate = state_initial;
      return MMSYSERR_NOERROR;

   }


   void wave_out::OnMultimediaOpen(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void wave_out::OnMultimediaDone(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::base, pbase, pobj);
      m_iBufferInMMSys--;
      #ifdef WINDOWS
      LPWAVEHDR lpwavehdr = (LPWAVEHDR) pbase->m_lparam.m_lparam;
      OutBufferDone(lpwavehdr);
      #endif
   }

   void wave_out::OnMultimediaClose(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void wave_out::OnBufferReady(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void wave_out::BufferReady(LPWAVEHDR lpwavehdr)
   {

      if(GetState() != StatePlaying)
      {
         TRACE("ERROR wave_out::BufferReady while GetState() != StatePlaying");
         return;
      }

#ifdef WINDOWS
      MMRESULT mmr;
      if(m_peffect != NULL)
      {
         m_peffect->Process16bits(
            (int16_t *) lpwavehdr->lpData,
            lpwavehdr->dwBytesRecorded / 2);
      }

      single_lock sLock(&m_csHandle, TRUE);
      mmr = waveOutWrite(
         m_hwaveout,
         lpwavehdr,
         sizeof(WAVEHDR));
      VERIFY(MMSYSERR_NOERROR == mmr);
      if(mmr == MMSYSERR_NOERROR)
      {
         m_iBufferInMMSys++;
      }
#endif
   }

   bool wave_out::Stop()
   {

      single_lock sLock(&m_csHandle, TRUE);

      if(m_estate != StatePlaying && m_estate != StatePaused)
         return false;

      m_eventStopped.ResetEvent();

      m_pprebuffer->Stop();

      m_estate = StateStopping;
#ifdef WINDOWS
      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.
      m_mmr = waveOutReset(m_hwaveout);
#endif


      if(m_mmr == MMSYSERR_NOERROR)
      {
         m_estate = StateOpened;
      }

      return m_mmr == MMSYSERR_NOERROR;

   }

   bool wave_out::Pause()
   {
      single_lock sLock(&m_csHandle, TRUE);
      ASSERT(m_estate == StatePlaying);

      if(m_estate != StatePlaying)
         return false;


#ifdef WINDOWS
      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.
      m_mmr = waveOutPause(m_hwaveout);
#endif

      ASSERT(m_mmr == MMSYSERR_NOERROR);

      if(m_mmr == MMSYSERR_NOERROR)
      {
         m_estate = StatePaused;
      }

      return m_mmr == MMSYSERR_NOERROR;
   }

   bool wave_out::Restart()
   {
      single_lock sLock(&m_csHandle, TRUE);
      ASSERT(m_estate == StatePaused);

      if(m_estate != StatePaused)
         return false;



#ifdef WINDOWS
      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.
      m_mmr = waveOutRestart(m_hwaveout);
#endif

      ASSERT(m_mmr == MMSYSERR_NOERROR);

      if(m_mmr == MMSYSERR_NOERROR)
      {
         m_estate = StatePlaying;
      }

      return m_mmr == MMSYSERR_NOERROR;
   }


   void wave_out::AttachEndEvent(event *pevent)
   {
      m_evptraEnd.add(pevent);
   }

   imedia::time wave_out::GetPositionMillisForSynch()
   {
      int64_t dwMillis = GetPositionMillis();
      int64_t dwPosition = m_pprebuffer->m_position * 8;
      dwPosition /= m_waveformatex.wBitsPerSample;
      dwPosition *= 1000;
      dwPosition /= m_waveformatex.nChannels * m_waveformatex.nSamplesPerSec;
      if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
         return dwMillis + dwPosition - m_pprebuffer->m_pdecoder->DecoderGetLostMillis(dwMillis + dwPosition) - (((int64_t) m_dwLostSampleCount) /  ((int64_t) m_waveformatex.nSamplesPerSec));
      else
         return dwMillis + dwPosition - ((m_dwLostSampleCount) * 1000 / m_waveformatex.nSamplesPerSec);
   }

   imedia::time wave_out::GetPositionMillis()
   {
      single_lock sLock(&m_csHandle, TRUE);


#ifdef WINDOWS
      MMRESULT                mmr;
      MMTIME                  mmt;
      mmt.wType = TIME_MS;

      if(m_hwaveout != NULL)
      {
         mmr = waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt));
         try
         {
            if (MMSYSERR_NOERROR != mmr)
            {
               TRACE( "waveOutGetPosition() returned %lu", (uint32_t)mmr);
               //      return MCIERR_DEVICE_NOT_READY;
               return 0;
            }
         }
         catch(...)
         {
            //return MCIERR_DEVICE_NOT_READY;
            return 0;
         }
         if(mmt.wType == TIME_BYTES)
         {
            int64_t i = mmt.u.cb;
            i *= 8 * 1000;
            i /= m_waveformatex.wBitsPerSample * m_waveformatex.nChannels * m_waveformatex.nSamplesPerSec;
            return i;

         }
         else
         {
            //*pTicks += mmt.u.ticks;
            return (uint32_t) mmt.u.ms;
         }
      }
      else
         return 0;
#endif

   }

   imedia::position wave_out::get_position_for_synch()
   {
      imedia::position position = get_position();
      if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
         return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->DecoderGetLostPositionOffset(position) - m_dwLostSampleCount * m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
      else
         return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
   }

   imedia::position wave_out::get_position()
   {
      single_lock sLock(&m_csHandle, TRUE);
#ifdef WINDOWS
      MMRESULT                mmr;
      MMTIME                  mmt;
      mmt.wType = TIME_BYTES;

      if(m_hwaveout != NULL)
      {
         mmr = waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt));
         try
         {
            if (MMSYSERR_NOERROR != mmr)
            {
               TRACE( "waveOutGetPosition() returned %lu", (uint32_t)mmr);
               return 0;
            }
         }
         catch(...)
         {
            return 0;
         }
         if(mmt.wType == TIME_MS)
         {
            imedia::position position = (uint32_t) mmt.u.ms;
            position *= m_waveformatex.wBitsPerSample * m_waveformatex.nChannels * m_waveformatex.nSamplesPerSec;
            position /= 8 * 1000;
            return position;
         }
         else
         {
            return (uint32_t) mmt.u.cb;
         }
      }
      else
         return 0;
#endif
   }

   int32_t wave_out::GetBufferInMMSystemCount()
   {
      return m_iBufferInMMSys;
   }

   void wave_out::OnClose(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      VERIFY(close() == MMSYSERR_NOERROR);
   }

   void wave_out::PostClose()
   {
      post_thread_message(MessageClose);
   }

   void * wave_out::GetBuffer(int32_t iBuffer)
   {
      return GetBuffer().PCMOutGetBuffer(iBuffer);
   }

   int32_t wave_out::GetBufferSize()
   {
      return GetBuffer().PCMOutGetInBufferSize();
   }

   MMRESULT wave_out::GetLastMMResult()
   {
      return m_mmr;
   }

   void wave_out::SetWaveStreamEffect(::iaudio::wave_stream_effect * peffect)
   {
      m_peffect = peffect;
   }

   wave_out * wave_out::BeginThread(sp(::ca2::application) papp)
   {
      wave_out * pwaveout = new wave_out(papp);
      pwaveout->begin(::ca2::scheduling_priority_highest);
      return pwaveout;
   }

   int32_t wave_out::GetBufferInMMSysCount()
   {
      return m_iBufferInMMSys;
   }

   audio_decode::decoder * wave_out::SetDecoder(audio_decode::decoder * pdecoder)
   {
      m_pprebuffer->SetDecoder(pdecoder);
      return pdecoder;
   }

   void wave_out::OutBufferDone(LPWAVEHDR lpwavehdr)
   {

      single_lock sl(&m_csOut, TRUE);
      if(m_pprebuffer->IsEOF() || GetState() == StateStopping)
      {
         if(GetBufferInMMSystemCount() <= 0)
         {
            OnPlaybackEnd();
         }
      }
      else
      {
         _Free(lpwavehdr);
      }

   }


   void wave_out::OnPreBufferDone(int32_t iId)
   {
      UNREFERENCED_PARAMETER(iId);
      single_lock sl(&m_csPreBuffer, TRUE);
   }


   void wave_out::_Free(LPWAVEHDR lpwavehdr)
   {
      m_lphdraFree.add(lpwavehdr);
      #ifdef WINDOWS
      if(!m_pprebuffer->IdFree((int32_t) lpwavehdr->dwUser))
         return;

      int32_t iCount = m_pprebuffer->GetChunkByteCount();

      memcpy(lpwavehdr->lpData, m_pprebuffer->GetOutBufferId((int32_t) lpwavehdr->dwUser), iCount);



      BufferReady(lpwavehdr);
      #endif
   }

   void wave_out::OnPlaybackEnd()
   {
      Stop();

      if(m_pprebuffer->m_pstreameffectOut != NULL)
      {
         ::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
         m_pprebuffer->m_pstreameffectOut = NULL;
         delete peffect;
      }

      m_eventStopped.SetEvent();

      m_pplayer->OnEvent(wave_player::EventPlaybackEnd);

   }



   HWAVEOUT wave_out::GetSafeHandle()
   {
      return m_hwaveout;
   }

   wave_buffer & wave_out::GetBuffer()
   {
      return *m_pwavebuffer;
   }

   wave_out::e_state wave_out::GetState()
   {
      return m_estate;
   }


} // namespace audio




