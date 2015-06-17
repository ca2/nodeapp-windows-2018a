#include "framework.h"


namespace multimedia
{


   namespace audio_xaudio7
   {


      factory_exchange::factory_exchange(sp(::axis::application) papp) :
         object(papp),
         ::multimedia::xaudio7::factory_exchange(papp)
      {

         System.factory().cloneable_large < wave_in               >  (System.type_info < ::multimedia::audio::wave_in                  > ());
         System.factory().cloneable_large < wave_out              >  (System.type_info < ::multimedia::audio::wave_out                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace audio_xaudio7


} // namespace multimedia


extern "C"
void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::audio_xaudio7::factory_exchange factoryexchange(papp);

}









