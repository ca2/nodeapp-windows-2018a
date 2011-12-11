#include "StdAfx.h"

namespace win
{

   factory_exchange::factory_exchange(::ca::application * papp) :
      ca(papp)
   {
      System.factory().cloneable_large < WinStdioFile         >  (System.get_type_info < ::ex1::text_file         > ());
      System.factory().cloneable_large < WinFile              >  (System.get_type_info < ::ex1::file              > ());
      System.factory().cloneable_large < WinFileSet           >  (System.get_type_info < ::ex2::file_set          > ());
      System.factory().cloneable < WinFileSystem        >  (System.get_type_info < ::ex1::file_system       > (), 1);
      System.factory().cloneable_small < WinFileException     >  (System.get_type_info < ::ex1::file_exception    > ());
      System.factory().cloneable < WinResource          >  (System.get_type_info < ::ex1::resource          > (), 1);
      System.factory().cloneable < application          >  (System.get_type_info < ::ex2::application       > (), 1);
      System.factory().cloneable < dir                  >  (System.get_type_info < ::ca::dir::system             > (), 1);
      System.factory().cloneable_small < folder_watch         >  (System.get_type_info < ::ex2::folder_watch      > ());
      System.factory().cloneable_large < dib                  >  (System.get_type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics             >  (System.get_type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap               >  (System.get_type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                  >  (System.get_type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                >  (System.get_type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette              >  (System.get_type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                  >  (System.get_type_info < ::ca::region                > ());
      System.factory().cloneable_large < font                 >  (System.get_type_info < ::ca::font               > ());
      System.factory().creatable < window_draw          >  (System.get_type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread               >  (System.get_type_info < ::ca::thread             > ());
      System.factory().creatable_large < window               >  (System.get_type_info < ::ca::window             > ());
      System.factory().cloneable_small < draw_dib             >  (System.get_type_info < ::ca::draw_dib           > ());
      System.factory().creatable < osi                  >  (System.get_type_info < ::ca::osi                > (), 1);
      System.factory().creatable < port_forward         >  (System.get_type_info < ::ca4::port_forward             > (), 1);
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