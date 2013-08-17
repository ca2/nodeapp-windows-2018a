#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class control_data_switch :
         public ::multimedia::audio_mixer::control_data
      {
      public:

         inline virtual int32_t GetObjectType()
         {
            return DataTypeMute;
         }


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





