#include "framework.h"


namespace backup
{

   view::view(sp(::ca::application) papp) :
      ca(papp),
      ::user::interaction(papp), 
      ::user::scroll_view(papp),
      ::userbase::view(papp),
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


