#include "framework.h"


namespace backup
{


   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container_base(papp),
      production::document(papp)
   {


   }


   document::~document()
   {


   }


} // namespace backup


