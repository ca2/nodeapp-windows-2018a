#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM device :
         virtual public ::multimedia::audio_mixer::device
      {
      public:


         HMIXER                        m_hMixer;
         MIXERCAPS                     m_mixercaps;


         device(::multimedia::audio_mixer::audio_mixer * pmixer);
         virtual ~device();


         ::multimedia::audio_mixer::audio_mixer * get_mixer();
         const char * GetSZPName();
         bool OnCommand(WPARAM wparam, LPARAM lparam);
         ::multimedia::result close();
         void MapDlgCtrlIDToLineControls(::multimedia::audio_mixer::source * pSource);
         void MapDlgCtrlIDToControls();
         void MapLineControls(::multimedia::audio_mixer::source * pSource);
         void OnMixerControlChange(uint32_t dwControlID);
         void OnMixerLineChange(uint32_t dwLineID);
         void map_lines();
         void map_controls();
         ::multimedia::result get_destination(uint32_t dwComponentType, ::multimedia::audio_mixer::destination ** ppDestination);
         ::multimedia::result initialize_destinations();
         ::multimedia::result initialize_capabilities();
         ::multimedia::result open(uint32_t uiMixerId, uint32_t dwCallback, uint32_t dwInstance, uint32_t fdwOpen);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





