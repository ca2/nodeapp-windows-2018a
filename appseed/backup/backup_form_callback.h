#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP form_callback :
      virtual public ::production::form_callback
   {
   public:
      form_callback();
      virtual ~form_callback();

      void on_update(::user::form * pview, sp(::user::impact) pSender, LPARAM lHint, ::object* phint);
      virtual void on_control_event(::user::control_event * pevent);
   };


} // namespace backup



