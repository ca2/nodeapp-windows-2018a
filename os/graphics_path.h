#pragma once

#undef new

#include <GdiPlus.h>


namespace win
{

   class CLASS_DECL_win graphics_path : 
      virtual public ::ca::graphics_path
   {
   public:

      
      Gdiplus::GraphicsPath *   m_ppath;


      graphics_path(::ca::application * papp);
      virtual ~graphics_path();


      virtual int_ptr get_os_data() const;


      virtual point last_point();


      virtual bool start_figure();
      virtual bool close_figure();


      virtual bool add_line(int x1, int y1, int x2, int y2);
      virtual bool add_line(point p1, point p2);

   };

} // namespace win

#define new DEBUG_NEW