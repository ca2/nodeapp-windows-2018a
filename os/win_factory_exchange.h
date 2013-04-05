#pragma once

namespace win
{

   class CLASS_DECL_win factory_exchange :
      virtual public ::ca::object
   {
   public:
      factory_exchange(::ca::applicationsp papp);
      virtual ~factory_exchange();
   };

} // namespace win