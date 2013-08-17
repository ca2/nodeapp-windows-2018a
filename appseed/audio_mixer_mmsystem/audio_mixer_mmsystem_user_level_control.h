#pragma once


namespace audio_mixer
{


   namespace user
   {


      class CLASS_DECL_CA2_MULTIMEDIA level_control :
         public control
      {
      public:


         enum e_orientation
         {
            orientation_vertical,
            orientation_horizontal,
         };


         level_control();
         virtual ~level_control();


         virtual void SetPos(int32_t iPos);
         virtual int32_t GetPos() const;


         virtual void SetOrientation(e_orientation eorientation);
         virtual void SetRange(int32_t iMin, int32_t iMax);
         virtual void SetPageSize(int32_t iPage);
         virtual void SetLineSize(int32_t iSize);

         ::audio_mixer::control_data_volume & get_data();


      };


   } // namespace user


} // namespace audio_mixer







