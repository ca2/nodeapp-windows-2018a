#include "framework.h"


namespace music
{


   namespace midi
   {


      midi_listener::midi_listener()
      {
         m_pmidicentral = NULL;
      }

      midi_listener::midi_listener(sp(midi) pcentral)
      {
         m_pmidicentral = NULL;
         SetMidiCentral(pcentral);
      }

      midi_listener::~midi_listener()
      {
         m_pmidicentral->RemoveListener(this);
      }

      void midi_listener::on_attribute_change(::ca2::signal_object * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }

      void midi_listener::SetMidiCentral(sp(midi)pcentral)
      {
         if(m_pmidicentral != NULL)
         {
            m_pmidicentral->RemoveListener(this);
         }
         m_pmidicentral = pcentral;
         pcentral->AddListener(this);
      }


   } // namespace midi


} // namespace music









