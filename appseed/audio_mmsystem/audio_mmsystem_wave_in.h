#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {


      class  CLASS_DECL_AUDIO_MMSYSTEM wave_in :
         virtual public ::multimedia::audio::wave_in
      {
      public:




         wave_in(sp(::ca2::application) papp);
         virtual ~wave_in();


         virtual bool wave_in_initialize_encoder();

         virtual ::multimedia::result wave_in_add_buffer(int32_t iBuffer);
         virtual ::multimedia::result add_buffer(LPWAVEHDR lpwavehdr);
         virtual void wave_in_remove_listener(wave_in_listener * plistener);
         virtual void wave_in_add_listener(wave_in_listener * plistener);

         virtual mutex * wave_in_get_mutex();
         virtual bool wave_in_is_opened();
         virtual bool wave_in_is_recording();
         virtual HWAVEIN wave_in_get_safe_HWAVEIN();
         virtual void * get_os_data();
         virtual uint32_t wave_in_get_analysis_millis();
         virtual bool wave_in_is_resetting();

         virtual uint32_t wave_in_get_state();
         virtual ::multimedia::audio::wave_format & wave_in_get_format();


         virtual ::multimedia::audio::wave_buffer & wave_in_get_buffer();

         // Operations
         ::multimedia::result wave_in_open(int32_t iBufferCount, int32_t iBufferSampleCount);
         ::multimedia::result wave_in_close();
         ::multimedia::result wave_in_stop();
         ::multimedia::result wave_in_start();
         ::multimedia::result wave_in_reset();

         // Event handlers
         virtual void translate_wave_in_message(::ca2::signal_object * pobj);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();
         virtual void pre_translate_message(::ca2::signal_object * pobj);

         static void CALLBACK wave_in_proc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

      };


   } // namespace audio_mmsystem


} // namespace multimedia




