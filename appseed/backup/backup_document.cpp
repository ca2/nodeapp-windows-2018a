#include "framework.h"


namespace backup
{


   document::document(::aura::application * papp) :
      element(papp),
      ::data::data_container_base(papp),
      production::document(papp)
   {


   }


   document::~document()
   {


   }


} // namespace backup


