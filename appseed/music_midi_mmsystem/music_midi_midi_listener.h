#pragma once


namespace music
{


   namespace midi
   {


      class midi;



      class CLASS_DECL_VERIWELL_MULTIMEDIA midi_listener :
         virtual public ::ca2::signalizable
      {
      public:


         sp(::music::midi::midi)       m_pmidicentral;





         midi_listener();
         midi_listener(sp(::music::midi::midi) pcentral);
         virtual ~midi_listener();



         void SetMidiCentral(sp(::music::midi::midi) pcentral);


         DECL_GEN_VSIGNAL(on_attribute_change)


      };



   } // namespace midi


} // namespace music






