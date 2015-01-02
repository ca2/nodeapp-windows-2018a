#include "framework.h"


namespace multimedia
{


   namespace xaudio7
   {


      factory_exchange::factory_exchange(sp(::axis::application) papp) :
         element(papp)
      {

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace xaudio7


} // namespace multimedia


void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::xaudio7::factory_exchange factoryexchange(papp);

}









