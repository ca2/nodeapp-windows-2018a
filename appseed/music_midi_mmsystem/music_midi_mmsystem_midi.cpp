#include "framework.h"


namespace music
{


   namespace midi
   {


      midi::midi(sp(::ca2::application) papp) :
         ca2(papp)
      {

         m_uiMidiOutDevice = MIDI_MAPPER; 

      }


      midi::~midi()
      {

      }


      bool midi::Initialize()
      {

         System.factory().creatable_small < ::karaoke::lyric_view_line_template >();

         return true;

      }



      void midi::SetMidiOutDevice(uint32_t uiDevice)
      {
         m_uiMidiOutDevice = uiDevice;
         m_listenerptra.send_attribute_change(::music::midi::attribute_midi_out_device);
      }

      uint32_t midi::GetMidiOutDevice()
      {
         return m_uiMidiOutDevice;
      }

      void midi::AddListener(midi_listener *plistener)
      {
         m_listenerptra.install_handlers(plistener);
      }

      void midi::RemoveListener(midi_listener *plistener)
      {
         m_listenerptra.remove(plistener);
      }


      bool midi::IsSoftKaraokeFile(const char * lpszPathName)
      {
         ::music::midi::sequence midiSeq(get_app());
         midiSeq.OpenFile(lpszPathName, ::music::midi::OpenForParsingLevel2);
         return midiSeq.GetFile().IsSoftKaraokeFile();
      }

      bool midi::IsXFFile(const char * lpcszPathName)
      {
         ::music::midi::sequence midiSeq(get_app());
         midiSeq.OpenFile(lpcszPathName, ::music::midi::OpenForParsing);
         return midiSeq.GetFile().IsXFFile();
      }


   } // namespace midi


} // namespace music









