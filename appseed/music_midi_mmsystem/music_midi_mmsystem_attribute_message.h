#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM attribute_message :
         public ::ca2::signal_object
      {
      public:


         e_attribute m_eattribute;


         attribute_message(::ca2::signal * psignal) : ::ca2::signal_object(psignal) {}


      };



   } // namespace midi_mmsystem


} // namespace music







