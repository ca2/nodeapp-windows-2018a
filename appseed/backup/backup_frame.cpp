#include "framework.h"


namespace backup
{


   frame::frame(sp(::aura::application) papp) :
      element(papp),
      production::frame(papp)
   {
      m_iFrameData = 10;

      m_dataid = "::ca2::backup::frame";
      
   }

   frame::~frame()
   {
   }


} // namespace backup


