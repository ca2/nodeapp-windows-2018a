#include "framework.h"


namespace draw2d_gdiplus
{


   brush::brush(::aura::application * papp) :
      ::object(papp)
   {

      m_pbrush = NULL;

   }

   brush::~brush()
   {

      ::aura::del(m_pbrush);

   }


#ifdef DEBUG

   void brush::dump(dump_context & dumpcontext) const
   {

      ::draw2d::object::dump(dumpcontext);

   }

#endif

   void * brush::get_os_data() const
   {

      if(m_pbrush == NULL || !m_bUpdated)
      {
         ::aura::del(((brush *) this)->m_pbrush);
         if(m_etype == type_solid)
         {
            try
            {
               ((brush *) this)->m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(argb_get_a_value(m_cr), argb_get_r_value(m_cr), argb_get_g_value(m_cr), argb_get_b_value(m_cr)));
            }
            catch(...)
            {
            }
         }
         else if(m_etype == type_linear_gradient_point_color)
         {
            try
            {
               ((brush *) this)->m_pbrush = new Gdiplus::LinearGradientBrush(
                  Gdiplus::PointF((Gdiplus::REAL) m_pt1.x,(Gdiplus::REAL) m_pt1.y),
                  Gdiplus::PointF((Gdiplus::REAL) m_pt2.x,(Gdiplus::REAL) m_pt2.y),
                  Gdiplus::Color(argb_get_a_value(m_cr1), argb_get_r_value(m_cr1), argb_get_g_value(m_cr1), argb_get_b_value(m_cr1)),
                  Gdiplus::Color(argb_get_a_value(m_cr2), argb_get_r_value(m_cr2), argb_get_g_value(m_cr2), argb_get_b_value(m_cr2)));
            }
            catch(...)
            {
            }
         }
         else if(m_etype == type_radial_gradient_color)
         {

            try
            {

               Gdiplus::GraphicsPath * gp = new Gdiplus::GraphicsPath();

               gp->AddEllipse((Gdiplus::REAL) (m_pt.x - m_size.cx / 2),(Gdiplus::REAL)(m_pt.y - m_size.cy / 2),(Gdiplus::REAL) (m_size.cx),(Gdiplus::REAL) (m_size.cy));

               Gdiplus::PathGradientBrush * pgb = new Gdiplus::PathGradientBrush(gp);

               Gdiplus::Color c1(argb_get_a_value(m_cr1),argb_get_r_value(m_cr1),argb_get_g_value(m_cr1),argb_get_b_value(m_cr1));
               Gdiplus::Color c2(argb_get_a_value(m_cr2),argb_get_r_value(m_cr2),argb_get_g_value(m_cr2),argb_get_b_value(m_cr2));

               INT c = 1;

               pgb->SetCenterPoint(Gdiplus::PointF((Gdiplus::REAL) m_pt.x,(Gdiplus::REAL) m_pt.y));
               pgb->SetCenterColor(c1);
               pgb->SetSurroundColors(&c2,&c);

               ((brush *) this)->m_pbrush = pgb;
            }
            catch(...)
            {
            }
         }
         else if (m_etype == type_pattern)
         {

            try
            {

               Gdiplus::Image * pimage = m_dib->get_bitmap().cast < bitmap >()->m_pbitmap;

               Gdiplus::TextureBrush * ptb = new Gdiplus::TextureBrush(pimage);

               ((brush *) this)->m_pbrush = ptb;

            }
            catch (...)
            {

            }

         }
         else
         {

            try
            {

               ((brush *) this)->m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(argb_get_a_value(m_cr), argb_get_r_value(m_cr), argb_get_g_value(m_cr), argb_get_b_value(m_cr)));

            }
            catch(...)
            {

            }

         }
      }

      if(m_pbrush != NULL)
      {
         ((brush *) this)->m_bUpdated = true;
      }

      return (void *) (Gdiplus::Brush *) m_pbrush;

   }

} // namespace draw2d_gdiplus


