#include "framework.h"


namespace backup
{


   document::document(::ca::application * papp) :
      ca(papp),
      production::document(papp)
   {


   }


   document::~document()
   {


   }


} // namespace backup


