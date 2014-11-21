#include "framework.h"


namespace backup
{

   view::view(::aura::application * papp) :
      element(papp),
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


