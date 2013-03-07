#include "framework.h"


#undef new


namespace win
{



   graphics_path::graphics_path(::ca::application * papp)
   {

      m_ppath = new Gdiplus::GraphicsPath();

      m_bHasPoint = false;

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


   point graphics_path::internal_last_point()
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


   bool graphics_path::internal_begin_figure(bool bFill, ::ca::e_fill_mode efillmode)
   {

      return m_ppath->StartFigure() == Gdiplus::Status::Ok;

   }

   bool graphics_path::internal_end_figure(bool bClose)
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

   bool graphics_path::internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
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
   
   bool graphics_path::internal_add_line(int32_t x, int32_t y)
   {

      point pt = last_point();

      bool bOk = m_ppath->AddLine(pt.x, pt.y, x, y) == Gdiplus::Status::Ok;

      m_bMove = false;

      return bOk;

   }

   point graphics_path::internal_current_point()
   {

      return last_point();

   }


   bool graphics_path::internal_add_move(int32_t x, int32_t y)
   {

      m_point.X   = (Gdiplus::REAL) x;
      m_point.Y   = (Gdiplus::REAL) y;
      m_bMove     = true;

      return true;

   }



   bool graphics_path::update()
   {

      internal_begin_figure(m_bFill, m_efillmode);

      for(int32_t i = 0; i < m_elementa.get_count(); i++)
      {

         set(m_elementa[i]);

      }

      return true;

   }


   bool graphics_path::set(const ::ca::graphics_path::element & e)
   {

      switch(e.m_etype)
      {
      case ::ca::graphics_path::element::type_arc:
         set(e.m_arc);
         break;
      case ::ca::graphics_path::element::type_line:
         set(e.m_line);
         break;
      case ::ca::graphics_path::element::type_end:
         internal_end_figure(e.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }

   bool graphics_path::set(const ::ca::graphics_path::arc & a)
   {

      //internal_add_arc();

      return true;

   }

   bool graphics_path::set(const ::ca::graphics_path::line & l)
   {

      internal_add_line((int32_t) l.m_x, (int32_t) l.m_y);

      return true;

   }


   bool graphics_path::set(const ::ca::graphics_path::move & p)
   {

      internal_add_move(p.m_x, p.m_y);


      return true;

   }


} // namespace win


#define new DEBUG_NEW