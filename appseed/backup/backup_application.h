#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP application :
      public production::application
   {
   public:


      sp(::user::single_document_template) m_ptemplate_html;


      application();
      virtual ~application();


      virtual bool initialize_application();
      virtual void term_instance() override;

      virtual bool bergedge_start();

      virtual void _001OnFileNew();

   };


} // namespace backup




