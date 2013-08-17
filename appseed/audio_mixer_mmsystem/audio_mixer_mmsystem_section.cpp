#include "framework.h"


namespace audio_mixer
{


   section::section(sp(::ca2::application) papp) :
      ca2(papp),
      ::ca2::section(papp)
   {

   }


   section::~section()
   {

   }

   
   bool section::initialize1()
   {

      System.factory().creatable_small < ::audio_mixer::destination > ();
      System.factory().creatable_small < ::audio_mixer::source > ();
      System.factory().creatable_small < ::audio_mixer::control > ();

      return true;
      
   }


   bool section::finalize() 
   {

      DestroyWindow();


      return true;

   }



   /*::multimedia::result section::CreateNewDevice(uint32_t uiMixerID)
   {
   ::multimedia::result mmrc;
   if(MMSYSERR_NOERROR != (mmrc = m_pmixer->set_new_device(
   uiMixerID,
   (uint32_t) m_oswindow_,
   0,
   CALLBACK_WINDOW)))
   {
   System.simple_message_box("Could not open audio_mixer device!");
   PostMessage(WM_CLOSE);
   return mmrc;
   }
   m_pmixer->m_pdevice->initialize_capabilities();
   return mmrc;
   }

   ::multimedia::result section::CreateNewDestination(uint32_t dwComponentType)
   {
   ::multimedia::result mmrc;
   ::audio_mixer::destination * pdestination = NULL;

   if(MMSYSERR_NOERROR != (mmrc =  m_pmixer->m_pdevice->get_destination(
   dwComponentType,
   &pdestination)))
   {
   System.simple_message_box("Could not open default audio_mixer destination!");
   PostMessage(WM_CLOSE);
   return mmrc;
   }

   m_destinationpa.add(pdestination);

   pdestination->initialize_source_info();

   pdestination->initialize_all_controls();

   m_pmixer->m_pdevice->map_lines();
   m_pmixer->m_pdevice->map_controls();

   pdestination->update_all_controls();

   return mmrc;
   }*/


   bool section::initialize()
   {
      //   m_pmixerthread = (::audio_mixer::thread *) __begin_thread(System.type_info < ::audio_mixer::thread > ());
      //   if(m_pmixerthread == NULL)
      //      return false;
      //   return true«
      return true;
   }


} // namespace audio_mixer


