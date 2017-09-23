#pragma once


namespace multimedia
{


   namespace mmsystem
   {


      CLASS_DECL_MULTIMEDIA_MMSYSTEM void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
      CLASS_DECL_MULTIMEDIA_MMSYSTEM void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, index iIndex);
      CLASS_DECL_MULTIMEDIA_MMSYSTEM LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, index iIndex);
      CLASS_DECL_MULTIMEDIA_MMSYSTEM LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, index iIndex);


      CLASS_DECL_MULTIMEDIA_MMSYSTEM ::multimedia::e_result translate(MMRESULT mmr);


   } // namespace mmsystem


} // namespace multimedia





