#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP form_callback :
      virtual public ::production::form_callback
   {
   public:
      form_callback();
      virtual ~form_callback();

      void on_update(::user::form * pview, ::view* pSender, LPARAM lHint, ::gen::object* phint);
      virtual bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };


} // namespace backup



