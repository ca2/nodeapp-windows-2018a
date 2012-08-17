#include "framework.h"

namespace backup
{

   view::view(::ca::application * papp) :
      ca(papp),
      ::user::interaction(papp), 
      ::user::scroll_view(papp),
      ::userbase::view(papp),
      production::view(papp)
   {
   }

   view::~view()
   {
   }

   production_class * view::create_production_class()
   {
      return new backup_class(get_app());
   }

} // namespace backup