#include "framework.h"


namespace win
{


   factory_exchange::factory_exchange(sp(::application) papp) :
      element(papp)
   {

      System.factory().cloneable_large    < stdio_file         >  (System.type_info < ::ca2::text_file         > ()        );
      System.factory().cloneable_large    < file               >  (System.type_info < ::file::file              > ()        );
      System.factory().cloneable_large    < file_set           >  (System.type_info < ::ca2::file_set          > ()        );
      System.factory().cloneable          < file_system        >  (System.type_info < ::ca2::file_system       > ()  , 1   );
      System.factory().cloneable          < WinResource        >  (System.type_info < ::ca2::resource          > ()  , 1   );
      System.factory().cloneable          < application        >  (System.type_info < ::ca2::application_base  > ()  , 1   );
      System.factory().cloneable          < dir                >  (System.type_info < ::ca2::dir::system       > ()  , 1   );
      System.factory().cloneable_small    < folder_watch       >  (System.type_info < ::ca2::folder_watch      > ()        );
      System.factory().creatable          < window_draw        >  (System.type_info < ::ca2::window_draw       > ()  , 1   );
      System.factory().creatable_large    < thread             >  (System.type_info < ::ca2::thread            > ()        );
      System.factory().creatable_large    < window             >  (System.type_info < ::ca2::window            > ()        );
      System.factory().creatable          < os                 >  (System.type_info < ::ca2::os                > ()  , 1   );
      System.factory().creatable          < port_forward       >  (System.type_info < ::ca2::port_forward      > ()  , 1   );
      System.factory().creatable_small    < copydesk           >  (System.type_info < ::ca2::copydesk          > ()  , 1   );
      System.factory().creatable_small    < crypt              >  (System.type_info < ::ca2::crypt             > ()  , 1   );
      System.factory().creatable_small    < ip_enum            >  (System.type_info < ::ca2::ip_enum           > ()        );
      System.factory().creatable          < printer            >  (System.type_info < ::user::printer          > ()  , 1   );

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace win



void ca2_factory_exchange(sp(::application) papp)
{
   win::factory_exchange factoryexchange(papp);
}



