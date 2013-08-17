#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA source :
      virtual public ::ca2::object
   {
   public:


      ::audio_mixer::destination        * m_pdestination;
      ::audio_mixer::control_array        m_mixercontrola;
      MIXERLINE                     m_mixerline;
      MIXERLINECONTROLS             m_mixerlinecontrols;
      bool                          m_bHasV001Controls;


      source(sp(::ca2::application) papp);
      source(source & source);
      virtual ~source();


      ::multimedia::result mixerGetLineInfo(uint32_t dwSource, uint32_t dwDestination, uint32_t fdwInfo);
      ::multimedia::result mixerGetLineInfo(uint32_t dwSource, ::audio_mixer::destination * pdestination);

      uint32_t GetLineID();
      const char * GetSZName();
      void update_all_controls();
      MIXERLINECONTROLS & get_mixer_line_controls();
      MIXERLINE & get_mixer_line();
      ::audio_mixer::control_array & get_control_array();

      ::audio_mixer::device * get_device();

      void SetDestination(::audio_mixer::destination * pdestination);
      ::audio_mixer::destination * get_destination();

      void OnMixerLineChange();
      //void OnArrayReallocation(void *pNewPointer);
      //::multimedia::result GetControl(uint32_t dwControlType, uint32_t dwControlFlags, ::audio_mixer::control ** ppControl);
      ::multimedia::result GetLineControls();

      inline bool HasV001Controls();

      source & operator = (const source & source);
      

   };


   class source_array :
      public spa(source)
   {
   public:
   };


} // namespace audio_mixer




