#pragma once

namespace win
{

   class CLASS_DECL_win factory_exchange :
      virtual public ::ca2::object
   {
   public:
      factory_exchange(sp(::ca2::application) papp);
      virtual ~factory_exchange();
   };

} // namespace win