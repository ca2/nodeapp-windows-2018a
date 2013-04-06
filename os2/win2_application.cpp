#include "framework.h"


namespace win2
{


   application::application(sp(::ca::application) papp)
   {

      set_app(papp);

      m_psystem      = papp->m_psystem;

      m_psession     = papp->m_psession;
      
      m_bLicense     = false;

   }

   application::~application()
   {

   }



   ::user::printer * application::get_printer(const char * pszDeviceName)
   {
      ::win2::printer * pprinter = new ::win2::printer(get_app());
      if(!pprinter->open(pszDeviceName))
      {
         delete pprinter;
         return NULL;
      }
      return pprinter;
   }


} // namespace win2
