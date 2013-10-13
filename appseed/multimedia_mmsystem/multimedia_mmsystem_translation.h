#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {


      void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
      void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
      LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
      LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);


      ::multimedia::e_result translate(MMRESULT mmr);


   } // namespace audio_mmsystem


} // namespace multimedia





