#pragma once


namespace multimedia
{


   namespace xaudio
   {


      class CLASS_DECL_MULTIMEDIA_XAUDIO factory_exchange :
         virtual public ::object
      {
      public:


         factory_exchange(sp(::axis::application) papp);
         virtual ~factory_exchange();

      };


   } // namespace xaudio


} // namespace multimedia






