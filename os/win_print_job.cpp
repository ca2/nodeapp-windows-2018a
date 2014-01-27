#include "framework.h"


namespace win
{


   print_job::print_job(sp(base_application) papp) :
      element(papp),
      ::print_job(papp),
      ::job(papp),
      ::user::job(papp),
      thread(papp)
   {

   }


   print_job::~print_job()
   {

   }


   int32_t print_job::run()
   {

      if(!m_phprinter->is_opened())
      {
         // TODO:
         // m_printer.open(&callback_form);
         throw "printer not opened";
      }

      if(m_pview == NULL)
      {
         throw "a view should collaborate";
      }

      ::draw2d::graphics * pgraphics = m_phprinter->create_graphics();
      
      
      DOCINFO docinfo;
      memset(&docinfo, 0, sizeof(docinfo));
      docinfo.cbSize = sizeof(docinfo);
      pgraphics->StartDocA(&docinfo);

      int32_t iPageCount = m_pview->get_total_page_count(this);
      int32_t iPageEnd = -1;
      if(m_iPageStart < 0)
         m_iPageStart = 0;
      if(m_iPageCount >= 0)
      {
         iPageEnd = min(m_iPageStart + m_iPageCount - 1, iPageCount - 1);
      }
      else
      {
         iPageEnd = iPageCount - 1;
      }

      pgraphics->m_pjob = this;
      for(m_iPrintingPage = m_iPageStart; m_iPrintingPage <= iPageEnd; m_iPrintingPage++)
      {
         pgraphics->StartPage();
         if(m_bCancel)
         {
            pgraphics->AbortDoc();
            return -1;
         }
         m_pview->_001OnDraw(pgraphics);
         if(m_bCancel)
         {
            pgraphics->AbortDoc();
            return -1;
         }
         pgraphics->EndPage();
         if(m_bCancel)
         {
            pgraphics->AbortDoc();
            return -1;
         }
         
      }

      pgraphics->EndDoc();


      return 0;

   }


} // namespace win





