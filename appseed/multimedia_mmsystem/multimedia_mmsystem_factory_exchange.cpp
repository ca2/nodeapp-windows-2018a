#include "framework.h"


namespace multimedia
{


   namespace mmsystem
   {


      factory_exchange::factory_exchange(sp(::base::application) papp) :
         element(papp)
      {

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace mmsystem


} // namespace multimedia


void ca2_factory_exchange(sp(::base::application) papp)
{

   ::multimedia::mmsystem::factory_exchange factoryexchange(papp);

}









