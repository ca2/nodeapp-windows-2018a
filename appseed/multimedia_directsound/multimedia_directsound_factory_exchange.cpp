#include "framework.h"


namespace multimedia
{


   namespace directsound
   {


      factory_exchange::factory_exchange(sp(::axis::application) papp) :
         ::object(papp)
      {

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace directsound


} // namespace multimedia


void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::directsound::factory_exchange factoryexchange(papp);

}









