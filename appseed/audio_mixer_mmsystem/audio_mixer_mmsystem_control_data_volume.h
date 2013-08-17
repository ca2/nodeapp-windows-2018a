#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class control_data_volume :
         public ::multimedia::audio_mixer::control_data
      {
      public:


         int32_t                             nRange;
         int32_t                             nPageInc;


         inline virtual int32_t GetObjectType()
         {
            return DataTypeVolume;
         }


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




