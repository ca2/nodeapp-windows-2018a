#pragma once


namespace multimedia
{


   namespace directsound
   {


      CLASS_DECL_MULTIMEDIA_DIRECTSOUND void translate(WAVEFORMATEX & formatex, ::multimedia::audio::wave_format * pwaveformat);
//      CLASS_DECL_MULTIMEDIA_DIRECTSOUND void translate(WAVEHDR & wavehdr, ::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
  //    CLASS_DECL_MULTIMEDIA_DIRECTSOUND LPWAVEHDR create_new_WAVEHDR(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);
    //  CLASS_DECL_MULTIMEDIA_DIRECTSOUND LPWAVEHDR get_os_data(::multimedia::audio::wave_buffer * pwavebuffer, int iIndex);


      CLASS_DECL_MULTIMEDIA_DIRECTSOUND ::multimedia::e_result translate(HRESULT mmr);


   } // namespace directsound


} // namespace multimedia





