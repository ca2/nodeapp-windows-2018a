#pragma once


namespace backup
{

   enum e_pane_view
   {
      pane_view_none,
      pane_view_backup,
      pane_view_options,
   };


   class CLASS_DECL_CA2_BACKUP pane_view_update_hint :
      public ::radix::object  
   {
   public:
      enum e_type
      {
         TypeGetView,
         TypeSetView,
         TypeOnShowView,
      };
      // DECLARE_DYNAMIC(pane_view_update_hint)
   public:
      void set_type(e_type e_type);
      bool is_type_of(e_type e_type);

      pane_view_update_hint();
      virtual ~pane_view_update_hint();

      e_pane_view m_eview;
   protected:
      e_type m_etype;

   };


} // namespace backup



