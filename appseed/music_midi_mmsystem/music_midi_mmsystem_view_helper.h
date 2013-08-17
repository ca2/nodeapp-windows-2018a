#pragma once


namespace music
{

   namespace midi_mmsystem
   {


      class document_helper;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM view_helper   :
         virtual public ::ca2::object
      {
      public:

         


         view_helper();
         virtual ~view_helper();


         sp(document_helper) HelperGetMidiDocHelper();

      };


   } // namespace midi_mmsystem


} // namespace music



