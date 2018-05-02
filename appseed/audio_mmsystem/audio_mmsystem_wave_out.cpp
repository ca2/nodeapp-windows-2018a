#include "framework.h"


namespace multimedia
{


   namespace audio_mmsystem
   {


      void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
      {

         if (uMsg == WOM_DONE)
         {

            LPWAVEHDR lpwavehdr = (LPWAVEHDR) dwParam1;

            ::PostThreadMessage(dwInstance, wave_out::message_free, lpwavehdr->dwUser, 0);

         }

      }


      wave_out::wave_out(::aura::application * papp) :
         ::object(papp),
         ::thread(papp),
         wave_base(papp),
         ::multimedia::audio::wave_out(papp),
         m_evFree(papp),
         m_mutexFree(papp)
      {

         m_pthreadFree = NULL;

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_hwaveout           = NULL;
         m_iBufferedCount     = 0;
         m_peffect            = NULL;

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


      void wave_out::term_thread()
      {

         ::multimedia::audio::wave_out::term_thread();

         ::thread::term_thread();

      }


      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample,::multimedia::audio::e_purpose epurpose)
      {

         synch_lock sl(m_pmutex);

         if (m_hwaveout != NULL && m_estate != state_initial)
         {

            return ::multimedia::result_success;

         }

         m_imediatime = 0;

         MMRESULT mmresult = MMSYSERR_NOERROR;

         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
         ASSERT(m_hwaveout == NULL);
         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag        = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = sizeof(m_waveformatex);

         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         if (m_pthreadFree == NULL)
         {

            m_pthreadFree = fork([this]()
            {

               ::get_thread()->set_thread_priority(::multithreading::priority_time_critical);

               MESSAGE msg;

               while (::GetMessage(&msg, NULL, 0, 0))
               {

                  if (msg.message == message_free)
                  {

                     if (m_pprebuffer->m_bPlay)
                     {

                        wave_out_buffer_ready(msg.wParam);

                     }
                     else
                     {

                        m_iBufferedCount--;

                        output_debug_string("message_free and not playing");

                        if (m_iBufferedCount <= 0)
                        {

                           wave_out_on_playback_end();

                        }

                     }

                  }

               }

               output_debug_string("quit");

            });

         }

         try
         {

            mmresult = waveOutOpen(&m_hwaveout, audiowave->m_uiWaveInDevice, wave_format(), (DWORD_PTR) &waveOutProc, (DWORD_PTR) m_pthreadFree->get_os_int(), CALLBACK_FUNCTION);

            if (mmresult == MMSYSERR_NOERROR)
            {

               TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutOpen: Success!!");

            }
            else
            {

               TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutOpen: ERROR %d!!", mmresult);

            }

            mmr = mmsystem::translate(mmresult);

            if (mmr == ::multimedia::result_success)
            {

               goto Opened;

            }

         }
         catch(const ::exception::exception &)
         {

            return ::multimedia::result_error;

         }
         catch(...)
         {

            return ::multimedia::result_error;

         }

         if(mmr != ::multimedia::result_success)
         {

            TRACE(::multimedia::strerror(mmr));

            return mmr;

         }

Opened:

         int iBufferCount;
         int iBufferSampleCount;

         if(epurpose == ::multimedia::audio::purpose_playback)
         {

            iBufferSampleCount = uiSamplesPerSec / 20;

            iBufferCount = 16;

         }
         else if(epurpose == ::multimedia::audio::purpose_playground)
         {

            iBufferSampleCount = uiSamplesPerSec / 40;

            iBufferCount = 4;

         }
         else
         {

            iBufferSampleCount = uiSamplesPerSec / 40;

            iBufferCount = 6;

         }

         int iBufferSize = iBufferSampleCount * uiBitsPerSample * uiChannelCount / 8;

         wave_out_get_buffer()->PCMOutOpen(this, iBufferSize, iBufferCount,128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, iBufferCount, iBufferSampleCount);

         index i;

         auto iSize = wave_out_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            mmresult = waveOutPrepareHeader(m_hwaveout, mmsystem::create_new_WAVEHDR(wave_out_get_buffer(), i), sizeof(WAVEHDR));

            if (mmresult == MMSYSERR_NOERROR)
            {

               TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutPrepareHeader: Success!!");

            }
            else
            {

               TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutPrepareHeader: ERROR %d!!", mmresult);

            }

            mmr = mmsystem::translate(mmresult);

            if (mmr != ::multimedia::result_success)
            {

               MMRESULT mmresult2;

               while (i >= 1)
               {

                  i--;

                  mmresult2 = waveOutUnprepareHeader(m_hwaveout, wave_hdr(i), sizeof(WAVEHDR));

                  if (mmresult2 == MMSYSERR_NOERROR)
                  {

                     TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutUnprepareHeader: Cascade Success");

                  }
                  else
                  {

                     TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutUnprepareHeader: Cascade ERROR %d!!", mmresult);

                  }

               }

               MMRESULT mmresult3;

               while (i >= 1)
               {

                  i--;

                  mmresult3 = waveOutClose(m_hwaveout);

                  if (mmresult3 == MMSYSERR_NOERROR)
                  {

                     TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutClose: Cascade Success");

                  }
                  else
                  {

                     TRACE("multimedia::audio_mmsystem::wave_out::wave_out_open_ex waveOutClose: Cascade ERROR %d!!", mmresult);

                  }

               }


               TRACE("ERROR !! Failed to prepare output device buffers");

               return mmr;

            }

         }

         m_estate = state_opened;

         m_epurpose = epurpose;

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

         ::multimedia::e_result mmr;

         index i;

         auto iSize =  wave_out_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            if(::multimedia::result_success != (mmr = mmsystem::translate(waveOutUnprepareHeader(m_hwaveout, wave_hdr(i), sizeof(WAVEHDR)))))
            {
               TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", mmr);
            }

            delete wave_hdr(i);

         }

         mmr = mmsystem::translate(waveOutClose(m_hwaveout));

         m_hwaveout = NULL;

         ::multimedia::audio::wave_out::wave_out_close();

         return ::multimedia::result_success;

      }


      void wave_out::OnMultimediaOpen(::message::message * pobj)
      {

         UNREFERENCED_PARAMETER(pobj);

      }


      void wave_out::OnMultimediaDone(::message::message * pobj)
      {

         //synch_lock sl(m_pmutex);

         try
         {

            m_imediatime = device_wave_out_get_position_millis();

            m_imediaposition = device_wave_out_get_position();

            SCAST_PTR(::message::base, pbase, pobj);

            LPWAVEHDR lpwavehdr = (LPWAVEHDR)pbase->m_lparam.m_lparam;

            if (lpwavehdr != NULL)
            {

               wave_out_out_buffer_done((int32_t)lpwavehdr->dwUser);

            }

         }
         catch (...)
         {

         }

      }

      void wave_out::wave_out_out_buffer_done(index iBuffer)
      {

         m_iBufferedCount--;

         wave_out_free(iBuffer);

      }


      void wave_out::OnMultimediaClose(::message::message * pobj)
      {

         UNREFERENCED_PARAMETER(pobj);

      }


      void wave_out::wave_out_buffer_ready(index iBuffer)
      {

         return wave_out_buffer_ready(wave_hdr(iBuffer));

      }



      void wave_out::wave_out_launch_buffers()
      {

         m_iBufferedCount = wave_out_get_buffer()->GetBufferCount();

         for (int32_t dw = 0; dw < wave_out_get_buffer()->GetBufferCount(); dw++)
         {

            ::PostThreadMessage(m_pthreadFree->get_os_int(), wave_out::message_free, dw, 0);

         }

      }

      void wave_out::wave_out_buffer_ready(LPWAVEHDR lpwavehdr)
      {

         synch_lock sl(m_pmutex);

         if(wave_out_get_state() != state_playing)
         {
            TRACE("ERROR wave_out::BufferReady while wave_out_get_state() != state_playing");
            return;
         }

         m_pprebuffer->read_buffer(lpwavehdr->lpData, m_pprebuffer->get_buffer_size(), lpwavehdr->dwUser);

         ::multimedia::e_result mmr = mmsystem::translate(waveOutWrite(m_hwaveout, lpwavehdr, sizeof(WAVEHDR)));

         //VERIFY(::multimedia::result_success == mmr);

         if(mmr != ::multimedia::result_success)
         {

            m_iBufferedCount--;

            throw resource_exception(get_app());

         }

      }


      ::multimedia::e_result wave_out::wave_out_stop()
      {

         synch_lock sl(m_pmutex);

         if(m_estate != state_playing && m_estate != state_paused)
            return ::multimedia::result_error;

         m_eventStopped.ResetEvent();

         m_pprebuffer->stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_mmsystem output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = mmsystem::translate(waveOutReset(m_hwaveout));

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_opened;

         }

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_pause()
      {

         single_lock sLock(m_pmutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_mmsystem output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.

         m_mmr = mmsystem::translate(waveOutPause(m_hwaveout));

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_restart()
      {

         synch_lock sl(m_pmutex);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_mmsystem output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = mmsystem::translate(waveOutRestart(m_hwaveout));

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

         return (m_positionStart * 1000 / m_pwaveformat->nSamplesPerSec)
                + device_wave_out_get_position_millis();

      }


      imedia_time wave_out::device_wave_out_get_position_millis()
      {

         synch_lock sl(m_pmutex);

         ::multimedia::e_result                mmr;

         MMTIME mmt = {};

         mmt.wType = TIME_BYTES;

         if(m_hwaveout != NULL)
         {

            mmr = mmsystem::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

            try
            {

               if (::multimedia::result_success != mmr)
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

               i /= m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;

               return i;

            }
            else
            {

               //*pTicks += mmt.u.ticks;

               return (uint32_t) mmt.u.ms;

            }

         }
         else
         {

            return 0;

         }

      }


      imedia_position wave_out::wave_out_get_position()
      {

         return m_positionStart + device_wave_out_get_position();

      }


      imedia_position wave_out::device_wave_out_get_position()
      {

         synch_lock sl(m_pmutex);

         ::multimedia::e_result                mmr;

         MMTIME                  mmt = {};

         mmt.wType = TIME_BYTES;

         if(m_hwaveout != NULL)
         {

            mmr = mmsystem::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

            try
            {

               if (::multimedia::result_success != mmr)
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

               imedia_position position = (uint32_t) mmt.u.ms;

               position *= m_pwaveformat->nSamplesPerSec;

               position /= 8 * 1000;

               return position;

            }
            else if (mmt.wType == TIME_BYTES)
            {

               return (mmt.u.cb * 8) / (m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels);

            }
            else
            {

               return 0;

            }

         }
         else
         {

            return 0;

         }

      }


      void wave_out::wave_out_free(int iBuffer)
      {


      }


      void wave_out::wave_out_free(LPWAVEHDR lpwavehdr)
      {


      }


      void wave_out::wave_out_on_playback_end()
      {

         wave_out::e_state estate = wave_out_get_state();

         wave_out_stop();

         if(m_pprebuffer->m_pstreameffectOut != NULL)
         {

            ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;

            m_pprebuffer->m_pstreameffectOut = NULL;

            delete peffect;

         }

         m_eventStopped.SetEvent();

         m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);

      }


      WAVEFORMATEX * wave_out::wave_format()
      {

         mmsystem::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }


      HWAVEOUT wave_out::wave_out_get_safe_HWAVEOUT()
      {

         if (this == NULL)
         {

            return NULL;

         }

         return m_hwaveout;

      }


      void * wave_out::get_os_data()
      {

         return m_hwaveout;

      }


      LPWAVEHDR wave_out::wave_hdr(index iBuffer)
      {

         return ::multimedia::mmsystem::get_os_data(wave_out_get_buffer(), iBuffer);

      }


   } // namespace audio_mmsystem


} // namespace multimedia



