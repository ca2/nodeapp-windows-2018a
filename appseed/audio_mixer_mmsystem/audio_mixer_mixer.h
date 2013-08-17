#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA audio_mixer :
      virtual public ::ca2::object
   {
   public:


      ::audio_mixer::device          * m_pdevice;
      ::audio_mixer::window            m_window;
      ::audio_mixer::callback        * m_pcallback;


      audio_mixer(sp(::ca2::application) papp);
      virtual ~audio_mixer();

      

      ::audio_mixer::callback * get_callback();
      void set_callback(::audio_mixer::callback * pcallback);


      ::multimedia::result set_new_device(uint32_t uiMixerId);


      bool OnCommand(WPARAM wparam, LPARAM lparam);
#ifdef WINDOWS
      void OnMixerControlChange(HMIXER hMixer, uint32_t dwControlID);
      void OnMixerLineChange(HMIXER hMixer, uint32_t dwLineID);
#endif


      static bool get_component_name(LPMIXERLINE pmxl, string & strComponent);
      static bool get_control_type_name(LPMIXERCONTROL pmxctrl, string & strTypeName);



   };



} // namespace audio_mixer