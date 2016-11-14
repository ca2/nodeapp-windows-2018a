#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {


      class  CLASS_DECL_AUDIO_MMSYSTEM wave_out :
         virtual public ::multimedia::audio::wave_out
      {
      public:

         class  CLASS_DECL_AUDIO_MMSYSTEM helper_thread :
            virtual public ::thread
         {
         public:

            wave_out * m_pwaveout;


            helper_thread(::aura::application * papp);
            virtual ~helper_thread();
            void install_message_handling(::message::dispatch * pinterface);
            DECL_GEN_SIGNAL(OnMultimediaOpen);
            DECL_GEN_SIGNAL(OnMultimediaDone);
            DECL_GEN_SIGNAL(OnMultimediaClose);


         };


         HWAVEOUT                         m_hwaveout;
         WAVEFORMATEX                     m_waveformatex;
         imedia_time                      m_imediatime;
         imedia_time                      m_imediaposition;
         helper_thread *                         m_phelperthread;

         wave_out(::aura::application * papp);
         virtual ~wave_out();


         void install_message_handling(::message::dispatch * pinterface);

         virtual imedia_time device_wave_out_get_position_millis();
         virtual imedia_time wave_out_get_position_millis();
         virtual imedia_position device_wave_out_get_position();
         virtual imedia_position wave_out_get_position();
         virtual void wave_out_buffer_ready(int iBuffer) override;
         virtual void wave_out_buffer_ready(LPWAVEHDR lpwavehdr);

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount) override;
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose) override;
         virtual ::multimedia::e_result wave_out_stop() override;
         virtual ::multimedia::e_result wave_out_close() override;
         virtual ::multimedia::e_result wave_out_pause() override;
         virtual ::multimedia::e_result wave_out_restart() override;
         virtual void * get_os_data();
         HWAVEOUT wave_out_get_safe_HWAVEOUT();

         virtual void wave_out_on_playback_end();
         virtual void wave_out_free(int iBuffer);
         virtual void wave_out_free(LPWAVEHDR lpwavehdr);

         virtual bool initialize_thread() override;
         virtual int32_t exit_instance();

         DECL_GEN_SIGNAL(OnMultimediaOpen);
         DECL_GEN_SIGNAL(OnMultimediaDone);
         DECL_GEN_SIGNAL(OnMultimediaClose);

//         virtual void wave_out_out_buffer_done(int iBuffer);
  //       virtual void wave_out_out_buffer_done(LPWAVEHDR lpwavehdr);

         WAVEFORMATEX * wave_format();
         LPWAVEHDR wave_hdr(int iBuffer);

      };


   } // namespace audio_mmsystem


} // namespace multimedia



