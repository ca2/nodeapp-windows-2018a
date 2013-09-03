#include "framework.h"


namespace draw2d_gdi
{


   factory_exchange::factory_exchange(application * papp) :
      element(papp)
   {

      System.factory().cloneable_large < dib                  >  (System.type_info < ::draw2d::dib                > ());
      System.factory().cloneable_large < graphics             >  (System.type_info < ::draw2d::graphics           > ());
      System.factory().cloneable_large < bitmap               >  (System.type_info < ::draw2d::bitmap             > ());
      System.factory().cloneable_large < pen                  >  (System.type_info < ::draw2d::pen                > ());
      System.factory().cloneable_large < brush                >  (System.type_info < ::draw2d::brush              > ());
      System.factory().cloneable_large < palette              >  (System.type_info < ::draw2d::palette            > ());
      System.factory().cloneable_large < region               >  (System.type_info < ::draw2d::region             > ());
      System.factory().cloneable_large < font                 >  (System.type_info < ::draw2d::font               > ());
      System.factory().cloneable_large < ::draw2d::path       >  (System.type_info < ::draw2d::path               > ());

      ::draw2d_gdi::dib::s_initialize();

   }


   factory_exchange::~factory_exchange()
   {

   }


} // namespace draw2d_gdi


void ca2_factory_exchange(sp(base_application) papp)
{
   draw2d_gdi::factory_exchange factoryexchange(papp);
}




