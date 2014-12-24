#include "framework.h"


namespace production
{


   document::document(::aura::application * papp):
      element(papp),
      ::data::data_container_base(papp),
      ::aura::document(papp)
   {
   }

   bool document::on_new_document()
   {

	   if (!::aura::document::on_new_document())
		   return FALSE;


      update_all_views(NULL, 0);


	   return TRUE;
   }

   document::~document()
   {
   }



   #ifdef DEBUG
   void document::assert_valid() const
   {
	   ::aura::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
	   ::aura::document::dump(dumpcontext);
   }
   #endif //DEBUG

   void document::data_on_after_change(::signal_details * pobj)
   {


      UNREFERENCED_PARAMETER(pobj);


   }

   bool document::on_open_document(var varFile)
   {

      string str = Application.file().as_string(varFile);

   //  m_document.load(str);

      update_all_views(NULL, 123);

      return TRUE;


   }


} // namespace production



