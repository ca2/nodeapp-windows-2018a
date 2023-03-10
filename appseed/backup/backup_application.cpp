#include "framework.h"


namespace backup
{


   application::application()
   {
      m_strAppName            = "backup";
      m_strBaseSupportId      = "ca2_backup";
   }

   application::~application()
   {
   }

   bool application::initialize_application()
   {

      if(!::core::application::init_instance())
         return false;


      m_eversion = ::production::production::version_backup;


      System.factory().creatable_small < document > ();
      System.factory().creatable_small < frame > ();
      System.factory().creatable_small < view > ();
      System.factory().creatable_small < pane_view > ();

      Session.filemanager()->m_strLevelUp = "levelup";


      m_pdoctemplateMain = new ::user::single_document_template(
      this,
      "production/frame",
      System.type_info < document > (),
      System.type_info < frame > (),
      System.type_info < pane_view > ());

      return true;
   }

   void application::term_instance()
   {

      try
      {

         production::application::term_instance();

      }
      catch (...)
      {

         m_error.m_iaErrorCode2.add(-1);

      }

   }


   bool application::bergedge_start()
   {
      return m_pdoctemplateMain->open_document_file(this, NULL, TRUE) != NULL;
   }


   void application::_001OnFileNew()
   {

      document_manager()->_001OnFileNew();

   }


} //namespace backup


