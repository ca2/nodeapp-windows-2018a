#pragma once

namespace win
{

   class CLASS_DECL_win factory_exchange :
      virtual public object
   {
   public:
      factory_exchange(sp(base_application) papp);
      virtual ~factory_exchange();
   };

} // namespace win