#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP application :
      public production::application
   {
   public:


      sp(::userbase::single_document_template) m_ptemplate_html;


      application();
      virtual ~application();


      virtual bool initialize_instance();
      virtual int32_t  exit_instance();

      virtual bool bergedge_start();

      virtual void _001OnFileNew();

   };


} // namespace backup




