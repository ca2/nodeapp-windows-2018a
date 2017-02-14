#pragma once

namespace wordcard
{

   class CLASS_DECL_CA2_WORDCARD form_callback :
      virtual public ::user::form_callback
   {
   public:
      form_callback();
      virtual ~form_callback();

      void on_update(::user::form * pview, sp(::user::impact) pSender, LPARAM lHint, ::object* phint);
      virtual bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };

} // namespace wordcard