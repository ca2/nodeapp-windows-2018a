#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA destination :
      public ::audio_mixer::source
   {
   public:

      
      ::audio_mixer::device *  m_pdevice;
      ::audio_mixer::source_array  m_mixersourcea; 

      
      destination(sp(::ca2::application) papp);
      destination(const destination & destination);
      virtual ~destination();


      uint32_t get_component_type();
      void update_all_controls();
      ::multimedia::result initialize_all_controls();
      ::multimedia::result initialize_source_info();

      ::audio_mixer::source_array & get_source_info();
      void operator delete(void *);

      destination & operator = (const destination & device);

      ::audio_mixer::device * get_device();
      void set_device(::audio_mixer::device * pdevice);

   };


   class destination_array :
      public spa(destination)
   {
   public:

   };


} // namespace audio_mixer



