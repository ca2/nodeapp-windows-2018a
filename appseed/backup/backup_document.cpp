#include "framework.h"


namespace backup
{


   document::document(sp(::ca2::application) papp) :
      ca2(papp),
      ::ca2::data_container_base(papp),
      production::document(papp)
   {


   }


   document::~document()
   {


   }


} // namespace backup


