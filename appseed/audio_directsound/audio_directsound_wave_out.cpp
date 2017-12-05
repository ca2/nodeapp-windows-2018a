#include "framework.h"
// thank you (casey) : tahnk oy coed prjsoetc China Cai Tao

namespace multimedia
{


   namespace audio_directsound
   {



      wave_out::wave_out(sp(::axis::application) papp) :
         ::object(papp),
         ::thread(papp),
         wave_base(papp),
         ::multimedia::audio::wave_out(papp)
      {




         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_pdirectsound       = NULL;
         m_psoundbuffer       = NULL;
         m_iBufferedCount     = 0;
         m_peffect            = NULL;
//         m_dwLostSampleCount  = 0;

      }

      wave_out::~wave_out()
      {

      }

      void wave_out::install_message_routing(::message::sender * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_routing(pinterface);

      }


      bool wave_out::init_thread()
      {

         if (!::multimedia::audio::wave_out::init_thread())
         {

            return false;

         }

         return true;

      }


      void wave_out::run()
      {

         ::thread::run();

      }


      void wave_out::term_thread()
      {

         ::multimedia::audio::wave_out::term_thread();
      }


      ::multimedia::e_result wave_out::wave_out_open(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount)
      {

         synch_lock sl(m_pmutex);

         if (m_pdirectsound != NULL && m_psoundbuffer != NULL && m_estate != state_initial)
         {

            return ::multimedia::result_success;

         }


         m_pthreadCallback = pthreadCallback;
         //::multimedia::e_result mmr;
         ASSERT(m_pdirectsound == NULL);
         ASSERT(m_psoundbuffer == NULL);
         ASSERT(m_estate == state_initial);

         // by (indirect casey)
         if(FAILED(DirectSoundCreate8(NULL,&m_pdirectsound,NULL)))
         {

            return ::multimedia::result_error;

         }


         if(FAILED(m_pdirectsound->SetCooperativeLevel((HWND) Application.m_puiMain->get_safe_handle(),DSSCL_PRIORITY)))
         {

            return ::multimedia::result_error;

         }


         m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels = 2;
         m_pwaveformat->nSamplesPerSec = 44100;
         m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;
         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         {

            DSBUFFERDESC BufferDescription = {};
            BufferDescription.dwSize = sizeof(BufferDescription);
            BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

            // NOTE(casey): "Create" a primary buffer
            // TODO(casey): DSBCAPS_GLOBALFOCUS?
            LPDIRECTSOUNDBUFFER PrimaryBuffer;
            if(SUCCEEDED(m_pdirectsound->CreateSoundBuffer(&BufferDescription,&PrimaryBuffer,0)))
            {
               HRESULT Error = PrimaryBuffer->SetFormat(wave_format());
               if(SUCCEEDED(Error))
               {
                  // NOTE(casey): We have finally set the format!
                  OutputDebugStringA("Primary buffer format was set.\n");
                  m_pdirectsound->Release();
                  return ::multimedia::result_error;
               }
               else
               {
                  // TODO(casey): Diagnostic
               }
            }
            else
            {
               // TODO(casey): Diagnostic
            }


         }



         //if(mmr = directsound::translate(waveOutOpen(
         //   &m_hwaveout,
         //   audiowave->m_uiWaveInDevice,
         //   wave_format(),
         //   get_os_int(),
         //   (uint32_t) 0,
         //   CALLBACK_THREAD))))
         //   goto Opened;
         //m_pwaveformat->nSamplesPerSec = 22050;
         //m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         //if(MMSYSERR_NOERROR == (mmr = directsound::translate(waveOutOpen(
         //   &m_hwaveout,
         //   WAVE_MAPPER,
         //   ,
         //   (uint32_t) get_os_int(),
         //   (uint32_t) 0,
         //   CALLBACK_THREAD))))
         //   goto Opened;
         //m_pwaveformat->nSamplesPerSec = 11025;
         //m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         //if(MMSYSERR_NOERROR == (mmr = directsound::translate(waveOutOpen(
         //   &m_hwaveout,
         //   WAVE_MAPPER,
         //   wave_format(),
         //   (uint32_t) get_os_int(),
         //   (uint32_t) 0,
         //   CALLBACK_THREAD))))
         //   goto Opened;

         //if(mmr != ::multimedia::result_success)
         //{
         //   return mmr;
         //}

//Opened:
         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;
         uint32_t uiBufferCount = iBufferCount;

         if(m_pwaveformat->nSamplesPerSec == 44100)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
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
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }

         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, uiBufferCount,128,m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(
         this, // callback thread (thread)
         m_pwaveformat->nChannels, // channel count
         uiBufferCount, // group count
         iBufferSampleCount); // group sample count


         // TODO(casey): DSBCAPS_GETCURRENTPOSITION2
         DSBUFFERDESC BufferDescription = {};
         BufferDescription.dwSize = sizeof(BufferDescription);
         BufferDescription.dwFlags = 0;
         BufferDescription.dwBufferBytes = uiBufferSize * uiBufferCount;
         BufferDescription.lpwfxFormat = wave_format();
         //LPDIRECTSOUNDBUFFER SecondaryBuffer;
         HRESULT Error = m_pdirectsound->CreateSoundBuffer(&BufferDescription,&m_psoundbuffer,0);
         if(FAILED(Error))
         {
            OutputDebugStringA("Secondary buffer created successfully.\n");
            return ::multimedia::result_error;
         }

         //int32_t i, iSize;
         //iSize = wave_out_get_buffer()->GetBufferCount();
         //for(i = 0; i < iSize; i++)
         //{

         //   if(MMSYSERR_NOERROR != (mmr =  directsound::translate(waveOutPrepareHeader(m_hwaveout, directsound::create_new_WAVEHDR(m_pwavebuffer, i), sizeof(WAVEHDR)))))
         //   {
         //      TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
         //      return mmr;
         //   }
         //}

         m_estate = state_opened;

         return ::multimedia::result_success;

      }

      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample)
      {

         synch_lock sl(m_pmutex);

         if(m_pdirectsound != NULL && m_psoundbuffer != NULL && m_estate != state_initial)
            return ::multimedia::result_success;

         m_iBuffer = 0;

         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
         ASSERT(m_pdirectsound == NULL);
         ASSERT(m_psoundbuffer == NULL);
         ASSERT(m_estate == state_initial);

         // by (indirect casey)
         if(FAILED(DirectSoundCreate8(NULL,&m_pdirectsound,NULL)))
         {

            return ::multimedia::result_error;

         }


         if(FAILED(m_pdirectsound->SetCooperativeLevel((HWND)Application.m_puiMain->get_safe_handle(),DSSCL_PRIORITY)))
         {

            return ::multimedia::result_error;

         }

         mmr = ::multimedia::result_success;
         m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels = 2;
         m_pwaveformat->nSamplesPerSec = 44100;
         m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;
         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         {

            DSBUFFERDESC BufferDescription = {};
            BufferDescription.dwSize = sizeof(BufferDescription);
            BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

            // NOTE(casey): "Create" a primary buffer
            // TODO(casey): DSBCAPS_GLOBALFOCUS?
            LPDIRECTSOUNDBUFFER PrimaryBuffer;
            if(FAILED(m_pdirectsound->CreateSoundBuffer(&BufferDescription,&PrimaryBuffer,0)))
            {
               // TODO(casey): Diagnostic
               m_pdirectsound->Release();
               return ::multimedia::result_error;
            }
            else
            {
               HRESULT Error = PrimaryBuffer->SetFormat(wave_format());
               if(FAILED(Error))
               {
                  // TODO(casey): Diagnostic
                  m_pdirectsound->Release();
                  return ::multimedia::result_error;
               }
               else
               {
                  // NOTE(casey): We have finally set the format!
                  OutputDebugStringA("Primary buffer format was set.\n");
               }
            }


         }

         if(mmr != ::multimedia::result_success)
         {

            TRACE("%s", ::multimedia::strerror(mmr));

            return mmr;

         }

//Opened:

         iBufferCount = 4;
         iBufferSampleCount = (1 << 10);

         uint32_t uiBufferSize = iBufferSampleCount * m_pwaveformat->nChannels * 2;



         // TODO(casey): DSBCAPS_GETCURRENTPOSITION2
         DSBUFFERDESC BufferDescription = {};
         BufferDescription.dwSize = sizeof(BufferDescription);
         BufferDescription.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY;
         BufferDescription.dwBufferBytes = uiBufferSize * iBufferCount;
         BufferDescription.lpwfxFormat = wave_format();
         //LPDIRECTSOUNDBUFFER SecondaryBuffer;
         HRESULT Error = m_pdirectsound->CreateSoundBuffer(&BufferDescription,&m_psoundbuffer,0);
         if(FAILED(Error))
         {
            OutputDebugStringA("Secondary buffer created successfully.\n");
            return ::multimedia::result_error;
         }
         //Query DirectSoundNotify
         if(FAILED(m_psoundbuffer->QueryInterface(IID_IDirectSoundNotify,(LPVOID *)&m_psoundnotify)))
         {
            OutputDebugString(_T("QueryInterface DirectSoundNotify Failed!"));
            //m_strLastError = _T("MyDirectSound SetFormat Failed!");
            return ::multimedia::result_error;
         }

         m_haEvent.remove_all();
         m_notifya.remove_all();

         for(index i = 0; i < iBufferCount; i++)
         {

            m_haEvent.add(CreateEvent(NULL,FALSE,FALSE,NULL));
            m_notifya.element_at_grow(i).dwOffset = (DWORD) (uiBufferSize * i);
            m_notifya.element_at_grow(i).hEventNotify = m_haEvent[i];
         }


         if(FAILED(m_psoundnotify->SetNotificationPositions((DWORD) m_notifya.get_count(),m_notifya.get_data())))
         {
            OutputDebugString(_T("Set NotificationPosition Failed!"));
            return ::multimedia::result_error;
         }



         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, iBufferCount,128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, iBufferCount, iBufferSampleCount);

         m_pprebuffer->SetMinL1BufferCount(wave_out_get_buffer()->GetBufferCount() + 4);

         m_estate = state_opened;

         return ::multimedia::result_success;

      }



      ::multimedia::e_result wave_out::wave_out_close()
      {

         synch_lock sl(m_pmutex);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return ::multimedia::result_success;

         //::multimedia::e_result mmr;

         ::count iSize;

         iSize =  wave_out_get_buffer()->GetBufferCount();

         //for(i = 0; i < iSize; i++)
         //{

         //   if(::multimedia::result_success != (mmr = directsound::translate(waveOutUnprepareHeader(m_hwaveout, wave_hdr(i), sizeof(WAVEHDR)))))
         //   {
         //      TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", mmr);
         //   }

         //   delete wave_hdr(i);

         //}

         //;mmr = directsound::translate(waveOutClose(m_hwaveout));

         //m_hwaveout = NULL;

         m_pprebuffer->Reset();

         m_estate = state_initial;

         return ::multimedia::result_success;

      }



      void wave_out::wave_out_buffer_ready(index iBuffer)
      {

         on_free(iBuffer);

         return;

      }


      void wave_out::on_free(index i)
      {

         synch_lock sl(m_pmutex);
         LPVOID lpvAudio1 = NULL,lpvAudio2 = NULL;
         DWORD dwBytesAudio1 = 0,dwBytesAudio2 = 0;
         DWORD dwRetSamples = 0,dwRetBytes = 0;

         //DWORD dwWrite;


         //         wave_out_out_buffer_done(i);

         if(i >= 0)
         {
            HRESULT hr = m_psoundbuffer->Lock((DWORD)(i * wave_out_get_buffer_size()), (DWORD) wave_out_get_buffer_size(),&lpvAudio1,&dwBytesAudio1,&lpvAudio2,&dwBytesAudio2,0);
            if(FAILED(hr))
            {
               return;
            }
            if(NULL == lpvAudio2)
            {
               memcpy(lpvAudio1,wave_out_get_buffer_data(i),dwBytesAudio1);
            }
            else
            {
               memcpy(lpvAudio1,wave_out_get_buffer_data(i),dwBytesAudio1);
               memcpy(lpvAudio2,(byte *)wave_out_get_buffer_data(i) + dwBytesAudio1,dwBytesAudio2);
            }

            //Unlock DirectSoundBuffer
            m_psoundbuffer->Unlock(lpvAudio1,dwBytesAudio1,lpvAudio2,dwBytesAudio2);
         }




      }


      ::multimedia::e_result wave_out::wave_out_stop()
      {

         synch_lock sl(m_pmutex);

         if(m_estate != state_playing && m_estate != state_paused)
            return ::multimedia::result_error;

         m_eventStopped.ResetEvent();

         m_pprebuffer->Stop();

         m_estate = state_stopping;

         //// waveOutReset
         //// The waveOutReset function stops playback on the given
         //// waveform-audio_directsound output device and resets the current position
         //// to zero. All pending playback buffers are marked as done and
         //// returned to the application.
         //m_mmr = directsound::translate(waveOutReset(m_hwaveout));

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_opened;

         }

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_pause()
      {

         synch_lock sl(m_pmutex);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_directsound output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.

         //m_mmr = directsound::translate(waveOutPause(m_hwaveout));

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_start(const imedia_position & position)
      {

         synch_lock sl(m_pmutex);

         if(m_estate == state_playing)
            return result_success;

         ASSERT(m_estate == state_opened || m_estate == state_stopped);

         m_estate = state_playing;

         for(index i = 0; i < wave_out_get_buffer()->GetBufferCount(); i++)
         {

            wave_out_free(i);

         }

         m_mmr = directsound::translate(m_psoundbuffer->Play(0,0,DSBPLAY_LOOPING));

         m_prunstepthread = new run_step_thread(this);


         return result_success;

      }


      ::multimedia::e_result wave_out::wave_out_restart()
      {

         synch_lock sl(m_pmutex);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_directsound output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = directsound::translate(m_psoundbuffer->Play(0,0,DSBPLAY_LOOPING));

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_playing;

         }

         return m_mmr;

      }


      /*imedia_time wave_out::GetPositionMillisForSynch()
      {
         int64_t dwMillis = GetPositionMillis();
         int64_t dwPosition = m_pprebuffer->m_position * 8;
         dwPosition /= m_pwaveformat->wBitsPerSample;
         dwPosition *= 1000;
         dwPosition /= m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return dwMillis + dwPosition - m_pprebuffer->m_pdecoder->audio_plugin_get_lost_millis(dwMillis + dwPosition) - (((int64_t) m_dwLostSampleCount) /  ((int64_t) m_pwaveformat->nSamplesPerSec));
         else
            return dwMillis + dwPosition - ((m_dwLostSampleCount) * 1000 / m_pwaveformat->nSamplesPerSec);
      }*/

      imedia_time wave_out::wave_out_get_position_millis()
      {

         synch_lock sl(m_pmutex);

         //::multimedia::e_result                mmr;

         return 0;

         //MMTIME                  mmt;

         //mmt.wType = TIME_MS;

         //if(m_hwaveout != NULL)
         //{

         //   mmr = directsound::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

         //   try
         //   {
         //      if (::multimedia::result_success != mmr)
         //      {
         //         TRACE( "waveOutGetPosition() returned %lu", (uint32_t)mmr);
         //         //      return MCIERR_DEVICE_NOT_READY;
         //         return 0;
         //      }
         //   }
         //   catch(...)
         //   {
         //      //return MCIERR_DEVICE_NOT_READY;
         //      return 0;
         //   }
         //   if(mmt.wType == TIME_BYTES)
         //   {
         //      int64_t i = mmt.u.cb;
         //      i *= 8 * 1000;
         //      i /= m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
         //      return i;

         //   }
         //   else
         //   {
         //      //*pTicks += mmt.u.ticks;
         //      return (uint32_t) mmt.u.ms;
         //   }
         //}
         //else
         //   return 0;


      }

      /*imedia_position wave_out::get_position_for_synch()
      {
         imedia_position position = get_position();
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->audio_plugin_get_lost_position_offset(position) - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         else
            return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      }*/

      imedia_position wave_out::wave_out_get_position()
      {

         synch_lock sl(m_pmutex);

         //::multimedia::e_result                mmr;

         //MMTIME                  mmt;

         //mmt.wType = TIME_BYTES;

         //if(m_hwaveout != NULL)
         //{
         //
         //   mmr = directsound::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

         //   try
         //   {

         //      if (::multimedia::result_success != mmr)
         //      {

         //         TRACE( "waveOutGetPosition() returned %lu", (uint32_t)mmr);

         //         return 0;

         //      }

         //   }
         //   catch(...)
         //   {
         //      return 0;
         //   }
         //   if(mmt.wType == TIME_MS)
         //   {
         //      imedia_position position = (uint32_t) mmt.u.ms;
         //      position *= m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
         //      position /= 8 * 1000;
         //      return position;
         //   }
         //   else
         //   {
         //      return (uint32_t) mmt.u.cb;
         //   }
         //}
         //else
         //   return 0;
         return 0;
      }


      void wave_out::wave_out_free(index iBuffer)
      {

         //wave_out_free(wave_hdr(iBuffer));

         ::multimedia::audio::wave_out::wave_out_free(iBuffer);

      }


      //void wave_out::wave_out_free(LPWAVEHDR lpwavehdr)
      //{


      //}

      void wave_out::wave_out_on_playback_end()
      {

         wave_out_stop();

         //if(m_pprebuffer->m_pstreameffectOut != NULL)
         //{
         //   ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
         //   m_pprebuffer->m_pstreameffectOut = NULL;
         //   delete peffect;
         //}

         m_eventStopped.SetEvent();

         m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);

      }



      WAVEFORMATEX * wave_out::wave_format()
      {

         directsound::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }

      //HWAVEOUT wave_out::wave_out_get_safe_HWAVEOUT()
      //{
      //
      //   if(this == NULL)
      //      return NULL;

      //   return m_hwaveout;

      //}

      void * wave_out::get_os_data()
      {
         //return m_hwaveout;
         return NULL;
      }

      //LPWAVEHDR wave_out::wave_hdr(int iBuffer)
      //{
      //   return ::multimedia::directsound::get_os_data(wave_out_get_buffer(), iBuffer);
      //}




      void wave_out::wave_out_run_step()
      {

         int iPlay =  -1;

         int r = WaitForMultipleObjects((DWORD) m_haEvent.get_count(), m_haEvent.get_data(),FALSE,INFINITE);

         if(r >= WAIT_OBJECT_0 && r < WAIT_OBJECT_0 + MAXIMUM_WAIT_OBJECTS)
         {

            iPlay = r - WAIT_OBJECT_0;

            ::ResetEvent(m_haEvent[iPlay]);


         }


         while(true)
         {

            int iNext = m_iBuffer + 1;

            if(iNext >= wave_out_get_buffer()->GetBufferCount())
               iNext = 0;

            if(iNext == iPlay)
               break;

            wave_out_out_buffer_done(iNext);

            m_iBuffer = iNext;

         }




      }


      wave_out::run_step_thread::run_step_thread(wave_out * pout):
         ::thread(pout->get_app())
      {
         m_pout = pout;
         begin();
      }


      void wave_out::run_step_thread::run()
      {

         while(thread_get_run() && m_pout->m_estate == wave_out::state_playing)
         {

            m_pout->wave_out_run_step();

         }

      }


   } // namespace audio_directsound


} // namespace multimedia






