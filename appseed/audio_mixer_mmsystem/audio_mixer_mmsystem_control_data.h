#pragma once


namespace audio_mixer
{


   class control_data
   {
   public:


      enum
      {
         TypeDefault = 1,
         TypeStereoBalance = 2,
         TypeStereoVolume = 3,
         TypeStereoBalanceLabel = 4,
         TypeStereoVolumeLabel = 5,
         TypeUniformMute = 6
      };


      enum
      {
         DataTypeBase = 0,
         DataTypeVolume = 1,
         DataTypeMute = 2
      };


      int32_t             m_iType;
      uint32_t            m_uiMixerID;
      uint32_t            m_uiLineID;
      uint32_t            m_uiControlID;
      index              m_iIndex;


      virtual int32_t GetObjectType();
      virtual int32_t get_type();


   };


} // namespace audio_mixer




