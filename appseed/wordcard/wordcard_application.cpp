#include "framework.h"


namespace wordcard
{


   application::application()
   {
      
      m_strAppName         = "wordcard";
      m_strBaseSupportId   = "ca2_wordcard";
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
		   "wordcard/frame",
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

   
   void application::on_request(::create * pcreate)
   {

      m_pdoctemplateMain->request_create(pcreate);

      sp(::user::document) pdoc = ::user::get_document(pcreate);

      if(pdoc.is_null())
         return;

   }


   bool application::_001OnCmdMsg(::aura::cmd_msg * pcommand)
   		
   {
      return ::core::application::_001OnCmdMsg(pcommand);
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


} // namespace wordcard


extern "C"
::aura::library * get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < wordcard::application > (papp, "nodeapp");

}



