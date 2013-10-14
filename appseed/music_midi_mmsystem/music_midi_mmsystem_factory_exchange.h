#pragma once


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM factory_exchange :
            virtual public ::multimedia::mmsystem::factory_exchange
         {
         public:


            factory_exchange(sp(base_application) papp);
            virtual ~factory_exchange();

         };


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music






