#pragma once


namespace multimedia
{


   namespace directsound
   {


      class CLASS_DECL_MULTIMEDIA_DIRECTSOUND factory_exchange :
         virtual public ::object
      {
      public:


         factory_exchange(sp(::axis::application) papp);
         virtual ~factory_exchange();

      };


   } // namespace directsound


} // namespace multimedia






