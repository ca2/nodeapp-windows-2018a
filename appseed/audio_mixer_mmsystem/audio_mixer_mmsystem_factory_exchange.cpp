#include "framework.h"


namespace audio_mmsystem
{


   factory_exchange::factory_exchange(sp(::ca2::application) papp) :
      ca2(papp)
   {

      System.factory().cloneable_large < wave_in                   >  (System.type_info < ::multimedia::audio::wave_in                  > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::draw2d::graphics             > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::draw2d::bitmap               > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::draw2d::pen                  > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::draw2d::brush                > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::draw2d::palette              > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::draw2d::region               > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::draw2d::font                 > ());
      System.factory().cloneable_large < path                  >  (System.type_info < ::draw2d::path                 > ());

      ::audio_mmsystem::dib::s_initialize();

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace audio_mmsystem



void ca2_factory_exchange(sp(::ca2::application) papp)
{
   audio_mmsystem::factory_exchange factoryexchange(papp);
}



