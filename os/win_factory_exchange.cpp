#include "framework.h"


namespace win
{


   factory_exchange::factory_exchange(::ca::application * papp) :
      ca(papp)
   {

      System.factory().cloneable_large <stdio_file             >  (System.template type_info < ::ca::text_file         > ());
      System.factory().cloneable_large < file                  >  (System.template type_info < ::ca::file              > ());
      System.factory().cloneable_large < file_set              >  (System.template type_info < ::ca::file_set          > ());
      System.factory().cloneable < file_system                 >  (System.template type_info < ::ca::file::system       > (), 1);
      System.factory().cloneable < WinResource                 >  (System.template type_info < ::ca::resource          > (), 1);
      System.factory().cloneable < application                 >  (System.template type_info < ::ca::application       > (), 1);
      System.factory().cloneable < dir                         >  (System.template type_info < ::ca::dir::system        > (), 1);
      System.factory().cloneable_small < folder_watch          >  (System.template type_info < ::ca::folder_watch      > ());
      System.factory().cloneable_large < dib                   >  (System.template type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics              >  (System.template type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap                >  (System.template type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System.template type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                 >  (System.template type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette               >  (System.template type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                >  (System.template type_info < ::ca::region             > ());
      System.factory().cloneable_large < font                  >  (System.template type_info < ::ca::font               > ());
      System.factory().cloneable_large < graphics_path         >  (System.template type_info < ::ca::graphics_path      > ());
      System.factory().creatable < window_draw                 >  (System.template type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System.template type_info < ::ca::thread             > ());
      System.factory().creatable_large < window                >  (System.template type_info < ::ca::window             > ());
      System.factory().creatable < os                          >  (System.template type_info < ::ca::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System.template type_info < ::ca::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System.template type_info < ::ca::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System.template type_info < ::ca::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System.template type_info < ::ca::ip_enum           > ());
      
      ::win::dib::s_initialize();

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace win



void ca2_factory_exchange(::ca::application * papp)
{
   win::factory_exchange factoryexchange(papp);
}



