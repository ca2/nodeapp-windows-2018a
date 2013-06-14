#include "framework.h"


#undef new


namespace win
{



   graphics_path::graphics_path(sp(::ca2::application) papp)
   {

      m_ppath = new Gdiplus::GraphicsPath();

      m_bHasPoint = false;

      m_ptInternal.X = 69;

      m_ptInternal.Y = 69;

      m_bHasPointInternal = false;

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

      if(!m_bUpdated)
      {
         ((graphics_path *)this)->defer_update();
      }

      return (void *) m_ppath;

   }


   point graphics_path::internal_last_point()
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


   bool graphics_path::internal_begin_figure(bool bFill, ::ca2::e_fill_mode efillmode)
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
   
   bool graphics_path::internal_add_line(int32_t x, int32_t y)
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

   point graphics_path::internal_current_point()
   {

      return last_point();

   }


   bool graphics_path::internal_add_move(int32_t x, int32_t y)
   {

      m_ptInternal.X   = (Gdiplus::REAL) x;
      m_ptInternal.Y   = (Gdiplus::REAL) y;
      m_bHasPointInternal     = true;


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


   bool graphics_path::set(const ::ca2::graphics_path::element & e)
   {

      switch(e.m_etype)
      {
      case ::ca2::graphics_path::element::type_arc:
         set(e.m_arc);
         break;
      case ::ca2::graphics_path::element::type_move:
         set(e.m_move);
         break;
      case ::ca2::graphics_path::element::type_line:
         set(e.m_line);
         break;
      case ::ca2::graphics_path::element::type_end:
         internal_end_figure(e.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }

   bool graphics_path::set(const ::ca2::graphics_path::arc & a)
   {

      //internal_add_arc();

      return true;

   }

   bool graphics_path::set(const ::ca2::graphics_path::line & l)
   {

      internal_add_line((int32_t) l.m_x, (int32_t) l.m_y);

      return true;

   }


   bool graphics_path::set(const ::ca2::graphics_path::move & p)
   {

      internal_add_move((int32_t) p.m_x, (int32_t) p.m_y);


      return true;

   }


} // namespace win


#define new DEBUG_NEW