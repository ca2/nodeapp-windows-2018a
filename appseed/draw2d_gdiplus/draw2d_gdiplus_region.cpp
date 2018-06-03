#include "framework.h"


#undef new


namespace draw2d_gdiplus
{


   region::region(const region & r) :
      ::object(r.get_app()),
      ::draw2d::region(r)
   {
      m_pregion = NULL;
      m_bUpdated = false;
   }


   region::region(::aura::application * papp) :
      ::object(papp)
   {

      m_pregion = NULL;
      m_bUpdated = false;

   }

   region::~region()
   {

      if(m_pregion != NULL)
      {
         delete m_pregion;
         m_pregion = NULL;
      }

   }

   /*
      int32_t region::GetRegionData(LPRGNDATA lpRgnData, int32_t nDataSize) const
      { ASSERT(get_os_data() != NULL); return (int32_t)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData); }
      void region::SetRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
      { ASSERT(get_os_data() != NULL); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2); }

      void region::SetRectRgn(LPCRECT lpRect)
      {
         ::SetRectRgn((HRGN)get_os_data(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      }

      int32_t region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int32_t nCombineMode)
      { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
      (HRGN)pRgn2->get_os_data(), nCombineMode); }
      int32_t region::CopyRgn(const ::draw2d::region* pRgnSrc)
      { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), NULL, RGN_COPY); }

      bool region::EqualRgn(const ::draw2d::region* pRgn) const
      {

         ASSERT(get_os_data() != NULL);

         return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != FALSE;

      }

      int32_t region::OffsetRgn(int32_t x, int32_t y)
      { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), x, y); }
      int32_t region::OffsetRgn(POINT point)
      { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y); }

      */
   bool region::get_bounding_box(LPRECT lprect) const
   {

      ((region *)this)->defer_update();

      Gdiplus::Rect rect;

      ((region *) this)->m_pregion->GetBounds(&rect, &Gdiplus::Graphics((HDC) NULL));


      lprect->left = rect.X;
      lprect->top = rect.Y;
      lprect->right = rect.X + rect.Width;
      lprect->bottom = rect.Y + rect.Height;

      return true;

   }
   /*
   bool region::PtInRegion(int32_t x, int32_t y) const
   {

      Gdiplus::PointF pointf((Gdiplus::REAL) x, (Gdiplus::REAL) y);

      ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      return m_pregion->IsVisible(pointf)  != FALSE;

   }
   */


   void region::max_bounding_box(LPRECTD lprect)
   {
      rect r;
      ((region*)this)->get_bounding_box(r);
      ::copy(lprect, r);
   }


   bool region::contains(POINT point) const
   {

      ((region *) (this))->defer_update();

      Gdiplus::PointF pointf((Gdiplus::REAL) point.x, (Gdiplus::REAL) point.y);

      ASSERT(get_os_data() != NULL);

      return m_pregion->IsVisible(pointf)  != FALSE;

   }
   /*
   bool region::RectInRegion(LPCRECT lpRect) const
   {
      //ASSERT(get_os_data() != NULL); return ::RectInRegion((HRGN)get_os_data(), lpRect);

      Gdiplus::RectF rectf((Gdiplus::REAL) lpRect->left, (Gdiplus::REAL) lpRect->top, (Gdiplus::REAL) (lpRect->right - lpRect->left), (Gdiplus::REAL) (lpRect->bottom - lpRect->top));

      ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      return m_pregion->IsVisible(rectf)  != FALSE;

   }
   */

   void * region::get_os_data() const
   {

      ((region *) this)->defer_update();

      return m_pregion;

   }


   void region::defer_update()
   {

      if(m_pregion == NULL || !m_bUpdated)
      {

         if(m_pregion != NULL)
         {

            ::aura::del(m_pregion);

         }

         m_pregion = get();

      }

   }


   bool region::destroy()
   {

      if(m_pregion != NULL)
      {

         delete m_pregion;

         m_pregion = NULL;

      }

      return ::draw2d::region::destroy();

   }

   Gdiplus::Region * region::get()
   {

      switch(m_etype)
      {
      case type_none:
         return new Gdiplus::Region();
      case type_rect:
         return get_rect();
      case type_oval:
         return get_oval();
      case type_polygon:
         return get_polygon();
      case type_poly_polygon:
         return get_polygon();
      case type_combine:
         return get_combine();
      default:
         ::exception::throw_not_implemented(get_app());
      }

      return NULL;

   }

   Gdiplus::Region * region::get_rect()
   {

      Gdiplus::GraphicsPath path;

      Gdiplus::RectF rect;

      rect.X      = (Gdiplus::REAL) m_x1;
      rect.Y      = (Gdiplus::REAL) m_y1;
      rect.Width  = (Gdiplus::REAL) (m_x2 - m_x1);
      rect.Height = (Gdiplus::REAL) (m_y2 - m_y1);

      path.AddRectangle(rect);

      return new Gdiplus::Region(&path);

   }


   Gdiplus::Region * region::get_oval()
   {

      Gdiplus::GraphicsPath path;

      path.AddEllipse((INT) m_x1, (INT) m_y1, (INT) (m_x2 - m_x1), (INT) (m_y2 - m_y1));

      return new Gdiplus::Region(&path);

   }

   Gdiplus::Region * region::get_polygon()
   {

      Gdiplus::GraphicsPath path;

      array < Gdiplus::PointF > pa;

      for(int32_t i = 0; i < m_nCount; i++)
      {
         pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[i].x, (Gdiplus::REAL) m_lppoints[i].y));
      }

      if(m_efillmode == ::draw2d::fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }

      path.AddPolygon(pa.get_data(), (int32_t) pa.get_count());

      return new Gdiplus::Region(&path);

   }

   Gdiplus::Region * region::get_poly_polygon()
   {
      Gdiplus::GraphicsPath path;

      array < Gdiplus::PointF > pa;

      if(m_efillmode == ::draw2d::fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }

      int32_t n = 0;

      for(int32_t i = 0; i < m_nCount; i++)
      {
         int32_t jCount = m_lppolycounts[i];
         pa.remove_all();
         for(int32_t j = 0; j < jCount; j++)
         {
            pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[n].x, (Gdiplus::REAL) m_lppoints[n].y));
            n++;
         }
         path.AddPolygon(pa.get_data(), (int32_t) pa.get_count());
         path.CloseFigure();
      }

      return new Gdiplus::Region(&path);

   }

   Gdiplus::Region * region::get_combine()
   {

      Gdiplus::Region * pregion = ((Gdiplus::Region *) m_pregion1->get_os_data())->Clone();
      Gdiplus::Region * pregionOp = (Gdiplus::Region *) m_pregion2->get_os_data();

      if(m_ecombine == combine_add)
      {
         pregion->Union(pregionOp);
      }
      else if(m_ecombine == combine_exclude)
      {
         pregion->Exclude(pregionOp);
      }
      else if(m_ecombine == combine_intersect)
      {
         pregion->Intersect(pregionOp);
      }
      else
      {
         pregion->Union(pregionOp);
      }

      return pregion;

   }

} // namespace draw2d_gdiplus