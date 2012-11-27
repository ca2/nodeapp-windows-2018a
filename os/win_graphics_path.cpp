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


   void * graphics_path::get_os_data() const
   {

      return (void *) m_ppath;

   }


   point graphics_path::last_point()
   {

      Gdiplus::PointF pf;

      if(m_ppath->GetLastPoint(&pf) != Gdiplus::Status::Ok)
         throw simple_exception(get_app(), "could not get last point from path");

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
   
   bool graphics_path::add_line(int x, int y)
   {

      point pt = last_point();

      return m_ppath->AddLine(pt.x, pt.y, x, y) == Gdiplus::Status::Ok;

   }

   point graphics_path::current_point()
   {

      return last_point();

   }


} // namespace win

#define new DEBUG_NEW