#pragma once


namespace audio_mmsystem
{


   enum e_message
   {

      audMessageCommand = WM_APP + 100,
      audMessagePreBufferDone,

   };

   class  CLASS_DECL_AUDIO_MMSYSTEM wave_pre_buffer_callback
   {
   public:

      
      virtual void OnPreBufferDone(int32_t iId);


   };



} // namespace audio_mmsystem




