#include "framework.h"


namespace multimedia
{


   namespace audio_xaudio7
   {


      wave_in::wave_in(sp(::axis::application) papp) :
         object(papp),
         ::thread(papp),
         wave_base(papp),
         ::multimedia::audio::wave_in(papp)
      {

         m_pencoder = NULL;
//         m_hwavein = NULL;
         m_estate = state_initial;
         m_bResetting = false;

      }

      wave_in::~wave_in()
      {
      }

      bool wave_in::initialize_instance()
      {
         TRACE("wave_in::initialize_instance %X\n", get_os_int());
         //SetMainWnd(NULL);
         //ASSERT(GetMainWnd() == NULL);
         set_thread_priority(::multithreading::priority_highest);
         m_evInitialized.SetEvent();
         return true;
      }

      int32_t wave_in::exit_thread()
      {
         m_eventExitInstance.SetEvent();
         return thread::exit_thread();
      }

      void wave_in::pre_translate_message(::signal_details * pobj)
      {
         SCAST_PTR(::message::base, pbase, pobj);
         //ASSERT(GetMainWnd() == NULL);
         //if(pbase->m_uiMessage == MM_WIM_OPEN ||
         //   pbase->m_uiMessage == MM_WIM_CLOSE ||
         //   pbase->m_uiMessage == MM_WIM_DATA)
         //{
         //   translate_wave_in_message(pbase);
         //   if(pbase->m_bRet)
         //      return;
         //}
         return thread::pre_translate_message(pbase);
      }

      ::multimedia::e_result wave_in::wave_in_open(int32_t iBufferCount, int32_t iBufferSampleCount)
      {

//         if(m_hwavein != NULL && m_estate != state_initial)
//         {
//            wave_in_initialize_encoder();
//
//            return ::multimedia::result_success;
//
//         }
//
//         single_lock sLock(&m_mutex, TRUE);
//         ::multimedia::e_result mmr;
//         ASSERT(m_hwavein == NULL);
//         ASSERT(m_estate == state_initial);
//
//         m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
//         m_pwaveformat->nChannels = 2;
//         m_pwaveformat->nSamplesPerSec = 44100;
//         m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8;
//         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
//         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
//         m_pwaveformat->cbSize = 0;
//         sp(::multimedia::audio::wave) audiowave = Application.audiowave();
//         m_iBuffer = 0;
//
//         if(MMSYSERR_NOERROR == (mmr = xaudio7::translate(waveInOpen(
//            &m_hwavein,
//            audiowave->m_uiWaveInDevice,
//            wave_format(),
//            get_os_int(),
//            (uint32_t) 0,
//            CALLBACK_THREAD))))
//            goto Opened;
//
//         m_pwaveformat->nSamplesPerSec = 22050;
//         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
//         if(MMSYSERR_NOERROR == (mmr = xaudio7::translate(waveInOpen(
//            &m_hwavein,
//            WAVE_MAPPER,
//            wave_format(),
//            (uint32_t) get_os_int(),
//            (uint32_t) 0,
//            CALLBACK_THREAD))))
//            goto Opened;
//         m_pwaveformat->nSamplesPerSec = 11025;
//         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
//         if(MMSYSERR_NOERROR == (mmr = xaudio7::translate(waveInOpen(
//            &m_hwavein,
//            WAVE_MAPPER,
//            wave_format(),
//            (uint32_t) get_os_int(),
//            (uint32_t) 0,
//            CALLBACK_THREAD))))
//            goto Opened;
//
//         if(mmr !=MMSYSERR_NOERROR)
//         {
//            if(mmr == MMSYSERR_ALLOCATED)
//            {
//               TRACE("Specified resource is already allocated.");
//            }
//            else if(mmr == MMSYSERR_BADDEVICEID)
//            {
//               TRACE("Specified device identifier is out of range.");
//            }
//            else if(mmr == WAVERR_BADFORMAT)
//            {
//               TRACE("Attempted to open with an unsupported waveform-audio_xaudio7 format.");
//            }
//            TRACE("ERROR OPENING WAVE INPUT DEVICE");
//            return mmr;
//         }
//
//Opened:
//         uint32_t uiBufferSizeLog2;
//         uint32_t uiBufferSize;
//         uint32_t uiAnalysisSize;
//         uint32_t uiAllocationSize;
//         uint32_t uiInterestSize;
//         uint32_t uiSkippedSamplesCount;
//
//         if(m_pwaveformat->nSamplesPerSec == 44100)
//         {
//            uiBufferSizeLog2 = 16;
//            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
//            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
//            if(iBufferCount > 0)
//            {
//               uiAllocationSize = iBufferCount * uiBufferSize;
//            }
//            else
//            {
//               uiAllocationSize = 8 * uiAnalysisSize;
//            }
//            uiInterestSize = 200;
//            uiSkippedSamplesCount = 2;
//         }
//         else if(m_pwaveformat->nSamplesPerSec == 22050)
//         {
//            uiBufferSizeLog2 = 9;
//            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
//            uiAnalysisSize = 16 * 1 << uiBufferSizeLog2;
//            uiAllocationSize = 4 * uiAnalysisSize;
//            uiInterestSize = 600;
//            uiSkippedSamplesCount = 1;
//         }
//         else if(m_pwaveformat->nSamplesPerSec == 11025)
//         {
//            uiBufferSizeLog2 = 9;
//            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
//            uiAnalysisSize = 8 * 1 << uiBufferSizeLog2;
//            uiAllocationSize = 4 * uiAnalysisSize;
//            uiInterestSize = 600;
//            uiSkippedSamplesCount = 1;
//         }
//         wave_in_get_buffer()->FFTOpen(
//            uiAllocationSize,
//            uiBufferSize,
//            uiAnalysisSize,
//            uiInterestSize,
//            uiSkippedSamplesCount);
//         
//         int32_t i, iSize;
//         
//         iSize = (int32_t) wave_in_get_buffer()->GetBufferCount();
//
//         for(i = 0; i < iSize; i++)
//         {
//            
//            if(MMSYSERR_NOERROR != (mmr = xaudio7::translate(waveInPrepareHeader(m_hwavein, xaudio7::create_new_WAVEHDR(wave_in_get_buffer(), i), sizeof(WAVEHDR)))))
//            {
//               TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
//               return mmr;
//            }
//
//            wave_in_add_buffer(i);
//
//         }
//
//         if(m_pencoder != NULL && !wave_in_initialize_encoder())
//         {
//
//            m_estate = state_opened;
//
//            wave_in_close();
//
//            return (::multimedia::e_result) -1;
//
//         }
//
//         m_estate = state_opened;
//
         return ::multimedia::result_success;

      }


      ::multimedia::e_result wave_in::wave_in_close()
      {

         single_lock sLock(&m_mutex, TRUE);

         //::multimedia::e_result mmr;

         //if(m_estate != state_opened && m_estate != state_stopped)
         //   return ::multimedia::result_success;

         //mmr = wave_in_reset();

         //int32_t i, iSize;

         //iSize = (int32_t) wave_in_get_buffer()->GetBufferCount();

         //for(i = 0; i < iSize; i++)
         //{

         //   if(::multimedia::result_success != (mmr = xaudio7::translate(waveInUnprepareHeader(m_hwavein, wave_hdr(i), sizeof(WAVEHDR)))))
         //   {
         //      TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer");
         //      //return mmr;
         //   }

         //   delete wave_hdr(i);

         //}

         //mmr = xaudio7::translate(waveInClose(m_hwavein));

         //m_hwavein = NULL;

         //m_estate = state_initial;

         return ::multimedia::result_success;

      }

      ::multimedia::e_result wave_in::wave_in_start()
      {
         
         single_lock sLock(&m_mutex, TRUE);
         
         if(m_estate == state_recording)
            return ::multimedia::result_success;
         
         //ASSERT(m_estate == state_opened || m_estate == state_stopped);

         if(m_estate != state_opened && m_estate != state_stopped)
            return ::multimedia::result_success;

         //::multimedia::e_result mmr;

         //if(::multimedia::result_success != (mmr = xaudio7::translate(waveInStart(m_hwavein))))
         //{
         //   TRACE("ERROR starting INPUT DEVICE ");
         //   return mmr;
         //}

         m_estate = state_recording;

         return ::multimedia::result_success;

      }

      ::multimedia::e_result wave_in::wave_in_stop()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate != state_recording)
            return ::multimedia::result_error;

         ::multimedia::e_result mmr;

         m_estate = state_stopping;

         //try
         //{

         //   if(::multimedia::result_success != (mmr = xaudio7::translate(waveInStop(m_hwavein))))
         //   {

         //      TRACE("wave_in::wave_in_stop : ERROR OPENING stopping INPUT DEVICE ");

         //   }

         //}
         //catch(...)
         //{

         //   TRACE("wave_in::wave_in_stop : Exception OPENING stopping INPUT DEVICE ");

         //}

         m_estate = state_stopped;

         m_eventStopped.SetEvent();

         return ::multimedia::result_success;

      }


      //void CALLBACK wave_in::wave_in_proc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
      //{

      //   UNREFERENCED_PARAMETER(hwi);
      //   UNREFERENCED_PARAMETER(dwInstance);
      //   UNREFERENCED_PARAMETER(dwParam1);
      //   UNREFERENCED_PARAMETER(dwParam2);
      //   if(uMsg == WIM_DATA)
      //   {
      //      ASSERT(FALSE);
      //      /*      uint32_t msSampleTime = timeGetTime();
      //      thread * pthread = (thread *) dwInstance;
      //      ASSERT(pthread != NULL);
      //      LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;
      //      LPWAVEPROCDATAMESSAGE lpxfwm = new WAVEPROCDATAMESSAGE;
      //      lpxfwm->bDelete = TRUE;
      //      lpxfwm->msSampleTime = msSampleTime;
      //      //      lpxfwm->tkSamplePosition = tkPosition;
      //      lpxfwm->lpWaveHdr = lpWaveHdr;
      //      pthread->post_thread_message(
      //      WM_USER,
      //      (WPARAM) WAVM_WAVE_PROC_DATA,
      //      (LPARAM) lpxfwm);
      //      //      i++;
      //      //      if( i > 2)
      //      //         i = 0;*/
      //   }

      //}


      ::multimedia::e_result wave_in::wave_in_reset()
      {

         single_lock sLock(&m_mutex, TRUE);

         m_bResetting = true;

         //if(m_hwavein == NULL)
         //{

         //   return ::multimedia::result_success;

         //}

         //::multimedia::e_result mmr;

         //if(m_estate == state_recording)
         //{

         //   if(::multimedia::result_success != (mmr = wave_in_stop()))
         //   {

         //      TRACE("wave_in::Reset error stopping input device");

         //      return mmr;

         //   }

         //}


         //try
         //{

         //   if(::multimedia::result_success != (mmr = xaudio7::translate(waveInReset(m_hwavein))))
         //   {
         //      
         //      TRACE("wave_in::Reset error resetting input device");

         //      return mmr;

         //   }

         //}
         //catch(...)
         //{
         //}

         //m_estate = state_opened;

         m_bResetting = false;

         return ::multimedia::result_success;

      }


      void wave_in::translate_wave_in_message(::signal_details * pobj)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         //ASSERT(pbase->m_uiMessage == MM_WIM_OPEN || pbase->m_uiMessage == MM_WIM_CLOSE || pbase->m_uiMessage == MM_WIM_DATA);

         //if(pbase->m_uiMessage == MM_WIM_DATA)
         //{
         //   
         //   m_iBuffer--;
         //   
         //   uint32_t msSampleTime = timeGetTime();
         //   
         //   LPWAVEHDR lpwavehdr = (LPWAVEHDR) pbase->m_lparam.m_lparam;

         //   wave_in_get_buffer()->get_buffer((int32_t) lpwavehdr->dwUser)->OnMultimediaDone();

         //   m_listenerset.wave_in_data_proc(this, msSampleTime, (int32_t) lpwavehdr->dwUser);

         //   if(m_pencoder != NULL)
         //   {
         //      m_pencoder->EncoderWriteBuffer(lpwavehdr->lpData, lpwavehdr->dwBytesRecorded);
         //   }

         //   if(!wave_in_is_resetting() && wave_in_is_recording())
         //   {
         //      wave_in_add_buffer((int32_t) lpwavehdr->dwUser);
         //   }

         //}

         pbase->m_bRet = true;

      }


      ::multimedia::e_result wave_in::wave_in_add_buffer(int32_t iBuffer)
      {
         
         //return wave_in_add_buffer(wave_hdr(iBuffer));

         return ::multimedia::result_success;

      }


      //::multimedia::e_result wave_in::wave_in_add_buffer(LPWAVEHDR lpwavehdr)
      //{

      //   ::multimedia::e_result mmr;

      //   if(::multimedia::result_success != (mmr = xaudio7::translate(waveInAddBuffer(m_hwavein, lpwavehdr, sizeof(WAVEHDR)))))
      //   {

      //      TRACE("ERROR OPENING Adding INPUT DEVICE buffer");

      //   }

      //   m_iBuffer++;

      //   return mmr;

      //}


      bool wave_in::wave_in_initialize_encoder()
      {

         if(m_pencoder == NULL)
            return false;

         if(!::multimedia::audio::wave_in::wave_in_initialize_encoder())
            return false;

         return true;

      }

      WAVEFORMATEX * wave_in::wave_format()
      {

         xaudio7::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }

      //HWAVEIN wave_in::wave_in_get_safe_HWAVEIN()
      //{
      //   
      //   if(this == NULL)
      //      return NULL;

      //   return m_hwavein;

      //}

      void * wave_in::get_os_data()
      {

         //return m_hwavein;

         return NULL;
      }


      //LPWAVEHDR wave_in::wave_hdr(int iBuffer)
      //{

      //   return ::multimedia::xaudio7::get_os_data(wave_in_get_buffer(), iBuffer);

      //}


   } // namespace audio_xaudio7


} // namespace multimedia






