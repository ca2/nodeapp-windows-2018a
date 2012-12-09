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

   bool graphics_path::add_line(int x1, int y1, int x2, int y2)
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
   
   bool graphics_path::add_line(int x, int y)
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


   bool graphics_path::add_move(int x, int y)
   {

      m_point.X   = (Gdiplus::REAL) x;
      m_point.Y   = (Gdiplus::REAL) y;
      m_bMove     = true;

      return true;

   }


} // namespace win

#define new DEBUG_NEW