#include "framework.h"

bool almost_integer(double d)
{

   if (fmod(fabs(d), 1.0) < 0.01)
   {

      return true;

   }

   return false;
}

namespace draw2d_gdiplus
{



   path::path(::aura::application * papp)
   {

      m_ppath = NULL;


   }


   path::~path()
   {

      destroy();

   }


   Gdiplus::GraphicsPath * path::get_os_path(Gdiplus::Graphics * pgraphics)
   {

      if(m_bUpdated)
         return m_ppath;

      destroy();

      create(pgraphics);

      return m_ppath;

   }


   pointd path::internal_last_point()
   {

      if(m_bHasPointInternal)
      {

         return pointd((LONG) m_ptInternal.X, (LONG) m_ptInternal.Y);

      }
      else
      {

         throw simple_exception(get_app(), "path does not have last point");

      }

   }


   bool path::internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode)
   {

      if (m_ppath == NULL)
      {

         return false;

      }

      try
      {

         return m_ppath->StartFigure() == Gdiplus::Status::Ok;

      }
      catch (...)
      {


      }


      return false;

   }

   bool path::internal_end_figure(bool bClose)
   {

      if(bClose)
      {

         if (m_ppath == NULL)
         {

            return false;

         }

         try
         {

            m_bHasPointInternal = false;

            return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

         }
         catch (...)
         {


         }

         return false;


      }
      else
      {
         m_bHasPointInternal = false;
         return true;

      }

   }

   bool path::internal_add_line(double x1, double y1, double x2, double y2)
   {


      if (m_ppath == NULL)
      {

         return false;

      }

      bool bOk1 = false;

      bool bOk2 = false;

      try
      {

         bOk1 = true;

         if (m_bHasPointInternal)
         {
            if (almost_integer(m_ptInternal.X)
                  && almost_integer(m_ptInternal.Y)
                  && almost_integer(x1)
                  && almost_integer(x2))
            {

               bOk1 = m_ppath->AddLine((INT) m_ptInternal.X, (INT)m_ptInternal.Y, (INT)x1, (INT)y1) == Gdiplus::Status::Ok;

            }
            else
            {

               bOk1 = m_ppath->AddLine(m_ptInternal.X, m_ptInternal.Y, (FLOAT)x1, (FLOAT)y1) == Gdiplus::Status::Ok;

            }

         }

         if (bOk1)
         {

            if (almost_integer(m_ptInternal.X)
                  && almost_integer(m_ptInternal.Y)
                  && almost_integer(x1)
                  && almost_integer(x2))
            {

               bOk2 = m_ppath->AddLine((INT) x1, (INT) y1, (INT) x2, (INT) y2) == Gdiplus::Status::Ok;

            }
            else
            {

               bOk2 = m_ppath->AddLine((FLOAT)x1, (FLOAT)y1, (FLOAT)x2, (FLOAT)y2) == Gdiplus::Status::Ok;

            }

         }

         m_ptInternal.X = (Gdiplus::REAL) x2;
         m_ptInternal.Y = (Gdiplus::REAL) y2;

         m_bHasPointInternal = true;

      }
      catch (...)
      {

      }

      return bOk1 && bOk2;

   }


   bool path::internal_add_rect(double x,double y, double cx, double cy)
   {

      if (m_ppath == NULL)
      {

         return false;

      }

      bool bOk2 = false;

      if (almost_integer(x) && almost_integer(cx) && almost_integer(y) && almost_integer(cy))
      {

         Gdiplus::Rect rect(
            convert < INT > (x),
            convert < INT > (y),
            convert < INT > (cx),
            convert < INT > (cy)
         );

         try
         {

            bOk2 = m_ppath->AddRectangle(rect) == Gdiplus::Status::Ok;

         }
         catch (...)
         {


         }

      }
      else
      {

         Gdiplus::RectF rect(
            convert < FLOAT >(x),
            convert < FLOAT >(y),
            convert < FLOAT >(cx),
            convert < FLOAT >(cy)
         );

         try
         {

            bOk2 = m_ppath->AddRectangle(rect) == Gdiplus::Status::Ok;

         }
         catch (...)
         {


         }
      }

      return bOk2;

   }

   bool path::internal_add_line(double x, double y)
   {

      bool bOk1 = true;

      if(m_bHasPointInternal)
      {

         if (almost_integer(m_ptInternal.X)
               && almost_integer(m_ptInternal.Y)
               && almost_integer(x)
               && almost_integer(y))
         {

            bOk1 = m_ppath->AddLine((INT) m_ptInternal.X, (INT)m_ptInternal.Y, (INT)x, (INT)y) == Gdiplus::Status::Ok;

         }
         else
         {

            bOk1 = m_ppath->AddLine((FLOAT)m_ptInternal.X, (FLOAT) m_ptInternal.Y, (FLOAT)x, (FLOAT)y) == Gdiplus::Status::Ok;

         }

      }

      m_bHasPointInternal = true;

      m_ptInternal.X = (Gdiplus::REAL) x;
      m_ptInternal.Y = (Gdiplus::REAL) y;

      return bOk1;

   }

   pointd path::internal_current_point()
   {

      return last_point();

   }


   bool path::internal_add_move(double x, double y)
   {

      if(!m_bHasPointInternal)
      {
         internal_begin_figure(m_bFill,m_efillmode);
      }

      m_ptInternal.X   = (Gdiplus::REAL) x;
      m_ptInternal.Y   = (Gdiplus::REAL) y;
      m_bHasPointInternal     = true;


      return true;

   }

   //bool path::internal_add_rect(double x,double y,double cx,double cy)
   //{

   //   if (m_ppath == NULL)
   //   {

   //      return false;

   //   }

   //   bool bOk2 = false;

   //   try
   //   {

   //      Gdiplus::RectF rectf((Gdiplus::REAL)x, (Gdiplus::REAL)y, (Gdiplus::REAL)cx, (Gdiplus::REAL)cy);

   //      bOk2 = m_ppath->AddRectangle(rectf) == Gdiplus::Status::Ok;

   //   }
   //   catch (...)
   //   {


   //   }

   //   return bOk2;

   //}


   //bool path::internal_add_move(double x,double y)
   //{

   //   if(!m_bHasPointInternal)
   //   {
   //      internal_begin_figure(m_bFill,m_efillmode);
   //   }

   //   m_ptInternal.X   = (Gdiplus::REAL) x;
   //   m_ptInternal.Y   = (Gdiplus::REAL) y;
   //   m_bHasPointInternal     = true;


   //   return true;

   //}


   bool path::create(Gdiplus::Graphics * pgraphics)
   {

      destroy();

      if(m_efillmode == ::draw2d::fill_mode_winding)
      {

         m_ppath = new Gdiplus::GraphicsPath(Gdiplus::FillModeWinding);

      }
      else
      {

         m_ppath = new Gdiplus::GraphicsPath(Gdiplus::FillModeAlternate);

      }

      m_bHasPoint = false;

      m_ptInternal.X = 69;

      m_ptInternal.Y = 69;

      m_bHasPointInternal = false;

      //if(m_elementa.get_count() != 1 && m_elementa[0].m_etype != element::type_string)
      {
         //internal_begin_figure(m_bFill,m_efillmode);
      }

      for(int32_t i = 0; i < m_elementa.get_count(); i++)
      {

         set(pgraphics, m_elementa(i));

      }

      return true;

   }


   bool path::destroy()
   {

      ::aura::del(m_ppath);

      return true;

   }


   bool path::set(Gdiplus::Graphics * pgraphics, const ::draw2d::path::element & e)
   {

      switch(e.m_etype)
      {
      case ::draw2d::path::element::type_arc:
         set(e.u.m_arc);
         break;
      case ::draw2d::path::element::type_move:
         set(e.u.m_move);
         break;
      case ::draw2d::path::element::type_line:
         set(e.u.m_line);
         break;
      case ::draw2d::path::element::type_rect:
         set(e.u.m_rect);
         break;
      case ::draw2d::path::element::type_string:
         set(pgraphics,e.m_stringpath);
         break;
      case ::draw2d::path::element::type_end:
         internal_end_figure(e.u.m_end.m_bClose);
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;

   }

   bool path::internal_add_arc(const RECT & rect, double iStart, double iAngle)
   {

      if (m_ppath == NULL)
      {

         return false;

      }

      try
      {


         ::Gdiplus::RectF rectf((Gdiplus::REAL) rect.left, (Gdiplus::REAL) rect.top, (Gdiplus::REAL) width(rect), (Gdiplus::REAL) height(rect));


         m_ppath->AddArc(rectf, (Gdiplus::REAL) iStart, (Gdiplus::REAL) iAngle);

         ::Gdiplus::PointF p;

         m_ppath->GetLastPoint(&p);

         m_ptInternal.X = p.X;
         m_ptInternal.Y = p.Y;

      }
      catch (...)
      {

      }

      return true;

   }


   bool path::internal_add_string(Gdiplus::Graphics * pgraphics,int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont)
   {

      if (m_ppath == NULL)
      {

         return false;

      }

      try
      {

         Gdiplus::FontFamily fontFamily;

         Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());

         wstring wstr(strText);

         Gdiplus::REAL dSize = (Gdiplus::REAL) spfont->m_dFontSize;

         Gdiplus::Unit unit = pgraphics->GetPageUnit();

         switch (unit)
         {

         case Gdiplus::UnitMillimeter:
            dSize = dSize * 25.4f / pgraphics->GetDpiY();
            break;

         case Gdiplus::UnitInch:

            dSize = dSize / pgraphics->GetDpiY();
            break;
         case Gdiplus::UnitPoint:

            dSize = dSize * 72.0f / pgraphics->GetDpiY();
            break;

         }

         INT iStyle = ((Gdiplus::Font *) spfont->get_os_data())->GetStyle();
         ((Gdiplus::Font *) spfont->get_os_data())->GetFamily(&fontFamily);
         //      Gdiplus::Status status;

         //Gdiplus::StringFormat format();

         format.SetFormatFlags(format.GetFormatFlags()
                               | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                               | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap
                               | Gdiplus::StringFormatFlagsNoFitBlackBox);


         format.SetLineAlignment(Gdiplus::StringAlignmentNear);


         m_ppath->AddString(wstr, (INT)wstr.get_length(), &fontFamily, iStyle, dSize, Gdiplus::Point(x, y), &format);

      }
      catch (...)
      {

      }

      return true;

   }


   bool path::set( const ::draw2d::path::arc & arc)
   {

      ::rect rect;

      rect.left = (LONG)(arc.m_xCenter - arc.m_dRadiusX);
      rect.right = (LONG)(arc.m_xCenter + arc.m_dRadiusX);
      rect.top = (LONG)(arc.m_yCenter - arc.m_dRadiusY);
      rect.bottom = (LONG)(arc.m_yCenter + arc.m_dRadiusY);
      //if(!m_bHasPointInternal)
      //{

      //   internal_add_move(arc.m_ptEnd.x,arc.m_ptEnd.y);

      //}

      bool bOk = internal_add_arc(rect,(int)(arc.m_dAngle1 * 180.0 / 3.1415),(int)(arc.m_dAngle* 180.0 / 3.1415));


      return bOk;

   }

   bool path::set(const ::draw2d::path::rect & rect)
   {


      return internal_add_rect(rect.m_x, rect.m_y, rect.m_cx, rect.m_cy);

   }

   bool path::set(const ::draw2d::path::line & l)
   {

      internal_add_line(l.m_x, l.m_y);

      return true;

   }


   bool path::set(const ::draw2d::path::move & p)
   {

      internal_add_move((int32_t) p.m_x, (int32_t) p.m_y);


      return true;

   }

   bool path::set(Gdiplus::Graphics * pgraphics, const ::draw2d::path::string_path & s)
   {

      internal_add_string(pgraphics,(int32_t)s.m_x,(int32_t)s.m_y,s.m_strText,s.m_spfont);

      return true;

   }


} // namespace draw2d_gdiplus


