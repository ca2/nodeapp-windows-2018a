#pragma once

namespace audio_mmsystem
{

   class CLASS_DECL_DRAW2D_GDIPLUS factory_exchange :
      virtual public ::ca2::object
   {
   public:
      factory_exchange(sp(::ca2::application) papp);
      virtual ~factory_exchange();
   };

} // namespace audio_mmsystem