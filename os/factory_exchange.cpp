#include "framework.h"


namespace win
{


   factory_exchange::factory_exchange(::ca::application * papp) :
      ca(papp)
   {
      System.factory().cloneable_large < WinStdioFile          >  (System.template type_info < ::ex1::text_file         > ());
      System.factory().cloneable_large < WinFile               >  (System.template type_info < ::ex1::file              > ());
      System.factory().cloneable_large < WinFileSet            >  (System.template type_info < ::ex2::file_set          > ());
      System.factory().cloneable < WinFileSystem               >  (System.template type_info < ::ex1::file_system       > (), 1);
      System.factory().cloneable < WinResource                 >  (System.template type_info < ::ex1::resource          > (), 1);
      System.factory().cloneable < application                 >  (System.template type_info < ::ex2::application       > (), 1);
      System.factory().cloneable < dir                         >  (System.template type_info < ::ca::dir::system        > (), 1);
      System.factory().cloneable_small < folder_watch          >  (System.template type_info < ::ex2::folder_watch      > ());
      System.factory().cloneable_large < dib                   >  (System.template type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics              >  (System.template type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap                >  (System.template type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System.template type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                 >  (System.template type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette               >  (System.template type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                >  (System.template type_info < ::ca::region             > ());
      System.factory().cloneable_large < font                  >  (System.template type_info < ::ca::font               > ());
      System.factory().creatable < window_draw                 >  (System.template type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System.template type_info < ::ca::thread             > ());
      System.factory().creatable_large < window                >  (System.template type_info < ::ca::window             > ());
      System.factory().cloneable_small < draw_dib              >  (System.template type_info < ::ca::draw_dib           > ());
      System.factory().creatable < os                          >  (System.template type_info < ::ca::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System.template type_info < ::ca4::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System.template type_info < ::ca4::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System.template type_info < ::ca4::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System.template type_info < ::ca4::ip_enum           > ());
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



