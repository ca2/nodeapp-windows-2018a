#include "framework.h"


namespace win2
{


   factory_exchange::factory_exchange(::ca::applicationsp papp) :
      ca(papp)
   {
      
      System.factory().creatable < ::win2::application         >  (System.type_info < ::cubebase::application > (), 1);
      
   }


   factory_exchange::~factory_exchange()
   {
   }

} // namespace win2



void ca2_factory_exchange(::ca::applicationsp papp)
{

   win2::factory_exchange factoryexchange(papp);
}

