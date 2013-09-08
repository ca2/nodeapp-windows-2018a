#include "framework.h"


namespace win
{


   factory_exchange::factory_exchange(sp(base_application) papp) :
      element(papp)
   {

      System.factory().cloneable_large    < stdio_file         >  (System.type_info < ::core::text_file           > ()        );
      System.factory().cloneable_large    < file               >  (System.type_info < ::file::binary_buffer       > ()        );
      System.factory().cloneable_large    < file_set           >  (System.type_info < ::core::file_set            > ()        );
      System.factory().cloneable          < file_system        >  (System.type_info < ::core::file_system         > ()  , 1   );
      System.factory().cloneable          < WinResource        >  (System.type_info < ::core::resource            > ()  , 1   );
      System.factory().cloneable          < application        >  (System.type_info < ::application_base          > ()  , 1   );
      System.factory().cloneable          < dir                >  (System.type_info < ::core::dir::system         > ()  , 1   );
      System.factory().cloneable_small    < folder_watch       >  (System.type_info < ::core::folder_watch        > ()        );
      System.factory().creatable          < window_draw        >  (System.type_info < ::user::window_draw         > ()  , 1   );
      System.factory().creatable_large    < thread             >  (System.type_info < ::thread                    > ()        );
      System.factory().creatable_large    < window             >  (System.type_info < ::user::window              > ()        );
      System.factory().creatable          < os                 >  (System.type_info < ::core::os                  > ()  , 1   );
      System.factory().creatable          < port_forward       >  (System.type_info < ::core::port_forward        > ()  , 1   );
      System.factory().creatable_small    < copydesk           >  (System.type_info < ::core::copydesk            > ()  , 1   );
      System.factory().creatable_small    < crypt              >  (System.type_info < ::core::crypt               > ()  , 1   );
      System.factory().creatable_small    < ip_enum            >  (System.type_info < ::core::ip_enum             > ()        );
      System.factory().creatable          < printer            >  (System.type_info < ::user::printer             > ()  , 1   );

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace win



void ca2_factory_exchange(sp(base_application) papp)
{
   win::factory_exchange factoryexchange(papp);
}



