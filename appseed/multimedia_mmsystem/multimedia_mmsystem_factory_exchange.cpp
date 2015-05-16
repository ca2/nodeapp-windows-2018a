#include "framework.h"


namespace multimedia
{


   namespace mmsystem
   {


      factory_exchange::factory_exchange(::aura::application * papp) :
         ::object(papp)
      {

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace mmsystem


} // namespace multimedia


void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::mmsystem::factory_exchange factoryexchange(papp);

}









