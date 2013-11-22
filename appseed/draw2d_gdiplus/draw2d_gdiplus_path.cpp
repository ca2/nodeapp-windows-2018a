#include "framework.h"


#undef new


namespace draw2d_gdiplus
{



   path::path(sp(base_application) papp)
   {

      m_ppath = NULL;


   }


   path::~path()
   {

      destroy();

   }


   void * path::get_os_data() const
   {

      defer_update();

      return (void *) m_ppath;

   }


   point path::internal_last_point()
   {

      if(m_bHasPointInternal)
      {

         return point((LONG) m_ptInternal.X, (LONG) m_ptInternal.Y);

      }
      else
      {

        throw simple_exception(get_app(), "path does not have last point");

      }

   }


   bool path::internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode)
   {

      return m_ppath->StartFigure() == Gdiplus::Status::Ok;

   }

   bool path::internal_end_figure(bool bClose)
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

   bool path::internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {

      bool bOk1 = true;

      if(m_bHasPointInternal)
      {

         bOk1 = m_ppath->AddLine(m_ptInternal.X, m_ptInternal.Y, (FLOAT) x1, (FLOAT) y1) == Gdiplus::Status::Ok;

      }

      bool bOk2 = m_ppath->AddLine(x1, y1, x2, y2) == Gdiplus::Status::Ok;

      m_ptInternal.X = (Gdiplus::REAL) x2;
      m_ptInternal.Y = (Gdiplus::REAL) y2;

      m_bHasPointInternal = true;

      return bOk1 && bOk2;

   }
   
   bool path::internal_add_line(int32_t x, int32_t y)
   {

      bool bOk1 = true;

      if(m_bHasPointInternal)
      {

         bOk1 = m_ppath->AddLine(m_ptInternal.X, m_ptInternal.Y, (FLOAT) x, (FLOAT) y) == Gdiplus::Status::Ok;

      }

      m_bHasPointInternal = true;

      m_ptInternal.X = (Gdiplus::REAL) x;
      m_ptInternal.Y = (Gdiplus::REAL) y;

      return bOk1;

   }

   point path::internal_current_point()
   {

      return last_point();

   }


   bool path::internal_add_move(int32_t x, int32_t y)
   {

      m_ptInternal.X   = (Gdiplus::REAL) x;
      m_ptInternal.Y   = (Gdiplus::REAL) y;
      m_bHasPointInternal     = true;


      return true;

   }



   bool path::create()
   {

      m_ppath = new Gdiplus::GraphicsPath();

      m_bHasPoint = false;

      m_ptInternal.X = 69;

      m_ptInternal.Y = 69;

      m_bHasPointInternal = false;


      internal_begin_figure(m_bFill, m_efillmode);

      for(int32_t i = 0; i < m_elementa.get_count(); i++)
      {

         set(m_elementa[i]);

      }

      return true;

   }


   bool path::destroy()
   {

      if(m_ppath != NULL)
      {

         delete m_ppath;

         m_ppath = NULL;

      }

      return true;

   }


   bool path::set(const ::draw2d::path::element & e)
   {

      switch(e.m_etype)
      {
      case ::draw2d::path::element::type_arc:
         set(e.m_arc);
         break;
      case ::draw2d::path::element::type_move:
         set(e.m_move);
         break;
      case ::draw2d::path::element::type_line:
         set(e.m_line);
         break;
      case ::draw2d::path::element::type_end:
         internal_end_figure(e.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }

   bool path::internal_add_arc(const RECT & rect, int iStart, int iAngle)
   {

      ::Gdiplus::RectF rectf((Gdiplus::REAL) rect.left, (Gdiplus::REAL) rect.top, (Gdiplus::REAL) width(rect), (Gdiplus::REAL) height(rect));


      m_ppath->AddArc(rectf, iStart, iAngle);

      return true;

   }


   bool path::set(const ::draw2d::path::arc & arc)
   {

      rect rect;

      rect.left = (LONG)(arc.m_xCenter - arc.m_dRadiusX);
      rect.right = (LONG)(arc.m_xCenter + arc.m_dRadiusX);
      rect.top = (LONG)(arc.m_yCenter - arc.m_dRadiusY);
      rect.bottom = (LONG)(arc.m_yCenter + arc.m_dRadiusY);

      return internal_add_arc(rect, (int)arc.m_dAngle1, (int)arc.m_dAngle2);

   }

   bool path::set(const ::draw2d::path::line & l)
   {

      internal_add_line((int32_t) l.m_x, (int32_t) l.m_y);

      return true;

   }


   bool path::set(const ::draw2d::path::move & p)
   {

      internal_add_move((int32_t) p.m_x, (int32_t) p.m_y);


      return true;

   }


} // namespace draw2d_gdiplus


#define new BASE_NEW