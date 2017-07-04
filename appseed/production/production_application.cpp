#include "framework.h"


namespace production
{


   application::application()
   {
      
      m_strAppName         = "production";
      m_strBaseSupportId   = "ca2_production";
      m_bLicense           = false;
      m_pview = NULL;

   }


   void application::construct()
   {

   }


   application::~application()
   {

   }


   bool application::initialize_application()
   {
      
      System.factory().creatable_small < document >();
      System.factory().creatable_small < frame >();
      System.factory().creatable_small < view >();
      System.factory().creatable_small < pane_view >();


      if(!::core::application::initialize_application())
         return false;

//      Session.filemanager().std().m_strLevelUp = "levelup";


	   m_pdoctemplateMain = new ::user::single_document_template(
         this,
		   "production/frame",
		   System.type_info < document > (),
		   System.type_info < frame > (),
		   System.type_info < pane_view > ());

      add_document_template(m_pdoctemplateMain);

      return true;

   }


   int32_t application::exit_application()
   {

      int32_t iExitCode = 0;

      try
      {

         iExitCode = ::core::application::exit_application();

      }
      catch(...)
      {

         iExitCode = -1;

      }

      return iExitCode;

   }

   bool application::bergedge_start()
   {
      return true;
   }

   void application::on_request(::create * pcreatecontext)
   {

      string strVersion = Application.file().as_string("C:\\ca2\\config\\nodeapp-windows\\production\\version.txt").trimmed();

      if (strVersion == "stage")
      {

         m_eversion = ::production::production::version_stage;

      }
      else
      {

         m_eversion = ::production::production::version_basis;

      }
      
      sp(::user::document) pdoc = m_pdoctemplateMain->open_document_file(pcreatecontext);

      if(pdoc.is_null())
         return;

      if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("start"))
      {
         m_pview->make_production();
      }
      else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_deferred"))
      {
         m_pview->production_loop(Application.command()->m_varTopicQuery["start_deferred"]);
      }


   }


   bool application::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
   		
   {
      return ::core::application::_001OnCmdMsg(pcmdmsg);
   }





   void application::OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::fs::item_array & itema)
   {
      UNREFERENCED_PARAMETER(pdata);
      if(itema.get_size() > 0)
      {
         ::ShellExecuteW(
            NULL, 
            L"open", 
            ::str::international::utf8_to_unicode(itema[0]->m_filepath),
            NULL,
            ::str::international::utf8_to_unicode(itema[0]->m_filepath.folder()),
            SW_SHOW);
      }
      
   }


} // namespace production


extern "C"
::aura::library * get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < production::application > (papp, "nodeapp");

}

