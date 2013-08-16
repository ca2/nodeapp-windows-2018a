#pragma once


namespace audio
{


   enum e_message
   {

      audMessageCommand = WM_APP + 100,
      audMessagePreBufferDone,

   };

   class CLASS_DECL_CA2_MULTIMEDIA wave_pre_buffer_callback
   {
   public:

      
      virtual void OnPreBufferDone(int32_t iId);


   };



} // namespace audio




