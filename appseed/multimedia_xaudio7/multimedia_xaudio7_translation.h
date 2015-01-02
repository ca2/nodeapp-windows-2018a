#pragma once


namespace multimedia
{


   namespace xaudio7
   {


      CLASS_DECL_MULTIMEDIA_XAUDIO7 void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
//      CLASS_DECL_MULTIMEDIA_XAUDIO7 void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
  //    CLASS_DECL_MULTIMEDIA_XAUDIO7 LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
    //  CLASS_DECL_MULTIMEDIA_XAUDIO7 LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);


      CLASS_DECL_MULTIMEDIA_XAUDIO7 ::multimedia::e_result translate(HRESULT mmr);


   } // namespace xaudio7


} // namespace multimedia





