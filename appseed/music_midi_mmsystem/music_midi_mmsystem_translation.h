#pragma once


#define MIDIMSG(status,channel,data1,data2) ( (uint32_t)((status<<4) | channel | (data1<<8) | (data2<<16)) )


namespace music
{


   namespace midi
   {

      
      namespace mmsystem
      {


         CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM ::multimedia::e_result translate_mmr(MMRESULT mmr);


      } // namespace mmsystem


   } // namespace midi


} // namespace music