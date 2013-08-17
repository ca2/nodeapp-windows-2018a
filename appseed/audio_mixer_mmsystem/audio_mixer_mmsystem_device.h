#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA device :
      virtual public ::ca2::object
   {
   public:


      ::audio_mixer::audio_mixer              * m_pmixer;
      uint32_t                      m_uiMixerID;
#ifdef WINDOWS
      HMIXER                        m_hMixer;
#endif

      map <uint32_t  , uint32_t  , ::audio_mixer::control  *, ::audio_mixer::control  *>   m_mapIDToControl;
      map <uint32_t  , uint32_t  , ::audio_mixer::control  *, ::audio_mixer::control  *>   m_mapDlgItemIDToControl;
      map <uint32_t  , uint32_t  , ::audio_mixer::source   *, ::audio_mixer::source   *>   m_mapIDToLine;
      //map <uint32_t   , uint32_t   , ::audio_mixer::control_data *, ::audio_mixer::control_data *>   m_mapCtrlIDToData;

      ::audio_mixer::destination_array    m_mixerdestinationa;
#ifdef WINDOWS
      MIXERCAPS                     m_mixercaps;
#endif

      device(::audio_mixer::audio_mixer * pmixer);
      virtual ~device();


      ::audio_mixer::audio_mixer * get_mixer();
      const char * GetSZPName();
      bool OnCommand(WPARAM wparam, LPARAM lparam);
      ::multimedia::result close();
      void MapDlgCtrlIDToLineControls(::audio_mixer::source * pSource);
      void MapDlgCtrlIDToControls();
      void MapLineControls(::audio_mixer::source * pSource);
      void OnMixerControlChange(uint32_t dwControlID);
      void OnMixerLineChange(uint32_t dwLineID);
      void map_lines();
      void map_controls();
      ::multimedia::result get_destination(uint32_t dwComponentType, ::audio_mixer::destination ** ppDestination);
      ::multimedia::result initialize_destinations();
      ::multimedia::result initialize_capabilities();
      ::multimedia::result open(uint32_t uiMixerId, uint32_t dwCallback, uint32_t dwInstance, uint32_t fdwOpen);


   };


} // namespace audio_mixer




