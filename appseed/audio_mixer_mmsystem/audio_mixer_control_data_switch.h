#pragma once


namespace audio_mixer
{


   class control_data_switch :
      public ::audio_mixer::control_data
   {
   public:

      inline virtual int32_t GetObjectType()
      {
         return DataTypeMute;
      }


   };


} // namespace audio_mixer




