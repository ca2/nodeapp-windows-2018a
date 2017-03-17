#include "framework.h"


namespace a_spa
{


   socket_handler::socket_handler(::aura::application * papp) :
      ::object(papp),
      ::sockets::socket_handler(papp)
   {

   }


} // namespace a_spa




