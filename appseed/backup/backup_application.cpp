#include "framework.h"


namespace backup
{


   application::application()
   {
      m_strAppName            = "backup";
      m_strBaseSupportId      = "votagus_ca2_backup";
   }

   application::~application()
   {
   }

   bool application::initialize_instance()
   {

      if(!cube2::application::initialize_instance())
         return false;


      m_eversion = ::production::production::version_backup;


      System.factory().creatable_small < document > ();
      System.factory().creatable_small < frame > ();
      System.factory().creatable_small < view > ();
      System.factory().creatable_small < pane_view > ();

      GetStdFileManagerTemplate()->m_strLevelUp = "levelup";


	   m_pdoctemplateMain = new ::userbase::single_document_template(
         this,
		   "production/frame",
         System.template type_info < document > (),
		   System.template type_info < frame > (),
		   System.template type_info < pane_view > ());

      return true;
   }

   int application::exit_instance()
   {
      
      int iExitCode = 0;

      try
      {

         iExitCode = production::application::exit_instance();

      }
      catch(...)
      {

         iExitCode = -1;

      }

      return iExitCode;

   }

   bool application::bergedge_start()
   {
      return m_pdoctemplateMain->open_document_file(NULL, TRUE) != NULL;
   }

   void application::_001OnFileNew()
   {
      userbase::application::m_pdocmanager->_001OnFileNew();
   }







} //namespace backup


CLASS_DECL_CA2_BACKUP ::ca::application * get_new_app()
{
   return new ::backup::application;
}
