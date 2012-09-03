#include "framework.h"

#undef new

namespace win
{

   graphics_path::graphics_path(::ca::application * papp)
   {

      m_ppath = new Gdiplus::GraphicsPath();

   }


   graphics_path::~graphics_path()
   {

      if(m_ppath != NULL)
      {

         delete m_ppath;

         m_ppath = NULL;

      }

   }


   int_ptr graphics_path::get_os_data() const
   {

      return (int_ptr) m_ppath;

   }


   point graphics_path::last_point()
   {

      Gdiplus::PointF pf;

      if(m_ppath->GetLastPoint(&pf) != Gdiplus::Status::Ok)
         throw simple_exception("could not get last point from path");

      return point((long) pf.X, (long) pf.Y);

   }


   bool graphics_path::start_figure()
   {

      return m_ppath->StartFigure() == Gdiplus::Status::Ok;

   }

   bool graphics_path::close_figure()
   {

      return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

   }

   
   bool graphics_path::add_line(int x1, int y1, int x2, int y2)
   {

      return m_ppath->AddLine(x1, y1, x2, y2) == Gdiplus::Status::Ok;

   }

   bool graphics_path::add_line(point p1, point p2)
   {

      return m_ppath->AddLine(p1.x, p1.y, p2.x, p2.y) == Gdiplus::Status::Ok;

   }

} // namespace win

#define new DEBUG_NEW