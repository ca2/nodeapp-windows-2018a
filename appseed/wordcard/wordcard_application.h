#pragma once


namespace wordcard
{


   class view;


   class CLASS_DECL_CA2_WORDCARD application :
      virtual public ::core::application
   {
   public:


      view * m_pview;
      
      
      sp(::user::single_document_template) m_pdoctemplateMain;


      application();
      virtual ~application();

      virtual void construct();

      virtual bool initialize_application();
      virtual int32_t  exit_application() override;

      virtual bool bergedge_start();

      virtual void on_request(::create * pcreate);

	   virtual bool route_command_message(::aura::cmd_msg * pcommand);
   		

      virtual void OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema);

   };


} // namespace wordcard


