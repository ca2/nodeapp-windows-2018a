#include "framework.h"


#undef new


namespace win
{

   
   region::region(::ca::application * papp) :
      ca(papp)
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


   int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   { ASSERT(get_os_data() != NULL); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData); }
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   { ASSERT(get_os_data() != NULL); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2); }
   
   void region::SetRectRgn(LPCRECT lpRect)
   { 
      ::SetRectRgn((HRGN)get_os_data(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); 
   }

   int region::CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int nCombineMode)
   { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   (HRGN)pRgn2->get_os_data(), nCombineMode); }
   int region::CopyRgn(const ::ca::region* pRgnSrc)
   { ASSERT(get_os_data() != NULL); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), NULL, RGN_COPY); }

   bool region::EqualRgn(const ::ca::region* pRgn) const
   {
      
      ASSERT(get_os_data() != NULL); 
      
      return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != FALSE;

   }

   int region::OffsetRgn(int x, int y)
   { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), x, y); }
   int region::OffsetRgn(POINT point)
   { ASSERT(get_os_data() != NULL); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y); }
   int region::GetRgnBox(LPRECT lpRect) const
   { ASSERT(get_os_data() != NULL); return ::GetRgnBox((HRGN)get_os_data(), lpRect); }
   
   bool region::PtInRegion(int x, int y) const
   { 

      Gdiplus::PointF pointf((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      
      ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      return m_pregion->IsVisible(pointf)  != FALSE;
   
   }

   bool region::PtInRegion(POINT point) const
   { 

      //return ::PtInRegion((HRGN)get_os_data(), point.x, point.y); 
      Gdiplus::PointF pointf((Gdiplus::REAL) point.x, (Gdiplus::REAL) point.y);
      
      ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      return m_pregion->IsVisible(pointf)  != FALSE;

   }

   bool region::RectInRegion(LPCRECT lpRect) const
   { 
      //ASSERT(get_os_data() != NULL); return ::RectInRegion((HRGN)get_os_data(), lpRect); 

      Gdiplus::RectF rectf((Gdiplus::REAL) lpRect->left, (Gdiplus::REAL) lpRect->top, (Gdiplus::REAL) (lpRect->right - lpRect->left), (Gdiplus::REAL) (lpRect->bottom - lpRect->top));
      
      ASSERT(get_os_data() != NULL); //return ::PtInRegion((HRGN)get_os_data(), x, y); 

      return m_pregion->IsVisible(rectf)  != FALSE;

   }

   int_ptr region::get_os_data() const
   {

      if(m_pregion == NULL || !m_bUpdated)
      {
         if(m_pregion != NULL)
         {
            delete m_pregion;
         }

         Gdiplus::GraphicsPath path;

         if(m_etype == type_elliptic)
         {
            path.AddEllipse((INT) m_pta[0].x, (INT) m_pta[0].y, (INT) (m_pta[1].x - m_pta[0].x), (INT) (m_pta[1].y - m_pta[0].y));
         }
         else if(m_etype == type_polygon)
         {
            raw_array < Gdiplus::PointF > pa;

            for(int i = 0; i < m_pta.get_size(); i++)
            {
               pa.add(Gdiplus::PointF((Gdiplus::REAL) m_pta[i].x, (Gdiplus::REAL) m_pta[i].y));
            }

            path.AddPolygon(pa.get_data(), (int) pa.get_count());
         }
         else if(m_etype == type_rect)
         {
            
            Gdiplus::RectF rect;

            rect.X      = (Gdiplus::REAL) m_pta[0].x;
            rect.Y      = (Gdiplus::REAL) m_pta[0].y;
            rect.Width  = (Gdiplus::REAL) (m_pta[1].x - m_pta[0].x);
            rect.Height = (Gdiplus::REAL) (m_pta[1].y - m_pta[0].y);

            path.AddRectangle(rect);

         }

         ((region *) this)->m_pregion = new Gdiplus::Region(&path);
         
      }

      if(m_pregion != NULL)
      {
         ((region *) this)->m_bUpdated = true;
      }



      return (int_ptr) (Gdiplus::Region *) m_pregion;
   }


} // namespace win