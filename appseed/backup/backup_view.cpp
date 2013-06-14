#include "framework.h"


namespace backup
{

   view::view(sp(::ca2::application) papp) :
      ca2(papp),
      ::user::interaction(papp), 
      ::user::scroll_view(papp),
      
      ::production::view(papp)
   {
   }


   view::~view()
   {
   }


   ::production::production * view::create_production()
   {
      return new ::backup::backup(get_app());
   }


} // namespace backup


