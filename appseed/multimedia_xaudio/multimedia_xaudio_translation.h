#pragma once


namespace multimedia
{


   namespace xaudio
   {


      CLASS_DECL_MULTIMEDIA_XAUDIO void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
//      CLASS_DECL_MULTIMEDIA_XAUDIO void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
  //    CLASS_DECL_MULTIMEDIA_XAUDIO LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
    //  CLASS_DECL_MULTIMEDIA_XAUDIO LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);


      CLASS_DECL_MULTIMEDIA_XAUDIO ::multimedia::e_result translate(HRESULT mmr);


   } // namespace xaudio


} // namespace multimedia





