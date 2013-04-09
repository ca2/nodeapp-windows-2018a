#include "framework.h"


namespace backup
{


   frame::frame(sp(::ca::application) papp) :
      ca(papp),
      production::frame(papp)
   {
      m_iFrameData = 10;

      m_dataid = "::ca::backup::frame";
      
   }

   frame::~frame()
   {
   }


} // namespace backup


