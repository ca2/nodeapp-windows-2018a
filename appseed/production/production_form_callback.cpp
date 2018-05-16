#include "framework.h"

namespace production
{

   form_callback::form_callback()
   {
   }

   form_callback::~form_callback()
   {
   }

   void form_callback::on_update(::user::form * pview, sp(::user::impact) pSender, LPARAM lHint, ::object* phint)
   {
      UNREFERENCED_PARAMETER(pview);
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(phint);
   }


   void form_callback::on_control_event(::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pview);
      UNREFERENCED_PARAMETER(pevent);
      return false;
   }

} // namespace production