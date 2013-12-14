#include "framework.h"


namespace production
{


   application::application()
   {
      
      m_strAppName         = "production";
      m_strBaseSupportId   = "ca2_production";
      m_bZipIsDir          = false;
      m_bLicense           = false;

   }


   void application::construct()
   {

   }


   application::~application()
   {

   }


   bool application::initialize_instance()
   {
      
      System.factory().creatable_small < document >();
      System.factory().creatable_small < frame >();
      System.factory().creatable_small < view >();
      System.factory().creatable_small < pane_view >();


      if(!::application::initialize_instance())
         return false;

      filemanager().std().m_strLevelUp = "levelup";


	   m_pdoctemplateMain = new ::user::single_document_template(
         this,
		   "production/frame",
		   System.type_info < document > (),
		   System.type_info < frame > (),
		   System.type_info < pane_view > ());

      userex()->add_document_template(m_pdoctemplateMain);

      return true;

   }


   int32_t application::exit_instance()
   {

      int32_t iExitCode = 0;

      try
      {

         iExitCode = ::application::exit_instance();

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

   void application::on_request(sp(::create_context) pcreatecontext)
   {

      string strVersion = file().as_string("C:\\ca2\\config\\nodeapp-windows\\production\\version.txt");

      if (strVersion == "stage")
      {

         m_eversion = ::production::production::version_stage;

      }
      else
      {

         m_eversion = ::production::production::version_basis;

      }
      
      m_pdoctemplateMain->open_document_file(pcreatecontext);

   }


   bool application::_001OnCmdMsg(base_cmd_msg * pcmdmsg)
   		
   {
      return ::application::_001OnCmdMsg(pcmdmsg);
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
            ::str::international::utf8_to_unicode(itema[0].m_strPath),
            NULL,
            ::str::international::utf8_to_unicode(System.dir().name(itema[0].m_strPath)),
            SW_SHOW);
      }
      
   }


} // namespace production


extern "C"
::core::library * get_new_library(sp(base_application) papp)
{

   return new ::core::single_application_library < production::application > (papp, "nodeapp");

}

