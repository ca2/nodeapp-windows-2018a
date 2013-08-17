#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {

      class  CLASS_DECL_AUDIO_MMSYSTEM wave_out :
         virtual public ::multimedia::audio::wave_out
      {
      public:

         array < LPWAVEHDR, LPWAVEHDR > m_lphdraFree;



         wave_out(sp(::ca2::application) papp);
         virtual ~wave_out();


         static wave_out * BeginThread(sp(::ca2::application) papp);


         virtual ::multimedia::audio_decode::decoder * wave_out_set_decoder(::multimedia::audio_decode::decoder * pdecoder);

         void install_message_handling(::ca2::message::dispatch * pinterface);

         virtual int32_t wave_out_get_buffered_buffer_count();
         virtual imedia::time wave_out_get_position_millis();
         virtual imedia::time wave_out_get_position_millis_for_synch();
         imedia::position wave_out_get_position();
         imedia::position wave_out_get_position_for_synch();
         void wave_out_attach_end_event(event * pevent);
         e_state wave_out_get_state();
         virtual void wave_out_buffer_ready(int iBuffer);
         virtual void wave_out_buffer_ready(LPWAVEHDR lpwavehdr);

         virtual ::multimedia::result wave_out_open(::ca2::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
         virtual ::multimedia::result wave_out_open_ex(::ca2::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample);
         virtual ::multimedia::result wave_out_start(const imedia::position & position);
         virtual bool wave_out_stop();
         virtual ::multimedia::result wave_out_close();
         virtual void wave_out_set_stream_effect(::iaudio::wave_stream_effect * peffect);
         virtual bool wave_out_pause();
         virtual bool wave_out_restart();
         //virtual ::multimedia::result wave_out_get_last_mm_result();
         virtual int32_t wave_out_get_buffer_size();
         virtual void * wave_out_get_buffer(int32_t iBuffer);
         virtual void * get_os_data();
         virtual HWAVEOUT GetSafeHandle();
         //virtual void wave_out_post_close();

         virtual void wave_out_on_playback_end();
         virtual void wave_out_free(int iBuffer);
         virtual void wave_out_free(LPWAVEHDR lpwavehdr);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();

         DECL_GEN_SIGNAL(OnMultimediaOpen)
         DECL_GEN_SIGNAL(OnMultimediaDone)
         DECL_GEN_SIGNAL(OnMultimediaClose)
//         DECL_GEN_SIGNAL(OnBufferReady)
//         DECL_GEN_SIGNAL(OnClose)

         virtual void wave_out_out_buffer_done(int iBuffer);
         virtual void wave_out_out_buffer_done(LPWAVEHDR lpwavehdr);
         virtual void wave_out_on_pre_buffer_done(int32_t iId);

      };


   } // namespace audio_mmsystem


} // namespace multimedia



