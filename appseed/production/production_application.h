#pragma once

namespace production
{

   class CLASS_DECL_CA2_PRODUCTION application :
      public ::plane::application
   {
   public:
      
      
      sp(::user::single_document_template) m_pdoctemplateMain;


      application();
      virtual ~application();

      virtual void construct();

      virtual bool initialize_instance();
      virtual int32_t  exit_instance();

      virtual bool bergedge_start();

      virtual void on_request(sp(::create_context) pcreatecontext);

	   virtual bool _001OnCmdMsg(base_cmd_msg * pcmdmsg);
   		

      virtual void OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema);

      production::e_version m_eversion;

   };


} // namespace production


