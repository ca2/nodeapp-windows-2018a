#pragma once


namespace production
{


   class view;


   class CLASS_DECL_CA2_PRODUCTION application :
      virtual public ::core::application,
      virtual public ::html::application
   {
   public:


      view * m_pview;
      
      
      sp(::user::single_document_template) m_pdoctemplateMain;


      application();
      virtual ~application();

      virtual void construct();

      virtual bool initialize_instance();
      virtual int32_t  exit_instance();

      virtual bool bergedge_start();

      virtual void on_request(sp(::create) pcreatecontext);

	   virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);
   		

      virtual void OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema);

      production::e_version m_eversion;

   };


} // namespace production


