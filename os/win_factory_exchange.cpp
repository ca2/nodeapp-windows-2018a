#include "framework.h"


namespace win
{


   factory_exchange::factory_exchange(sp(::ca::application) papp) :
      ca(papp)
   {

      System.factory().cloneable_large <stdio_file             >  (System.type_info < ::ca::text_file         > ());
      System.factory().cloneable_large < file                  >  (System.type_info < ::ca::file              > ());
      System.factory().cloneable_large < file_set              >  (System.type_info < ::ca::file_set          > ());
      System.factory().cloneable < file_system                 >  (System.type_info < ::ca::file_system       > (), 1);
      System.factory().cloneable < WinResource                 >  (System.type_info < ::ca::resource          > (), 1);
      System.factory().cloneable < application                 >  (System.type_info < ::ca::application_base       > (), 1);
      System.factory().cloneable < dir                         >  (System.type_info < ::ca::dir::system        > (), 1);
      System.factory().cloneable_small < folder_watch          >  (System.type_info < ::ca::folder_watch      > ());
      System.factory().cloneable_large < dib                   >  (System.type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::ca::region             > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::ca::font               > ());
      System.factory().cloneable_large < graphics_path         >  (System.type_info < ::ca::graphics_path      > ());
      System.factory().creatable < window_draw                 >  (System.type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System.type_info < ::ca::thread             > ());
      System.factory().creatable_large < window                >  (System.type_info < ::ca::window             > ());
      System.factory().creatable < os                          >  (System.type_info < ::ca::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System.type_info < ::ca::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System.type_info < ::ca::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System.type_info < ::ca::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System.type_info < ::ca::ip_enum           > ());
      
      ::win::dib::s_initialize();

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace win



void ca2_factory_exchange(sp(::ca::application) papp)
{
   win::factory_exchange factoryexchange(papp);
}



