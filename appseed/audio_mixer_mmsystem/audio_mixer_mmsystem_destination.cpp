#include "framework.h"


namespace audio_mixer
{

   destination::destination(sp(::ca2::application) papp) :
      ca2(papp),
      ::audio_mixer::source(papp)
   {

      m_mixersourcea.set_app(papp);
      m_pdevice = NULL;
      m_pdestination = this;

   }

   destination::destination(const destination & destination) :
      ::ca2::ca2(((class destination &) destination).get_app()),
      ::audio_mixer::source(((class destination &)destination).get_app())
   {

      m_mixersourcea.set_app(((class destination &)destination).get_app());

      operator = (destination);

   }

   destination::~destination()
   {
      //    Carray<::audio_mixer::source, ::audio_mixer::source &>::~Carray<::audio_mixer::source, ::audio_mixer::source &>();
      //CMixerLine::~CMixerLine();
   }

   void destination::operator delete(void *it)
   {
      ((destination *) it)->~destination();
      free(it);

   }

   ::multimedia::result destination::initialize_source_info()
   {
      ::audio_mixer::source *     lpSource;

      int32_t iConnections = (int32_t) m_mixerline.cConnections;

      m_mixersourcea.set_size(iConnections);

      for (int32_t i = 0; i < iConnections; i++)
      {
         lpSource = m_mixersourcea(i);
         lpSource->SetDestination(this);
         lpSource->mixerGetLineInfo(i, this);
      }
      return MMSYSERR_NOERROR;
   }

   ::multimedia::result destination::initialize_all_controls()
   {
      GetLineControls();

      ::audio_mixer::source_array & sourcea = m_mixersourcea;

      for(int32_t i = 0; i < sourcea.get_size(); i++)
      {
         sourcea[i].GetLineControls();
      }
      return true;
   }

   void destination::update_all_controls()
   {
      ::audio_mixer::source::update_all_controls();
      ::audio_mixer::source_array & sourcea = m_mixersourcea;

      for(int32_t i = 0; i < sourcea.get_size(); i++)
      {
         sourcea[i].update_all_controls();
      }
   }


   destination & destination::operator = (const destination & destination)
   {

      m_pdevice = destination.m_pdevice;

      return *this;

   }


   uint32_t destination::get_component_type()
   {

      return m_mixerline.dwComponentType;

   }


   ::audio_mixer::device * destination::get_device()
   {

      return m_pdevice;

   }


   void destination::set_device(::audio_mixer::device * pdevice)
   {

      m_pdevice = pdevice;

   }


   ::audio_mixer::source_array & destination::get_source_info()
   {

      return m_mixersourcea;

   }


} // namespace audio_mixer





