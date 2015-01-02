#pragma once


namespace multimedia
{


   namespace audio_xaudio7
   {


      class CLASS_DECL_AUDIO_XAUDIO7 factory_exchange :
         virtual public ::multimedia::xaudio7::factory_exchange
      {
      public:


         factory_exchange(sp(::axis::application) papp);
         virtual ~factory_exchange();

      };


   } // namespace audio_xaudio7


} // namespace multimedia






