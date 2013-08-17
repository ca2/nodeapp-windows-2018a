#include "framework.h"


namespace music
{


   namespace midi
   {


      view_helper::view_helper()
      {

      }

      view_helper::~view_helper()
      {

      }

      sp(document_helper) view_helper::HelperGetMidiDocHelper()
      {
         sp(::user::view) pview = (this);
         ASSERT(pview);
         sp(::user::document_interface) pdoc = pview->get_document();
         ASSERT(pdoc);
         sp(document_helper) phelper = (pdoc);
         ASSERT(phelper);
         return phelper;

      }


   } // namespace midi


} // namespace music









