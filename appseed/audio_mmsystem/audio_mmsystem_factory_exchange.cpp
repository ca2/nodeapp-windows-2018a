#include "framework.h"


namespace multimedia
{


   namespace audio_mmsystem
   {


      factory_exchange::factory_exchange(::aura::application * papp) :
         ::object(papp),
         ::multimedia::mmsystem::factory_exchange(papp)
      {

         System.factory().cloneable_large < wave_in               >  (System.type_info < ::multimedia::audio::wave_in                  > ());
         System.factory().cloneable_large < wave_out              >  (System.type_info < ::multimedia::audio::wave_out                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace audio_mmsystem


} // namespace multimedia


void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::audio_mmsystem::factory_exchange factoryexchange(papp);

}









