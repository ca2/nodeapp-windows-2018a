#include "framework.h"


namespace audio
{


   wave_in::wave_in(sp(::ca2::application) papp) :
      ca2(papp),
      thread(papp),
      m_eventExitInstance(papp),
      m_eventStopped(papp),
      m_evInitialized(papp)
   {
      m_pencoder = NULL;
      m_hWaveIn = NULL;
      m_estate = state_initial;
      m_bResetting = false;
      //m_pthreadCallback = NULL;
      m_iRefCount = 0;
      m_pwavebuffer = new wave_buffer(papp);
      if(m_pwavebuffer == NULL)
         throw new memory_exception(papp);
   }

   wave_in::~wave_in()
   {
      ASSERT(m_iRefCount == 0);
      if(m_pwavebuffer != NULL)
      {
         delete m_pwavebuffer;
      }
   }

   bool wave_in::initialize_instance()
   {
      TRACE("wave_in::initialize_instance %X\n", get_os_int());
      //SetMainWnd(NULL);
      //ASSERT(GetMainWnd() == NULL);
      set_thread_priority(::ca2::scheduling_priority_highest);
      m_evInitialized.SetEvent();
      return true;
   }

   int32_t wave_in::exit_instance()
   {
      m_eventExitInstance.SetEvent();
      return thread::exit_instance();
   }

   void wave_in::pre_translate_message(::ca2::signal_object * pobj) 
   {
      SCAST_PTR(::ca2::message::base, pbase, pobj);
      //ASSERT(GetMainWnd() == NULL);
      if(pbase->m_uiMessage == MM_WIM_OPEN ||
         pbase->m_uiMessage == MM_WIM_CLOSE ||
         pbase->m_uiMessage == MM_WIM_DATA)
      {
         TranslateWaveInMessage(pbase);
         if(pbase->m_bRet)
            return;
      }
      return thread::pre_translate_message(pbase);
   }

   MMRESULT wave_in::open(
      int32_t iBufferCount,
      int32_t iBufferSampleCount)
   {
      if(m_hWaveIn != NULL && m_estate != state_initial)
      {
         InitializeEncoder();
         return MMSYSERR_NOERROR;
      }

      single_lock sLock(&m_csHandle, TRUE);
      MMRESULT mmr;
      ASSERT(m_hWaveIn == NULL);
      ASSERT(m_estate == state_initial);

      m_waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
      m_waveFormatEx.nChannels = 2;
      m_waveFormatEx.nSamplesPerSec = 44100;
      m_waveFormatEx.wBitsPerSample = sizeof(WAVEBUFFERDATA) * 8;
      m_waveFormatEx.nBlockAlign = m_waveFormatEx.wBitsPerSample * m_waveFormatEx.nChannels / 8;
      m_waveFormatEx.nAvgBytesPerSec = m_waveFormatEx.nSamplesPerSec * m_waveFormatEx.nBlockAlign;
      m_waveFormatEx.cbSize = 0;
      sp(::audio::wave) audiowave = Application.audiowave();
      m_iBuffer = 0;

      if(MMSYSERR_NOERROR == (mmr = waveInOpen(
         &m_hWaveIn,            
         audiowave->m_uiWaveInDevice,            
         &m_waveFormatEx,  
         get_os_int(),
         (uint32_t) 0,
         CALLBACK_THREAD)))
         goto Opened;
      m_waveFormatEx.nSamplesPerSec = 22050;
      m_waveFormatEx.nAvgBytesPerSec = m_waveFormatEx.nSamplesPerSec * m_waveFormatEx.nBlockAlign;
      if(MMSYSERR_NOERROR == (mmr = waveInOpen(
         &m_hWaveIn,            
         WAVE_MAPPER,            
         &m_waveFormatEx,  
         (uint32_t) get_os_int(),
         (uint32_t) 0,
         CALLBACK_THREAD)))
         goto Opened;
      m_waveFormatEx.nSamplesPerSec = 11025;
      m_waveFormatEx.nAvgBytesPerSec = m_waveFormatEx.nSamplesPerSec * m_waveFormatEx.nBlockAlign;
      if(MMSYSERR_NOERROR == (mmr = waveInOpen(
         &m_hWaveIn,            
         WAVE_MAPPER,            
         &m_waveFormatEx,  
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

      if(m_waveFormatEx.nSamplesPerSec == 44100)
      {
         uiBufferSizeLog2 = 16;
         uiBufferSize = m_waveFormatEx.nChannels * 2 * iBufferSampleCount; // 512 kbytes
         uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
         if(iBufferCount > 0)
         {
            uiAllocationSize = iBufferCount * uiBufferSize;
         }
         else
         {
            uiAllocationSize = 8 * uiAnalysisSize;
         }
         uiInterestSize = 200;
         uiSkippedSamplesCount = 2;
      }
      else if(m_waveFormatEx.nSamplesPerSec == 22050)
      {
         uiBufferSizeLog2 = 9;
         uiBufferSize = 4 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 16 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 600;
         uiSkippedSamplesCount = 1;
      }
      else if(m_waveFormatEx.nSamplesPerSec == 11025)
      {
         uiBufferSizeLog2 = 9;
         uiBufferSize = 2 * 1 << uiBufferSizeLog2;
         uiAnalysisSize = 8 * 1 << uiBufferSizeLog2;
         uiAllocationSize = 4 * uiAnalysisSize;
         uiInterestSize = 600;
         uiSkippedSamplesCount = 1;
      }
      GetBuffer().FFTOpen(
         uiAllocationSize,
         uiBufferSize,
         uiAnalysisSize,
         uiInterestSize,
         uiSkippedSamplesCount);
      int32_t i, iSize;
      iSize = (int32_t) GetBuffer().GetBufferCount();
      for(i = 0; i < iSize; i++)
      {
         if(MMSYSERR_NOERROR != (mmr =  waveInPrepareHeader(
            m_hWaveIn,  
            &GetBuffer().get_item(i)->m_wavehdr,
            sizeof(WAVEHDR))))
         {
            TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
            return mmr;
         }
         AddBuffer(i);
      }
      if(m_pencoder != NULL && !InitializeEncoder())
      {
         m_estate = StateOpened;
         close();
         return (MMRESULT) -1;
      }
      m_estate = StateOpened;
      return MMSYSERR_NOERROR;
   }

   wave_buffer & wave_in::GetBuffer()
   {
      return *m_pwavebuffer;
   }

   MMRESULT wave_in::close()
   {
      single_lock sLock(&m_csHandle, TRUE);

      MMRESULT mmr;

      if(m_estate != StateOpened && m_estate != StateStopped)
         return MMSYSERR_NOERROR;

      mmr = Reset();

      int32_t i, iSize;
      iSize = (int32_t) GetBuffer().GetBufferCount();
      for(i = 0; i < iSize; i++)
      {
         if(MMSYSERR_NOERROR != (mmr = waveInUnprepareHeader(
            m_hWaveIn,  
            GetBuffer().GetHdr(i),
            sizeof(WAVEHDR))))
         {
            TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer");
            //return mmr;
         }
      }

      mmr = waveInClose(m_hWaveIn);
      m_hWaveIn = NULL;
      m_estate = state_initial;
      return MMSYSERR_NOERROR;

   }

   MMRESULT wave_in::Start()
   {
      single_lock sLock(&m_csHandle, TRUE);
      if(m_estate == StateRecording)
         return MMSYSERR_NOERROR;
      //ASSERT(m_estate == StateOpened || m_estate == StateStopped);
      if(m_estate != StateOpened &&
         m_estate != StateStopped)
         return MMSYSERR_NOERROR;
      MMRESULT mmr;
      if(MMSYSERR_NOERROR != (mmr = waveInStart(
         m_hWaveIn)))
      {
         TRACE("ERROR starting INPUT DEVICE ");
         return mmr;
      }
      m_estate = StateRecording;
      return MMSYSERR_NOERROR;

   }

   MMRESULT wave_in::Stop()
   {
      
      single_lock sLock(&m_csHandle, TRUE);
      
      if(m_estate != StateRecording)
         return MMSYSERR_ERROR;

      MMRESULT mmr;

      m_estate = state_stopping;

      try
      {
         if(MMSYSERR_NOERROR != (mmr = waveInStop(m_hWaveIn)))
         {
            TRACE("wave_in::Stop : ERROR OPENING stopping INPUT DEVICE ");
         }
      }
      catch(...)
      {
         TRACE("wave_in::Stop : Exception OPENING stopping INPUT DEVICE ");
      }

      m_estate = StateStopped;

      m_eventStopped.SetEvent();

      return MMSYSERR_NOERROR;

   }

   void CALLBACK wave_in::WaveInProc(HWAVEIN hwi, uint32_t uMsg, uint32_t dwInstance, uint32_t dwParam1, uint32_t dwParam2)
   {

      UNREFERENCED_PARAMETER(hwi);
      UNREFERENCED_PARAMETER(dwInstance);
      UNREFERENCED_PARAMETER(dwParam1);
      UNREFERENCED_PARAMETER(dwParam2);
      if(uMsg == WIM_DATA)
      {
         ASSERT(FALSE);
         /*      uint32_t msSampleTime = timeGetTime();
         thread * pthread = (thread *) dwInstance;
         ASSERT(pthread != NULL);
         LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;
         LPWAVEPROCDATAMESSAGE lpxfwm = new WAVEPROCDATAMESSAGE;
         lpxfwm->bDelete = TRUE;
         lpxfwm->msSampleTime = msSampleTime;
         //      lpxfwm->tkSamplePosition = tkPosition;
         lpxfwm->lpWaveHdr = lpWaveHdr;
         pthread->post_thread_message(
         WM_USER,
         (WPARAM) WAVM_WAVE_PROC_DATA,
         (LPARAM) lpxfwm);
         //      i++;
         //      if( i > 2)
         //         i = 0;*/
      }
   }


   LPWAVEFORMATEX wave_in::GetFormatEx()
   {
      return &m_waveFormatEx;
   }

   uint32_t wave_in::GetState()
   {
      return m_estate;
   }

   MMRESULT wave_in::Reset()
   {
      single_lock sLock(&m_csHandle, TRUE);
      m_bResetting = true;

      if(m_hWaveIn == NULL)
      {
         return MMSYSERR_ERROR;
      }

      MMRESULT mmr;
      if(m_estate == StateRecording)
      {
         if(MMSYSERR_NOERROR != (mmr = Stop()))
         {
            TRACE("wave_in::Reset error stopping input device");
            return mmr;
         }
      }
      try
      {
         if(MMSYSERR_NOERROR != (mmr = waveInReset(
            m_hWaveIn)))
         {
            TRACE("wave_in::Reset error resetting input device");
            return mmr;
         }
      }
      catch(...)
      {
      }
      m_estate = StateOpened;
      m_bResetting = false;
      return MMSYSERR_NOERROR;
   }

   bool wave_in::IsResetting()
   {
      return m_bResetting;
   }


   uint32_t wave_in::GetAnalysisMillis()
   {
      return (uint32_t) ((double) GetBuffer().m_uiAnalysisSize * GetBuffer().m_uiSkippedSamplesCount * 1000.0 / m_waveFormatEx.nSamplesPerSec);
   }

   HWAVEIN wave_in::GetSafeHwavein()
   {
      return m_hWaveIn;
   }

   void wave_in::TranslateWaveInMessage(::ca2::signal_object * pobj)
   {
      SCAST_PTR(::ca2::message::base, pbase, pobj);
      ASSERT(
         pbase->m_uiMessage == MM_WIM_OPEN ||
         pbase->m_uiMessage == MM_WIM_CLOSE ||
         pbase->m_uiMessage == MM_WIM_DATA);

      if(pbase->m_uiMessage == MM_WIM_DATA)
      {
         m_iBuffer--;
         uint32_t msSampleTime = timeGetTime();
         LPWAVEHDR lpwavehdr = (LPWAVEHDR) pbase->m_lparam.m_lparam;
         GetBuffer().GetBuffer((int32_t) lpwavehdr->dwUser).OnMultimediaDone();
         m_listenerset.WaveInDataProc(
            this,
            msSampleTime,
            lpwavehdr);

         if(m_pencoder != NULL)
         {
            m_pencoder->EncoderWriteBuffer(lpwavehdr->lpData, lpwavehdr->dwBytesRecorded);
         }

         if(!IsResetting() && IsRecording())
         {
            AddBuffer(lpwavehdr);
         }

      }
      pbase->m_bRet = true;
   }

   bool wave_in::IsRecording()
   {
      return m_estate == StateRecording;
   }

   bool wave_in::IsOpened()
   {
      return m_estate == StateOpened;
   }

   critical_section & wave_in::GetHandleCriticalSection()
   {
      return m_csHandle;
   }

   int32_t wave_in::AddRef()
   {
      int32_t iCount = ++m_iRefCount;
      return iCount;
   }

   int32_t wave_in::Release()
   {
      int32_t iCount = --m_iRefCount;
      if(iCount == 0)
      {
         if(IsRecording())
         {
            Reset();
         }
         if(IsOpened())
         {
            close();
         }
      }
      return iCount;
   }

   void wave_in::AddListener(wave_in_listener *plistener)
   {
      m_listenerset.add_unique(plistener);
      plistener->HookWaveIn(this);
   }

   void wave_in::RemoveListener(wave_in_listener *plistener)
   {
      m_listenerset.remove(plistener);
   }

   MMRESULT wave_in::AddBuffer(int32_t iBuffer)
   {
      return AddBuffer(GetBuffer().GetHdr(iBuffer));
   }

   MMRESULT wave_in::AddBuffer(LPWAVEHDR lpwavehdr)
   {
      MMRESULT mmr;
      if(MMSYSERR_NOERROR != (mmr = waveInAddBuffer(
         GetSafeHwavein(),  
         lpwavehdr,
         sizeof(WAVEHDR))))
      {
         TRACE("ERROR OPENING Adding INPUT DEVICE buffer");
      }
      m_iBuffer++;
      return mmr;
   }

   bool wave_in::InitializeEncoder()
   {
      if(m_pencoder == NULL)
         return false;
      m_pencoder->EncoderSetSamplesPerSec(m_waveFormatEx.nSamplesPerSec);
      m_pencoder->EncoderSetBitsPerSample(m_waveFormatEx.wBitsPerSample);
      m_pencoder->EncoderSetChannelCount(m_waveFormatEx.nChannels);
      m_pencoder->EncoderInitialize();
      return true;
   }


} // namespace audio




