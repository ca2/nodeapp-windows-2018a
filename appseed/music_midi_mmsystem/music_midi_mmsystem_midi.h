#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi :
         virtual public ::ca2::object
      {
      public:


         midi_listener_set             m_listenerptra;
         uint32_t                      m_uiMidiOutDevice;


         midi(sp(::ca2::application) papp);
         virtual ~midi();


         void RemoveListener(midi_listener * plistener);
         void AddListener(midi_listener * plistener);
         uint32_t GetMidiOutDevice();
         void SetMidiOutDevice(uint32_t uiDevice);
         bool Initialize();


         bool IsSoftKaraokeFile(const char * lpszPathName);
         bool IsXFFile(const char * lpcszPathName);


      };



   } // namespace midi_mmsystem


} // namespace music




