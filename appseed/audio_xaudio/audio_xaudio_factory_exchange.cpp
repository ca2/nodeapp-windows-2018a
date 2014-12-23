#include "framework.h"


namespace multimedia
{


   namespace audio_xaudio
   {


      factory_exchange::factory_exchange(sp(::axis::application) papp) :
         element(papp),
         ::multimedia::xaudio::factory_exchange(papp)
      {

         System.factory().cloneable_large < wave_in               >  (System.type_info < ::multimedia::audio::wave_in                  > ());
         System.factory().cloneable_large < wave_out              >  (System.type_info < ::multimedia::audio::wave_out                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace audio_xaudio


} // namespace multimedia


extern "C"
void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::audio_xaudio::factory_exchange factoryexchange(papp);

}









