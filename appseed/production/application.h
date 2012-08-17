#pragma once

namespace production
{

   class CLASS_DECL_CA2_PRODUCTION application :
      public cube::application
   {
   public:
      
      
      ::userbase::single_document_template * m_pdoctemplateMain;


      application();
      virtual ~application();

      virtual void construct();

      virtual bool initialize_instance();
      virtual int  exit_instance();

      virtual bool bergedge_start();

      virtual void on_request(::ca::create_context * pcreatecontext);

	   virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);
   		

      virtual void OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema);

      production_class::e_version m_eversion;

   };


} // namespace production


