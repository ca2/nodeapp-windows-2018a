#include "framework.h"


namespace backup
{


   document::document(sp(base_application) papp) :
      element(papp),
      ::ca2::data_container_base(papp),
      production::document(papp)
   {


   }


   document::~document()
   {


   }


} // namespace backup


