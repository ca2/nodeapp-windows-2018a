#include "framework.h"


#undef new


namespace win
{

   graphics_path::graphics_path(::ca::application * papp)
   {

      m_ppath = new Gdiplus::GraphicsPath();

      m_bMove = false;

   }


   graphics_path::~graphics_path()
   {

      if(m_ppath != NULL)
      {

         delete m_ppath;

         m_ppath = NULL;

      }

   }


   void * graphics_path::get_os_data() const
   {

      return (void *) m_ppath;

   }


   point graphics_path::last_point()
   {

      Gdiplus::PointF pf;

      if(m_bMove)
      {

         pf = m_point;

      }
      else
      {

         if(m_ppath->GetLastPoint(&pf) != Gdiplus::Status::Ok)
            throw simple_exception(get_app(), "could not get last point from path");

      }

      return point((long) pf.X, (long) pf.Y);

   }


   bool graphics_path::begin_figure(bool bFill, ::ca::e_fill_mode efillmode)
   {

      return m_ppath->StartFigure() == Gdiplus::Status::Ok;

   }

   bool graphics_path::end_figure(bool bClose)
   {

      if(bClose)
      {

         return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

      }
      else
      {

         return true;

      }

   }

   bool graphics_path::add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {

      bool bOk1 = true;

      if(m_bMove)
      {
         
         bOk1 = m_ppath->AddLine(m_point.X, m_point.Y, (FLOAT) x1, (FLOAT) y1) == Gdiplus::Status::Ok;

         m_bMove = false;

      }

      bool bOk2 = m_ppath->AddLine(x1, y1, x2, y2) == Gdiplus::Status::Ok;

      return bOk1 && bOk2;

   }
   
   bool graphics_path::add_line(int32_t x, int32_t y)
   {

      point pt = last_point();

      bool bOk = m_ppath->AddLine(pt.x, pt.y, x, y) == Gdiplus::Status::Ok;

      m_bMove = false;

      return bOk;

   }

   point graphics_path::current_point()
   {

      return last_point();

   }


   bool graphics_path::add_move(int32_t x, int32_t y)
   {

      m_point.X   = (Gdiplus::REAL) x;
      m_point.Y   = (Gdiplus::REAL) y;
      m_bMove     = true;

      return true;

   }



   bool graphics::set(const ::ca::graphics_path * ppathParam)
   {

      ::lnx::graphics_path * ppath = dynamic_cast < ::lnx::graphics_path * > ((::ca::graphics_path *) ppathParam);

      for(int32_t i = 0; i < ppath->m_elementa.get_count(); i++)
      {

         set(ppath->m_elementa[i]);

      }

      if(ppath->m_efillmode == ::ca::fill_mode_alternate)
      {

         cairo_set_fill_rule(m_pdc, CAIRO_FILL_RULE_EVEN_ODD);

      }
      else
      {

         cairo_set_fill_rule(m_pdc, CAIRO_FILL_RULE_WINDING);

      }


      return true;

   }


   bool graphics::set(const ::lnx::graphics_path::element & e)
   {

      switch(e.m_etype)
      {
      case simple_path::element::type_arc:
         set(e.m_arc);
         break;
      case simple_path::element::type_line:
         set(e.m_line);
         break;
      case simple_path::element::type_close:
         cairo_close_path(m_pdc);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }

   bool graphics::set(const ::lnx::graphics_path::arc & a)
   {

      cairo_translate(m_pdc, a.m_xCenter, a.m_yCenter);

      cairo_scale(m_pdc, 1.0, a.m_dRadiusY / a.m_dRadiusX);

      cairo_arc(m_pdc, 0.0, 0.0, a.m_dRadiusX, a.m_dAngle1, a.m_dAngle2);

      cairo_scale(m_pdc, 1.0, a.m_dRadiusX / a.m_dRadiusY);

      cairo_translate(m_pdc, -a.m_xCenter, -a.m_yCenter);

      return true;

   }

   bool graphics::set(const ::lnx::graphics_path::line & l)
   {

      if(!cairo_has_current_point(m_pdc))
      {

         cairo_move_to(m_pdc, l.m_x, l.m_y);

      }
      else
      {

         cairo_line_to(m_pdc, l.m_x, l.m_y);

      }

      return true;

   }


   bool graphics::set(const ::lnx::graphics_path::move & p)
   {

      cairo_move_to(m_pdc, p.m_x, p.m_y);


      return true;

   }

} // namespace win

#define new DEBUG_NEW