#pragma once


namespace audio_mixer
{


   class control_data_volume :
      public ::audio_mixer::control_data
   {
   public:
      
      
      int32_t                             nRange;
      int32_t                             nPageInc;
      
      
      inline virtual int32_t GetObjectType()
      {
         return DataTypeVolume;
      }


   };


} // namespace audio_mixer




