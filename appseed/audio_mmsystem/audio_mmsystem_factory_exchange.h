#pragma once


namespace multimedia
{


   namespace audio_mmsystem
   {


      class CLASS_DECL_AUDIO_MMSYSTEM factory_exchange :
         virtual public ::multimedia::mmsystem::factory_exchange
      {
      public:


         factory_exchange(sp(base_application) papp);
         virtual ~factory_exchange();

      };


   } // namespace audio_mmsystem


} // namespace multimedia






