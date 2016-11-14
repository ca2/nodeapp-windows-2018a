#pragma once

class millis_timer
{

public:


   HANDLE hTimerQueue;
   HANDLE gDoneEvent;

   millis_timer()
   {

      // Use an event object to track the TimerRoutine execution
      gDoneEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
      if(NULL == gDoneEvent)
      {
         throw - 1;
      }
      // Create the timer queue.
      hTimerQueue = CreateTimerQueue();
      if(NULL == hTimerQueue)
      {
         throw - 1;
      }
   }

   ~millis_timer()
   {

      CloseHandle(gDoneEvent);

      // Delete all timers in the timer queue.
      DeleteTimerQueue(hTimerQueue);


   }


   static VOID CALLBACK TimerRoutine(PVOID lpParam,BOOLEAN TimerOrWaitFired)
   {
      millis_timer * ptimer = (millis_timer *)lpParam;

      SetEvent(ptimer->gDoneEvent);

      ptimer->on_millis_timer_step();
   }

   bool wait(int millis)
   {

      ::ResetEvent(gDoneEvent);

      HANDLE hTimer = NULL;

      // Set a timer to call the timer routine in 10 seconds.
      if(!CreateTimerQueueTimer(&hTimer,hTimerQueue,(WAITORTIMERCALLBACK)TimerRoutine,this,millis,0,0))
      {
         return false;
      }

      if(WaitForSingleObject(gDoneEvent,INFINITE) != WAIT_OBJECT_0)
         return false;

      //      DeleteTimerQueue(hTimer);

      return true;
   }
   bool timer(int millis)
   {

      ::ResetEvent(gDoneEvent);

      HANDLE hTimer = NULL;

      // Set a timer to call the timer routine in 10 seconds.
      if(!CreateTimerQueueTimer(&hTimer,hTimerQueue,(WAITORTIMERCALLBACK)TimerRoutine,this,millis,millis,0))
      {
         return false;
      }

      if(WaitForSingleObject(gDoneEvent,INFINITE) != WAIT_OBJECT_0)
         return false;

      //      DeleteTimerQueue(hTimer);

      return true;
   }

   virtual void on_millis_timer_step()
   {
   }

};

namespace multimedia
{


   namespace audio_xaudio7
   {


      class  CLASS_DECL_AUDIO_XAUDIO7 wave_out :
         virtual public ::multimedia::audio::wave_out,
         virtual public IXAudio2VoiceCallback
      {
      public:


            //HANDLE streamEndEventHandle;
            //VoiceCallback(): streamEndEventHandle(CreateEvent(NULL,FALSE,FALSE,NULL)){}
            //~VoiceCallback()
            //{
               //CloseHandle(streamEndEventHandle);
            //}

            // Called when the voice has just finished playing a contiguous audio stream.
            STDMETHOD_(void,OnStreamEnd())
            {
               
            }

            // Unused methods in this application
            STDMETHOD_(void,OnVoiceProcessingPassEnd()) { }
            STDMETHOD_(void,OnVoiceProcessingPassStart(UINT32 SamplesRequired)) {    }
            STDMETHOD_(void,OnBufferEnd(void * pBufferContext))  ;
            STDMETHOD_(void,OnBufferStart(void * pBufferContext)) {    }
            STDMETHOD_(void,OnLoopEnd(void * pBufferContext)) {    }
            STDMETHOD_(void,OnVoiceError(void * pBufferContext,HRESULT Error)) { }
         class run_step_thread :
            virtual public ::thread
         {
         public:

            wave_out * m_pout;

            run_step_thread(wave_out * pout);

            virtual int32_t run();

         };

         IXAudio2 *                       m_pxaudio;
         IXAudio2MasteringVoice *         m_pvoice;
         IXAudio2SourceVoice *            m_psourcevoice;

         run_step_thread *                m_prunstepthread;

         int                              m_iBuffer;

         WAVEFORMATEX                     m_waveformatex;



         wave_out(sp(::axis::application) papp);
         virtual ~wave_out();

         
         ::multimedia::e_result wave_out_start(const imedia_position & position);
         virtual bool  on_run_step();
         void install_message_handling(::message::dispatch * pinterface);

         virtual imedia_time wave_out_get_position_millis();
         imedia_position wave_out_get_position();
         virtual void wave_out_buffer_ready(int iBuffer);
         //virtual void wave_out_buffer_ready(LPWAVEHDR lpwavehdr);

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample);
         virtual ::multimedia::e_result wave_out_stop();
         virtual ::multimedia::e_result wave_out_close();
         virtual ::multimedia::e_result wave_out_pause();
         virtual ::multimedia::e_result wave_out_restart();
         virtual void * get_os_data();
         //HWAVEOUT wave_out_get_safe_HWAVEOUT();

         virtual void wave_out_on_playback_end();
         virtual void wave_out_free(int iBuffer);
         //virtual void wave_out_free(LPWAVEHDR lpwavehdr);

         virtual bool initialize_thread() override;
         virtual int32_t exit_instance();

         virtual int32_t run();

         //DECL_GEN_SIGNAL(OnMultimediaOpen);
         //DECL_GEN_SIGNAL(OnMultimediaDone);
         //DECL_GEN_SIGNAL(OnMultimediaClose);

//         virtual void wave_out_out_buffer_done(int iBuffer);
  //       virtual void wave_out_out_buffer_done(LPWAVEHDR lpwavehdr);

         WAVEFORMATEX * wave_format();
         //LPWAVEHDR wave_hdr(int iBuffer);
         virtual void wave_out_run_step();
      };


   } // namespace audio_xaudio7


} // namespace multimedia



