#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {


      class  CLASS_DECL_AUDIO_MMSYSTEM wave_in :
         virtual public ::multimedia::audio::wave_in
      {
      public:


         HWAVEIN              m_hwavein;

         WAVEFORMATEX         m_waveformatex;


         wave_in(::aura::application * papp);
         virtual ~wave_in();


         virtual bool wave_in_initialize_encoder();

         virtual ::multimedia::e_result wave_in_add_buffer(int32_t iBuffer);
         virtual ::multimedia::e_result wave_in_add_buffer(LPWAVEHDR lpwavehdr);

         HWAVEIN wave_in_get_safe_HWAVEIN();
         virtual void * get_os_data();

         ::multimedia::e_result wave_in_open(int32_t iBufferCount, int32_t iBufferSampleCount);
         ::multimedia::e_result wave_in_close();
         ::multimedia::e_result wave_in_stop();
         ::multimedia::e_result wave_in_start();
         ::multimedia::e_result wave_in_reset();

         virtual void translate_wave_in_message(::message::message * pobj);

         virtual bool init_thread() override;
         virtual void term_thread() override;
         virtual void pre_translate_message(::message::message * pobj);

         static void CALLBACK wave_in_proc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

         WAVEFORMATEX * wave_format();
         LPWAVEHDR wave_hdr(int iBuffer);


      };


   } // namespace audio_mmsystem


} // namespace multimedia




