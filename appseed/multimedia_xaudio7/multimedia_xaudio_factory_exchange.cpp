#include "framework.h"


namespace multimedia
{


   namespace xaudio
   {


      factory_exchange::factory_exchange(sp(::axis::application) papp) :
         element(papp)
      {

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace xaudio


} // namespace multimedia


void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::xaudio::factory_exchange factoryexchange(papp);

}









